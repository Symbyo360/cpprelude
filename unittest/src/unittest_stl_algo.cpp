#include <cpprelude/bucket_array.h>
#include <cpprelude/dlinked_list.h>
#include <cpprelude/slinked_list.h>
#include <cpprelude/dynamic_array.h>
#include <cpprelude/hash_array.h>
#include <algorithm>
#include "catch.hpp"

using namespace cpprelude;

TEST_CASE("dynamic array stl test", "[dynamic array stl]") 
{
	dynamic_array<i32> arr;
	
	//Modifying sequence operations
	SECTION("CASE REMOVE")
	{
		arr = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 2, 32, 24, 140 };
		std::remove(arr.begin(), arr.end(), 2);
		CHECK_FALSE(arr.count() == 12);
		CHECK(std::find(arr.begin(), arr.end(), 2) == arr.end());
	}
	
	SECTION("CASE REVERSE")
	{
		for (int i = 0; i < 20; i++)
		{
			arr.insert_back(i);
		}
		std::reverse(arr.begin(), arr.end());
		for (int i = 19; i >= 0; i--)
			CHECK(arr[19 - i] == i);
	}

	//sorting operations
	SECTION("CASE SORT")
	{
		for (int i = 0; i < 20; i++)
			arr.insert_back(rand());
		std::sort(arr.begin(), arr.end());
		CHECK(std::is_sorted(arr.begin(), arr.end()));
	}
	
	SECTION("CASE STABLE SORT")
	{
		for (int i = 0; i < 20; i++)
			arr.insert_back(rand());
		std::stable_sort(arr.begin(), arr.end());
		CHECK(std::is_sorted(arr.begin(), arr.end()));
			
	}

	SECTION("CASE SORT ON A LARGE NUMBER")
	{
		for (int i = 0; i < 1000000; ++i)
			arr.insert_back(rand());
		std::sort(arr.begin(), arr.end());
		CHECK(std::is_sorted(arr.begin(), arr.end()));
	}

	SECTION("CASE STABLE SORT ON A LARGE NUMBER")
	{
		for (int i = 0; i < 1000000; ++i)
			arr.insert_back(rand());
		std::stable_sort(arr.begin(), arr.end());
		CHECK(std::is_sorted(arr.begin(), arr.end()));
	}

	SECTION("CASE IS SORTED UNTIL")
	{
		arr = { 1, 3, 5, 7, 9, 13, 20, 10, 32, 24, 140 };
		CHECK(*(std::is_sorted_until(arr.begin(), arr.end())) == 10);
	}
	
	SECTION("CASE PARTIAL SORT")
	{
		arr = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 32, 24, 140 };
		std::partial_sort(arr.begin(), arr.begin() + 7 , arr.end());
		CHECK(std::is_sorted_until(arr.begin(), arr.end()) == arr.begin() + 8);
	}

	SECTION("CASE NTH ELEMENT")
	{
		arr = { 5, 6, 4, 3, 2, 6, 7, 9, 3 };
		std::nth_element(arr.begin(), arr.begin() + arr.count() / 2, arr.end());
		CHECK(arr[arr.count() / 2] == 5);
	}
	//Binary search operations ( on sorted ranges )
	SECTION("CASE LOWER BOUND")
	{
		for (int i = 0; i < 10; i++)
			arr.insert_back(i);
		CHECK(*(std::lower_bound(arr.begin(), arr.end(), 5)) == 5);
	}

	SECTION("CASE UPPER BOUND")
	{		
		for (int i = 0; i < 10; i++)
			arr.insert_back(i);
		CHECK(*(std::upper_bound(arr.begin(), arr.end(), 5)) == 6);
	}

	SECTION("CASE BINARY SEARCH")
	{
		arr = { 1, 3, 5, 7, 9, 13, 20, 140 };
		CHECK(std::binary_search(arr.begin(), arr.end(), 13));
		CHECK_FALSE(std::binary_search(arr.begin(), arr.end(), 25));
	}
	
	//Set operations ( on sorted ranges )
	SECTION("CASE INCLUDES")
	{
		arr = { 1, 3, 5, 7, 9, 13, 20, 140 };
		dynamic_array<i32> d = { 3, 5, 9 };
		CHECK(std::includes(arr.begin(), arr.end(), d.begin(), d.end()));
		d = {4, 100, 15};
	    CHECK_FALSE(std::includes(arr.begin(), arr.end(), d.begin(), d.end()));
	}
}

TEST_CASE("bucket array stl test", "[bucket array stl]")
{
	bucket_array<int, 5> b_arr;
	
	//Modifying sequence operations
	SECTION("CASE REMOVE")
	{
		for (int i = 0; i < 40; i++)
			b_arr.insert_back(i);
		CHECK_FALSE(std::find(b_arr.begin(), b_arr.end(), 4)== b_arr.end());
		std::remove(b_arr.begin(), b_arr.end(), 4);
		CHECK(std::find(b_arr.begin(), b_arr.end(), 4) == b_arr.end());
	}

	SECTION("CASE REVERSE")
	{
		for (int i = 0; i < 20; i++)
			b_arr.insert_back(i);
		
		std::reverse(b_arr.begin(), b_arr.end());
		for (int i = 19; i >= 0; i--)
			CHECK(b_arr[19 - i] == i);
	}
	
	//sorting operations
	SECTION("CASE SORT")
	{
		for (int i = 0; i < 20; i++)
			b_arr.insert_back(rand());
		std::sort(b_arr.begin(), b_arr.end());
		CHECK(std::is_sorted(b_arr.begin(), b_arr.end()));
	}


	SECTION("CASE STABLE SORT")
	{
		for (int i = 0; i < 20; i++)
			b_arr.insert_back(rand());
		std::stable_sort(b_arr.begin(), b_arr.end());
		CHECK(std::is_sorted(b_arr.begin(), b_arr.end()));

	}

	SECTION("CASE SORT ON A LARGE NUMBER")
	{
		for (int i = 0; i < 1000001; ++i)
			b_arr.insert_back(rand());
		std::sort(b_arr.begin(), b_arr.end());
		CHECK(std::is_sorted(b_arr.begin(), b_arr.end()));
	}
	
	SECTION("CASE STABLE SORT ON A LARGE NUMBER")
	{
		for (int i = 0; i < 1000001; ++i)
			b_arr.insert_back(rand());
		std::stable_sort(b_arr.begin(), b_arr.end());
		CHECK(std::is_sorted(b_arr.begin(), b_arr.end()));
	}

	SECTION("CASE IS SORTED UNTIL")
	{
		b_arr = { 1, 3, 5, 7, 9, 13, 20, 10, 32, 24, 140 };
		CHECK(*(std::is_sorted_until(b_arr.begin(), b_arr.end())) == 10);
	}

	SECTION("CASE PARTIAL SORT")
	{
		b_arr = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 32, 24, 140, 90, 120, 4 };
		std::partial_sort(b_arr.begin(), b_arr.begin() + 7, b_arr.end());
		CHECK(std::is_sorted_until(b_arr.begin(), b_arr.end()) == b_arr.begin() + 8);
	}

	SECTION("CASE NTH ELEMENT")
	{
		b_arr = { 5, 6, 4, 3, 2, 6, 7, 9, 3 };
		std::nth_element(b_arr.begin(), b_arr.begin() + b_arr.count() / 2, b_arr.end());
		CHECK(b_arr[b_arr.count() / 2] == 5 );
	}

	//Binary search operations ( on sorted ranges )
	SECTION("CASE LOWER BOUND")
	{
		b_arr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		CHECK(*(std::lower_bound(b_arr.begin(), b_arr.end(), 5)) == 5);
	}

	SECTION("CASE UPPER BOUND")
	{
		b_arr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		CHECK(*(std::upper_bound(b_arr.begin(), b_arr.end(), 5)) == 6);
	}

	SECTION("CASE BINARY SEARCH")
	{
		b_arr = { 1, 3, 5, 7, 9, 13, 20, 140 };
		CHECK(std::binary_search(b_arr.begin(), b_arr.end(), 13));
		CHECK_FALSE(std::binary_search(b_arr.begin(), b_arr.end(), 25));
	}

	//Set operations ( on sorted ranges )
	SECTION("CASE INCLUDES")
	{
		b_arr = { 1, 3, 5, 7, 9, 13, 20, 140 };
		dynamic_array<i32> d = { 3, 5, 9 };
		CHECK(std::includes(b_arr.begin(), b_arr.end(), d.begin(), d.end()));
		d = { 4, 100, 15 };
		CHECK_FALSE(std::includes(b_arr.begin(), b_arr.end(), d.begin(), d.end()));
	}
}

TEST_CASE("doubly linked list stl test", "[doubly linked list stl]")
{
	dlinked_list<i32> dlist;

	//Modifying sequence operations
	SECTION("CASE REMOVE")
	{
		dlist = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 2, 32, 24, 140 };
		std::remove(dlist.begin(), dlist.end(), 2);
		CHECK_FALSE(dlist.count() == 12);
		CHECK(std::find(dlist.begin(), dlist.end(), 2) == dlist.end());
	}

	SECTION("CASE REVERSE")
	{
		for (int i = 0; i < 20; i++)
		{
			dlist.insert_back(i);
		}
		std::reverse(dlist.begin(), dlist.end());
		for (int i = 19; i >= 0; i--)
			CHECK(dlist[19 - i] == i);
	}


	SECTION("CASE IS SORTED UNTIL")
	{
		dlist = { 1, 3, 5, 7, 9, 13, 20, 10, 32, 24, 140 };
		CHECK(*(std::is_sorted_until(dlist.begin(), dlist.end())) == 10);
	}

	//Binary search operations ( on sorted ranges )
	SECTION("CASE LOWER BOUND")
	{
		for (int i = 0; i < 10; i++)
			dlist.insert_back(i);
		CHECK(*(std::lower_bound(dlist.begin(), dlist.end(), 5)) == 5);
	}

	SECTION("CASE UPPER BOUND")
	{
		for (int i = 0; i < 10; i++)
			dlist.insert_back(i);
		CHECK(*(std::upper_bound(dlist.begin(), dlist.end(), 5)) == 6);
	}

	SECTION("CASE BINARY SEARCH")
	{
		dlist = { 1, 3, 5, 7, 9, 13, 20, 140 };
		CHECK(std::binary_search(dlist.begin(), dlist.end(), 13));
		CHECK_FALSE(std::binary_search(dlist.begin(), dlist.end(), 25));
	}

	//Set operations ( on sorted ranges )
	SECTION("CASE INCLUDES")
	{
		dlist = { 1, 3, 5, 7, 9, 13, 20, 140 };
		dlinked_list <i32> d = { 3, 5, 9 };
		CHECK(std::includes(dlist.begin(), dlist.end(), d.begin(), d.end()));
		d = { 4, 100, 15 };
		CHECK_FALSE(std::includes(dlist.begin(), dlist.end(), d.begin(), d.end()));
	}
}

TEST_CASE("singly linked list stl test", "[singly linked list stl]")
{
	slinked_list<i32> slist;

	//Modifying sequence operations
	SECTION("CASE REMOVE")
	{
		slist = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 2, 32, 24, 140 };
		std::remove(slist.begin(), slist.end(), 2);
		CHECK_FALSE(slist.count() == 12);
		CHECK(std::find(slist.begin(), slist.end(), 2) == slist.end());
	}

	//sorting operations

	SECTION("CASE IS SORTED UNTIL")
	{
		slist = { 1, 3, 5, 7, 9, 13, 20, 10, 32, 24, 140 };
		CHECK(*(std::is_sorted_until(slist.begin(), slist.end())) == 10);
	}

	// Binary search operations (on sorted ranges)
	SECTION("CASE LOWER BOUND")
	{
		for (int i = 9; i >= 0; --i)
			slist.insert_front(i);
		CHECK(*(std::lower_bound(slist.begin(), slist.end(), 5)) == 5);
	}

	SECTION("CASE UPPER BOUND")
	{
		for (int i = 9; i >= 0; --i)
			slist.insert_front(i);
		CHECK(*(std::upper_bound(slist.begin(), slist.end(), 5)) == 6);
	}

	SECTION("CASE BINARY SEARCH")
	{
		slist = { 1, 3, 5, 7, 9, 13, 20, 140 };
		CHECK(std::binary_search(slist.begin(), slist.end(), 13));
		CHECK_FALSE(std::binary_search(slist.begin(), slist.end(), 25));
	}

	//Set operations ( on sorted ranges )
	SECTION("CASE INCLUDES")
	{
		slist = { 1, 3, 5, 7, 9, 13, 20, 140 };
		slinked_list<i32> d = { 3, 5, 9 };
		CHECK(std::includes(slist.begin(), slist.end(), d.begin(), d.end()));
		d = { 4, 100, 15 };
		CHECK_FALSE(std::includes(slist.begin(), slist.end(), d.begin(), d.end()));
	}
}

TEST_CASE("hash array stl test", "[hash array stl]")
{
	//NOTE: some operations for forward iterator will not be available for hahs array
	//for it has a key, the algorithms implemented in STL deal with the key as constant
	//i.e: remove operation can not be performed on hash arrays
	hash_array<i32, i32> h_arr;
	hash_array<i32, string> str_h_arr; 
	std::vector<int> v;

	SECTION("CASE SWAP")
	{
		v = { 3, 1, 9, 7, 5, 13, 2, 10, 8 , 2, 32, 24, 140 };
		for (int i = 0; i < v.size(); ++i) 
			h_arr.insert(i, v[i]);

		i32 x = h_arr[0];
		i32 y = h_arr[4];
		std::swap(h_arr[0], h_arr[4]);
		CHECK(h_arr[0] == y);
		CHECK(h_arr[4] == x);
	}

	SECTION("CASE LOWER BOUND")
	{
		for (int i = 1000; i >= 0; --i)
			h_arr.insert(i, i + 24);
		CHECK(*(std::lower_bound(h_arr.begin(), h_arr.end(), 40)) == 40);
	}

	SECTION("CASE UPPER BOUND")
	{
		for (int i = 1000; i >= 0; --i)
			h_arr.insert(i, i + 9);
		CHECK(*(std::upper_bound(h_arr.begin(), h_arr.end(), 10)) == 11);
		
	}
}

TEST_CASE("tree map stl test", "[tree map stl]")
{

}


