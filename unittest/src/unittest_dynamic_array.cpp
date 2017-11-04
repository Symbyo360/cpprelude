#include "catch.hpp"
#include <cpprelude/dynamic_array.h>

using namespace cpprelude;

TEST_CASE("dynamic_array test", "[dynamic_array]")
{
	dynamic_array<i32> array;

	SECTION("Case 01")
	{
		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);

		for(usize i = 0; i < 128; ++i)
			array.insert_back(i);

		for(usize i = 0; i < 128; ++i)
			CHECK(array[i] == i);
	}

	SECTION("Case 02")
	{
		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);

		array.reserve(100);
		CHECK(array.capacity() >= 100);

		array.expand_back(100, -5);

		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == -5);
	}

	SECTION("Case 03")
	{
		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);

		for(usize i = 0; i < 128; ++i)
			array.insert_back(i);

		for(usize i = 0; i < 128; ++i)
			CHECK(array[i] == i);

		CHECK(array.count() == 128);
		CHECK(array.capacity() >= 128);
	}

	SECTION("Case 04")
	{
		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);

		array.expand_back(64, -1);

		auto it = array.data();
		for(usize i = 0; i < array.count(); ++i)
			CHECK(*it++ == -1);

		CHECK(array.count() == 64);
		CHECK(array.capacity() == 64);
	}

	SECTION("Case 05")
	{
		dynamic_array<i32> array2;

		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);
		CHECK(array2.count() == 0);
		CHECK(array2.capacity() == 0);

		array2 = array;

		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);
		CHECK(array2.count() == 0);
		CHECK(array2.capacity() == 0);

		for(i32 i = 0; i < 64; ++i)
			array.insert_back(i);

		CHECK(array.count() == 64);
		CHECK(array.capacity() >= 64);

		array2 = array;

		for(i32 i = 0; i < 64; ++i)
			CHECK(array2[i] == i);

		CHECK(array2.count() == array.count());
		CHECK(array2.capacity() == array.capacity());
	}

	SECTION("Case 06")
	{

		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);

		for(i32 i = 0; i < 64; ++i)
			array.insert_back(i);

		CHECK(array.count() == 64);
		CHECK(array.capacity() >= 64);

		dynamic_array<i32> array2(array);

		for(i32 i = 0; i < 64; ++i)
			CHECK(array2[i] == i);

		for(i32 i = 0; i < 64; ++i)
			CHECK(array[i] == i);

		CHECK(array2.count() == array.count());
		CHECK(array2.capacity() == array.capacity());
	}

	SECTION("Case 07")
	{

		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);

		for(usize i = 0; i < 64; ++i)
			array.insert_back(i);

		CHECK(array.count() == 64);
		CHECK(array.capacity() >= 64);

		dynamic_array<i32> array2(std::move(array));

		for(usize i = 0; i < 64; ++i)
			CHECK(array2[i] == i);

		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i);

		CHECK(array2.count() != array.count());
		CHECK(array2.capacity() != array.capacity());
		CHECK(array2.count() == 64);
		CHECK(array2.capacity() >= 64);
		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);
	}

	SECTION("Case 08")
	{
		dynamic_array<i32> array2;

		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);
		CHECK(array2.count() == 0);
		CHECK(array2.capacity() == 0);

		array2 = array;

		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);
		CHECK(array2.count() == 0);
		CHECK(array2.capacity() == 0);

		for(i32 i = 0; i < 64; ++i)
			array.insert_back(i);

		CHECK(array.count() == 64);
		CHECK(array.capacity() >= 64);

		array2 = std::move(array);

		for(i32 i = 0; i < 64; ++i)
			CHECK(array2[i] == i);

		CHECK(array2.count() != array.count());
		CHECK(array2.capacity() != array.capacity());
		CHECK(array2.count() == 64);
		CHECK(array2.capacity() >= 64);
		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);
	}

	SECTION("Case 09")
	{
		dynamic_array<i32> array2 = {1, 2, 3, 4, 5};
		for(usize i = 0; i < array2.count(); ++i)
			CHECK(array2[i] == i+1);
	}

	SECTION("Case 10")
	{
		array.insert_back({1, 2, 3, 4, 5});

		CHECK(array.count() == 5);
		CHECK(array.capacity() == 5);
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i+1);

		array.insert_back({6, 7, 8, 9});
		array.insert_back(10);

		CHECK(array.count() == 10);
		CHECK(array.capacity() >= 10);
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i+1);
	}

	SECTION("Case 11")
	{
		array.insert_back({1, 2, 3, 4, 5});

		CHECK(array.count() == 5);
		CHECK(array.capacity() == 5);
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i+1);

		array.insert_back({6, 7, 8, 9});
		array.insert_back(10);

		CHECK(array.count() == 10);
		CHECK(array.capacity() >= 10);
		for(usize i = 0; i < array.count(); ++i)
			CHECK(array[i] == i+1);

		array.clear();
		CHECK(array.count() == 0);
		CHECK(array.capacity() >= 10);

		array.insert_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
		CHECK(array.count() == 10);
		CHECK(array.capacity() >= 10);

		array.reset();
		CHECK(array.count() == 0);
		CHECK(array.capacity() == 0);

	}

	SECTION("Case 12")
	{
		array.insert_back({1, 2, 3, 4, 5, 6, 7, 8, 9});

		usize i = 1;
		for(auto number: array)
			CHECK(number == i++);

		i = 1;
		for(auto& number: array)
			CHECK(number == i++);

		i = 1;
		for(const auto& number: array)
			CHECK(number == i++);

		for(auto& number: array)
			number++;

		i = 2;
		for(const auto& number: array)
			CHECK(number == i++);
	}

	SECTION("Case 13")
	{
		CHECK(array.begin() == array.end());
		CHECK(array.empty() == true);

		array.insert_back({1, 2, 3, 4});

		CHECK(array.empty() == false);
		CHECK(array.begin() != array.end());

		auto it = array.begin();
		CHECK(it == array.begin());
		it = next(it);
		CHECK(it != array.begin());
		it = prev(it);
		CHECK(it == array.begin());

		it = next(it);
		it = next(it);
		it = next(it);
		CHECK(it != array.begin());
		it = next(it);
		CHECK(it == array.end());
	}

	SECTION("Case 14")
	{
		for(usize i = 0; i < 64; i++)
			array.insert_back(i);

		auto old_capacity = array.capacity();
		array.shrink_to_fit();
		CHECK(old_capacity >= array.capacity());
		CHECK(array.capacity() == 64);

		usize i = 0;
		for(auto number: array)
			CHECK(number == i++);

		array.shrink_back(32);
		CHECK(array.count() == 32);
		CHECK(array.capacity() == 32);

		i = 0;
		for(auto number: array)
			CHECK(number == i++);
	}

	SECTION("Case 15")
	{
		array.insert_back({1, 2, 3, 4, 5});
		array.remove_back(2);
		CHECK(array.count() == 3);
		usize i = 1;
		for(auto number: array)
			CHECK(number == i++);
	}
}