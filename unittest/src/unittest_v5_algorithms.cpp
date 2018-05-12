#include "catch.hpp"
#include <cpprelude/Algorithms.h>
#include <cpprelude/Dynamic_Array.h>

using namespace cppr;

TEST_CASE("algorithms", "[algorithms]")
{
	SECTION("Case 01")
	{
		CHECK(min(1, 3) == 1);
		CHECK(min(4, -1) == -1);
		CHECK(min(4, 4) == 4);
	}

	SECTION("Case 02")
	{
		CHECK(max(1, 3) == 3);
		CHECK(max(4, -1) == 4);
		CHECK(min(4, 4) == 4);
	}

	SECTION("Case 03")
	{
		CHECK(clamp(1, -2, 2) == 1);
		CHECK(clamp(4, -2, 2) == 2);
		CHECK(clamp(-3, -2, 2) == -2);
		CHECK(clamp(-2, -2, 2) == -2);
		CHECK(clamp(2, -2, 2) == 2);
	}

	SECTION("Case 04")
	{
		Dynamic_Array<usize> array({1, 2, 3, 4, 5, 6});
		CHECK(is_sorted(array.all()) == true);
	}

	SECTION("Case 05")
	{
		Dynamic_Array<usize> array;
		heap_sort(array.all());
		CHECK(is_sorted(array.all()) == true);

		array.insert_back(1);
		heap_sort(array.all());
		CHECK(is_sorted(array.all()) == true);

		for(usize i = 0; i < 10; ++i)
			array.insert_back(i);
		shuffle(array.all());
		CHECK(is_sorted(array.all()) == false);
		heap_sort(array.all());
		CHECK(is_sorted(array.all()) == true);
	}

	SECTION("Case 06")
	{
		Dynamic_Array<usize> array;
		merge_sort(array.all());
		CHECK(is_sorted(array.all()) == true);

		array.insert_back(1);
		merge_sort(array.all());
		CHECK(is_sorted(array.all()) == true);

		for(usize i = 0; i < 10; ++i)
			array.insert_back(i);
		shuffle(array.all());
		CHECK(is_sorted(array.all()) == false);
		merge_sort(array.all());
		CHECK(is_sorted(array.all()) == true);
	}

	SECTION("Case 07")
	{
		Dynamic_Array<usize> array;
		quick_sort(array.all());
		CHECK(is_sorted(array.all()) == true);

		array.insert_back(1);
		quick_sort(array.all());
		CHECK(is_sorted(array.all()) == true);

		for(usize i = 0; i < 10; ++i)
			array.insert_back(i);
		shuffle(array.all());
		CHECK(is_sorted(array.all()) == false);
		quick_sort(array.all());
		CHECK(is_sorted(array.all()) == true);
	}
}