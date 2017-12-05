#pragma once

#include "cpprelude/defines.h"
#include <thread>
#include <atomic>
#include <chrono>
#include <limits>

namespace cpprelude
{
	struct binary_semaphore
	{
		std::atomic<bool> _counter;

		binary_semaphore()
			:_counter(true)
		{}

		isize
		take()
		{
			bool false_value = false;
			auto old = _counter.load();
			if(old == true && _counter.compare_exchange_weak(old, false_value))
				return 1;
			return -1;
		}

		isize
		give()
		{
			bool true_value = true;
			auto old = _counter.load();
			if(old == false && _counter.compare_exchange_weak(old, true_value))
				return 1;
			return -1;
		}

		isize
		wait_take()
		{
			while(true)
			{
				isize ticket = take();
				if (ticket == -1)
					std::this_thread::yield();
				else
					return ticket;
			}
		}

		isize
		wait_give()
		{
			while(true)
			{
				isize ticket = give();
				if(ticket == -1)
					std::this_thread::yield();
				else
					return ticket;
			}
		}

		bool
		available() const
		{
			return _counter.load(std::memory_order_acquire) == true;
		}
	};

	template<isize max_count>
	struct count_semaphore
	{
		binary_semaphore _lock;
		std::atomic<isize> _counter;

		count_semaphore()
			:_counter(max_count)
		{}

		isize
		take()
		{
			if(_lock.take() != -1)
			{
				auto ticket = _counter.load();
				if(ticket > 0)
				{
					--_counter;
					_lock.give();
					return ticket;
				}

				_lock.give();
				return -1;
			}
			return -1;
		}

		isize
		give()
		{
			if(_lock.take() != -1)
			{
				auto ticket = _counter.load();
				if(ticket < max_count)
				{
					++_counter;
					_lock.give();
					return ticket;
				}

				_lock.give();
				return -1;
			}
			return -1;
		}

		isize
		wait_take()
		{
			while(true)
			{
				isize ticket = take();
				if (ticket == -1)
					std::this_thread::yield();
				else
					return ticket;
			}
		}

		isize
		wait_give()
		{
			while(true)
			{
				isize ticket = give();
				if (ticket == -1)
					std::this_thread::yield();
				else
					return ticket;
			}
		}

		bool
		available() const
		{
			return _counter.load(std::memory_order_acquire) > 0;
		}

	};

	enum class thread_model: u8
	{
		swsr, //single writer single reader
		swmr  //single writer multi reader
	};

	using thread_context = u8;
	constexpr u8 _thread_context_none  = 0;
	constexpr u8 _thread_context_read  = 1;
	constexpr u8 _thread_context_write = 2;

	template<typename T>
	struct thread_unique
	{
		T value;
		binary_semaphore _lock;

		thread_unique()
		{}

		thread_unique(const thread_unique&) = delete;

		thread_unique(thread_unique&&) = default;

		thread_unique&
		operator=(const thread_unique&) = delete;

		thread_unique&
		operator=(thread_unique&&) = default;

		bool
		read(thread_context& context)
		{
			if(context & _thread_context_read)
				return true;

			if(_lock.wait_take() != -1)
			{
				context |= _thread_context_read;
				return true;
			}

			return false;
		}

		bool
		read_release(thread_context& context)
		{
			if(!(context & _thread_context_read))
				return true;

			if(_lock.wait_give() != -1)
			{
				context &= ~_thread_context_read;
				return true;
			}

			return false;
		}

		bool
		write(thread_context& context)
		{
			if(context & _thread_context_write)
				return true;

			if(_lock.wait_take() != -1)
			{
				context |= _thread_context_write;
				return true;
			}

			return false;
		}

		bool
		write_release(thread_context& context)
		{
			if(!(context & _thread_context_write))
				return true;

			if(_lock.wait_give() != -1)
			{
				context &= ~_thread_context_write;
				return true;
			}

			return false;
		}

		thread_context
		new_context()
		{
			return _thread_context_none;
		}
	};

	template<typename T>
	struct thread_multi_reader
	{
		T value;
		binary_semaphore _lock;
		std::atomic<usize> _reading;
		std::atomic<bool> _writing;

		thread_multi_reader()
			:_reading(0), _writing(false)
		{}

		thread_multi_reader(const thread_multi_reader&) = delete;

		thread_multi_reader(thread_multi_reader&&) = default;

		thread_multi_reader&
		operator=(const thread_multi_reader&) = delete;

		thread_multi_reader&
		operator=(thread_multi_reader&&) = default;

		bool
		read(thread_context& context)
		{
			if(context & _thread_context_read)
				return true;

			if(_lock.wait_take() != -1)
			{
				if(_writing == false)
				{
					context |= _thread_context_read;
					++_reading;
					_lock.wait_give();
					return true;
				}

				_lock.wait_give();
				return false;
			}

			return false;
		}

		bool
		read_release(thread_context& context)
		{
			if(!(context & _thread_context_read))
				return true;

			if(_lock.wait_take() != -1)
			{
				if(_reading > 0)
				{
					context &= ~_thread_context_read;
					--_reading;
					_lock.wait_give();
					return true;
				}

				_lock.wait_give();
				return false;
			}

			return false;
		}

		bool
		write(thread_context& context)
		{
			if(context & _thread_context_write)
				return true;

			if(_lock.wait_take() != -1)
			{
				if(_writing == false && _reading == 0)
				{
					context |= _thread_context_write;
					_writing = true;
					_lock.wait_give();
					return true;
				}

				_lock.wait_give();
				return false;
			}

			return false;
		}

		bool
		write_release(thread_context& context)
		{
			if(!(context & _thread_context_write))
				return true;

			if(_lock.wait_take() != -1)
			{
				if(_writing)
				{
					context &= ~_thread_context_write;
					_writing = false;
					_lock.wait_give();
					return true;
				}

				_lock.wait_give();
				return false;
			}

			return false;
		}

		thread_context
		new_context()
		{
			return _thread_context_none;
		}
	};
}
