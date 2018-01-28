#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/memory.h"
#include "cpprelude/platform.h"
#include "cpprelude/io.h"
#include "cpprelude/bufio.h"

namespace cpprelude
{
	inline static usize
	_strlen(const byte *it, usize limit = -1)
	{
		if (limit == 0 || it == nullptr)
			return 0;

		usize result = 0;
		while(*it++ != 0 && result < limit)
			++result;
		return result;
	}

	struct rune
	{
		u32 data;

		rune()
			:data(0)
		{}

		rune(u32 c)
			:data(c)
		{}

		bool
		operator==(const rune& other) const
		{
			return data == other.data;
		}

		bool
		operator!=(const rune& other) const
		{
			return data != other.data;
		}

		bool
		operator<(const rune& other) const
		{
			return data < other.data;
		}

		bool
		operator>(const rune& other) const
		{
			return data > other.data;
		}

		bool
		operator<=(const rune& other) const
		{
			return data <= other.data;
		}

		bool
		operator>=(const rune& other) const
		{
			return data >= other.data;
		}
	};

	inline static usize
	print_str(io_trait *trait, const rune &c)
	{
		byte *ptr = (byte*)(&c);
		usize size = _strlen(ptr);
		return trait->write(make_slice<byte>(ptr, size));
	}

	inline static usize
	print_str(bufout_trait *trait, const rune &c)
	{
		byte *ptr = (byte*)(&c);
		usize size = _strlen(ptr);
		return trait->write(make_slice<byte>(ptr, size));
	}

	struct rune_iterator
	{
		const byte *_ptr = nullptr;

		rune_iterator(const byte* ptr)
			:_ptr(ptr)
		{}

		rune_iterator&
		operator++()
		{
			i8 data = *_ptr;
			if(data == 0)
				return *this;

			++_ptr;
			while(*_ptr)
			{
				if((*_ptr & 0xC0) == 0x80)
					++_ptr;
				else
					break;
			}
			return *this;
		}

		rune_iterator
		operator++(int)
		{
			if(*_ptr == 0)
				return *this;
			
			auto result = *this;
			++_ptr;
			while(*_ptr)
			{
				if((*_ptr & 0xC0) == 0x80)
					++_ptr;
				else
					break;
			}
			return result;
		}

		rune_iterator&
		operator--()
		{
			--_ptr;
			while(*_ptr)
			{
				if((*_ptr & 0xC0) == 0x80)
					--_ptr;
				else
					break;
			}
			return *this;
		}

		rune_iterator
		operator--(int)
		{
			auto result = *this;
			--_ptr;
			while(*_ptr)
			{
				if((*_ptr & 0xC0) == 0x80)
					--_ptr;
				else
					break;
			}
			return result;
		}

		bool
		operator==(const rune_iterator& other) const
		{
			return _ptr == other._ptr;
		}

		bool
		operator!=(const rune_iterator& other) const
		{
			return _ptr != other._ptr;
		}

		rune
		operator*() const
		{
			if (_ptr == nullptr || *_ptr == 0)
				return rune(0);

			const byte* ptr = _ptr;

			// byte result[sizeof(rune)] = {0};
			rune result_rune = 0;
			byte* result = (byte*)&result_rune.data;
			u8 ix = 0;
			result[ix] = *ptr;

			++ix;
			++ptr;
			while(*ptr)
			{
				if ((*ptr & 0xC0) == 0x80)
					result[ix] = *ptr;
				else
					break;
				++ptr;
				++ix;
			}

			return result_rune;
		}

		operator const byte*() const
		{
			return _ptr;
		}
	};

	struct string
	{
		using iterator = rune_iterator;
		using const_iterator = rune_iterator;

		slice<byte> _data;
		mutable usize _count = static_cast<usize>(-1);
		memory_context* _context = platform->global_memory;

		API_CPPR string();
		API_CPPR string(memory_context* context);
		API_CPPR explicit string(usize size, memory_context* context = platform->global_memory);
		API_CPPR string(const char* data, memory_context* context = platform->global_memory);
		API_CPPR string(const slice<byte>& data, memory_context* context = platform->global_memory);
		API_CPPR string(slice<byte>&& data, memory_context* context = platform->global_memory);

		API_CPPR string(const string&);
		API_CPPR string(const string&, memory_context* context);

		API_CPPR string&
		operator=(const string&);

		API_CPPR string&
		operator=(const char* data);

		API_CPPR string(string&&);
		API_CPPR string(string&&, memory_context* context);

		API_CPPR string&
		operator=(string&&);

		API_CPPR ~string();

		API_CPPR usize
		count() const;

		API_CPPR usize
		size() const;

		API_CPPR usize
		capacity() const;

		API_CPPR bool
		empty() const;

		API_CPPR const byte*
		data() const;

		API_CPPR byte
		operator[](usize index) const;

		API_CPPR bool
		operator==(const string& str) const;

		API_CPPR bool
		operator!=(const string& str) const;

		API_CPPR bool
		operator<(const string& str) const;

		API_CPPR bool
		operator>(const string& str) const;

		API_CPPR bool
		operator<=(const string& str) const;

		API_CPPR bool
		operator>=(const string& str) const;

		API_CPPR const_iterator
		begin() const;

		API_CPPR const_iterator
		cbegin() const;

		API_CPPR const_iterator
		end() const;

		API_CPPR const_iterator
		cend() const;

		API_CPPR void
		concat(const string& other);

		API_CPPR string
		substr(usize start, usize size, memory_context *context = nullptr);

		API_CPPR string
		substr(rune_iterator begin, rune_iterator end, memory_context *context = nullptr);

		API_CPPR string
		view(usize start, usize size) const;

		API_CPPR string
		view(rune_iterator begin, rune_iterator end) const;

		API_CPPR bool
		is_view() const;

		API_CPPR slice<byte>
		decay();

		API_CPPR slice<byte>
		decay_continuous();
	};

	API_CPPR cpprelude::string
	operator"" _cs(const char* str, usize str_count);

	inline static usize
	print_bin(io_trait *trait, const cpprelude::string& str)
	{
		//remove the last null from the string when printing it
		byte *ptr = str._data.ptr;
		usize size = _strlen(ptr, str._data.size);

		return trait->write(make_slice<byte>(ptr, size));
	}

	inline static usize
	print_bin(bufout_trait *trait, const cpprelude::string& str)
	{
		//remove the last null from the string when printing it
		byte *ptr = str._data.ptr;
		usize size = _strlen(ptr, str._data.size);

		return trait->write(make_slice<byte>(ptr, size));
	}


	inline static usize
	print_bin(io_trait *trait, const char* str)
	{
		return trait->write(make_slice((byte*)str, _strlen(str)));
	}

	inline static usize
	print_bin(bufout_trait *trait, const char* str)
	{
		return trait->write(make_slice((byte*)str, _strlen(str)));
	}


	inline static usize
	print_str(io_trait *trait, const cpprelude::string& str)
	{
		//remove the last null from the string when printing it
		byte *ptr = str._data.ptr;
		usize size = _strlen(ptr, str._data.size);

		return trait->write(make_slice<byte>(ptr, size));
	}

	inline static usize
	print_str(bufout_trait *trait, const cpprelude::string& str)
	{
		//remove the last null from the string when printing it
		byte *ptr = str._data.ptr;
		usize size = _strlen(ptr, str._data.size);

		return trait->write(make_slice<byte>(ptr, size));
	}


	inline static usize
	print_str(io_trait *trait, const char* str)
	{
		return trait->write(make_slice((byte*)str, _strlen(str)));
	}

	inline static usize
	print_str(bufout_trait *trait, const char* str)
	{
		return trait->write(make_slice((byte*)str, _strlen(str)));
	}


	inline static usize
	scan_bin(io_trait *trait, cpprelude::string& str)
	{
		auto result = trait->read(str._data.view_bytes(0, str._data.size - 1));
		str._data[result] = 0;
		return result ;
	}

	inline static usize
	scan_bin(bufin_trait *trait, cpprelude::string& str)
	{
		auto result = trait->read(str._data.view_bytes(0, str._data.size - 1));
		str._data[result] = 0;
		return result ;
	}


	inline static usize
	scan_bin(io_trait *trait, cpprelude::string&& str)
	{
		return scan_bin(trait, str);
	}

	inline static usize
	scan_bin(bufin_trait *trait, cpprelude::string&& str)
	{
		return scan_bin(trait, str);
	}

	inline static usize
	scan_str(io_trait *trait, cpprelude::string& str)
	{
		auto result = trait->read(str._data.view_bytes(0, str._data.size - 1));
		str._data[result] = 0;
		return result;
	}

	inline static usize
	scan_str(io_trait *trait, cpprelude::string&& str)
	{
		return scan_str(trait, str);
	}
}
