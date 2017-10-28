#include "catch.hpp"
#include <cpprelude/hash_array.h>

#include <cpprelude/slinked_list.h>

using namespace cpprelude;

TEST_CASE("hash_array test", "[hash_array]")
{
	hash_array<usize, bool> array;

	REQUIRE(array.empty() == true);
	REQUIRE(array.count() == 0);

	SECTION("Case 01")
	{
		array.insert(100, true);
		CHECK(array.empty() == false);
		CHECK(array.count() == 1);
		CHECK(array[100] == true);

		for(usize i = 1;
			i <= 100;
			++i)
			array.insert(i, false);

		CHECK(array.empty() == false);
		CHECK(array.count() == 100);
		CHECK(array.capacity() >= 100);
		CHECK(array[100] == false);
	}

	SECTION("Case 02")
	{
		array.insert(120, true);

		CHECK(array[120] == true);
		array[100] = false;
		CHECK(array[100] == false);

		array.insert(1);
		auto it = array.lookup(1);
		CHECK(it != array.end());
		CHECK(*it == 1);
		it.value() = true;
		CHECK(array[1] == true);

		CHECK(array.lookup(12) == array.end());
		array.clear();

		CHECK(array.empty() == true);
		CHECK(array.count() == 0);

		array.reserve(1000);
		CHECK(array.capacity() >= 1000);
	}

	SECTION("Case 03")
	{
		for(usize i = 0;
			i < 128;
			++i)
		{
			if(i % 2 == 0)
				array.insert(i, true);
			else
				array.insert(i, false);
		}

		CHECK(array.empty() == false);
		CHECK(array.count() == 128);

		for(auto it = array.cbegin();
			it != array.cend();
			++it)
		{
			if(*it % 2 == 0)
				CHECK(it.value() == true);
			else
				CHECK(it.value() == false);
		}

		usize i = 0;
		for(auto& key: array.keys())
			++i;

		CHECK(i == array.count());

		i = 0;
		for(auto& value: array.values())
			++i;

		CHECK(i == array.count());

		for(i = 0;
			i < 128;
			++i)
			array.remove(i);


		CHECK(array.empty() == true);
		CHECK(array.count() == 0);
	}

	SECTION("Case 04")
	{
		for(usize i = 0;
			i < 128;
			++i)
		{
			array.insert(i, true);
		}

		for(usize i = 0;
			i < 256;
			++i)
		{
			if(i < 128)
				CHECK(array.remove(i));
			else
				CHECK(array.remove(i) == false);
		}

		CHECK(array.empty() == true);
		CHECK(array.count() == 0);

		for(usize i = 0;
			i < 2048;
			++i)
		{
			usize k = rand();
			array.insert(k, true);
		}

		slinked_list<usize> keys;

		for(auto& key: array.keys())
		{
			keys.insert_front(key);
		}

		CHECK(keys.count() == array.count());

		usize i = 0;
		for(auto& key: keys)
		{
			++i;
			if(i % 2 == 0)
			{
				CHECK(array.lookup(key) != array.end());
				CHECK(array.remove(key));
			}
			else
			{
				CHECK(array.remove(array.lookup(key)));
			}
		}

		CHECK(array.empty() == true);
		CHECK(array.count() == 0);
	}

	SECTION("Case 05")
	{
		hash_array<string, usize> str_hash;

		str_hash["abcd"] = 1;
		str_hash["ab"] = 1;
		str_hash["ba"] = 1;
		str_hash["dcba"] = 1;
		str_hash["fegh"] = 1;
		str_hash["cdab"] = 1;

		CHECK(str_hash.empty() == false);
		CHECK(str_hash.count() == 6);
	}
}
