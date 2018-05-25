#include "catch.hpp"
#include <cpprelude/Array.h>

using namespace cppr;


TEST_CASE("Array test", "[array]")
{
	Array<i32, 128> array1;

	for(usize i = 0; i < array1.count(); ++i)
		array1[i] = i;

	for(usize i = 0; i < array1.count(); ++i)
		REQUIRE(array1[i] == i);

	SECTION("Case 01")
	{
		CHECK(array1.count() == 128);

		for(usize i = 0; i < array1.count(); ++i)
			CHECK(array1[i] == i);
	}

	SECTION("Case 02")
	{
		Array<i32, 128> array2;
		CHECK(array1.count() == 128);
		CHECK(array2.count() == 128);

		array2 = array1;

		for(usize i = 0; i < array1.count(); ++i)
			CHECK(array2[i] == i);
	}

	SECTION("Case 03")
	{
		Array<i32, 128> array2;
		CHECK(array1.count() == 128);
		CHECK(array2.count() == 128);

		auto it1 = array1.data();
		auto it2 = array2.data();
		for(usize i = 0; i < array1.count(); ++i)
			*(it2++) = *(it1++);

		it2 = array2.data();
		for(usize i = 0; i < array2.count(); ++i)
			CHECK(*it2++ == i);
	}

	SECTION("Case 04")
	{
		Array<i32, 5> array2 = {1, 2, 3, 4, 5};

		for(usize i = 0; i < array2.count(); ++i)
			CHECK(array2[i] == i+1);
	}
}
