#include "catch.hpp"
#include <cpprelude/v5/Memory_Stream.h>
#include <cpprelude/v5/Buffered_Stream.h>
#include <cpprelude/v5/String.h>
#include <cpprelude/v5/IO.h>

using namespace cpprelude;

TEST_CASE("Memory_Stream v5", "[Memory_Stream]")
{
	SECTION("Case 01")
	{
		Memory_Stream stream;

		CHECK(stream.size() == 0);
		CHECK(stream.empty() == true);
		CHECK(stream.capacity() == 0);
	}

	SECTION("Case 02")
	{
		Memory_Stream stream;

		i32 a = 123;
		CHECK(vprints(stream, a) == 3);
		a *= -1;
		CHECK(vprints(stream, a) == 4);

		CHECK(stream.empty() == false);
		CHECK(stream.size() == 7);

		stream.clear();

		CHECK(stream.empty() == true);
		CHECK(stream.size() == 0);

		stream.reserve(100);

		CHECK(stream.empty() == true);
		CHECK(stream.size() == 0);
		CHECK(stream.capacity() >= 100);

		stream.reset();

		CHECK(stream.empty() == true);
		CHECK(stream.size() == 0);
		CHECK(stream.capacity() == 0);
	}

	SECTION("Case 03")
	{
		Memory_Stream stream;

		CHECK(vprintf(stream, "{} {}\n", "Mostafa", "سعد") != 0);
		CHECK(stream.move_to_start() == true);

		Buffered_Stream buf_stream(stream);
		String first_name, last_name;
		CHECK(vreads(buf_stream, first_name, last_name) != 0);
		CHECK(first_name == "Mostafa");
		CHECK(last_name == "سعد");
	}

	SECTION("Case 04")
	{
		Memory_Stream stream;

		CHECK(vprintf(stream, "{} {}\n", "Mostafa", "سعد") != 0);
		CHECK(stream.move_to_start() == true);

		Memory_Stream other(std::move(stream));

		Buffered_Stream buf_stream(other);
		String first_name, last_name;
		CHECK(vreads(buf_stream, first_name, last_name) != 0);
		CHECK(first_name == "Mostafa");
		CHECK(last_name == "سعد");
	}
}
