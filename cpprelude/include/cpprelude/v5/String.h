#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/defaults.h"
#include "cpprelude/v5/Owner.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"
#include "cpprelude/v5/Ranges.h"
#include <cstring>

namespace cpprelude
{
	struct String
	{
		using Data_Type = byte;
		using Range_Type = String_Range;
		using Const_Range_Type = String_Range;
		using iterator = String_Iterator;
		using const_iterator = String_Iterator;

		Memory_Context mem_context;
		Owner<byte> _bytes;
		usize _bytes_size;
		mutable usize _count;

		String(const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _bytes_size(0),
			 _count(-1)
		{}

		String(usize count, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _bytes_size(0),
			 _count(-1)
		{
			_bytes = mem_context.template alloc<byte>(count);
			_bytes[0] = 0;
		}

		String(const char* str, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _bytes_size(0),
			 _count(-1)
		{
			_bytes_size = std::strlen(str) + 1;
			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move(_bytes, own((byte*)str, _bytes_size), _bytes_size);
		}

		String(const char* str, usize size, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _bytes_size(0),
			 _count(-1)
		{
			_bytes_size = size + 1;
			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move(_bytes, own((byte*)str, size), size);
			_bytes[size] = 0;
		}

		String(const Owner<byte>& data, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _count(-1)
		{
			_bytes_size = data.count();
			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move(_bytes, data, _bytes_size);
		}

		String(Owner<byte>&& data, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _bytes(std::move(data)),
			 _bytes_size(_bytes.count()),
			 _count(-1)
		{}

		String(const String& other)
			:mem_context(other.mem_context),
			 _bytes_size(other._bytes_size),
			 _count(other._count)
		{
			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move(_bytes, other._bytes, _bytes_size);
		}

		String(const String& other, const Memory_Context& context)
			:mem_context(context),
			 _bytes_size(other._bytes_size),
			 _count(other._count)
		{
			_bytes = mem_context.template alloc<byte>(_bytes_size);
			move(_bytes, other._bytes, _bytes_size);
		}

		String(String&& other)
			:mem_context(std::move(other.mem_context)),
			 _bytes(std::move(other._bytes)),
			 _bytes_size(other._bytes_size),
			 _count(other._count)
		{
			other._bytes_size = 0;
			other._count = -1;
		}

		~String()
		{
			reset();
		}

		String&
		operator=(const String& other)
		{
			_bytes_size = other._bytes_size;
			if(_bytes.count() < other._bytes.count())
			{
				mem_context.template free<byte>(_bytes);
				_bytes = mem_context.template alloc<byte>(_bytes_size);
			}

			move(_bytes, other._bytes, _bytes_size);
			_count = other._count;
			return *this;
		}

		String&
		operator=(const char* str)
		{
			_bytes_size = std::strlen(str) + 1;
			if(_bytes.count() < _bytes_size)
			{
				mem_context.template free<byte>(_bytes);
				_bytes = mem_context.template alloc<byte>(_bytes_size);
			}

			move(_bytes, own((byte*)str, _bytes_size), _bytes_size);
			_count = -1;
			return *this;
		}

		String&
		operator=(String&& other)
		{
			mem_context = std::move(other.mem_context);
			_bytes = std::move(other._bytes);
			_bytes_size = other._bytes_size;
			_count = other._count;

			other._bytes_size = 0;
			other._count = -1;
			return *this;
		}


		usize
		count() const
		{
			if(_count == -1)
				_count = internal::_utf8_count_chars(_bytes.range(0, _bytes_size));
			return _count;
		}

		usize
		size() const
		{
			return _bytes_size;
		}

		usize
		capacity() const
		{
			return _bytes.count();
		}

		bool
		empty() const
		{
			return _bytes_size == 0;
		}

		const byte*
		data() const
		{
			return _bytes.ptr;
		}

		const byte&
		operator[](usize index) const
		{
			return _bytes[index];
		}

		bool
		operator==(const String& other) const
		{
			if(_bytes_size != other._bytes_size)
				return false;

			for(usize i = 0; i < _bytes_size; ++i)
				if(_bytes[i] != other._bytes[i])
					return false;
			return true;
		}

		bool
		operator==(const Range_Type& str_range) const
		{
			usize limit = count();
			if(limit != str_range.count())
				return false;

			auto this_it = begin();
			auto range_it = str_range.begin();
			for(usize i = 0; i < limit; ++i, ++this_it, ++range_it)
				if(*this_it != *range_it)
					return false;
			return true;
		}

		bool
		operator!=(const String& other) const
		{
			return !operator==(other);
		}

		bool
		operator!=(const Range_Type& str_range) const
		{
			return !operator==(str_range);
		}

		bool
		operator<(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) < 0;
		}

		bool
		operator<(const Range_Type& str_range) const
		{
			return internal::_strcmp(_bytes.all(), str_range.bytes) < 0;
		}

		bool
		operator>(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) > 0;
		}

		bool
		operator>(const Range_Type& str_range) const
		{
			return internal::_strcmp(_bytes.all(), str_range.bytes) > 0;
		}

		bool
		operator<=(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) <= 0;
		}

		bool
		operator<=(const Range_Type& str_range) const
		{
			return internal::_strcmp(_bytes.all(), str_range.bytes) <= 0;
		}

		bool
		operator>=(const String& other) const
		{
			return internal::_strcmp(_bytes.all(), other._bytes.all()) >= 0;
		}

		bool
		operator>=(const Range_Type& str_range) const
		{
			return internal::_strcmp(_bytes.all(), str_range.bytes) >= 0;
		}

		void
		concat(const String& other)
		{
			if((_bytes.count() - _bytes_size) < other._bytes_size)
			{
				usize double_cap = _bytes.count() * 2;
				usize fit = _bytes_size + other._bytes_size;
				usize new_cap = double_cap > fit ? double_cap : fit;
				auto new_bytes = mem_context.template alloc<byte>(new_cap);
				if (_bytes_size != 0)
				{
					move(new_bytes, _bytes, _bytes_size);
					mem_context.template free<byte>(_bytes);
				}
				_bytes = std::move(new_bytes);
			}

			if (_bytes_size != 0)
			{
				//for the null termination
				--_bytes_size;
				move<byte>(_bytes.range(_bytes_size, _bytes_size + other._bytes_size),
						   other._bytes.all(),
						   other._bytes_size);
			}
			else
			{
				move<byte>(_bytes.all(),
						   other._bytes.all(),
						   other._bytes_size);
			}
			_bytes_size += other._bytes_size;
			_count = -1;
		}

		void
		concat(const char* str)
		{
			usize str_bytes_size = std::strlen(str) + 1;
			if((_bytes.count() - _bytes_size) < str_bytes_size)
			{
				usize double_cap = _bytes.count() * 2;
				usize fit = _bytes_size + str_bytes_size;
				usize new_cap = double_cap > fit ? double_cap : fit;
				auto new_bytes = mem_context.template alloc<byte>(new_cap);
				if (_bytes_size != 0)
				{
					move(new_bytes, _bytes, _bytes_size);
					mem_context.template free<byte>(_bytes);
				}
				_bytes = std::move(new_bytes);
			}

			if (_bytes_size != 0)
			{
				//for the null termination
				--_bytes_size;
				move<byte>(_bytes.range(_bytes_size, _bytes_size + str_bytes_size),
						   Slice<byte>((byte*)str, str_bytes_size),
						   str_bytes_size);
			}
			else
			{
				move<byte>(_bytes.all(),
						   Slice<byte>((byte*)str, str_bytes_size),
						   str_bytes_size);
			}
			_bytes_size += str_bytes_size;
			_count = -1;
		}

		String
		substr(usize start, usize end, const Memory_Context& context = os->global_memory) const
		{
			auto start_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), start);
			auto end_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), end);

			String result(_bytes.ptr + start_bytes_offset, end_bytes_offset - start_bytes_offset, context);
			result._count = end - start;
			return result;
		}

		void
		clear()
		{
			_bytes_size = 0;
			_count = -1;
		}

		void
		reset()
		{
			mem_context.template free<byte>(_bytes);
			_bytes_size = 0;
			_count = -1;
		}

		void
		reserve(usize expected_count)
		{
			if((_bytes.count() - _bytes_size) < expected_count)
			{
				usize double_cap = _bytes.count() * 2;
				usize fit = _bytes_size + expected_count;
				usize new_cap = double_cap > fit ? double_cap : fit;
				auto new_bytes = mem_context.template alloc<byte>(new_cap);
				if(_bytes_size != 0)
				{
					move(new_bytes, _bytes, _bytes_size);
					mem_context.template free<byte>(_bytes);
				}
				_bytes = std::move(new_bytes);
			}
		}

		Const_Range_Type
		all() const
		{
			return Const_Range_Type(_bytes.all(), count());
		}

		Const_Range_Type
		range(usize start, usize end) const
		{
			auto start_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), start);
			auto end_bytes_offset = internal::_utf8_count_runes_to(_bytes.all(), end);

			return Const_Range_Type(_bytes.range(start_bytes_offset, end_bytes_offset), end - start);
		}

		/**
		 * @return     An Iterator to the beginning of this container
		 */
		iterator
		begin()
		{
			return _bytes.ptr;
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		begin() const
		{
			return _bytes.ptr;
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		cbegin() const
		{
			return _bytes.ptr;
		}

		/**
		 * @return     An Iterator to the end of the container
		 */
		iterator
		end()
		{
			return _bytes.ptr + _bytes_size;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		end() const
		{
			return _bytes.ptr + _bytes_size;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		cend() const
		{
			return _bytes.ptr + _bytes_size;
		}
	};

	inline static isize
	strcmp(const String& a, const String& b)
	{
		return internal::_strcmp(a._bytes.all(), b._bytes.all());
	}

	inline static isize
	strcmp(const String& a, const String_Range& b)
	{
		return internal::_strcmp(a._bytes.all(), b.bytes);
	}

	inline static isize
	strcmp(const String_Range& a, const String& b)
	{
		return internal::_strcmp(a.bytes.all(), b._bytes.all());
	}

	inline static isize
	strcmp(const String_Range& a, const String_Range& b)
	{
		return internal::_strcmp(a.bytes.all(), b.bytes);
	}

	inline static String_Range
	literal(const char* str)
	{
		usize str_size = std::strlen(str) + 1;
		String_Range result;
		result.bytes.ptr = (byte*)str;
		result.bytes.size = str_size;
		result._count = str_size - 1;
		return result;
	}
}