#include <cpprelude/bucket_array.h>
#include <cpprelude/dlinked_list.h>
#include <cpprelude/slinked_list.h>
#include <cpprelude/dynamic_array.h>
#include <cpprelude/hash_array.h>
#include <cpprelude/tree_map.h>
#include <algorithm>
#include "catch.hpp"

using namespace cpprelude;

TEST_CASE("dynamic array stl test", "[dynamic_array stl]")
{
	dynamic_array<i32> arr;
	
	//Modifying sequence operations
	SECTION("Case 01")
	{
		arr = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 2, 32, 24, 140 };
		std::remove(arr.begin(), arr.end(), 2);
		CHECK_FALSE(arr.count() == 12);
		CHECK(std::find(arr.begin(), arr.end(), 2) == arr.end());
	}
	
	SECTION("Case 02")
	{
		for (int i = 0; i < 20; i++)
			arr.insert_back(i);

		std::reverse(arr.begin(), arr.end());

		for (int i = 19; i >= 0; i--)
			CHECK(arr[19 - i] == i);
	}

	//sorting operations
	SECTION("Case 03")
	{
		for (int i = 0; i < 20; i++)
			arr.insert_back(rand());

		std::sort(arr.begin(), arr.end());

		CHECK(std::is_sorted(arr.begin(), arr.end()));
	}
	
	SECTION("Case 04")
	{
		for (int i = 0; i < 20; i++)
			arr.insert_back(rand());

		std::stable_sort(arr.begin(), arr.end());

		CHECK(std::is_sorted(arr.begin(), arr.end()));
	}

	SECTION("Case 05")
	{
		for (int i = 0; i < 10000; ++i)
			arr.insert_back(rand());

		std::sort(arr.begin(), arr.end());

		CHECK(std::is_sorted(arr.begin(), arr.end()));
	}

	SECTION("Case 06")
	{
		for (int i = 0; i < 10000; ++i)
			arr.insert_back(rand());

		std::stable_sort(arr.begin(), arr.end());

		CHECK(std::is_sorted(arr.begin(), arr.end()));
	}

	SECTION("Case 07")
	{
		arr = { 1, 3, 5, 7, 9, 13, 20, 10, 32, 24, 140 };
		CHECK(*(std::is_sorted_until(arr.begin(), arr.end())) == 10);
	}
	
	SECTION("Case 08")
	{
		arr = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 32, 24, 140 };
		std::partial_sort(arr.begin(), arr.begin() + 7 , arr.end());
		CHECK(std::is_sorted_until(arr.begin(), arr.end()) == arr.begin() + 8);
	}

	SECTION("Case 09")
	{
		arr = { 5, 6, 4, 3, 2, 6, 7, 9, 3 };
		std::nth_element(arr.begin(), arr.begin() + arr.count() / 2, arr.end());
		CHECK(arr[arr.count() / 2] == 5);
	}

	//Binary search operations ( on sorted ranges )
	SECTION("Case 10")
	{
		for (int i = 0; i < 10; i++)
			arr.insert_back(i);

		CHECK(*(std::lower_bound(arr.begin(), arr.end(), 5)) == 5);
	}

	SECTION("Case 11")
	{		
		for (int i = 0; i < 10; i++)
			arr.insert_back(i);

		CHECK(*(std::upper_bound(arr.begin(), arr.end(), 5)) == 6);
	}

	SECTION("Case 12")
	{
		arr = { 1, 3, 5, 7, 9, 13, 20, 140 };
		CHECK(std::binary_search(arr.begin(), arr.end(), 13));
		CHECK_FALSE(std::binary_search(arr.begin(), arr.end(), 25));
	}
	
	//Set operations ( on sorted ranges )
	SECTION("Case 13")
	{
		arr = { 1, 3, 5, 7, 9, 13, 20, 140 };
		dynamic_array<i32> d = { 3, 5, 9 };

		CHECK(std::includes(arr.begin(), arr.end(), d.begin(), d.end()));

		d = {4, 15, 100};
	    CHECK_FALSE(std::includes(arr.begin(), arr.end(), d.begin(), d.end()));
	}
}

TEST_CASE("bucket array stl test", "[bucket_array stl]")
{
	bucket_array<int, 5> b_arr;
	
	//Modifying sequence operations
	SECTION("Case 01")
	{
		for (int i = 0; i < 40; i++)
			b_arr.insert_back(i);

		CHECK_FALSE(std::find(b_arr.begin(), b_arr.end(), 4)== b_arr.end());

		std::remove(b_arr.begin(), b_arr.end(), 4);

		CHECK(std::find(b_arr.begin(), b_arr.end(), 4) == b_arr.end());
	}

	SECTION("Case 02")
	{
		for (int i = 0; i < 20; i++)
			b_arr.insert_back(i);
		
		std::reverse(b_arr.begin(), b_arr.end());

		for (int i = 19; i >= 0; i--)
			CHECK(b_arr[19 - i] == i);
	}
	
	//sorting operations
	SECTION("Case 03")
	{
		for (int i = 0; i < 20; i++)
			b_arr.insert_back(rand());

		std::sort(b_arr.begin(), b_arr.end());

		CHECK(std::is_sorted(b_arr.begin(), b_arr.end()));
	}

	SECTION("Case 04")
	{
		for (int i = 0; i < 20; i++)
			b_arr.insert_back(rand());

		std::stable_sort(b_arr.begin(), b_arr.end());

		CHECK(std::is_sorted(b_arr.begin(), b_arr.end()));
	}

	SECTION("Case 05")
	{
		for (int i = 0; i < 10001; ++i)
			b_arr.insert_back(rand());

		std::sort(b_arr.begin(), b_arr.end());

		CHECK(std::is_sorted(b_arr.begin(), b_arr.end()));
	}
	
	SECTION("Case 06")
	{
		for (int i = 0; i < 10001; ++i)
			b_arr.insert_back(rand());

		std::stable_sort(b_arr.begin(), b_arr.end());

		CHECK(std::is_sorted(b_arr.begin(), b_arr.end()));
	}

	SECTION("Case 07")
	{
		b_arr = { 1, 3, 5, 7, 9, 13, 20, 10, 32, 24, 140 };
		CHECK(*(std::is_sorted_until(b_arr.begin(), b_arr.end())) == 10);
	}

	SECTION("Case 08")
	{
		b_arr = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 32, 24, 140, 90, 120, 4 };
		std::partial_sort(b_arr.begin(), b_arr.begin() + 7, b_arr.end());
		CHECK(std::is_sorted_until(b_arr.begin(), b_arr.end()) == b_arr.begin() + 8);
	}

	SECTION("Case 09")
	{
		b_arr = { 5, 6, 4, 3, 2, 6, 7, 9, 3 };
		std::nth_element(b_arr.begin(), b_arr.begin() + b_arr.count() / 2, b_arr.end());
		CHECK(b_arr[b_arr.count() / 2] == 5 );
	}

	//Binary search operations ( on sorted ranges )
	SECTION("Case 10")
	{
		b_arr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		CHECK(*(std::lower_bound(b_arr.begin(), b_arr.end(), 5)) == 5);
	}

	SECTION("Case 11")
	{
		b_arr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		CHECK(*(std::upper_bound(b_arr.begin(), b_arr.end(), 5)) == 6);
	}

	SECTION("Case 12")
	{
		b_arr = { 1, 3, 5, 7, 9, 13, 20, 140 };
		CHECK(std::binary_search(b_arr.begin(), b_arr.end(), 13));
		CHECK_FALSE(std::binary_search(b_arr.begin(), b_arr.end(), 25));
	}

	//Set operations ( on sorted ranges )
	SECTION("Case 13")
	{
		b_arr = { 1, 3, 5, 7, 9, 13, 20, 140 };
		dynamic_array<i32> d = { 3, 5, 9 };

		CHECK(std::includes(b_arr.begin(), b_arr.end(), d.begin(), d.end()));

		d = { 4, 15, 100 };
		CHECK_FALSE(std::includes(b_arr.begin(), b_arr.end(), d.begin(), d.end()));
	}
}

TEST_CASE("doubly linked list stl test", "[dlinked_list stl]")
{
	dlinked_list<i32> dlist;

	//Modifying sequence operations
	SECTION("Case 01")
	{
		dlist = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 2, 32, 24, 140 };

		std::remove(dlist.begin(), dlist.end(), 2);

		CHECK_FALSE(dlist.count() == 12);
		CHECK(std::find(dlist.begin(), dlist.end(), 2) == dlist.end());
	}

	SECTION("Case 02")
	{
		for (int i = 0; i < 20; i++)
			dlist.insert_back(i);

		std::reverse(dlist.begin(), dlist.end());

		for (int i = 19; i >= 0; i--)
			CHECK(dlist[19 - i] == i);
	}


	SECTION("Case 03")
	{
		dlist = { 1, 3, 5, 7, 9, 13, 20, 10, 32, 24, 140 };
		CHECK(*(std::is_sorted_until(dlist.begin(), dlist.end())) == 10);
	}

	//Binary search operations ( on sorted ranges )
	SECTION("Case 04")
	{
		for (int i = 0; i < 10; i++)
			dlist.insert_back(i);

		CHECK(*(std::lower_bound(dlist.begin(), dlist.end(), 5)) == 5);
	}

	SECTION("Case 05")
	{
		for (int i = 0; i < 10; i++)
			dlist.insert_back(i);

		CHECK(*(std::upper_bound(dlist.begin(), dlist.end(), 5)) == 6);
	}

	SECTION("Case 06")
	{
		dlist = { 1, 3, 5, 7, 9, 13, 20, 140 };
		CHECK(std::binary_search(dlist.begin(), dlist.end(), 13));
		CHECK_FALSE(std::binary_search(dlist.begin(), dlist.end(), 25));
	}

	//Set operations ( on sorted ranges )
	SECTION("Case 07")
	{
		dlist = { 1, 3, 5, 7, 9, 13, 20, 140 };
		dlinked_list <i32> d = { 3, 5, 9 };

		CHECK(std::includes(dlist.begin(), dlist.end(), d.begin(), d.end()));

		d = { 4, 15, 100 };
		CHECK_FALSE(std::includes(dlist.begin(), dlist.end(), d.begin(), d.end()));
	}
}

TEST_CASE("singly linked list stl test", "[slinked_list stl]")
{
	slinked_list<i32> slist;

	//Modifying sequence operations
	SECTION("Case 01")
	{
		slist = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 2, 32, 24, 140 };

		std::remove(slist.begin(), slist.end(), 2);
		CHECK_FALSE(slist.count() == 12);
		CHECK(std::find(slist.begin(), slist.end(), 2) == slist.end());
	}

	//sorting operations
	SECTION("Case 02")
	{
		slist = { 1, 3, 5, 7, 9, 13, 20, 10, 32, 24, 140 };
		CHECK(*(std::is_sorted_until(slist.begin(), slist.end())) == 10);
	}

	// Binary search operations (on sorted ranges)
	SECTION("Case 03")
	{
		for (int i = 9; i >= 0; --i)
			slist.insert_front(i);

		CHECK(*(std::lower_bound(slist.begin(), slist.end(), 5)) == 5);
	}

	SECTION("Case 04")
	{
		for (int i = 9; i >= 0; --i)
			slist.insert_front(i);

		CHECK(*(std::upper_bound(slist.begin(), slist.end(), 5)) == 6);
	}

	SECTION("Case 05")
	{
		slist = { 1, 3, 5, 7, 9, 13, 20, 140 };
		CHECK(std::binary_search(slist.begin(), slist.end(), 13));
		CHECK_FALSE(std::binary_search(slist.begin(), slist.end(), 25));
	}

	//Set operations ( on sorted ranges )
	SECTION("Case 06")
	{
		slist = { 1, 3, 5, 7, 9, 13, 20, 140 };
		slinked_list<i32> d = { 3, 5, 9 };

		CHECK(std::includes(slist.begin(), slist.end(), d.begin(), d.end()));

		d = { 4, 15, 100 };
		CHECK_FALSE(std::includes(slist.begin(), slist.end(), d.begin(), d.end()));
	}
}

TEST_CASE("hash array stl test", "[hash_array stl]")
{
	//NOTE: some operations for forward iterator will not be available for hash_array
	//for it has a key, the algorithms implemented in STL deal with the key as constant
	//i.e: remove operation can not be performed on hash arrays
	hash_array<i32, i32> h_arr;
	hash_array<i32, byte> ch_arr;
	std::vector<int> v;

	SECTION("Case 01")
	{
		for (int i = 1000; i >= 0; --i)
			h_arr.insert(i, i + 24);

		CHECK(*(std::lower_bound(h_arr.begin(), h_arr.end(), 40)) == 40);
	}

	SECTION("Case 02")
	{
		for (int i = 1000; i >= 0; --i)
			h_arr.insert(i, i + 9);

		CHECK(*(std::upper_bound(h_arr.begin(), h_arr.end(), 10)) == 11);
	}

	//Set operations
	SECTION("Case 03")
	{
		v = {'a', 'b', 'c', 'f', 'h', 'x'};
		for(int i = 0; i < v.size(); ++i)
			ch_arr[i] = v[i];

		hash_array<i32, byte> str_tree2;
		str_tree2.insert(0, v[0]);
		str_tree2.insert(1, v[4]);
		str_tree2.insert(2, v[3]);
		CHECK(std::includes(ch_arr.begin(), ch_arr.end(), str_tree2.begin(), str_tree2.end()));
	}
}

TEST_CASE("tree map stl test", "[tree_map stl]")
{
	tree_map<i32, i32> tree;
	tree_map<i32, byte> str_tree;

	SECTION("Case 01")
	{
		for (int i = 1000; i >= 0; --i)
			tree.insert(i, i + 24);
		CHECK(*(std::lower_bound(tree.begin(), tree.end(), 40)) == 40);
	}

	SECTION("Case 02")
	{
		for (int i = 1000; i >= 0; --i)
			tree.insert(i, i + 9);
		CHECK(*(std::upper_bound(tree.begin(), tree.end(), 10)) == 11);
	}

	//Set operations
	SECTION("Case 03")
	{
		std::vector<char> v = {'a', 'b', 'c', 'f', 'h', 'x'};
		for(int i = 0; i < v.size(); ++i)
			str_tree[i] = v[i];

		tree_map<i32, byte> str_tree2;
		str_tree2.insert(0, v[0]);
		str_tree2.insert(1, v[3]);
		str_tree2.insert(2, v[2]);
		CHECK(std::includes(str_tree.begin(), str_tree.end(), str_tree2.begin(), str_tree2.end()));
	}
}
