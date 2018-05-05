#pragma once
#include "cpprelude/api.h"
#include "cpprelude/v5/IO_Trait.h"
#include "cpprelude/v5/Ranges.h"
#include "cpprelude/v5/Result.h"
#include "cpprelude/v5/OS.h"
#include <cstdio>
#include <cerrno>
#include <limits>
#include <cstdlib>

namespace cpprelude
{
	//forward declare the print to error stream functions for panicing purposes
	template<typename ... TArgs>
	inline static usize
	print_err(TArgs&& ... args);

	template<typename ... TArgs>
	inline static usize
	println_err(TArgs&& ... args);

	template<typename ... TArgs>
	inline static usize
	printf_err(const String_Range& format, TArgs&& ... args);

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
			precision(-1),
			type(TYPE_NONE)
		{}
	};


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
					println_err("[panic]: invalid index in vprintf statement");
					os->dump_callstack();
					std::abort();
				}
				format.index = value_index.value;
				manual_indexing = true;
			}
			else
			{
				if(manual_indexing)
				{
					println_err("[panic]: cannot mix manual indexing with automatic indexing in a vprintf statement");
					os->dump_callstack();
					std::abort();
				}
			}

			//check the colon
			c = *it;
			if (c != ':')
			{
				//there's no format specifiers
				//then we should end the format with a }
				if (c != '}')
				{
					println_err("[panic]: missing } in format specifiers in a vprintf statement");
					os->dump_callstack();
					std::abort();
				}
				//we found the } then we increment
				else
				{
					++it;
				}
				return;
			}
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
					println_err("[panic]: invalid width in vprintf statement");
					os->dump_callstack();
					std::abort();
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
					println_err("[panic]: invalid precision in vprintf statement");
					os->dump_callstack();
					std::abort();
				}
				format.precision = value_precision.value;
			}

			c = *it;
			if (c == 'c')
			{
				format.type = Parsed_Format::TYPE_RUNE;
				++it;
			}
			else if (c == 'd')
			{
				format.type = Parsed_Format::TYPE_DECIMAL;
				++it;
			}
			else if(c == 'b')
			{
				format.type = Parsed_Format::TYPE_BINARY;
				++it;
			}
			else if (c == 'o')
			{
				format.type = Parsed_Format::TYPE_OCTAL;
				++it;
			}
			else if (c == 'x')
			{
				format.type = Parsed_Format::TYPE_HEX_SMALL;
				++it;
			}
			else if (c == 'X')
			{
				format.type = Parsed_Format::TYPE_HEX_CAPITAL;
				++it;
			}
			else if (c == 'e')
			{
				format.type = Parsed_Format::TYPE_EXP_SMALL;
				++it;
			}
			else if (c == 'E')
			{
				format.type = Parsed_Format::TYPE_EXP_CAPITAL;
				++it;
			}
			else if (c == 'f')
			{
				format.type = Parsed_Format::TYPE_FLOAT;
				++it;
			}
			else if (c == 'g')
			{
				format.type = Parsed_Format::TYPE_GENERAL_SMALL;
				++it;
			}
			else if (c == 'G')
			{
				format.type = Parsed_Format::TYPE_GENERAL_CAPITAL;
				++it;
			}

			c = *it;
			//check the end brace
			if(c != '}')
			{
				println_err("[panic]: missing } in format specifiers in a vprintf statement");
				os->dump_callstack();
				std::abort();
			}

			++it;
			return;
		}

		template<typename T, usize BUFFER_SIZE>
		inline static usize
		_print_integer(IO_Trait* trait, const Parsed_Format& format, const char* pattern, T value)
		{
			usize result = 0;

			//respect the sign
			if (value >= 0 &&
				format.type == Parsed_Format::TYPE_DECIMAL &&
				format.sign == Parsed_Format::SIGN_POSITIVE)
			{
				char sign = '+';
				result += trait->write(Slice<byte>((byte*)&sign, 1));
			}
			else if(value >= 0 &&
					format.type == Parsed_Format::TYPE_DECIMAL &&
					format.sign == Parsed_Format::SIGN_SPACE)
			{
				char sign = ' ';
				result += trait->write(Slice<byte>((byte*)&sign, 1));
			}

			//render the value and calc it's written size
			byte buffer[BUFFER_SIZE];
			int written_size = std::snprintf(buffer, BUFFER_SIZE, pattern, value);
			if(written_size < 0)
				return 0;

			//respect right and center align
			usize pad_size = 0, pad_str_size = 0;
			Parsed_Format::ALIGN alignment = format.alignment;
			if(format.width > (written_size + result))
			{
				pad_size = format.width - (written_size + result);
				pad_str_size = std::strlen((char*)&format.pad.data);
				//default alignment is LEFt
				if(alignment == Parsed_Format::ALIGN_NONE)
					alignment = Parsed_Format::ALIGN_LEFT;
			}

			if (pad_size != 0 &&
				alignment == Parsed_Format::ALIGN_RIGHT)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
			}
			else if(pad_size != 0 &&
					alignment == Parsed_Format::ALIGN_CENTER)
			{
				usize i;
				for(i = 0; i < pad_size / 2; ++i)
					result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
				pad_size -= i;
			}

			//write the rendered value
			result += trait->write(Slice<byte>(buffer, written_size));

			//respect left and center align
			if (pad_size != 0 &&
				alignment == Parsed_Format::ALIGN_CENTER)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
			}
			else if(pad_size != 0 &&
					alignment == Parsed_Format::ALIGN_LEFT)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
			}

			return result;
		}

		template<typename T, usize BUFFER_SIZE, usize PERCISION>
		inline static usize
		_print_real(IO_Trait* trait, const Parsed_Format& _format, T value)
		{
			auto format = _format;

			//first respect the pattern
			const char* PATTERN = nullptr;
			switch(format.type)
			{
				case Parsed_Format::TYPE_EXP_SMALL:
					PATTERN = "%.*e";
					break;
				case Parsed_Format::TYPE_EXP_CAPITAL:
					PATTERN = "%.*E";
					break;
				case Parsed_Format::TYPE_FLOAT:
					PATTERN = "%.*f";
					break;
				case Parsed_Format::TYPE_GENERAL_SMALL:
					PATTERN = "%.*g";
					break;
				case Parsed_Format::TYPE_GENERAL_CAPITAL:
				default:
					PATTERN = "%.*G";
					break;
			}

			usize result = 0;

			//second respect the sign
			if (value >= 0 &&
				format.sign == Parsed_Format::SIGN_POSITIVE)
			{
				char s = '+';
				result += trait->write(Slice<byte>((byte*)&s, 1));
			}
			else if(value >= 0 &&
					format.sign == Parsed_Format::SIGN_SPACE)
			{
				char s = ' ';
				result += trait->write(Slice<byte>((byte*)&s, 1));
			}

			if(format.precision == -1)
				format.precision = PERCISION;

			//third render the value and calc it's written size
			byte buffer[BUFFER_SIZE];
			int written_size = std::snprintf(buffer, BUFFER_SIZE, PATTERN, format.precision, value);
			if(written_size < 0) return 0;

			//fourth respect right and center align
			usize pad_size = 0, pad_str_size = 0;
			if(format.width > (written_size + result))
			{
				pad_size = format.width - (written_size + result);
				pad_str_size = std::strlen((char*)&format.pad.data);
				//default alignment is LEFT
				if (format.alignment == Parsed_Format::ALIGN_NONE)
					format.alignment = Parsed_Format::ALIGN_LEFT;
			}

			if (pad_size != 0 &&
				format.alignment == Parsed_Format::ALIGN_RIGHT)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
			}
			else if(pad_size != 0 &&
					format.alignment == Parsed_Format::ALIGN_CENTER)
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
				format.alignment == Parsed_Format::ALIGN_CENTER)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
			}
			else if(pad_size != 0 &&
					format.alignment == Parsed_Format::ALIGN_LEFT)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
			}

			return result;
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


	//print byte
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& format, byte value)
	{
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Parsed_Format::TYPE_DECIMAL:
				PATTERN = "%hhd";
				break;
			case Parsed_Format::TYPE_HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hhx";
				else
					PATTERN = "%hhx";
				break;
			case Parsed_Format::TYPE_HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hhX";
				else
					PATTERN = "%hhX";
				break;
			case Parsed_Format::TYPE_OCTAL:
				if(format.prefix)
					PATTERN = "%#hho";
				else
					PATTERN = "%hho";
				break;
			case Parsed_Format::TYPE_RUNE:
			case Parsed_Format::TYPE_NONE:
			default:
				PATTERN = "%c";
				break;
		}

		return internal::_print_integer<byte, 6>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 8-bit signed int
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& _format, i8 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Parsed_Format::TYPE_RUNE:
				PATTERN = "%c";
				break;
			case Parsed_Format::TYPE_HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hhx";
				else
					PATTERN = "%hhx";
				break;
			case Parsed_Format::TYPE_HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hhX";
				else
					PATTERN = "%hhX";
				break;
			case Parsed_Format::TYPE_OCTAL:
				if(format.prefix)
					PATTERN = "%#hho";
				else
					PATTERN = "%hho";
				break;
			case Parsed_Format::TYPE_DECIMAL:
			case Parsed_Format::TYPE_NONE:
			default:
				PATTERN = "%hhd";
				format.type = Parsed_Format::TYPE_DECIMAL;
				break;
		}

		return internal::_print_integer<i8, 6>(trait,
											   format,
											   PATTERN,
											   value);
	}

	//print 8-bit unsigned int
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& _format, u8 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Parsed_Format::TYPE_RUNE:
				PATTERN = "%c";
				break;
			case Parsed_Format::TYPE_HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hhx";
				else
					PATTERN = "%hhx";
				break;
			case Parsed_Format::TYPE_HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hhX";
				else
					PATTERN = "%hhX";
				break;
			case Parsed_Format::TYPE_OCTAL:
				if(format.prefix)
					PATTERN = "%#hho";
				else
					PATTERN = "%hho";
				break;
			case Parsed_Format::TYPE_DECIMAL:
			case Parsed_Format::TYPE_NONE:
			default:
				PATTERN = "%hhu";
				format.type = Parsed_Format::TYPE_DECIMAL;
				break;
		}

		return internal::_print_integer<u8, 6>(trait,
											   format,
											   PATTERN,
											   value);
	}

	//print 16-bit signed int
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& _format, i16 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Parsed_Format::TYPE_RUNE:
				PATTERN = "%c";
				break;
			case Parsed_Format::TYPE_HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hx";
				else
					PATTERN = "%hx";
				break;
			case Parsed_Format::TYPE_HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hX";
				else
					PATTERN = "%hX";
				break;
			case Parsed_Format::TYPE_OCTAL:
				if(format.prefix)
					PATTERN = "%#ho";
				else
					PATTERN = "%ho";
				break;
			case Parsed_Format::TYPE_DECIMAL:
			case Parsed_Format::TYPE_NONE:
			default:
				PATTERN = "%hd";
				format.type = Parsed_Format::TYPE_DECIMAL;
				break;
		}

		return internal::_print_integer<i16, 10>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 16-bit unsigned int
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& _format, u16 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Parsed_Format::TYPE_RUNE:
				PATTERN = "%c";
				break;
			case Parsed_Format::TYPE_HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hx";
				else
					PATTERN = "%hx";
				break;
			case Parsed_Format::TYPE_HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hX";
				else
					PATTERN = "%hX";
				break;
			case Parsed_Format::TYPE_OCTAL:
				if(format.prefix)
					PATTERN = "%#ho";
				else
					PATTERN = "%ho";
				break;
			case Parsed_Format::TYPE_DECIMAL:
			case Parsed_Format::TYPE_NONE:
			default:
				PATTERN = "%hu";
				format.type = Parsed_Format::TYPE_DECIMAL;
				break;
		}

		return internal::_print_integer<u16, 10>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 32-bit signed int
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& _format, i32 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Parsed_Format::TYPE_RUNE:
				PATTERN = "%c";
				break;
			case Parsed_Format::TYPE_HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#x";
				else
					PATTERN = "%x";
				break;
			case Parsed_Format::TYPE_HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#X";
				else
					PATTERN = "%X";
				break;
			case Parsed_Format::TYPE_OCTAL:
				if(format.prefix)
					PATTERN = "%#o";
				else
					PATTERN = "%o";
				break;
			case Parsed_Format::TYPE_DECIMAL:
			case Parsed_Format::TYPE_NONE:
			default:
				PATTERN = "%d";
				format.type = Parsed_Format::TYPE_DECIMAL;
				break;
		}

		return internal::_print_integer<i32, 20>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 32-bit unsigned int
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& _format, u32 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Parsed_Format::TYPE_RUNE:
				PATTERN = "%c";
				break;
			case Parsed_Format::TYPE_HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#x";
				else
					PATTERN = "%x";
				break;
			case Parsed_Format::TYPE_HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#X";
				else
					PATTERN = "%X";
				break;
			case Parsed_Format::TYPE_OCTAL:
				if(format.prefix)
					PATTERN = "%#o";
				else
					PATTERN = "%o";
				break;
			case Parsed_Format::TYPE_DECIMAL:
			case Parsed_Format::TYPE_NONE:
			default:
				PATTERN = "%u";
				format.type = Parsed_Format::TYPE_DECIMAL;
				break;
		}

		return internal::_print_integer<u32, 20>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 64-bit signed int
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& _format, i64 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		#if defined(OS_WINDOWS)
		{
			switch(format.type)
			{
				case Parsed_Format::TYPE_RUNE:
					PATTERN = "%c";
					break;
				case Parsed_Format::TYPE_HEX_SMALL:
					if(format.prefix)
						PATTERN = "%#llx";
					else
						PATTERN = "%llx";
					break;
				case Parsed_Format::TYPE_HEX_CAPITAL:
					if(format.prefix)
						PATTERN = "%#llX";
					else
						PATTERN = "%llX";
					break;
				case Parsed_Format::TYPE_OCTAL:
					if(format.prefix)
						PATTERN = "%#llo";
					else
						PATTERN = "%llo";
					break;
				case Parsed_Format::TYPE_DECIMAL:
				case Parsed_Format::TYPE_NONE:
				default:
					PATTERN = "%lld";
					format.type = Parsed_Format::TYPE_DECIMAL;
					break;
			}
		}
		#elif defined(OS_LINUX)
		{
			switch(format.type)
			{
				case Parsed_Format::TYPE_RUNE:
					PATTERN = "%c";
					break;
				case Parsed_Format::TYPE_HEX_SMALL:
					if(format.prefix)
						PATTERN = "%#lx";
					else
						PATTERN = "%lx";
					break;
				case Parsed_Format::TYPE_HEX_CAPITAL:
					if(format.prefix)
						PATTERN = "%#lX";
					else
						PATTERN = "%lX";
					break;
				case Parsed_Format::TYPE_OCTAL:
					if(format.prefix)
						PATTERN = "%#lo";
					else
						PATTERN = "%lo";
					break;
				case Parsed_Format::TYPE_DECIMAL:
				case Parsed_Format::TYPE_NONE:
				default:
					PATTERN = "%ld";
					format.type = Parsed_Format::TYPE_DECIMAL;
					break;
			}
		}
		#endif

		return internal::_print_integer<i64, 40>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 64-bit unsigned int
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& _format, u64 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		#if defined(OS_WINDOWS)
		{
			switch(format.type)
			{
				case Parsed_Format::TYPE_RUNE:
					PATTERN = "%c";
					break;
				case Parsed_Format::TYPE_HEX_SMALL:
					if(format.prefix)
						PATTERN = "%#llx";
					else
						PATTERN = "%llx";
					break;
				case Parsed_Format::TYPE_HEX_CAPITAL:
					if(format.prefix)
						PATTERN = "%#llX";
					else
						PATTERN = "%llX";
					break;
				case Parsed_Format::TYPE_OCTAL:
					if(format.prefix)
						PATTERN = "%#llo";
					else
						PATTERN = "%llo";
					break;
				case Parsed_Format::TYPE_DECIMAL:
				case Parsed_Format::TYPE_NONE:
				default:
					PATTERN = "%llu";
					format.type = Parsed_Format::TYPE_DECIMAL;
					break;
			}
		}
		#elif defined(OS_LINUX)
		{
			switch(format.type)
			{
				case Parsed_Format::TYPE_RUNE:
					PATTERN = "%c";
					break;
				case Parsed_Format::TYPE_HEX_SMALL:
					if(format.prefix)
						PATTERN = "%#lx";
					else
						PATTERN = "%lx";
					break;
				case Parsed_Format::TYPE_HEX_CAPITAL:
					if(format.prefix)
						PATTERN = "%#lX";
					else
						PATTERN = "%lX";
					break;
				case Parsed_Format::TYPE_OCTAL:
					if(format.prefix)
						PATTERN = "%#lo";
					else
						PATTERN = "%lo";
					break;
				case Parsed_Format::TYPE_DECIMAL:
				case Parsed_Format::TYPE_NONE:
				default:
					PATTERN = "%lu";
					format.type = Parsed_Format::TYPE_DECIMAL;
					break;
			}
		}
		#endif

		return internal::_print_integer<u64, 40>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 32-bit float
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& _format, r32 value)
	{
		return internal::_print_real<r32, 128, 6>(trait, _format, value);
	}

	//print 64-bit float
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& _format, r64 value)
	{
		return internal::_print_real<r64, 265, 12>(trait, _format, value);
	}

	//print pointer as string
	inline static usize
	print_str(IO_Trait *trait, const Parsed_Format& format, void* ptr)
	{
		return print_str(trait, format, reinterpret_cast<usize>(ptr));
	}

	//print string literal
	inline static usize
	print_str(IO_Trait* trait, const Parsed_Format& format, const String_Range& str)
	{
		usize result = 0;
		//respect right and center align
		usize written_size = str.bytes.size;
		usize pad_size = 0, pad_str_size = 0;
		Parsed_Format::ALIGN alignment = format.alignment;
		if(format.width > (written_size + result))
		{
				pad_size = format.width - (written_size + result);
				pad_str_size = std::strlen((char*)&format.pad.data);
				//default alignment is LEFt
				if(alignment == Parsed_Format::ALIGN_NONE)
					alignment = Parsed_Format::ALIGN_LEFT;
		}

		if (pad_size != 0 &&
			alignment == Parsed_Format::ALIGN_RIGHT)
		{
			for(usize i = 0; i < pad_size; ++i)
				result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
		}
		else if(pad_size != 0 &&
				alignment == Parsed_Format::ALIGN_CENTER)
		{
			usize i;
			for(i = 0; i < pad_size / 2; ++i)
				result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
			pad_size -= i;
		}

		result += trait->write(str.bytes.template convert<byte>());

		//respect left and center align
		if (pad_size != 0 &&
			alignment == Parsed_Format::ALIGN_CENTER)
		{
			for(usize i = 0; i < pad_size; ++i)
				result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
		}
		else if(pad_size != 0 &&
				alignment == Parsed_Format::ALIGN_LEFT)
		{
			for(usize i = 0; i < pad_size; ++i)
				result += trait->write(Slice<byte>((byte*)&format.pad, pad_str_size));
		}

		return result;
	}

	inline static usize
	print_str(IO_Trait *trait, const Parsed_Format& format, const char* str)
	{
		return print_str(trait, format, String_Range(str));
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
	vprintf(IO_Trait* trait, const String_Range& str)
	{
		return print_str(trait, Parsed_Format(), str);
	}

	template<typename ... TArgs>
	inline static usize
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
					println_err("[panic]: incomplete format specifier i.e. only a single '{' were found with no closing '}' in vprintf statement");
					os->dump_callstack();
					std::abort();
					//return result;
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

				Parsed_Format format;
				if(!manual_indexing)
					format.index = default_index++;
				internal::_parse_format(rune_forward_it, rune_end, format, manual_indexing);

				//print the value
				if(format.index >= values_count)
				{
					//TODO(Moustapha): panic here
					println_err("[panic]: index out of range in vprintf statement");
					os->dump_callstack();
					std::abort();
					//return result;
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
			result += trait->write(
						Slice<byte>((byte*)rune_back_it.ptr,
									rune_forward_it.ptr - rune_back_it.ptr));
		}

		return result;
	}


	//most used public interface
	API_CPPR void
	_acquire_print_lock();

	API_CPPR void
	_release_print_lock();

	API_CPPR void
	_acquire_print_err_lock();

	API_CPPR void
	_release_print_err_lock();

	template<typename ... TArgs>
	inline static usize
	print(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_lock();
		result = vprints(os->unbuf_stdout, std::forward<TArgs>(args)...);
		_release_print_lock();
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	print_err(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_err_lock();
		result = vprints(os->unbuf_stderr, std::forward<TArgs>(args)...);
		_release_print_err_lock();
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	println(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_lock();
		result = vprints(os->unbuf_stdout, std::forward<TArgs>(args)..., "\n");
		_release_print_lock();
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	println_err(TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_err_lock();
		result = vprints(os->unbuf_stderr, std::forward<TArgs>(args)..., "\n");
		_release_print_err_lock();
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	printf(const String_Range& format, TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_lock();
		result = vprintf(os->unbuf_stdout, format, std::forward<TArgs>(args)...);
		_release_print_lock();
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	printf_err(const String_Range& format, TArgs&& ... args)
	{
		usize result = 0;
		_acquire_print_err_lock();
		result = vprintf(os->unbuf_stderr, format, std::forward<TArgs>(args)...);
		_release_print_err_lock();
		return result;
	}


	//read functions
	inline static bool
	_is_whitespace(byte c)
	{
		return (c == ' '  ||
				c == '\f' ||
				c == '\n' ||
				c == '\r' ||
				c == '\t' ||
				c == '\v');
	}

	inline static void
	_guarantee_text_chunk(Bufio_Trait* trait, usize REQUEST_SIZE)
	{
		usize requested_size = 0;
		usize last_size = -1;

		//skip all the spaces at the start of the string
		while (true)
		{
			auto bytes = trait->peek(requested_size);

			if(bytes.size == last_size)
				break;

			usize whitespace_count = 0;
			for (auto& byte : bytes)
			{
				if (_is_whitespace(byte))
				{
					++whitespace_count;
				}
				else
				{
					break;
				}
			}
			trait->skip(whitespace_count);
			if (whitespace_count < bytes.size)
				break;

			requested_size += REQUEST_SIZE;
			last_size = bytes.size;
		}

		//guarantee that we didn't chop of early
		//i.e. [12345]13412\r\n
		//so we have to check for whitespaces at the end also
		//set the requested size to 0 to get the already buffered data
		requested_size = 0;
		last_size = -1;
		while (true)
		{
			bool found_whitespace = false;

			auto bytes = trait->peek(requested_size);

			if(bytes.size == last_size)
				break;

			last_size = bytes.size;

			for (;!bytes.empty();
				bytes.pop_back())
			{
				if (_is_whitespace(bytes.back()))
				{
					found_whitespace = true;
					break;
				}
			}

			if (found_whitespace)
				break;

			requested_size += REQUEST_SIZE;
		}
	}

	inline static usize
	_read_u64(Bufio_Trait* trait, u64& value, int base)
	{
		_guarantee_text_chunk(trait, 40);
		auto peeked_content = trait->peek();

		//we cannot parse negative numbers
		if(peeked_content[0] == '-')
			return 0;

		auto end = peeked_content.end();
		u64 tmp_value = std::strtoull(peeked_content.begin(), &end, base);

		if(errno == ERANGE)
			return 0;

		value = tmp_value;

		return end - peeked_content.begin();
	}

	inline static usize
	_read_i64(Bufio_Trait* trait, i64& value, int base)
	{
		_guarantee_text_chunk(trait, 40);
		auto peeked_content = trait->peek();

		auto end = peeked_content.end();
		i64 tmp_value = std::strtoll(peeked_content.begin(), &end, base);

		if(errno == ERANGE)
			return 0;

		value = tmp_value;

		return end - peeked_content.begin();
	}

	inline static usize
	_read_r32(Bufio_Trait *trait, r32& value)
	{
		_guarantee_text_chunk(trait, 40);
		auto peeked_content = trait->peek();

		auto end = peeked_content.end();
		r32 tmp_value = std::strtof(peeked_content.begin(), &end);

		if(errno == ERANGE)
			return 0;

		value = tmp_value;

		return end - peeked_content.begin();
	}

	inline static usize
	_read_r64(Bufio_Trait *trait, r64& value)
	{
		_guarantee_text_chunk(trait, 40);
		auto peeked_content = trait->peek();

		auto end = peeked_content.end();
		r64 tmp_value = std::strtod(peeked_content.begin(), &end);

		if(errno == ERANGE)
			return 0;

		value = tmp_value;

		return end - peeked_content.begin();
	}

	//read_bin
	template<typename T>
	inline static usize
	read_bin(IO_Trait* trait, T& value)
	{
		return trait->read(Slice<byte>((byte*)&value, sizeof(T)));
	}

	template<typename T>
	inline static usize
	read_bin(IO_Trait* trait, Slice<T>& values)
	{
		return trait->read(values.template convert<byte>());
	}

	namespace internal
	{
		inline static void
		_variadic_read_binary_helper(IO_Trait* trait, usize& size)
		{
			return;
		}

		template<typename TFirst, typename ... TArgs>
		inline static void
		_variadic_read_binary_helper(IO_Trait* trait,
									 usize& size,
									 TFirst&& first_arg,
									 TArgs&& ... args)
		{
			size += read_bin(trait, std::forward<TFirst>(first_arg));
			_variadic_read_binary_helper(trait, size, std::forward<TArgs>(args)...);
		}
	}

	template<typename ... TArgs>
	inline static usize
	vreadb(IO_Trait* trait, TArgs&& ... args)
	{
		usize result = 0;
		internal::_variadic_read_binary_helper(trait, result, std::forward<TArgs>(args)...);
		return result;
	}

	//signed functions
	#define READ_STR_SIGNED(TYPE) \
	inline static usize \
	read_str(Bufio_Trait* trait, TYPE& value) \
	{ \
		i64 tmp_value = 0; \
		usize parsed_size = _read_i64(trait, tmp_value, 10); \
		if(parsed_size == 0) \
			return 0;\
		if (tmp_value < std::numeric_limits<TYPE>::lowest() || \
			tmp_value > std::numeric_limits<TYPE>::max()) \
		{ \
			return 0; \
		} \
		value = static_cast<TYPE>(tmp_value); \
		return trait->skip(parsed_size); \
	}

	READ_STR_SIGNED(byte)
	READ_STR_SIGNED(i8)
	READ_STR_SIGNED(i16)
	READ_STR_SIGNED(i32)
	READ_STR_SIGNED(i64)

	#undef READ_STR_SIGNED

	//unsigned functions
	#define READ_STR_UNSIGNED(TYPE) \
	inline static usize \
	read_str(Bufio_Trait* trait, TYPE& value) \
	{ \
		u64 tmp_value = 0; \
		usize parsed_size = _read_u64(trait, tmp_value, 10); \
		if(parsed_size == 0) \
			return 0;\
		if (tmp_value > std::numeric_limits<TYPE>::max()) \
		{ \
			return 0; \
		} \
		value = static_cast<TYPE>(tmp_value); \
		return trait->skip(parsed_size); \
	}

	READ_STR_UNSIGNED(u8)
	READ_STR_UNSIGNED(u16)
	READ_STR_UNSIGNED(u32)
	READ_STR_UNSIGNED(u64)

	#undef READ_STR_UNSIGNED

	inline static usize
	read_str(Bufio_Trait* trait, void*& value)
	{
		u64 tmp_value = 0;
		usize parsed_size = _read_u64(trait, tmp_value, 16);
		if(parsed_size == 0)
			return 0;
		if(tmp_value > std::numeric_limits<usize>::max())
			return 0;
		value = reinterpret_cast<void*>(tmp_value);
		return trait->skip(parsed_size);
	}

	inline static usize
	read_str(Bufio_Trait* trait, r32& value)
	{
		r32 tmp_value = 0;
		usize parsed_size = _read_r32(trait, tmp_value);
		if(parsed_size == 0)
			return 0;
		value = tmp_value;
		return trait->skip(parsed_size);
	}

	inline static usize
	read_str(Bufio_Trait* trait, r64& value)
	{
		r64 tmp_value = 0;
		usize parsed_size = _read_r64(trait, tmp_value);
		if(parsed_size == 0)
			return 0;
		value = tmp_value;
		return trait->skip(parsed_size);
	}

	namespace internal
	{
		inline static void
		_variadic_read_string_helper(Bufio_Trait* trait, usize& size)
		{
			return;
		}

		template<typename TFirst, typename ... TArgs>
		inline static void
		_variadic_read_string_helper(Bufio_Trait* trait,
									 usize& size,
									 TFirst&& first_arg,
									 TArgs&& ... args)
		{
			size += read_str(trait, std::forward<TFirst>(first_arg));
			_variadic_read_string_helper(trait, size, std::forward<TArgs>(args)...);
		}
	}

	template<typename ... TArgs>
	inline static usize
	vreads(Bufio_Trait* trait, TArgs&& ... args)
	{
		usize result = 0;
		internal::_variadic_read_string_helper(trait, result, std::forward<TArgs>(args)...);
		return result;
	}

	template<typename ... TArgs>
	inline static usize
	read(TArgs&& ... args)
	{
		return vreads(os->buf_stdin, std::forward<TArgs>(args)...);
	}
}