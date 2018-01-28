#include "catch.hpp"
#include <cpprelude/stream.h>
#include <cpprelude/file.h>
#include <cpprelude/bufio.h>
#include <cpprelude/fmt.h>
#include <cpprelude/string.h>
#include <stdio.h>

using namespace cpprelude;

TEST_CASE("buf_reader test", "[buf_reader]")
{
	SECTION("Case 01")
	{
		memory_stream mem_stream;

		u64 x = 0;
		CHECK(vprintb(mem_stream, x) == sizeof(u64));

		x = 123;
		CHECK(vprintb(mem_stream, x) == sizeof(u64));

		CHECK(mem_stream.move_to_start() == true);

		buf_reader reader(mem_stream);
		CHECK(vscanb(reader, x) == 1);
		CHECK(x == 0);
		CHECK(vscanb(reader, x) == 1);
		CHECK(x == 123);
	}

	SECTION("Case 02")
	{
		memory_stream mem_stream;

		CHECK(vprints(mem_stream, "my name is mostafa\n") != 0);
		CHECK(vprints(mem_stream, "my age is 24\n") != 0);

		CHECK(mem_stream.move_to_start() == true);

		buf_reader reader(mem_stream);
		string str;

		CHECK(vscanln(reader, str) != 0);
		CHECK(str == "my name is mostafa"_cs);

		CHECK(vscanln(reader, str) != 0);
		CHECK(str == "my age is 24"_cs);

		CHECK(vscanln(reader, str) == 0);
	}

	SECTION("Case 03")
	{
		auto stream = view_string_as_memory_stream("1 123 123456 123456789"_cs);
		buf_reader reader(stream);
		i8 a;
		i16 b;
		i32 c;
		i64 d;

		CHECK(vscans(reader, a, b, c, d) == 4);
		CHECK(a == 1);
		CHECK(b == 123);
		CHECK(c == 123456);
		CHECK(d == 123456789);
	}

	SECTION("Case 04")
	{
		auto stream = view_string_as_memory_stream("1 123 123456 123456789"_cs);
		buf_reader reader(stream);
		u8 a;
		u16 b;
		u32 c;
		u64 d;

		CHECK(vscans(reader, a, b, c, d) == 4);
		CHECK(a == 1);
		CHECK(b == 123);
		CHECK(c == 123456);
		CHECK(d == 123456789);
	}

	SECTION("Case 05")
	{
		auto stream = view_string_as_memory_stream("-1 -123 -123456 -123456789"_cs);
		buf_reader reader(stream);
		i8 a;
		i16 b;
		i32 c;
		i64 d;

		CHECK(vscans(reader, a, b, c, d) == 4);
		CHECK(a == -1);
		CHECK(b == -123);
		CHECK(c == -123456);
		CHECK(d == -123456789);
	}

	SECTION("Case 06")
	{
		auto stream = view_string_as_memory_stream("1234.123141 123.685875678"_cs);
		buf_reader reader(stream);
		r32 a;
		r64 b;

		CHECK(vscans(reader, a, b) == 2);
		CHECK(a == 1234.123141f);
		CHECK(b == 123.685875678);
	}

	SECTION("Case 07")
	{
		auto stream = view_string_as_memory_stream("abcd"_cs);
		buf_reader reader(stream);
		byte a,b,c,d;

		CHECK(vscans(reader, a, b, c, d) == 4);
		CHECK(a == 'a');
		CHECK(b == 'b');
		CHECK(c == 'c');
		CHECK(d == 'd');
	}
}

TEST_CASE("buf_writer test", "[buf_writer]")
{
	SECTION("Case 01")
	{
		memory_stream mem_stream;

		buf_writer writer(mem_stream);

		u64 x = 0;
		CHECK(vprintb(writer, x) == sizeof(u64));

		x = 1234;
		CHECK(vprintb(writer, x) == sizeof(u64));

		CHECK(vprints(writer, x) == 4);

		CHECK(writer.flush() <= (2 * sizeof(u64)) + 4);

		CHECK(writer.available() == 0);
	}
}
