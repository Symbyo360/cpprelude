#include "catch.hpp"
#include <cpprelude/Hash_Map.h>
#include <cpprelude/String.h>

using namespace cppr;

TEST_CASE("Hash_Map", "[Hash_Map]")
{
	SECTION("Case 01")
	{
		Hash_Map<usize, bool> map;

		CHECK(map.count() == 0);
		CHECK(map.empty() == true);
		CHECK(map.capacity() == 0);
	}

	SECTION("Case 02")
	{
		Hash_Map<usize, bool> map;

		CHECK(map.count() == 0);
		CHECK(map.empty() == true);
		CHECK(map.capacity() == 0);

		Hash_Map<usize, bool> other(map);

		CHECK(other.count() == 0);
		CHECK(other.empty() == true);
		CHECK(other.capacity() == 0);
	}

	SECTION("Case 03")
	{
		Hash_Map<usize, bool> map;

		CHECK(map.count() == 0);
		CHECK(map.empty() == true);
		CHECK(map.capacity() == 0);

		map.reserve(10);
		CHECK(map.count() == 0);
		CHECK(map.empty() == true);
		CHECK(map.capacity() >= 10);
	}

	SECTION("Case 04")
	{
		Hash_Map<usize, bool> map;

		for(usize i = 0; i < 64; ++i)
			CHECK(map.insert(i)->key == i);

		CHECK(map.count() == 64);
		CHECK(map.empty() == false);
		CHECK(map.capacity() >= 64);
	}

	SECTION("Case 05")
	{
		Hash_Map<usize, bool> map;

		for(usize i = 0; i < 64; ++i)
			map[i] = true;

		CHECK(map.count() == 64);
		CHECK(map.empty() == false);
		CHECK(map.capacity() >= 64);
	}

	SECTION("Case 06")
	{
		Hash_Map<usize, bool> map;

		for(usize i = 0; i < 64; ++i)
			map[i] = true;

		Hash_Map<usize, bool> other(map);

		CHECK(map.count() == 64);
		CHECK(map.empty() == false);
		CHECK(map.capacity() >= 64);

		CHECK(other.count() == 64);
		CHECK(other.empty() == false);
		CHECK(other.capacity() >= 64);
	}

	SECTION("Case 07")
	{
		Hash_Map<usize, bool> map;

		for(usize i = 0; i < 64; ++i)
			map[i] = true;

		Hash_Map<usize, bool> other(std::move(map));

		CHECK(other.count() == 64);
		CHECK(other.empty() == false);
		CHECK(other.capacity() >= 64);
	}

	SECTION("Case 08")
	{
		Hash_Map<usize, bool> map;

		for(usize i = 0; i < 64; ++i)
			map[i] = true;

		Hash_Map<usize, bool> other;
		other = map;

		CHECK(map.count() == 64);
		CHECK(map.empty() == false);
		CHECK(map.capacity() >= 64);

		CHECK(other.count() == 64);
		CHECK(other.empty() == false);
		CHECK(other.capacity() >= 64);
	}

	SECTION("Case 09")
	{
		Hash_Map<usize, bool> map;

		for(usize i = 0; i < 64; ++i)
			map[i] = true;

		Hash_Map<usize, bool> other;
		other = std::move(map);

		CHECK(other.count() == 64);
		CHECK(other.empty() == false);
		CHECK(other.capacity() >= 64);
	}

	SECTION("Case 10")
	{
		Hash_Map<usize, bool> map;

		map[1] = true;
		map[2] = true;
		map[3] = true;
		map[4] = true;
		map[5] = true;

		CHECK(map.lookup(1)->key == 1);
		CHECK(map.lookup(2)->value == true);
		CHECK(map.lookup(6) == map.end());
	}

	SECTION("Case 11")
	{
		Hash_Map<usize, bool> map;

		for(usize i = 0; i < 64; ++i)
			map[i] = true;

		for(usize i = 0; i < 64; ++i)
			if(i % 2 == 0)
				map.remove(i);

		CHECK(map.count() == 32);
		CHECK(map.empty() == false);
		CHECK(map.capacity() >= 64);
	}

	SECTION("Case 12")
	{
		Hash_Map<usize, bool> map;

		for(usize i = 0; i < 64; ++i)
			map[i] = true;

		for(usize i = 0; i < 64; ++i)
			if(i % 2 == 0)
				map.remove(i);

		CHECK(map.count() == 32);
		CHECK(map.empty() == false);
		CHECK(map.capacity() >= 64);

		map.clear();

		CHECK(map.count() == 0);
		CHECK(map.empty() == true);
		CHECK(map.capacity() >= 64);
	}

	SECTION("Case 13")
	{
		Hash_Map<usize, bool> map;

		for(usize i = 0; i < 64; ++i)
			map[i] = true;

		for(usize i = 0; i < 64; ++i)
			if(i % 2 == 0)
				map.remove(i);

		CHECK(map.count() == 32);
		CHECK(map.empty() == false);
		CHECK(map.capacity() >= 64);

		map.reset();

		CHECK(map.count() == 0);
		CHECK(map.empty() == true);
		CHECK(map.capacity() == 0);
	}

	SECTION("Case 14")
	{
		Hash_Map<usize, bool> map;

		for(usize i = 0; i < 64; ++i)
			map[i] = true;

		for(usize i = 0; i < 64; ++i)
			if(i % 2 == 0)
				map.remove(i);

		usize i = 1;
		for(const auto& num: map)
		{
			CHECK(num.key == i);
			i += 2;
		}
	}

	SECTION("Case 15")
	{
		Hash_Map<String, usize> map;

		map["Moka"]++;
		map["Moka"]++;
		map["Moka"]++;

		map["Koko"]++;

		CHECK(map.count() == 2);
		CHECK(map.empty() == false);

		CHECK(map.lookup("Moka")->value == 3);
		CHECK(map.lookup("Koko")->value == 1);

		Hash_Map<String, usize> other(map);

		CHECK(other.count() == 2);
		CHECK(other.empty() == false);

		CHECK(other.lookup("Moka")->value == 3);
		CHECK(other.lookup("Koko")->value == 1);
	}

	struct Custom_Type{};

	SECTION("Case 16")
	{
		Custom_Type a, b, c;

		Hash_Map<Custom_Type*, usize> map;

		map[&a]++;
		map[&b]++;
		map[&a]++;

		CHECK(map.count() == 2);
		CHECK(map[&a] == 2);
		CHECK(map[&b] == 1);
		CHECK(map.lookup(&c) == map.end());
	}
}