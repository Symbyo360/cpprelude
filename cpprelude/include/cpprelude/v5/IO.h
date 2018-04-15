#pragma once

#include "cpprelude/v5/IO_Trait.h"
#include "cpprelude/v5/Ranges.h"
#include "cpprelude/v5/Result.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace cpprelude
{
	//print binary data
	namespace internal
	{
		inline static void
		_variadic_print_binary_helper(IO_Trait* trait, usize& size)
		{
			return;
		}

		template<typename TFirst, typename ... TArgs>
		inline static void
		_variadic_print_binary_helper(IO_Trait *trait,
									  usize& size,
									  TFirst&& first_arg,
									  TArgs&& ... args)
		{
			size += print_bin(trait, std::forward<TFirst>(first_arg));
			_variadic_print_binary_helper(trait, size, std::forward<TArgs>(args)...);
		}

		struct Parsed_Format
		{
			enum ALIGN
			{
				ALIGN_NONE,		//invalid alignment
				ALIGN_LEFT,		//< left aligns the value
				ALIGN_RIGHT,	//> right aligns the value
				ALIGN_CENTER,	//^ center aligns the value
				ALIGN_EQUAL		//= used with numbers only,
								//it's used for printing values like +000120.
			};

			enum SIGN
			{
				SIGN_NONE,		//invalid sign
				SIGN_POSITIVE,	//always print signs before the number
				SIGN_NEGATIVE,	//print only negative signs
				SIGN_SPACE		//print one space for the positive numbers
			};

			enum TYPE
			{
				TYPE_NONE,			//invalid type
				TYPE_BINARY,		//binary type
				TYPE_RUNE,			//rune type
				TYPE_DECIMAL,		//decimal integer
				TYPE_OCTAL,			//octal integer
				TYPE_HEX_SMALL, 	//hex integer with small case letters
				TYPE_HEX_CAPITAL,	//hex integer with capital case letters
				//float only types
				TYPE_EXP_SMALL, 		//exponent format with small case e
				TYPE_EXP_CAPITAL,		//exponent format with capital case E
				TYPE_FLOAT,				//floating point number default percision is 6
				TYPE_GENERAL_SMALL,		//converts floating-point number to decimal or decimal
										//exponent notation depending on the value and the precision
				TYPE_GENERAL_CAPITAL	//converts floating-point number to decimal or decimal
										//exponent notation depending on the value and the precision
			};

			usize index;
			ALIGN alignment;
			Rune  pad;
			SIGN  sign;
			bool  prefix;
			usize width;
			usize precision;
			TYPE type;

			Parsed_Format()
				:index(0),
				 alignment(ALIGN_NONE),
				 pad(' '),
				 sign(SIGN_NONE),
				 prefix(false),
				 width(0),
				 precision(0),
				 type(TYPE_NONE)
			{}
		};

		template<typename T>
		inline static usize
		_generic_print_str_function(void* _self, const Parsed_Format& format, IO_Trait* trait)
		{
			T* value = (T*)_self;
			return print_str(trait, format, *value);
		}

		struct Generic_Print_Str_Value
		{
			using print_func = usize(*)(void*, const Parsed_Format&, IO_Trait*);
			void* _self;
			print_func _print;

			template<typename T>
			Generic_Print_Str_Value(const T& value)
			{
				_self = (void*)(&value);
				_print = _generic_print_str_function<T>;
			}

			usize
			print(IO_Trait* trait, const Parsed_Format& format)
			{
				return _print(_self, format, trait);
			}
		};

		inline static void
		_variadic_print_string_helper(IO_Trait* trait, usize& size)
		{
			return;
		}

		template<typename TFirst, typename ... TArgs>
		inline static void
		_variadic_print_string_helper(IO_Trait *trait,
									  usize& size,
									  TFirst&& first_arg,
									  TArgs&& ... args)
		{
			size += print_str(trait, Parsed_Format(), std::forward<TFirst>(first_arg));
			_variadic_print_string_helper(trait, size, std::forward<TArgs>(args)...);
		}

		inline static bool
		_is_digit(Rune c)
		{
			return (c == '0' ||
					c == '1' ||
					c == '2' ||
					c == '3' ||
					c == '4' ||
					c == '5' ||
					c == '6' ||
					c == '7' ||
					c == '8' ||
					c == '9');
		}

		inline static bool
		_is_type(Rune c)
		{
			return (c == 'b' ||
					c == 'c' ||
					c == 'd' ||
					c == 'o' ||
					c == 'x' ||
					c == 'X' ||
					c == 'e' ||
					c == 'E' ||
					c == 'f' ||
					c == 'g' ||
					c == 'G');
		}

		inline static Result<usize, bool>
		_parse_value_index(String_Iterator& it, const String_Iterator& end)
		{
			bool error = true;
			usize result = 0;

			auto& ptr = it.ptr;
			while(ptr != end.ptr)
			{
				auto c = *ptr;
				if(!_is_digit(c))
				{
					return { result, error };
				}
				else
				{
					result *= 10;
					result += c - '0';
					error = false;
				}
				++ptr;
			}

			return { result, error };
		}

		inline static void
		_parse_format(String_Iterator& it,
					  const String_Iterator& end,
					  Parsed_Format& format,
					  bool& manual_indexing)
		{
			Rune c = *it;
			//user defined index
			if (_is_digit(c))
			{
				auto value_index = _parse_value_index(it, end);
				//if there's an error
				if (value_index.error == true)
				{
					//TODO(Moustapha): panic here
					return;
				}
				format.index = value_index.value;
				manual_indexing = true;
			}
			else
			{
				if(manual_indexing)
				{
					//TODO(Moustapha): panic here
					return;
				}
			}

			//check the colon
			c = *it;
			if (c != ':')
				return;
			++it;

			auto after_colon = it;
			++it;

			c = *it;
			//check the alignment
			if (c == '<')
			{
				format.alignment = Parsed_Format::ALIGN_LEFT;
				format.pad = *after_colon;
				++it;
			}
			else if (c == '>')
			{
				format.alignment = Parsed_Format::ALIGN_RIGHT;
				format.pad = *after_colon;
				++it;
			}
			else if (c == '^')
			{
				format.alignment = Parsed_Format::ALIGN_CENTER;
				format.pad = *after_colon;
				++it;
			}
			else
			{
				it = after_colon;
				c = *it;
				if (c == '<')
				{
					format.alignment = Parsed_Format::ALIGN_LEFT;
					++it;
				}
				else if (c == '>')
				{
					format.alignment = Parsed_Format::ALIGN_RIGHT;
					++it;
				}
				else if (c == '^')
				{
					format.alignment = Parsed_Format::ALIGN_CENTER;
					++it;
				}
			}

			c = *it;
			//check the sign
			if (c == '+')
			{
				format.sign = Parsed_Format::SIGN_POSITIVE;
				++it;
			}
			else if (c == '-')
			{
				format.sign = Parsed_Format::SIGN_NEGATIVE;
				++it;
			}
			else if (c == ' ')
			{
				format.sign = Parsed_Format::SIGN_SPACE;
				++it;
			}

			c = *it;
			//check the #
			if (c == '#')
			{
				format.prefix = true;
				++it;
			}

			c = *it;
			//check the width
			if (_is_digit(c))
			{
				auto value_width = _parse_value_index(it, end);
				//if there's an error
				if (value_width.error == true)
				{
					//TODO(Moustapha): panic here
					return;
				}
				format.width = value_width.value;
			}

			c = *it;
			//check the precision
			if (c == '.')
			{
				++it;
				auto value_precision = _parse_value_index(it, end);
				//if there's an error
				if (value_precision.error == true)
				{
					//TODO(Moustapha): panic here
					return;
				}
				format.precision = value_precision.value;
			}

			c = *it;
			if (c == 'c')
			{
				format.type = internal::Parsed_Format::TYPE_RUNE;
				++it;
			}
			else if (c == 'd')
			{
				format.type = internal::Parsed_Format::TYPE_DECIMAL;
				++it;
			}
			else if (c == 'o')
			{
				format.type = internal::Parsed_Format::TYPE_OCTAL;
				++it;
			}
			else if (c == 'x')
			{
				format.type = internal::Parsed_Format::TYPE_HEX_SMALL;
				++it;
			}
			else if (c == 'X')
			{
				format.type = internal::Parsed_Format::TYPE_HEX_CAPITAL;
				++it;
			}
			else if (c == 'e')
			{
				format.type = internal::Parsed_Format::TYPE_EXP_SMALL;
				++it;
			}
			else if (c == 'E')
			{
				format.type = internal::Parsed_Format::TYPE_EXP_CAPITAL;
				++it;
			}
			else if (c == 'f')
			{
				format.type = internal::Parsed_Format::TYPE_FLOAT;
				++it;
			}
			else if (c == 'g')
			{
				format.type = internal::Parsed_Format::TYPE_GENERAL_SMALL;
				++it;
			}
			else if (c == 'G')
			{
				format.type = internal::Parsed_Format::TYPE_GENERAL_CAPITAL;
				++it;
			}

			c = *it;
			//check the end brace
			if(c != '}')
			{
				//TODO(Moustapha): panic here
				return;
			}

			++it;
			return;
		}
	}

	template<typename T>
	inline static usize
	print_bin(IO_Trait* trait, const T& value)
	{
		return trait->write(Slice<byte>((byte*)&value, sizeof(T)));
	}

	template<typename T>
	inline static usize
	print_bin(IO_Trait* trait, const Slice<T>& values)
	{
		return trait->write(values.template convert<byte>());
	}

	template<typename ... TArgs>
	inline static usize
	vprintb(IO_Trait *trait, TArgs&& ... args)
	{
		usize result = 0;
		internal::_variadic_print_binary_helper(trait, result, std::forward<TArgs>(args)...);
		return result;
	}



	inline static usize
	print_str(IO_Trait* trait, const internal::Parsed_Format& _format, byte value)
	{
		internal::Parsed_Format format = _format;

		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case internal::Parsed_Format::TYPE_DECIMAL:
				PATTERN = "%hhd";
				break;
			case internal::Parsed_Format::TYPE_HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hhx";
				else
					PATTERN = "%hhx";
				break;
			case internal::Parsed_Format::TYPE_HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hhX";
				else
					PATTERN = "%hhX";
				break;
			case internal::Parsed_Format::TYPE_OCTAL:
				if(format.prefix)
					PATTERN = "%#hho";
				else
					PATTERN = "%hho";
				break;
			case internal::Parsed_Format::TYPE_RUNE:
			case internal::Parsed_Format::TYPE_NONE:
			default:
				PATTERN = "%c";
				break;
		}

		usize result = 0;

		//second respect the sign
		if (value >= 0 &&
			(format.type != internal::Parsed_Format::TYPE_RUNE ||
			 format.type != internal::Parsed_Format::TYPE_NONE) &&
			format.sign == internal::Parsed_Format::SIGN_POSITIVE)
		{
			char s = '+';
			result += trait->write(Slice<byte>((byte*)&s, 1));
		}
		else if(value >= 0 &&
				(format.type != internal::Parsed_Format::TYPE_RUNE ||
			 	 format.type != internal::Parsed_Format::TYPE_NONE) &&
				format.sign == internal::Parsed_Format::SIGN_SPACE)
		{
			char s = ' ';
			result += trait->write(Slice<byte>((byte*)&s, 1));
		}

		//third render the value and calc it's written size
		constexpr usize BUFFER_SIZE = 6;
		byte buffer[BUFFER_SIZE];
		int written_size = std::snprintf(buffer, BUFFER_SIZE, PATTERN, value);
		if(written_size < 0) return 0;

		//fourth respect right and center align
		usize pad_size = 0, pad_str_size = 0;
		if(format.width > (written_size + result))
		{
			pad_size = format.width - (written_size + result);
			pad_str_size = std::strlen((char*)&format.pad.data);
			//default alignment is LEFT
			if (format.alignment == internal::Parsed_Format::ALIGN_NONE)
				format.alignment = internal::Parsed_Format::ALIGN_LEFT;
		}

		if (pad_size != 0 &&
			format.alignment == internal::Parsed_Format::ALIGN_RIGHT)
		{
			for(usize i = 0; i < pad_size; ++i)
				result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
		}
		else if(pad_size != 0 &&
				format.alignment == internal::Parsed_Format::ALIGN_CENTER)
		{
			usize i;
			for(i = 0; i < pad_size / 2; ++i)
				result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
			pad_size -= i;
		}

		//fifth write the rendered value
		result += trait->write(Slice<byte>(buffer, written_size));

		//sixth respect left and center align
		if (pad_size != 0 &&
			format.alignment == internal::Parsed_Format::ALIGN_CENTER)
		{
			for(usize i = 0; i < pad_size; ++i)
				result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
		}
		else if(pad_size != 0 &&
				format.alignment == internal::Parsed_Format::ALIGN_LEFT)
		{
			for(usize i = 0; i < pad_size; ++i)
				result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
		}

		return result;
	}

	#define DEFINE_PRINT_STR(TYPE, PATTERN, BUFFER_SIZE)\
	inline static usize\
	print_str(IO_Trait *trait, TYPE value)\
	{\
		byte buffer[BUFFER_SIZE];\
		int written_size = std::snprintf(buffer, BUFFER_SIZE, PATTERN, value);\
		if(written_size < 0) return 0;\
		return trait->write(Slice<byte>(buffer, written_size));\
	}

	DEFINE_PRINT_STR(byte, "%c", 6)
	DEFINE_PRINT_STR(i8, "%hhd", 6)
	DEFINE_PRINT_STR(u8, "%hhu", 6)
	DEFINE_PRINT_STR(i16, "%hd", 10)
	DEFINE_PRINT_STR(u16, "%hu", 10)
	DEFINE_PRINT_STR(i32, "%d", 20)
	DEFINE_PRINT_STR(u32, "%u", 20)

	//long in linux land is 8 byte howeven in windows universe it's 4 byte so we do this
	#if defined(OS_WINDOWS)

	DEFINE_PRINT_STR(i64, "%lld", 40)
	DEFINE_PRINT_STR(u64, "%llu", 40)

	#elif defined(OS_LINUX)

	DEFINE_PRINT_STR(i64, "%ld", 40)
	DEFINE_PRINT_STR(u64, "%lu", 40)

	#endif

	DEFINE_PRINT_STR(r32, "%g", 128)
	DEFINE_PRINT_STR(r64, "%.12f", 256)

	#undef DEFINE_PRINT_STR

	//print pointer as string
	inline static usize
	print_str(IO_Trait *trait, void* ptr)
	{
		usize value = reinterpret_cast<usize>(ptr);
		byte buffer[80];
		int written_size = std::snprintf(buffer, 80, "0x%" PRIXPTR, value);
		if(written_size < 0) return 0;
		return trait->write(Slice<byte>(buffer, written_size));
	}

	//print string literal
	inline static usize
	print_str(IO_Trait* trait, const String_Range& str)
	{
		if(str.bytes.back() == 0)
			return trait->write(str.bytes.range(0, str.bytes.size - 1).template convert<byte>());
		else
			return trait->write(str.bytes.template convert<byte>());
	}

	inline static usize
	print_str(IO_Trait *trait, const char* str)
	{
		usize str_size = std::strlen(str);
		return trait->write(Slice<byte>((byte*)str, str_size));
	}

	template<typename ... TArgs>
	inline static usize
	vprints(IO_Trait* trait, TArgs&& ... args)
	{
		usize result = 0;
		internal::_variadic_print_string_helper(trait, result, std::forward<TArgs>(args)...);
		return result;
	}

	inline static usize
	vprintf(IO_Trait* trait, const String_Range& format)
	{
		return 0;
	}

	template<typename ... TArgs>
	constexpr inline static usize
	vprintf(IO_Trait* trait, const String_Range& format, TArgs&& ... args)
	{
		usize result = 0;
		internal::Generic_Print_Str_Value values[] = { args... };
		//size of the variadic template
		usize values_count = sizeof...(args);
		//default index for the {default_index}
		usize default_index = 0;
		//switched to manual indexing
		bool manual_indexing = false;

		//we go forward until we encounter a { then we write
		//the chunk defined by [rune_back_it, rune_forward_it)
		auto rune_forward_it = format.begin();
		auto rune_back_it = rune_forward_it;
		auto rune_end = format.end();

		//loop through the format string
		while(rune_forward_it != rune_end)
		{
			Rune c = *rune_forward_it;

			//if this is a { then this maybe an argument
			if(c == '{')
			{
				//if there's some chunk to be written then we write it
				if(rune_back_it.ptr < rune_forward_it.ptr)
				{
					result += trait->write(
								Slice<byte>((byte*)rune_back_it.ptr,
											rune_forward_it.ptr - rune_back_it.ptr));
					//after writing the chunk we set the back iterator to the forward iterator
					rune_back_it = rune_forward_it;
				}

				//check if this is also a { thus detecting the pattern {{
				//this will print only a {
				++rune_forward_it;
				//if this is the end of the string then we encountered an error
				if(rune_forward_it == rune_end)
				{
					//TODO(Moustapha): panic here
					return result;
				}

				c = *rune_forward_it;
				//if we detected the {{ then write only one { and continue
				if(c == '{')
				{
					char open_brace = '{';
					result += trait->write(Slice<byte>((byte*)&open_brace, 1));

					//go to the next rune and reset the back it
					++rune_forward_it;
					rune_back_it = rune_forward_it;
					//continue the upper loop
					continue;
				}

				internal::Parsed_Format format;
				if(!manual_indexing)
					format.index = default_index++;
				internal::_parse_format(rune_forward_it, rune_end, format, manual_indexing);

				//print the value
				if(format.index >= values_count)
				{
					//TODO(Moustapha): panic here
					return result;
				}

				//actual call to the print_str
				result += values[format.index].print(trait, format);

				rune_back_it = rune_forward_it;
			}
			//not a special character to take it into account
			else
			{
				++rune_forward_it;
			}
		}

		//if there's some remaining text we flush it and exit
		if(rune_back_it.ptr < rune_forward_it.ptr)
		{
			//this is adjustment size is for the null termination
			usize adjustment_size = 0;
			//if null terminated then add one to the adjustment_size
			//to be subtracted later from the byte range to print
			if(*rune_forward_it.ptr == 0)
				++adjustment_size;
			result += trait->write(
						Slice<byte>((byte*)rune_back_it.ptr,
									(rune_forward_it.ptr - rune_back_it.ptr) - adjustment_size));
		}

		return result;
	}
}