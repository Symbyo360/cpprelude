#include "catch.hpp"
#include <cpprelude/iterator.h>
#include <cpprelude/tree_map.h>
#include <cpprelude/algorithm.h>
#include <cpprelude/dynamic_array.h>
#include <cpprelude/string.h>
#include <iostream>

using namespace cpprelude;

TEST_CASE("tree_map test", "[tree_map]")
{

	SECTION("Case 01")
	{
		tree_map<cpprelude::byte, bool> tree;
		tree.insert('e');
		CHECK(tree._is_red_black_tree());
		tree.insert('c');
		CHECK(tree._is_red_black_tree());
		tree.insert('b');
		CHECK(tree._is_red_black_tree());
		tree.insert('d');
		CHECK(tree._is_red_black_tree());
		tree.insert('g');
		CHECK(tree._is_red_black_tree());
		tree.insert('f');
		CHECK(tree._is_red_black_tree());
		tree.insert('h');
		CHECK(tree._is_red_black_tree());
	}

	SECTION("Case 2")
	{
		tree_map<usize, bool> tree;

		tree.insert(359);
		CHECK(tree._is_red_black_tree());
		tree.insert(966);
		CHECK(tree._is_red_black_tree());
		tree.insert(105);
		CHECK(tree._is_red_black_tree());
		tree.insert(115);
		CHECK(tree._is_red_black_tree());
		tree.insert(81);
		CHECK(tree._is_red_black_tree());
		tree.insert(255);
		CHECK(tree._is_red_black_tree());
		tree.insert(74);
		CHECK(tree._is_red_black_tree());
		tree.insert(236);
		CHECK(tree._is_red_black_tree());
		tree.insert(809);
		CHECK(tree._is_red_black_tree());
	}

	SECTION("Case 03")
	{
		tree_map<usize, bool> array;
		array.insert(1);
		CHECK(array.count() == 1);

		array.insert(2);
		CHECK(array.count() == 2);

		array.insert(3);
		CHECK(array.count() == 3);
		array.insert(3);

		dynamic_array<usize> arr;
		auto insert = [&arr](tree_map<usize, bool>::iterator it, void* data) {
			arr.insert_back(*it);
		};
		array.inorder_traverse(insert);

		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array._is_red_black_tree());
	}

	////Check right right case
	SECTION("Case 04")
	{
		tree_map<usize, bool> array;
		array.insert(1);
		CHECK(array.count() == 1);

		array.insert(2);
		CHECK(array.count() == 2);

		array.insert(3);
		CHECK(array.count() == 3);

		dynamic_array<usize> arr;
		auto insert = [&arr](tree_map<usize, bool>::iterator it, void* data) {
			arr.insert_back(*it);
		};
		array.inorder_traverse(insert);

		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array._is_red_black_tree());
	}

	//Check left left case
	SECTION("Case 05")
	{
		tree_map<usize, bool> array;
		array.insert(3);
		CHECK(array.count() == 1);

		array.insert(2);
		CHECK(array.count() == 2);

		array.insert(1);
		CHECK(array.count() == 3);

		dynamic_array<usize> arr;
		auto insert = [&arr](tree_map<usize, bool>::iterator it, void* data) {
			arr.insert_back(*it);
		};
		array.inorder_traverse(insert);

		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array._is_red_black_tree());
	}

	//Check right left case
	SECTION("Case 06")
	{
		tree_map<usize, bool> array;
		array.insert(1);
		CHECK(array.count() == 1);

		array.insert(3);
		CHECK(array.count() == 2);

		array.insert(2);
		CHECK(array.count() == 3);

		dynamic_array<usize> arr;
		auto insert = [&arr](tree_map<usize, bool>::iterator it, void* data) {
			arr.insert_back(*it);
		};
		array.inorder_traverse(insert);

		CHECK(arr.count() == 3);

		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array._is_red_black_tree());
	}

	//Check left right case
	SECTION("Case 07")
	{
		tree_map<usize, bool> array;
		array.insert(3);
		CHECK(array.count() == 1);

		array.insert(1);
		CHECK(array.count() == 2);

		array.insert(2);
		CHECK(array.count() == 3);

		dynamic_array<usize> arr;
		auto insert = [&arr](tree_map<usize, bool>::iterator it, void* data) {
			arr.insert_back(*it);
		};
		array.inorder_traverse(insert);

		CHECK(arr.count() == 3);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array._is_red_black_tree());
	}

	//node type is pair
	SECTION("Case 08")
	{
		using sPair_node = details::pair_node<usize, std::string>;
		using sIterator = tree_map<usize, std::string>::iterator;
		tree_map<usize, std::string> tree;

		dynamic_array<usize> arr1;
		auto insert = [&arr1](sIterator it, void* data) {
			arr1.insert_back(*it);
		};

		sPair_node f(2, "22");
		CHECK(f == 2);
		CHECK(f.value == "22");

		tree.insert(sPair_node(3, "33"));
		CHECK(tree.count() == 1);

		tree.insert(sPair_node(1, "11"));
		CHECK(tree.count() == 2);

		tree.insert(sPair_node(2, "22"));
		CHECK(tree.count() == 3);

		tree.inorder_traverse(insert);

		CHECK(arr1.count() == 3);
		CHECK(cpprelude::is_sorted(arr1.begin(), arr1.count()));

		CHECK(tree._is_red_black_tree());
		//search
		tree[1] = "11";
		auto h = tree.lookup(1);
		CHECK(h.key() == 1);
		CHECK(h.value() == "11");
	}

	SECTION("Case 09")
	{
		tree_set<usize> tree;

		usize i = 3;
		tree.insert(i);
		tree.insert(5);


		tree_set<usize> tree2(tree);

		dynamic_array<usize> arr;
		auto insert = [&arr](tree_set<usize>::iterator it, void* data) {
			arr.insert_back(*it);
		};

		tree2.inorder_traverse(insert);

		CHECK(tree2.count() == 2);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(tree2._is_red_black_tree());

		tree2.insert(4);
		//Assignment operator
		tree = tree2;

		CHECK(tree.count() == 3);

		arr.reset();

		tree.inorder_traverse(insert);

		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(tree2._is_red_black_tree());
	}

	SECTION("Case 10")
	{
		tree_set<usize> array;
		array.insert(3);
		CHECK(array.count() == 1);

		array.insert(1);
		CHECK(array.count() == 2);

		array.insert(2);
		CHECK(array.count() == 3);

		dynamic_array<usize> arr;
		auto insert = [&arr](tree_set<usize>::iterator it, void* data) {
			arr.insert_back(*it);
		};

		array.inorder_traverse(insert);
		CHECK(arr.count() == 3);

		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array._is_red_black_tree());
		CHECK(arr.count() == 3);
		//search
		CHECK(array.lookup(3) != array.end());

		CHECK(array.lookup(99) == array.end());
		auto res = array.lookup(1);
		bool f = res != array.end() && *res == 1;
		CHECK(f);
	}

	SECTION("Case 11")
	{
		tree_set<usize> array;
		array.insert(3);
		CHECK(array.count() == 1);

		array.insert(1);
		CHECK(array.count() == 2);

		array.insert(2);
		//auto fn = print_fn<usize>;
		//array.inorder_traverse(fn);
		CHECK(array._is_red_black_tree());
	}

	//insertion
	SECTION("Case 12")
	{
		tree_set<usize> tree;
		tree.insert(15);
		tree.insert(1);
		tree.insert(20);
		tree.insert(3);
		tree.insert(13);
		tree.insert(14);
		tree.insert(45);
		tree.insert(34);
		CHECK(tree.count() == 8);

		dynamic_array<usize> arr;
		auto insert = [&arr](tree_set<usize>::iterator it, void* data) {
			arr.insert_back(*it);
		};

		tree.inorder_traverse(insert);
		CHECK(arr.count() == 8);
		CHECK(tree._is_red_black_tree());
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
	}

	//deleting root
	SECTION("Case 13")
	{
		tree_set<usize> array;
		array.insert(3);
		CHECK(array.count() == 1);
		CHECK(array._is_red_black_tree());

		//deleting non-existed element
		array.remove(0);
		CHECK(array.count() == 1);
		CHECK(array._is_red_black_tree());

		//deleting root wuth no children
		array.remove(3);
		CHECK(array.count() == 0);
		CHECK(array._is_red_black_tree());

	}

	//deleting root with one child
	SECTION("Case 14")
	{
		tree_map<usize, bool> array;

		//deleting root with left children
		array.insert(3);
		array.insert(1);
		CHECK(array.count() == 2);
		array.remove(3);
		CHECK(array.count() == 1);
		CHECK(array.root()->key == 1);

		CHECK(array._is_red_black_tree());

		tree_map<usize, bool> array1;
		//deleting root with right children

		array1.insert(3);
		array1.insert(4);
		CHECK(array1.count() == 2);
		array1.remove(3);
		CHECK(array1.count() == 1);
		CHECK(array1.root()->key == 4);
		CHECK(array._is_red_black_tree());
	}

	//deleting root with two children
	SECTION("Case 15")
	{
		tree_map<usize, bool> array;

		array.insert(3);
		array.insert(1);
		array.insert(6);
		CHECK(array.count() == 3);
		array.remove(3);
		CHECK(array.count() == 2);
		CHECK(array.root()->key == 6);
		array.insert(3);
		CHECK(array.count() == 3);
		CHECK(array._is_red_black_tree());
	}

	//deleting node with two children ex 1
	//Case 3 & 4
	SECTION("Case 16")
	{
		tree_map<usize, bool> array;
		usize count = 0;
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == 8);
		//array.inorder_traverse(print);
		array.remove(15);
		CHECK(array.count() == 7);
		//array.inorder_traverse(print);

		dynamic_array<usize> arr;
		auto insert = [&arr](tree_map<usize, bool>::iterator it, void* data) {
			arr.insert_back(*it);
		};
		array.inorder_traverse(insert);

		CHECK(arr.count() == 7);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array._is_red_black_tree());
	}

	//deleting node with two children ex 2
	//Case 3 & 4
	SECTION("Case 17")
	{
		tree_map<usize, bool> array;
		usize count = 0;
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.remove(15);
		CHECK(array.count() == --count);
		//array.inorder_traverse(print);
		dynamic_array<usize> arr;
		auto insert = [&arr](tree_map<usize, bool>::iterator it, void* data) {
			arr.insert_back(*it);
		};

		array.inorder_traverse(insert);
		CHECK(arr.count() == count);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array._is_red_black_tree());
	}

	//deleting node with one children ex 1
	SECTION("Case 18")
	{
		tree_map<usize, bool> array;
		usize count = 0;
		//deleting root with left children
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.remove(12);
		CHECK(array.count() == --count);
		//array.inorder_traverse(print);
		dynamic_array<usize> arr;
		auto insert = [&arr](tree_map<usize, bool>::iterator it, void* data) {
			arr.insert_back(*it);
		};
		array.inorder_traverse(insert);
		CHECK(arr.count() == count);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array._is_red_black_tree());
	}

	//deleting node with one children ex 2
	SECTION("Case 19")
	{
		tree_map<usize, bool> array;
		usize count = 0;
		//deleting root with left children
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.remove(20);
		CHECK(array.count() == --count);
		//array.inorder_traverse(print);
		dynamic_array<usize> arr;
		auto insert = [&arr](tree_map<usize, bool>::iterator it, void* data) {
			arr.insert_back(*it);
		};
		array.inorder_traverse(insert);
		CHECK(arr.count() == count);
		CHECK(cpprelude::is_sorted(arr.begin(), arr.count()));
		CHECK(array._is_red_black_tree());
	}

	//Case 2 ex 1
	SECTION("Case 20")
	{
		tree_map<usize, bool> array;
		usize count = 0;
		//deleting root with left children
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//array.inorder_traverse(print);
		array.remove(20);
		CHECK(array.count() == --count);
		CHECK(array._is_red_black_tree());
		array.remove(14);
		CHECK(array.count() == --count);
		CHECK(array._is_red_black_tree());
		array.remove(12);
		CHECK(array.count() == --count);
		CHECK(array._is_red_black_tree());
		//array.inorder_traverse(print);
	}


	SECTION("Case 21")
	{
		tree_map<usize, bool> array;
		usize count = 0;
		array.insert(10);
		CHECK(array.count() == ++count);
		array.insert(9);
		CHECK(array.count() == ++count);
		array.insert(15);
		CHECK(array.count() == ++count);
		array.insert(1);
		CHECK(array.count() == ++count);
		array.insert(5);
		CHECK(array.count() == ++count);
		array.insert(12);
		CHECK(array.count() == ++count);
		array.insert(20);
		CHECK(array.count() == ++count);
		array.insert(14);
		CHECK(array.count() == ++count);
		array.insert(19);
		CHECK(array.count() == ++count);
		//Case 2 ex 2
		array.remove(20);
		CHECK(array._is_red_black_tree());
		CHECK(array.count() == --count);
		array.remove(14);
		CHECK(array._is_red_black_tree());
		CHECK(array.count() == --count);
		//Case 2
		array.remove(19);
		CHECK(array._is_red_black_tree());
		CHECK(array.count() == --count);
		CHECK(array._is_red_black_tree());
		array.remove(15);
		CHECK(array._is_red_black_tree());
		CHECK(array.count() == --count);
		//Case 4
		array.remove(12);
		CHECK(array._is_red_black_tree());
		CHECK(array.count() == --count);
	}

	SECTION("Case 22")
	{
		// Creating an empty set		
		tree_set<usize> set_1;
		// Inserting keys/elements
		set_1.insert(3);
		set_1.insert(5);
		//Copying the content of set_1 to set_2
		tree_set<usize> set_2(set_1);
		//Lambda function that inserts elements in an array + taking a data and adding it to the elements.
		dynamic_array<usize> arr;
		auto insert = [&arr](tree_set<usize>::iterator it, usize* data) {
			arr.insert_back(*it + *(data));
		};
		//Traversing the set in an inorder way and adding the exisisting elements to 1
		usize x = 1;
		set_2.inorder_traverse(insert, &x);
		CHECK(*set_2.min() == 3);
		CHECK(*set_2.max() == 5);
	}

	SECTION("Case 23")
	{
		// Creating an empty set.
		tree_map<usize, std::string> tree;
		auto append = [](tree_map<usize, std::string>::iterator it, std::string* data) {
			it.value() += *data;
			//std::cout << it.key() << " " << it.value() <<std::endl;
		};
		//inserting some pairs
		tree.insert(3, "33");
		tree.insert(2, "22");
		//this one has no value
		tree.insert(1);
		//using [] to change value at key 1
		tree[1] = "44";
		//searching for that key and printing its value
		auto _ = tree.lookup(1);
		//std::cout << h.value() << std::endl;    
		//Applying insert-lambda function by calling inorder_traverse
		std::string aa = "AA";
		tree.inorder_traverse(append, &aa);
	}

	SECTION("Case 24")
	{
		tree_map<string, usize> str_map;

		str_map["abcd"] = 1;
		str_map["ab"] = 1;
		str_map["ba"] = 1;
		str_map["dcba"] = 1;
		str_map["fegh"] = 1;
		str_map["cdab"] = 1;

		CHECK(str_map.empty() == false);
		CHECK(str_map.count() == 6);
	}
}
