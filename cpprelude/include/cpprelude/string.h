#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/memory.h"
#include "cpprelude/platform.h"
#include <ostream>

namespace cpprelude
{
	struct rune
	{
		u32 data;

		API_CPPR bool
		operator==(const rune& other) const;

		API_CPPR bool
		operator!=(const rune& other) const;

		API_CPPR bool
		operator<(const rune& other) const;

		API_CPPR bool
		operator>(const rune& other) const;

		API_CPPR bool
		operator<=(const rune& other) const;

		API_CPPR bool
		operator>=(const rune& other) const;
	};

	inline static std::ostream&
	operator<<(std::ostream& out,const rune& c)
	{
		out << reinterpret_cast<const char*>(&c.data);
		return out;
	}

	inline static std::wostream&
	operator<<(std::wostream& out, const rune& c)
	{
		out << reinterpret_cast<const char*>(&c.data);
		return out;
	}

	struct rune_iterator
	{
		const byte *_ptr = nullptr;

		API_CPPR rune_iterator(const byte* ptr);

		API_CPPR rune_iterator&
		operator++();

		API_CPPR rune_iterator
		operator++(int);

		API_CPPR rune_iterator&
		operator--();

		API_CPPR rune_iterator
		operator--(int);

		API_CPPR bool
		operator==(const rune_iterator& other) const;

		API_CPPR bool
		operator!=(const rune_iterator& other) const;

		API_CPPR rune
		operator*() const;

		API_CPPR operator const byte*() const;
	};

	struct string
	{
		using iterator = rune_iterator;
		using const_iterator = rune_iterator;

		slice<byte> _data;
		mutable usize _count = static_cast<usize>(-1);
		memory_context* _context = platform.global_memory;

		API_CPPR string();
		API_CPPR string(memory_context* context);
		API_CPPR explicit string(usize size, memory_context* context = platform.global_memory);
		API_CPPR string(const char* data, memory_context* context = platform.global_memory);
		API_CPPR string(const slice<byte>& data, memory_context* context = platform.global_memory);
		API_CPPR string(slice<byte>&& data, memory_context* context = platform.global_memory);

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
	};

	API_CPPR cpprelude::string
	operator"" _cs(const char* str, usize str_count);

	inline static std::ostream&
	operator<<(std::ostream& out,const cpprelude::string& str)
	{
		out << reinterpret_cast<const char*>(str._data.ptr);
		return out;
	}

	inline static std::wostream&
	operator<<(std::wostream& out, const cpprelude::string& str)
	{
		out << reinterpret_cast<const char*>(str._data.ptr);
		return out;
	}
}
