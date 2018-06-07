#pragma once
#include "cpprelude/api.h"
#include "cpprelude/IO_Trait.h"
#include "cpprelude/Ranges.h"
#include "cpprelude/Result.h"
#include "cpprelude/OS.h"
#include <cstdio>
#include <cerrno>
#include <limits>
#include <cstdlib>

namespace cppr
{
	/**
	 * [[markdown]]
	 * # The IO
	 * Here's a listing of the function provided by this file
	 * - **print_err**: prints to the stderr
	 * - **println_err**: just like the above but adds a newline at the end
	 * - **printf_err**: print a formatted string to the stderr
	 * - **print**: prints to the stdout
	 * - **println**: just like the above but adds a newline at the end
	 * - **printf**: print a formatted string to the stdout
	 * - **vprintb**: variadic print binary function that's used to print values as binary data to a specific IO_Trait
	 * - **vprints**: variadic print string function that's used to print values as string data to a specific IO_Trait
	 * - **vprintf**: variadic print formatted string function that's used to print values as string data to a specific IO_Trait
	 * - **read**: reads values as strings from the buffered stdin
	 * - **vreadb**: reads values as binary data from a specific IO_Trait
	 * - **vreads**: reads values as string values from a specific Bufio_Trait
	 * 
	 * Note that the core functions are the ones prefixed with a `v` like `vprintb`, `vreads` ... etc, other functions just use/wrap them with the stdio handles
	 * 
	 * The print strategy is that you define a `print_str` if you want to print the type as string
	 * or a `print_bin` if you want to print it as a binary
	 * We already define a `print_bin`, `print_str`, `read_bin`, and `read_str` for the basic types
	 * 
	 * ## Custom Print String Function
	 * ```C++
	 * inline static usize
	 * print_str(IO_Trait* trait, const Print_Format& format, const Your_Type& value);
	 * ```
	 * If you define this function then it will used to print your type by the `vprints` or `vprintf` function
	 * As you may notice if it's called from `vprintf` function then the format style will be filled and passed along for you to do the necessary formatting of your type
	 * If it's called from `vprints` then the format will be default initialized
	 * You can have a look at the `Print_Format` structure
	 * 
	 * ## Custom Print Binary Function
	 * ```C++
	 * inline static usize
	 * print_bin(IO_Trait* trait, const Your_Type& value)
	 * {
	 * 	return trait->write(Slice<byte>((byte*)&value, sizeof(Your_Type)));
	 * }
	 * ```
	 * If you define this function then it will be used when you use the `vprintb` function
	 * The default behaviour for your custom types is that we just dump the binary representation of it in memory to the IO_Trait so if that suits you then there's no need to provide a custom function
	 * 
	 * ## Custom Read String Function
	 * ```C++
	 * inline static usize
	 * read_str(Bufio_Trait* trait, Your_Type& value);
	 * ```
	 * If you define this function then it will be used when you use the `vreads` function
	 * 
	 * ## Custom Read Binary Function
	 * ```C++
	 * inline static usize
	 * read_bin(IO_Trait* trait, Your_Type& value)
	 * ```
	 * If you define this function then it will be used when you use the `vreadb` function
	 * The default behaviour for your custom types is that we just read the `sizeof(Your_Type)` from the IO_Trait so if that suits you then there's no need to provide a custom function
	 * 
	 * ## Formatted Print Documentation
	 * The print formatted string has a simple grammar to specify different styles of printing values
	 * 
	 * `vprintf("Normal String");`
	 * You can just print a normal string
	 * 
	 * 
	 * `vprintf("Formatted String {}", value);`
	 * You can specify the placeholder of a value using the `{}` where you want the values to be printed
	 * 
	 * 
	 * `vprintf("My Name is {}, and my age is {}", "Mostafa", 25);`
	 * You can have multiple values and send them as arguments to the function and they will be used in the same order producing the result of `My Name is Mostafa, and my age is 25`
	 * 
	 * 
	 * `vprintf("My Name is {0}, and my age is {1}, Hello {0}", "Mostafa", 25);`
	 * You can specify the index of the variable to be printed in the location by using its index so the above line prints `My Name is Mostafa, and my age is 25, Hello Mostafa`
	 * 
	 * 
	 * Note that you cannot mix unindexed placeholder and indexed placeholders in the same print statement i.e. `My Name is {0}, and my age is {}, Hello {0}` This will result in an error
	 * 
	 * ### The Anatomy of a placeholder
	 * `{}`
	 * This is the simplist placeholder that could ever exists and it's un-indexed
	 * 
	 * `{0}`
	 * This is an indexed placeholder
	 * 
	 * `{0:_<+#20.5f}`
	 * This is the most complex placeholder
	 * So the anatomy is simple there's a left part which only contains the index which can be optional and then there's a colon then there's some style specifiers
	 * - `_`: this is the padding letter to be used to fill empty spaces so in this case it's `_` but it can be anything you like
	 * - `<`: this is the alignment specifier which in this case do a left alignment of the text according to the specified width
	 * 	- `<`: left alignment
	 * 	- `>`: right alignment
	 * 	- `^`: center alignment
	 * - `+`: this is the sign spcifier which in this case prints the sign of the number always
	 * 	- `+`: always prints the sign of the number so it prints `+` for positive and `-` for negative numbers
	 * 	- `-`: prints the sign of the negative numbers only and this is the default
	 * 	- ` `: a space prints the sign of the negative numbers only and prints an empty space for the positive numbers
	 * - `#`: this is the prefix specifier which print the `0x` prefix in the case of the hex values and `0o` in case of octal values
	 * - `20`: this is the width specifier which specifies the width of this placeholders in letters so in this case this placeholder has a 20 letter width and this will be used in things like the alignement ... etc. this can be any number you like.
	 * - `.5`: this is the precision specifiers which is valid only with floating point values and it specifies that we should print to the 5th place after the `.` and this can be any number you like i.e. `.2`, `.10`, ... etc.
	 * - `f`: this is the type formatter in this case it tells the format to treat the value as a floating point number
	 * 	- `c`: this specifies the value to be printed as a letter
	 * 	- `d`: this specifies the value to be printed as a decimal number
	 * 	- `o`: this specifies the value to be printed as an octal number
	 * 	- `x`: this specifies the value to be printed as a hex number with small letters `0xabcdef`
	 * 	- `X`: this specifies the value to be printed as a hex number with capital letters `0xABCDEF`
	 * 	- `e`: this is a floating point number only format which prints the value in scientific exp format using the small `e` letter
	 * 	- `E`: this is a floating point number only format which prints the value in scientific exp format using the capital `E` letter
	 * 	- `f`: this is a floating point number only format which prints the value in floating point style `1.50` the default precision is 6 digits for `r32` and 6 digits for `r64`
	 * 	- `g`: this is a floating point number only format which prints floating-point number to decimal or decimal exponent notation using a small letter `e` depending on the value and the precision. i.e. if the value is `1.230000` when printed with `f` specifier then it will be `1.23` with the `g` specifier, similarly `1.000000` with the `f` will be `1` with the `g`
	 * 	- `G`: this is a floating point number only format which prints floating-point number to decimal or decimal exponent notation using a capital letter `E` depending on the value and the precision. i.e. if the value is `1.230000` when printed with `f` specifier then it will be `1.23` with the `G` specifier, similarly `1.000000` with the `f` will be `1` with the `G`
	 * 
	 * 
	 * Please note that this is only a format request so the `print_str` may choose to not respect those requests if they are invalid or meaningless to the value i.e. `f` specifier with an integer value
	 * You can print `{` curly braces by using the `{{` in the format string but you don't need to escape the `}`
	 * ##Examples
	 * Input:
	 * ```C++
	 * cppr::printf("Hello, my name is {}, my age is {:.2f}", "Mostafa", 25.0f);
	 * ```
	 * Output:
	 * ```
	 * Hello, my name is Mostafa, my age is 25.00
	 * ```
	 * 
	 * Input:
	 * ```C++
	 * for(usize i = 0; i < 10; ++i)
	 * 	cppr::printf("My Random number is {:_^+10}\n", rand());
	 * ```
	 * Output:
	 * ```
	 * My Random number is +___41____
	 * My Random number is +__18467__
	 * My Random number is +__6334___
	 * My Random number is +__26500__
	 * My Random number is +__19169__
	 * My Random number is +__15724__
	 * My Random number is +__11478__
	 * My Random number is +__29358__
	 * My Random number is +__26962__
	 * My Random number is +__24464__
	 * ```
	 * 
	 * Input:
	 * ```C++
	 * cppr::printf("Person {{ Name: {}, Age: {} }\n", "Mostafa", 25);
	 * ```
	 * Output:
	 * ```
	 * Person { Name: Mostafa, Age: 25 }
	 * ```
	 */

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

	/**
	 * @brief      The Print string format which specifies the desired style of the print_str function
	 */
	struct Print_Format
	{
		/**
		 * @brief      Alignment of the printed string value
		 * 
		 * - **NONE**: The invalid alignment
		 * - **LEFT**: '<' left aligns the value
		 * - **RIGHT**: '>' right aligns the value
		 * - **CENTER**: '^' center aligns the value
		 * - **EQUAL**: '=' used with numbers only for printing values like +000120
		 */
		enum class ALIGN
		{
			NONE,
			LEFT,
			RIGHT,
			CENTER,
			EQUAL
		};

		/**
		 * @brief      Sign style of the printed numbers
		 * 
		 * - **NONE**: The invalid sign
		 * - **POSITIVE**: always print signs before the number
		 * - **NEGATIVE**: print only negative signs
		 * - **SPACE**: print one space for positive numbers
		 */
		enum class SIGN
		{
			NONE,
			POSITIVE,
			NEGATIVE,
			SPACE
		};

		/**
		 * @brief      The type style of the printed value
		 * 
		 * - **NONE**: The invalid type
		 * - **BINARY**: print in binary format
		 * - **RUNE**: print as a rune
		 * - **DECIMAL**: print as decimal value
		 * - **OCTAL**: print as octal value
		 * - **HEX_SMALL**: print as a hexadecimal value with small 'abcdef' letters
		 * - **HEX_CAPITAL**: print as a hexadecimal value with capital `ABCDEF` letters
		 * - The following options are for floating point only
		 * - **EXP_SMALL**: print floats as exponent format with small case 'e'
		 * - **EXP_CAPITAL**: print floats as exponent format with capital case 'E'
		 * - **FLOAT**: print as floating point value
		 * - **GENERAL_SMALL**: converts floating point number to decimal or decimal small exponent notation depending on the value and precision
		 * - **GENERAL_CAPITAL**: converts floating point number to decimal or decimal capital exponent notation depending on the value and precision
		 */
		enum class TYPE
		{
			NONE,
			BINARY,
			RUNE,
			DECIMAL,
			OCTAL,
			HEX_SMALL,
			HEX_CAPITAL,

			//float only types
			EXP_SMALL,
			EXP_CAPITAL,
			FLOAT,
			GENERAL_SMALL,
			GENERAL_CAPITAL
		};

		usize index;
		ALIGN alignment;
		Rune  pad;
		SIGN  sign;
		bool  prefix;
		usize width;
		usize precision;
		TYPE type;

		Print_Format()
			:index(0),
			alignment(ALIGN::NONE),
			pad(' '),
			sign(SIGN::NONE),
			prefix(false),
			width(0),
			precision(-1),
			type(TYPE::NONE)
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
		_generic_print_str_function(void* _self, const Print_Format& format, IO_Trait* trait)
		{
			T* value = (T*)_self;
			return print_str(trait, format, *value);
		}

		struct Generic_Print_Str_Value
		{
			using print_func = usize(*)(void*, const Print_Format&, IO_Trait*);
			void* _self;
			print_func _print;

			template<typename T>
			Generic_Print_Str_Value(const T& value)
			{
				_self = (void*)(&value);
				_print = _generic_print_str_function<T>;
			}

			usize
			print(IO_Trait* trait, const Print_Format& format)
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
			size += print_str(trait, Print_Format(), std::forward<TFirst>(first_arg));
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
					  Print_Format& format,
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
				format.alignment = Print_Format::ALIGN::LEFT;
				format.pad = *after_colon;
				++it;
			}
			else if (c == '>')
			{
				format.alignment = Print_Format::ALIGN::RIGHT;
				format.pad = *after_colon;
				++it;
			}
			else if (c == '^')
			{
				format.alignment = Print_Format::ALIGN::CENTER;
				format.pad = *after_colon;
				++it;
			}
			else
			{
				it = after_colon;
				c = *it;
				if (c == '<')
				{
					format.alignment = Print_Format::ALIGN::LEFT;
					++it;
				}
				else if (c == '>')
				{
					format.alignment = Print_Format::ALIGN::RIGHT;
					++it;
				}
				else if (c == '^')
				{
					format.alignment = Print_Format::ALIGN::CENTER;
					++it;
				}
			}

			c = *it;
			//check the sign
			if (c == '+')
			{
				format.sign = Print_Format::SIGN::POSITIVE;
				++it;
			}
			else if (c == '-')
			{
				format.sign = Print_Format::SIGN::NEGATIVE;
				++it;
			}
			else if (c == ' ')
			{
				format.sign = Print_Format::SIGN::SPACE;
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
				format.type = Print_Format::TYPE::RUNE;
				++it;
			}
			else if (c == 'd')
			{
				format.type = Print_Format::TYPE::DECIMAL;
				++it;
			}
			else if(c == 'b')
			{
				format.type = Print_Format::TYPE::BINARY;
				++it;
			}
			else if (c == 'o')
			{
				format.type = Print_Format::TYPE::OCTAL;
				++it;
			}
			else if (c == 'x')
			{
				format.type = Print_Format::TYPE::HEX_SMALL;
				++it;
			}
			else if (c == 'X')
			{
				format.type = Print_Format::TYPE::HEX_CAPITAL;
				++it;
			}
			else if (c == 'e')
			{
				format.type = Print_Format::TYPE::EXP_SMALL;
				++it;
			}
			else if (c == 'E')
			{
				format.type = Print_Format::TYPE::EXP_CAPITAL;
				++it;
			}
			else if (c == 'f')
			{
				format.type = Print_Format::TYPE::FLOAT;
				++it;
			}
			else if (c == 'g')
			{
				format.type = Print_Format::TYPE::GENERAL_SMALL;
				++it;
			}
			else if (c == 'G')
			{
				format.type = Print_Format::TYPE::GENERAL_CAPITAL;
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
		_print_integer(IO_Trait* trait, const Print_Format& format, const char* pattern, T value)
		{
			usize result = 0;

			//respect the sign
			if (value >= 0 &&
				format.type == Print_Format::TYPE::DECIMAL &&
				format.sign == Print_Format::SIGN::POSITIVE)
			{
				char sign = '+';
				result += trait->write(make_slice(&sign));
			}
			else if(value >= 0 &&
					format.type == Print_Format::TYPE::DECIMAL &&
					format.sign == Print_Format::SIGN::SPACE)
			{
				char sign = ' ';
				result += trait->write(make_slice(&sign, 1));
			}

			//render the value and calc it's written size
			byte buffer[BUFFER_SIZE];
			int written_size = std::snprintf(buffer, BUFFER_SIZE, pattern, value);
			if(written_size < 0)
				return 0;

			//respect right and center align
			usize pad_size = 0, pad_str_size = 0;
			Print_Format::ALIGN alignment = format.alignment;
			if(format.width > (written_size + result))
			{
				pad_size = format.width - (written_size + result);
				pad_str_size = std::strlen((char*)&format.pad.data);
				//default alignment is LEFt
				if(alignment == Print_Format::ALIGN::NONE)
					alignment = Print_Format::ALIGN::LEFT;
			}

			if (pad_size != 0 &&
				alignment == Print_Format::ALIGN::RIGHT)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
			}
			else if(pad_size != 0 &&
					alignment == Print_Format::ALIGN::CENTER)
			{
				usize i;
				for(i = 0; i < pad_size / 2; ++i)
					result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
				pad_size -= i;
			}

			//write the rendered value
			result += trait->write(make_slice(buffer, written_size));

			//respect left and center align
			if (pad_size != 0 &&
				alignment == Print_Format::ALIGN::CENTER)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
			}
			else if(pad_size != 0 &&
					alignment == Print_Format::ALIGN::LEFT)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
			}

			return result;
		}

		template<typename T, usize BUFFER_SIZE, usize PERCISION>
		inline static usize
		_print_real(IO_Trait* trait, const Print_Format& _format, T value)
		{
			auto format = _format;

			//first respect the pattern
			const char* PATTERN = nullptr;
			switch(format.type)
			{
				case Print_Format::TYPE::EXP_SMALL:
					PATTERN = "%.*e";
					break;
				case Print_Format::TYPE::EXP_CAPITAL:
					PATTERN = "%.*E";
					break;
				case Print_Format::TYPE::FLOAT:
					PATTERN = "%.*f";
					break;
				case Print_Format::TYPE::GENERAL_SMALL:
					PATTERN = "%.*g";
					break;
				case Print_Format::TYPE::GENERAL_CAPITAL:
				default:
					PATTERN = "%.*G";
					break;
			}

			usize result = 0;

			//second respect the sign
			if (value >= 0 &&
				format.sign == Print_Format::SIGN::POSITIVE)
			{
				char s = '+';
				result += trait->write(make_slice(&s));
			}
			else if(value >= 0 &&
					format.sign == Print_Format::SIGN::SPACE)
			{
				char s = ' ';
				result += trait->write(make_slice(&s));
			}

			if(format.precision == static_cast<usize>(-1))
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
				if (format.alignment == Print_Format::ALIGN::NONE)
					format.alignment = Print_Format::ALIGN::LEFT;
			}

			if (pad_size != 0 &&
				format.alignment == Print_Format::ALIGN::RIGHT)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
			}
			else if(pad_size != 0 &&
					format.alignment == Print_Format::ALIGN::CENTER)
			{
				usize i;
				for(i = 0; i < pad_size / 2; ++i)
					result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
				pad_size -= i;
			}

			//fifth write the rendered value
			result += trait->write(make_slice(buffer, written_size));

			//sixth respect left and center align
			if (pad_size != 0 &&
				format.alignment == Print_Format::ALIGN::CENTER)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
			}
			else if(pad_size != 0 &&
					format.alignment == Print_Format::ALIGN::LEFT)
			{
				for(usize i = 0; i < pad_size; ++i)
					result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
			}

			return result;
		}
	}

	/**
	 * @brief      Writes the binary value to the provided IO_Trait/stream
	 *
	 * @param      trait  The IO_Trait to write to
	 * @param[in]  value  The value to be written
	 *
	 * @tparam     T      Type of the written value
	 *
	 * @return     The size of the written value in bytes
	 */
	template<typename T>
	inline static usize
	print_bin(IO_Trait* trait, const T& value)
	{
		return trait->write(make_slice((byte*)&value, sizeof(T)));
	}

	/**
	 * @brief      Writes the data of the provided slice to the IO_Trait/stream
	 *
	 * @param      trait  The IO_Trait to write to
	 * @param[in]  values  The values to be written
	 *
	 * @tparam     T      Type of the written values
	 *
	 * @return     The size of the written value in bytes
	 */
	template<typename T>
	inline static usize
	print_bin(IO_Trait* trait, const Slice<T>& values)
	{
		return trait->write(values.template convert<byte>());
	}

	/**
	 * @brief      Writes the data of the provided values to the IO_Trait/stream
	 *
	 * @param      trait  The IO_Trait to write to
	 * @param[in]  args  The values to be written
	 *
	 * @tparam     TArgs      Types of the written values
	 *
	 * @return     The size of the written value in bytes
	 */
	template<typename ... TArgs>
	inline static usize
	vprintb(IO_Trait *trait, TArgs&& ... args)
	{
		usize result = 0;
		internal::_variadic_print_binary_helper(trait, result, std::forward<TArgs>(args)...);
		return result;
	}


	//print byte

	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& format, byte value)
	{
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Print_Format::TYPE::DECIMAL:
				PATTERN = "%hhd";
				break;
			case Print_Format::TYPE::HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hhx";
				else
					PATTERN = "%hhx";
				break;
			case Print_Format::TYPE::HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hhX";
				else
					PATTERN = "%hhX";
				break;
			case Print_Format::TYPE::OCTAL:
				if(format.prefix)
					PATTERN = "%#hho";
				else
					PATTERN = "%hho";
				break;
			case Print_Format::TYPE::RUNE:
			case Print_Format::TYPE::NONE:
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
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& _format, i8 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Print_Format::TYPE::RUNE:
				PATTERN = "%c";
				break;
			case Print_Format::TYPE::HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hhx";
				else
					PATTERN = "%hhx";
				break;
			case Print_Format::TYPE::HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hhX";
				else
					PATTERN = "%hhX";
				break;
			case Print_Format::TYPE::OCTAL:
				if(format.prefix)
					PATTERN = "%#hho";
				else
					PATTERN = "%hho";
				break;
			case Print_Format::TYPE::DECIMAL:
			case Print_Format::TYPE::NONE:
			default:
				PATTERN = "%hhd";
				format.type = Print_Format::TYPE::DECIMAL;
				break;
		}

		return internal::_print_integer<i8, 6>(trait,
											   format,
											   PATTERN,
											   value);
	}

	//print 8-bit unsigned int
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& _format, u8 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Print_Format::TYPE::RUNE:
				PATTERN = "%c";
				break;
			case Print_Format::TYPE::HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hhx";
				else
					PATTERN = "%hhx";
				break;
			case Print_Format::TYPE::HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hhX";
				else
					PATTERN = "%hhX";
				break;
			case Print_Format::TYPE::OCTAL:
				if(format.prefix)
					PATTERN = "%#hho";
				else
					PATTERN = "%hho";
				break;
			case Print_Format::TYPE::DECIMAL:
			case Print_Format::TYPE::NONE:
			default:
				PATTERN = "%hhu";
				format.type = Print_Format::TYPE::DECIMAL;
				break;
		}

		return internal::_print_integer<u8, 6>(trait,
											   format,
											   PATTERN,
											   value);
	}

	//print 16-bit signed int
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& _format, i16 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Print_Format::TYPE::RUNE:
				PATTERN = "%c";
				break;
			case Print_Format::TYPE::HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hx";
				else
					PATTERN = "%hx";
				break;
			case Print_Format::TYPE::HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hX";
				else
					PATTERN = "%hX";
				break;
			case Print_Format::TYPE::OCTAL:
				if(format.prefix)
					PATTERN = "%#ho";
				else
					PATTERN = "%ho";
				break;
			case Print_Format::TYPE::DECIMAL:
			case Print_Format::TYPE::NONE:
			default:
				PATTERN = "%hd";
				format.type = Print_Format::TYPE::DECIMAL;
				break;
		}

		return internal::_print_integer<i16, 10>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 16-bit unsigned int
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& _format, u16 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Print_Format::TYPE::RUNE:
				PATTERN = "%c";
				break;
			case Print_Format::TYPE::HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#hx";
				else
					PATTERN = "%hx";
				break;
			case Print_Format::TYPE::HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#hX";
				else
					PATTERN = "%hX";
				break;
			case Print_Format::TYPE::OCTAL:
				if(format.prefix)
					PATTERN = "%#ho";
				else
					PATTERN = "%ho";
				break;
			case Print_Format::TYPE::DECIMAL:
			case Print_Format::TYPE::NONE:
			default:
				PATTERN = "%hu";
				format.type = Print_Format::TYPE::DECIMAL;
				break;
		}

		return internal::_print_integer<u16, 10>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 32-bit signed int
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& _format, i32 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Print_Format::TYPE::RUNE:
				PATTERN = "%lc";
				break;
			case Print_Format::TYPE::HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#x";
				else
					PATTERN = "%x";
				break;
			case Print_Format::TYPE::HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#X";
				else
					PATTERN = "%X";
				break;
			case Print_Format::TYPE::OCTAL:
				if(format.prefix)
					PATTERN = "%#o";
				else
					PATTERN = "%o";
				break;
			case Print_Format::TYPE::DECIMAL:
			case Print_Format::TYPE::NONE:
			default:
				PATTERN = "%d";
				format.type = Print_Format::TYPE::DECIMAL;
				break;
		}

		return internal::_print_integer<i32, 20>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 32-bit unsigned int
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& _format, u32 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		switch(format.type)
		{
			case Print_Format::TYPE::RUNE:
				PATTERN = "%c";
				break;
			case Print_Format::TYPE::HEX_SMALL:
				if(format.prefix)
					PATTERN = "%#x";
				else
					PATTERN = "%x";
				break;
			case Print_Format::TYPE::HEX_CAPITAL:
				if(format.prefix)
					PATTERN = "%#X";
				else
					PATTERN = "%X";
				break;
			case Print_Format::TYPE::OCTAL:
				if(format.prefix)
					PATTERN = "%#o";
				else
					PATTERN = "%o";
				break;
			case Print_Format::TYPE::DECIMAL:
			case Print_Format::TYPE::NONE:
			default:
				PATTERN = "%u";
				format.type = Print_Format::TYPE::DECIMAL;
				break;
		}

		return internal::_print_integer<u32, 20>(trait,
												 format,
												 PATTERN,
												 value);
	}

	//print 64-bit signed int
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& _format, i64 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		#if defined(OS_WINDOWS)
		{
			switch(format.type)
			{
				case Print_Format::TYPE::RUNE:
					PATTERN = "%c";
					break;
				case Print_Format::TYPE::HEX_SMALL:
					if(format.prefix)
						PATTERN = "%#llx";
					else
						PATTERN = "%llx";
					break;
				case Print_Format::TYPE::HEX_CAPITAL:
					if(format.prefix)
						PATTERN = "%#llX";
					else
						PATTERN = "%llX";
					break;
				case Print_Format::TYPE::OCTAL:
					if(format.prefix)
						PATTERN = "%#llo";
					else
						PATTERN = "%llo";
					break;
				case Print_Format::TYPE::DECIMAL:
				case Print_Format::TYPE::NONE:
				default:
					PATTERN = "%lld";
					format.type = Print_Format::TYPE::DECIMAL;
					break;
			}
		}
		#elif defined(OS_LINUX)
		{
			switch(format.type)
			{
				case Print_Format::TYPE::RUNE:
					PATTERN = "%c";
					break;
				case Print_Format::TYPE::HEX_SMALL:
					if(format.prefix)
						PATTERN = "%#lx";
					else
						PATTERN = "%lx";
					break;
				case Print_Format::TYPE::HEX_CAPITAL:
					if(format.prefix)
						PATTERN = "%#lX";
					else
						PATTERN = "%lX";
					break;
				case Print_Format::TYPE::OCTAL:
					if(format.prefix)
						PATTERN = "%#lo";
					else
						PATTERN = "%lo";
					break;
				case Print_Format::TYPE::DECIMAL:
				case Print_Format::TYPE::NONE:
				default:
					PATTERN = "%ld";
					format.type = Print_Format::TYPE::DECIMAL;
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
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& _format, u64 value)
	{
		auto format = _format;
		//first respect the pattern
		const char* PATTERN = nullptr;
		#if defined(OS_WINDOWS)
		{
			switch(format.type)
			{
				case Print_Format::TYPE::RUNE:
					PATTERN = "%c";
					break;
				case Print_Format::TYPE::HEX_SMALL:
					if(format.prefix)
						PATTERN = "%#llx";
					else
						PATTERN = "%llx";
					break;
				case Print_Format::TYPE::HEX_CAPITAL:
					if(format.prefix)
						PATTERN = "%#llX";
					else
						PATTERN = "%llX";
					break;
				case Print_Format::TYPE::OCTAL:
					if(format.prefix)
						PATTERN = "%#llo";
					else
						PATTERN = "%llo";
					break;
				case Print_Format::TYPE::DECIMAL:
				case Print_Format::TYPE::NONE:
				default:
					PATTERN = "%llu";
					format.type = Print_Format::TYPE::DECIMAL;
					break;
			}
		}
		#elif defined(OS_LINUX)
		{
			switch(format.type)
			{
				case Print_Format::TYPE::RUNE:
					PATTERN = "%c";
					break;
				case Print_Format::TYPE::HEX_SMALL:
					if(format.prefix)
						PATTERN = "%#lx";
					else
						PATTERN = "%lx";
					break;
				case Print_Format::TYPE::HEX_CAPITAL:
					if(format.prefix)
						PATTERN = "%#lX";
					else
						PATTERN = "%lX";
					break;
				case Print_Format::TYPE::OCTAL:
					if(format.prefix)
						PATTERN = "%#lo";
					else
						PATTERN = "%lo";
					break;
				case Print_Format::TYPE::DECIMAL:
				case Print_Format::TYPE::NONE:
				default:
					PATTERN = "%lu";
					format.type = Print_Format::TYPE::DECIMAL;
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
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& _format, r32 value)
	{
		return internal::_print_real<r32, 128, 6>(trait, _format, value);
	}

	//print 64-bit float
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& _format, r64 value)
	{
		return internal::_print_real<r64, 265, 12>(trait, _format, value);
	}

	//print pointer as string
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait *trait, const Print_Format& format, void* ptr)
	{
		return print_str(trait, format, reinterpret_cast<usize>(ptr));
	}

	//print string literal
	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& format, const String_Range& str)
	{
		usize result = 0;
		//respect right and center align
		usize written_size = str.bytes.size;
		usize pad_size = 0, pad_str_size = 0;
		Print_Format::ALIGN alignment = format.alignment;
		if(format.width > (written_size + result))
		{
				pad_size = format.width - (written_size + result);
				pad_str_size = std::strlen((char*)&format.pad.data);
				//default alignment is LEFt
				if(alignment == Print_Format::ALIGN::NONE)
					alignment = Print_Format::ALIGN::LEFT;
		}

		if (pad_size != 0 &&
			alignment == Print_Format::ALIGN::RIGHT)
		{
			for(usize i = 0; i < pad_size; ++i)
				result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
		}
		else if(pad_size != 0 &&
				alignment == Print_Format::ALIGN::CENTER)
		{
			usize i;
			for(i = 0; i < pad_size / 2; ++i)
				result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
			pad_size -= i;
		}

		result += trait->write(str.bytes.template convert<byte>());

		//respect left and center align
		if (pad_size != 0 &&
			alignment == Print_Format::ALIGN::CENTER)
		{
			for(usize i = 0; i < pad_size; ++i)
				result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
		}
		else if(pad_size != 0 &&
				alignment == Print_Format::ALIGN::LEFT)
		{
			for(usize i = 0; i < pad_size; ++i)
				result += trait->write(make_slice((byte*)&format.pad, pad_str_size));
		}

		return result;
	}

	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait* trait, const Print_Format& format, Rune r)
	{
		return print_str(trait, format, make_strrng((byte*)&r, internal::_utf8_sequence_length(r.data)));
	}

	/**
	 * @brief      Prints a value in string form
	 *
	 * @param      trait   The IO_Trait to print to
	 * @param[in]  format  The format of the string
	 * @param[in]  value   The value to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
	inline static usize
	print_str(IO_Trait *trait, const Print_Format& format, const char* str)
	{
		return print_str(trait, format, make_strrng(str));
	}

	/**
	 * @brief      Prints values to the IO_Trait in a string form
	 *
	 * @param      trait      The IO_Trait to print to
	 * @param[in]  args       The arguments to be printed
	 *
	 * @tparam     TArgs      arguments types to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
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
		return print_str(trait, Print_Format(), str);
	}

	inline static usize
	vprintf(IO_Trait* trait, const char* str)
	{
		return vprintf(trait, make_strrng(str));
	}

	/**
	 * @brief      Prints a formated values
	 *
	 * @param      trait      The IO_Trait to print to
	 * @param[in]  format     The format string
	 * @param[in]  args       The arguments to be printed
	 *
	 * @tparam     TArgs      arguments types to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
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
								make_slice((byte*)rune_back_it.ptr,
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
					result += trait->write(make_slice(&open_brace));

					//go to the next rune and reset the back it
					++rune_forward_it;
					rune_back_it = rune_forward_it;
					//continue the upper loop
					continue;
				}

				Print_Format format;
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
						make_slice((byte*)rune_back_it.ptr,
									rune_forward_it.ptr - rune_back_it.ptr));
		}

		return result;
	}

	template<typename ... TArgs>
	inline static usize
	vprintf(IO_Trait* trait, const char* format, TArgs&& ... args)
	{
		return vprintf(trait, make_strrng(format), std::forward<TArgs>(args)...);
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

	/**
	 * @brief      Print values to the unbuffered stdout
	 *
	 * @param[in]  args       The arguments to be printed
	 *
	 * @tparam     TArgs      arguments types to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
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

	/**
	 * @brief      Print values to the unbuffered stderr
	 *
	 * @param[in]  args       The arguments to be printed
	 *
	 * @tparam     TArgs      arguments types to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
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

	/**
	 * @brief      Print values + newline at the end of the values to the unbuffered stdout
	 *
	 * @param[in]  args       The arguments to be printed
	 *
	 * @tparam     TArgs      arguments types to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
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

	/**
	 * @brief      Print values + newline at the end of the values to the unbuffered stderr
	 *
	 * @param[in]  args       The arguments to be printed
	 *
	 * @tparam     TArgs      arguments types to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
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

	/**
	 * @brief      Prints a formatted values to the unbuffered stdout
	 *
	 * @param[in]  format     The format string
	 * @param[in]  args       The arguments to be printed
	 *
	 * @tparam     TArgs      arguments types to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
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
	printf(const char* format, TArgs&& ... args)
	{
		return printf(make_strrng(format), std::forward<TArgs>(args)...);
	}

	/**
	 * @brief      Prints a formatted values to the unbuffered stderr
	 *
	 * @param[in]  format     The format string
	 * @param[in]  args       The arguments to be printed
	 *
	 * @tparam     TArgs      arguments types to be printed
	 *
	 * @return     size of the printed value in bytes
	 */
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

	template<typename ... TArgs>
	inline static usize
	printf_err(const char* format, TArgs&& ... args)
	{
		return printf_err(make_strrng(format), std::forward<TArgs>(args)...);
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

	/**
	 * @brief      Reads a value as binary format.
	 *
	 * @param      trait  The IO_Trait to read from
	 * @param      value  The value to be read
	 *
	 * @tparam     T      Type of the read value
	 *
	 * @return     size of the read value in bytes
	 */
	template<typename T>
	inline static usize
	read_bin(IO_Trait* trait, T& value)
	{
		return trait->read(make_slice((byte*)&value, sizeof(T)));
	}

	/**
	 * @brief      Reads a slice of values as binary format.
	 *
	 * @param      trait  The IO_Trait to read from
	 * @param      values  The slice of values to be read
	 *
	 * @tparam     T      Type of the read values
	 *
	 * @return     size of the read value in bytes
	 */
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

	/**
	 * @brief      Reads values from the IO_Trait in a binary form
	 *
	 * @param      trait  The IO_Trait to read from
	 * @param[in]  args   The values to be read
	 *
	 * @tparam     TArgs      Types of the args to be read
	 *
	 * @return     size of the read value in bytes
	 */
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

	/**
	 * @brief      Reads an address in a string form.
	 *
	 * @param      trait  The Bufio_Trait to read from
	 * @param      value  The value to be read
	 *
	 * @return     size of the read value in bytes
	 */
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

	/**
	 * @brief      Reads a value in a string form.
	 *
	 * @param      trait  The Bufio_Trait to read from
	 * @param      value  The value to be read
	 *
	 * @return     size of the read value in bytes
	 */
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

	/**
	 * @brief      Reads a value in a string form.
	 *
	 * @param      trait  The Bufio_Trait to read from
	 * @param      value  The value to be read
	 *
	 * @return     size of the read value in bytes
	 */
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

	inline static usize
	read_str(Bufio_Trait* trait, Rune& value)
	{
		_guarantee_text_chunk(trait, KILOBYTES(1));
		auto bytes = trait->peek();
		usize non_whitespace_count = 0;
		for(const auto& byte: bytes)
		{
			if(_is_whitespace(byte))
				break;
			++non_whitespace_count;
		}

		if(non_whitespace_count == 0)
			return 0;

		Rune rune;
		byte* result = (byte*)&rune;
		const byte* it = bytes.ptr;
		*result++ = *it++;
		while (*it && ((*it & 0xC0) == 0x80))
			*result++ = *it++;
		value = rune;
		return trait->skip(it - bytes.ptr);
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

	/**
	 * @brief      Reads values in a string form from the Bufio_Trait
	 *
	 * @param      trait  The Bufio_Trait to read from
	 * @param[in]  args   values to be read
	 *
	 * @tparam     TArgs      Types of the args to be read
	 *
	 * @return     size of the read values in bytes
	 */
	template<typename ... TArgs>
	inline static usize
	vreads(Bufio_Trait* trait, TArgs&& ... args)
	{
		usize result = 0;
		internal::_variadic_read_string_helper(trait, result, std::forward<TArgs>(args)...);
		return result;
	}

	/**
	 * @brief      Reads values in a string form from the buffered stdin
	 *
	 * @param[in]  args   values to be read
	 *
	 * @tparam     TArgs      Types of the args to be read
	 *
	 * @return     size of the read values in bytes
	 */
	template<typename ... TArgs>
	inline static usize
	read(TArgs&& ... args)
	{
		return vreads(os->buf_stdin, std::forward<TArgs>(args)...);
	}
}