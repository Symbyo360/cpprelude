#include "cpprelude/string.h"
#include "cpprelude/fmt.h"
#include "cpprelude/memory.h"
#include <cstring>
#include <cstdarg>

namespace cpprelude
{
	inline usize
	_count_runes(const byte* data)
	{
		usize result = 0;
		if(data)
			while(*data) result += (*data++ & 0xC0) != 0x80;
		return result;
	}

	inline usize
	_count_runes_limit(const byte* data, usize limit)
	{
		usize result = 0;
		if(data && limit)
			while(*data && --limit) result += (*data++ & 0xC0) != 0x80;
		return result;
	}

	inline usize
	_count_runes_to(const byte* data, usize to)
	{
		auto old_data = data;
		usize count = 0;
		usize result = 0;
		if(data)
		{
			while(true)
			{
				count += (*data++ & 0xC0) != 0x80;

				if(count == to + 1)
				{
					result = (data - old_data) - 1;
					break;
				}

				if (*data == 0)
				{
					result = (data - old_data);
					break;
				}
			}
		}
		return result;
	}

	inline isize
	_strcmp(const string& a, const string& b)
	{
		auto a_it = a.begin();
		auto b_it = b.begin();

		while(a_it != a.end() && b_it != b.end())
		{
			if(*a_it != *b_it)
				break;

			++a_it;
			++b_it;
		}

		if (a_it == a.end() && b_it == b.end())
			return 0;

		auto last_a = *a_it;
		auto last_b = *b_it;

		if(last_a > last_b)
			return 1;
		else if(last_a < last_b)
			return -1;
		else
			return 0;
	}

	string::string()
	{}

	string::string(memory_context* context)
		:_context(context)
	{}

	string::string(usize size, memory_context* context)
		:_context(context)
	{
		_data = _context->template alloc<byte>(size);
		_data[0U] = 0;
	}

	string::string(const char* data, memory_context* context)
	{
		_context = context;

		usize data_size = strlen(data);

		_data = _context->template alloc<byte>(data_size + 1);
		memcpy(_data.ptr, data, data_size);

		_data[data_size] = 0;
	}

	string::string(const slice<byte>& data, memory_context* context)
	{
		_context = context;

		_data = _context->template alloc<byte>(data.count());

		copy_slice(_data, data);
	}

	string::string(slice<byte>&& data, memory_context* context)
	{
		_context = context;
		_data = std::move(data);
	}

	string::string(const string& other)
	{
		//if the context is nullptr it means that it's a constant string
		if(other._context)
		{
			_context = other._context;

			_data = _context->template alloc<byte>(other._data.size);

			copy_slice(_data, other._data);

			_count = other._count;
		}
		else
		{
			_data = other._data;
			_count = other._count;
			_context = other._context;
		}
	}

	string::string(const string& other, memory_context* context)
	{
		_context = context;

		_data = _context->template alloc<byte>(other._data.size);

		copy_slice(_data, other._data);

		_count = other._count;
	}	

	string&
	string::operator=(const string& other)
	{
		if (other._context)
		{
			_context = other._context;
			_context->template realloc<byte>(_data, other._data.size);
			copy_slice(_data, other._data);
			_count = other._count;
		}
		else
		{
			_context = other._context;
			_data = other._data;
			_count = other._count;
		}
		return *this;
	}

	string&
	string::operator=(const char* data)
	{
		usize data_size = strlen(data);

		_context->template realloc<byte>(_data, data_size + 1);
		memcpy(_data.ptr, data, data_size);

		_data[data_size] = 0;

		_count = static_cast<usize>(-1);

		return *this;
	}

	string::string(string&& other)
	{
		_data = std::move(other._data);
		_count = other._count;
		_context = other._context;

		other._count = static_cast<usize>(-1);
		other._context = nullptr;
	}

	string::string(string&& other, memory_context* context)
	{
		_data = std::move(other._data);
		_count = other._count;
		_context = context;

		other._count = static_cast<usize>(-1);
		other._context = nullptr;
	}

	string&
	string::operator=(string&& other)
	{
		_context = other._context;
		_data = std::move(other._data);
		_count = other._count;

		other._context = nullptr;
		other._count = static_cast<usize>(-1);
		return *this;
	}

	string::~string()
	{
		if(_data.valid() && _context)
			_context->template free<byte>(_data);
		_count = static_cast<usize>(-1);
		_context = nullptr;
	}

	usize
	string::count() const
	{
		if(_count == static_cast<usize>(-1))
			_count = _count_runes_limit(_data.ptr, _data.size);
		return _count;
	}

	usize
	string::size() const
	{
		return _data.size;
	}

	usize
	string::capacity() const
	{
		return _data.size;
	}

	bool
	string::empty() const
	{
		return count() == 0;
	}

	const byte*
	string::data() const
	{
		return _data.ptr;
	}

	byte
	string::operator[](usize index) const
	{
		return _data[index];
	}

	bool
	string::operator==(const string& str) const
	{
		return _strcmp(*this, str) == 0;
	}

	bool
	string::operator!=(const string& str) const
	{
		return _strcmp(*this, str) != 0;
	}

	bool
	string::operator<(const string& str) const
	{
		return _strcmp(*this, str) < 0;
	}

	bool
	string::operator>(const string& str) const
	{
		return _strcmp(*this, str) > 0;
	}

	bool
	string::operator<=(const string& str) const
	{
		return _strcmp(*this, str) <= 0;
	}

	bool
	string::operator>=(const string& str) const
	{
		return _strcmp(*this, str) >= 0;
	}

	string::const_iterator
	string::begin() const
	{
		return const_iterator(_data.ptr);
	}

	string::const_iterator
	string::cbegin() const
	{
		return const_iterator(_data.ptr);
	}

	string::const_iterator
	string::end() const
	{
		if (_data.ptr == nullptr)
			return const_iterator(nullptr);
		return const_iterator(_data.ptr + _data.size - 1);
	}

	string::const_iterator
	string::cend() const
	{
		if (_data.ptr == nullptr)
			return const_iterator(nullptr);
		return const_iterator(_data.ptr + _data.size - 1);
	}

	void
	string::concat(const string& other)
	{
		if (other._data.size == 0)
			return;
		usize old_size = _data.size;
		//-1 for the two null termination added
		if (_data.size > 0)
			--old_size;
		_context->template realloc<byte>(_data, old_size + other._data.size);
		copy_slice(_data.view_bytes(old_size, _data.size - old_size), other._data, other._data.size);
		_data[_data.size - 1] = 0;
		_count = static_cast<usize>(-1);
	}

	string
	string::substr(usize start, usize size, memory_context *context)
	{
		if(context == nullptr)
			context = _context;

		usize start_byte_index = _count_runes_to(_data, start);
		//it's the index starting from the offsetted ptr so essentially it's a size
		usize end_byte_size = _count_runes_to(_data.ptr + start_byte_index, size);
		auto subslice = _data.view(start_byte_index, end_byte_size);
		string result(subslice.size + 1, context);
		copy_slice(result._data, subslice, subslice.size);
		result._data[subslice.size] = 0;
		return result;
	}

	string
	string::substr(rune_iterator begin, rune_iterator end, memory_context *context)
	{
		if(context == nullptr)
			context = _context;

		usize start_byte_index = begin._ptr - _data.ptr;
		usize end_byte_size = end._ptr - begin._ptr;
		auto subslice = _data.view(start_byte_index, end_byte_size);
		string result(subslice.size + 1, context);
		copy_slice(result._data, subslice, subslice.size);
		result._data[subslice.size] = 0;
		return result;
	}

	string
	string::view(usize start, usize size) const
	{
		usize start_byte_index = _count_runes_to(_data, start);
		//it's the index starting from the offsetted ptr so essentially it's a size
		usize end_byte_size = _count_runes_to(_data.ptr + start_byte_index, size);
		//+1 for the imaginary 0
		auto subslice = _data.view(start_byte_index, end_byte_size + 1);
		return string(std::move(subslice), nullptr);
	}

	string
	string::view(rune_iterator begin, rune_iterator end) const
	{
		usize start_byte_index = begin._ptr - _data.ptr;
		usize end_byte_size = end._ptr - begin._ptr;
		auto subslice = _data.view(start_byte_index, end_byte_size + 1);
		return string(std::move(subslice), nullptr);
	}

	bool
	string::is_view() const
	{
		return _context == nullptr;
	}

	slice<byte>
	string::decay()
	{
		_count = static_cast<usize>(-1);
		return std::move(_data);
	}

	slice<byte>
	string::decay_continuous()
	{
		return decay();
	}

	string
	operator"" _cs(const char* str, usize str_count)
	{
		string result;
		//the +1 for the 0 null at the end
		result._data = make_slice<byte>((byte*)(str), str_count + 1);
		result._context = nullptr;
		result._count = _count_runes(result._data.ptr);
		return result;
	}
}