#include "catch.hpp"
#include <cpprelude/v5/dynamic_array.h>

using namespace cpprelude;

TEST_CASE("dynamic_array v5", "[dynamic_array]")
{
	SECTION("Case 01")
	{
		Dynamic_Array<usize> array;
		CHECK(array.capacity() == 0);
		CHECK(array.empty() == true);
		CHECK(array.count() == 0);
	}

	SECTION("Case 02")
	{
		Dynamic_Array<usize> array({1, 2, 3, 4});
		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);
	}

	SECTION("Case 03")
	{
		Dynamic_Array<usize> array(5);
		CHECK(array.capacity() >= 5);
		CHECK(array.empty() == false);
		CHECK(array.count() == 5);
	}

	SECTION("Case 04")
	{
		Dynamic_Array<usize> array(5, 7);
		CHECK(array.capacity() >= 5);
		CHECK(array.empty() == false);
		CHECK(array.count() == 5);

		for(auto range = array.all(); !range.empty(); range.pop_front())
			CHECK(range.front() == 7);
	}

	SECTION("Case 05")
	{
		Dynamic_Array<usize> array({1, 2, 3, 4});
		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		Dynamic_Array<usize> other(array);
		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		CHECK(other.capacity() >= 4);
		CHECK(other.empty() == false);
		CHECK(other.count() == 4);

		CHECK(other[0] == 1);
		CHECK(other[1] == 2);
		CHECK(other[2] == 3);
		CHECK(other[3] == 4);
	}

	SECTION("Case 06")
	{
		Dynamic_Array<usize> array(5, 7);
		CHECK(array.capacity() >= 5);
		CHECK(array.empty() == false);
		CHECK(array.count() == 5);

		Dynamic_Array<usize> other(std::move(array));
		CHECK(array.capacity() == 0);
		CHECK(array.empty() == true);
		CHECK(array.count() == 0);

		CHECK(other.capacity() >= 5);
		CHECK(other.empty() == false);
		CHECK(other.count() == 5);

		for(auto range = other.all(); !range.empty(); range.pop_front())
			CHECK(range.front() == 7);
	}

	SECTION("Case 07")
	{
		Dynamic_Array<usize> array({1, 2, 3, 4});
		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		Dynamic_Array<usize> other = array;
		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		CHECK(other.capacity() >= 4);
		CHECK(other.empty() == false);
		CHECK(other.count() == 4);
	}

	SECTION("Case 08")
	{
		Dynamic_Array<usize> array(5, 7);
		CHECK(array.capacity() >= 5);
		CHECK(array.empty() == false);
		CHECK(array.count() == 5);

		Dynamic_Array<usize> other(std::move(array));
		CHECK(array.capacity() == 0);
		CHECK(array.empty() == true);
		CHECK(array.count() == 0);

		CHECK(other.capacity() >= 5);
		CHECK(other.empty() == false);
		CHECK(other.count() == 5);

		for(auto range = other.all(); !range.empty(); range.pop_front())
			CHECK(range.front() == 7);

		for(auto range = array.all(); !range.empty(); range.pop_front())
			CHECK(false);
	}

	SECTION("Case 09")
	{
		Dynamic_Array<usize> array;
		CHECK(array.capacity() == 0);
		array.reserve(10);
		CHECK(array.capacity() >= 10);
	}

	SECTION("Case 10")
	{
		Dynamic_Array<usize> array;
		CHECK(array.capacity() == 0);
		CHECK(array.count() == 0);

		array.expand_back(10);
		CHECK(array.capacity() >= 10);
		CHECK(array.count() == 10);
	}

	SECTION("Case 11")
	{
		Dynamic_Array<usize> array;
		CHECK(array.capacity() == 0);
		CHECK(array.count() == 0);

		array.expand_back(10, 7);
		CHECK(array.capacity() >= 10);
		CHECK(array.count() == 10);

		for(auto range = array.all(); !range.empty(); range.pop_front())
			CHECK(range.front() == 7);
	}

	SECTION("Case 12")
	{
		Dynamic_Array<usize> array;
		array.shrink_back(10);

		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);
		CHECK(array.empty() == true);

		array.insert_back(7);
		array.shrink_back(10);

		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);
		CHECK(array.empty() == true);
	}

	SECTION("Case 13")
	{
		Dynamic_Array<usize> array({1, 2, 3, 4});
		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		array.remove_back();

		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == false);
		CHECK(array.count() == 3);

		array.remove_back(10);

		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == true);
		CHECK(array.count() == 0);
	}

	SECTION("Case 14")
	{
		Dynamic_Array<usize> array({1, 2, 3, 4});
		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		array.clear();
		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == true);
		CHECK(array.count() == 0);
	}

	SECTION("Case 15")
	{
		Dynamic_Array<usize> array({1, 2, 3, 4});
		CHECK(array.capacity() >= 4);
		CHECK(array.empty() == false);
		CHECK(array.count() == 4);

		array.reset();
		CHECK(array.capacity() >= 0);
		CHECK(array.empty() == true);
		CHECK(array.count() == 0);
	}

	SECTION("Case 16")
	{
		Dynamic_Array<usize> array;

		array.reserve(100);
		CHECK(array.capacity() >= 100);

		array.insert_back(7);
		array.shrink_to_fit();
		CHECK(array.capacity() == 1);
	}

	SECTION("Case 17")
	{
		Dynamic_Array<usize> array({1, 2, 3, 4});
		CHECK(array.front() == 1);
		CHECK(array.back() == 4);
	}
}