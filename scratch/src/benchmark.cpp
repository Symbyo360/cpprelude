#include "benchmark.h"
#include "stopwatch.h"

#include <cpprelude/dynamic_array.h>
#include <cpprelude/v5/Dynamic_Array.h>
#include <vector>

#include <forward_list>
#include <cpprelude/v5/Single_List.h>

#include <list>
#include <cpprelude/v5/Double_List.h>

#include <cpprelude/v5/Stack_Array.h>
#include <cpprelude/v5/Stack_List.h>
#include <stack>

#include <cpprelude/v5/Ring_Array.h>
#include <cpprelude/v5/Queue_List.h>
#include <cpprelude/v5/Priority_Queue.h>
#include <queue>

#include <cpprelude/v5/Algorithms.h>
#include <cpprelude/algorithm.h>
#include <algorithm>

#include <cpprelude/v5/Tree_Map.h>
#include <map>

#include <cpprelude/v5/String.h>
#include <string>

#include <cpprelude/v5/Hash_Map.h>
#include <unordered_set>
#include <unordered_map>

#include <cpprelude/v5/IO.h>

// #include <cpprelude/v5/Result.h>
// #include <optional>
// #include <variant>
//old

#include <cpprelude/stack_array.h>
#include <cpprelude/stack_list.h>

#include <cpprelude/slinked_list.h>
#include <cpprelude/dlinked_list.h>
#include <cpprelude/bucket_array.h>
#include <deque>

#include <cpprelude/queue_list.h>
#include <cpprelude/queue_array.h>
#include <cpprelude/priority_queue.h>

#include <cpprelude/threading.h>
#include <functional>

#include <cpprelude/string.h>
#include <sstream>
#include <cpprelude/stream.h>
#include <cpprelude/fmt.h>

#include <cpprelude/hash_array.h>
#include <unordered_map>
#include <cpprelude/tree_map.h>

#include <cpprelude/allocator.h>

#include <iostream>
#include <cpprelude/micro_benchmark.h>
#include <cpprelude/memory_watcher.h>

using namespace cpprelude;

cpprelude::arena_t arena(MEGABYTES(100));

Dynamic_Array<usize> RANDOM_ARRAY;

void
generate_random_data(usize limit)
{
	RANDOM_ARRAY.clear();
	for(usize i = 0; i < limit; ++i)
		RANDOM_ARRAY.insert_back(rand());
}

//new benchmark
usize
bm_v5_Dynamic_Array(workbench* bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		Dynamic_Array<usize> array;

		for(usize i = 0; i < limit; ++i)
			array.insert_back(i + r);

		for(const auto& number: array)
			if(number % 3 == 0)
				r += number;
	bench->watch.stop();

	return r;
}

usize
bm_vector(workbench* bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		std::vector<usize> array;

		for(usize i = 0; i < limit; ++i)
			array.push_back(i + r);

		for (const auto& number : array)
			if (number % 3 == 0)
				r += number;
	bench->watch.stop();

	return r;
}


usize
bm_v5_Single_List(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		Single_List<usize> list;
		for(usize i = 0; i < limit; ++i)
			list.insert_front(i + r);

		for (const auto& number : list)
			if (number % 2 == 0)
				r += number;
	bench->watch.stop();
	return r;
}


usize
bm_forward_list(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		std::forward_list<usize> list;
		for(usize i = 0; i < limit; ++i)
			list.push_front(i + r);

		for(auto& number: list)
			if(number % 2 == 0)
				r += number;
	bench->watch.stop();
	return r;
}

usize
bm_v5_Double_List(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		Double_List<usize> list;
		for(usize i = 0; i < limit; ++i)
			if((i + r) % 2 == 0)
				list.emplace_back(i + r);
			else
				list.emplace_front(i + r);

		for(const auto& number: list)
			if(number % 2 == 0)
				r += number;
	bench->watch.stop();
	return r;
}


usize
bm_list(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		std::list<usize> list;
		for(usize i = 0; i < limit; ++i)
			if((i + r) % 2 == 0)
				list.push_back(i + r);
			else
				list.push_front(i + r);

		for(const auto& number: list)
			if(number % 2 == 0)
				r += number;
	bench->watch.stop();
	return r;
}


usize
bm_v5_Stack_Array(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		Stack_Array<usize> stack;
		for (usize i = 0; i < limit; ++i)
			if ((i + r) % 2 == 0)
				stack.push(i + r);

		while (!stack.empty())
		{
			r += stack.top();
			stack.pop();
		}
	bench->watch.stop();
	return r;
}

usize
bm_v5_Stack_List(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		Stack_List<usize> stack;
		for (usize i = 0; i < limit; ++i)
			if ((i + r) % 2 == 0)
				stack.push(i + r);

		while (!stack.empty())
		{
			r += stack.top();
			stack.pop();
		}
	bench->watch.stop();
	return r;
}

usize
bm_stack(workbench *bench, usize limit)
{
	usize r = rand();
	bench->watch.start();
		std::stack<usize> stack;
		for (usize i = 0; i < limit; ++i)
			if ((i + r) % 2 == 0)
				stack.push(i + r);

		while (!stack.empty())
		{
			r += stack.top();
			stack.pop();
		}
	bench->watch.stop();
	return r;
}

usize
bm_v5_Ring_Array(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		Ring_Array<usize> que;
		que.reserve(limit);
		for (usize i = 0; i < limit; ++i)
			que.insert_front(i + r);

		while(!que.empty())
		{
			r += que.front();
			que.remove_front();
		}
	bench->watch.stop();
	return r;
}

usize
bm_v5_Queue_List(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		Queue_List<usize> que;
		for (usize i = 0; i < limit; ++i)
			que.enqueue(i + r);

		while (!que.empty())
		{
			r += que.front();
			que.dequeue();
		}
	bench->watch.stop();
	return r;
}

usize
bm_queue(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		std::queue<usize> que;
		for (usize i = 0; i < limit; ++i)
			que.push(i + r);

		while (!que.empty())
		{
			r += que.front();
			que.pop();
		}
	bench->watch.stop();
	return r;
}


usize
bm_v5_Priority_Queue(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		Priority_Queue<usize> array;
		for(usize i = 0; i < limit; ++i)
			array.enqueue(i + r);

		while(!array.empty())
		{
			if(array.front() % 2 == 0)
				r += array.front();
			array.dequeue();
		}
	bench->watch.stop();
	return r;
}

usize
bm_priority_queue(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		std::priority_queue<usize> array;
		for(usize i = 0; i < limit; ++i)
			array.push(i + r);

		while(!array.empty())
		{
			if(array.top() % 2 == 0)
				r += array.top();
			array.pop();
		}
	bench->watch.stop();
	return r;
}

usize
bm_v5_heap_sort(workbench* bench, usize limit)
{
	usize r = rand();

	Dynamic_Array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(RANDOM_ARRAY[i]);

	bench->watch.start();
		cpprelude::heap_sort(array.all());
	bench->watch.stop();
	for(usize i = 0; i < array.count(); ++i)
		if((i+r) % 3 == 0)
			r += array[i];

	assert(cpprelude::is_sorted(array.all()));

	return r;
}

usize
bm_std_heap_sort(workbench* bench, usize limit)
{
	usize r = rand();

	std::vector<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push_back(RANDOM_ARRAY[i]);

	bench->watch.start();
		std::make_heap(array.begin(), array.end());
		std::sort_heap(array.begin(), array.end());
	bench->watch.stop();
	for(usize i = 0; i < array.size(); ++i)
		if((i+r) % 3 == 0)
			r += array[i];

	assert(std::is_sorted(array.begin(), array.end()));

	return r;
}


usize
bm_v5_insertion_sort(workbench* bench, usize limit)
{
	usize r = rand();

	Dynamic_Array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(RANDOM_ARRAY[i]);

	bench->watch.start();
		cpprelude::insertion_sort(array.all());
	bench->watch.stop();
	for(usize i = 0; i < array.count(); ++i)
		if((i+r) % 3 == 0)
			r += array[i];

	assert(cpprelude::is_sorted(array.all()));

	return r;
}

usize
bm_insertion_sort(workbench* bench, usize limit)
{
	usize r = rand();

	dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(RANDOM_ARRAY[i]);

	bench->watch.start();
		cpprelude::insertion_sort(array.begin(), array.count());
	bench->watch.stop();
	for(usize i = 0; i < array.count(); ++i)
		if((i+r) % 3 == 0)
			r += array[i];

	assert(cpprelude::is_sorted(array.begin(), array.count()));

	return r;
}


usize
bm_v5_merge_sort(workbench* bench, usize limit)
{
	usize r = rand();

	Dynamic_Array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(RANDOM_ARRAY[i]);

	bench->watch.start();
		cpprelude::merge_sort(array.all());
	bench->watch.stop();
	for(usize i = 0; i < array.count(); ++i)
		if((i+r) % 3 == 0)
			r += array[i];

	assert(cpprelude::is_sorted(array.all()));

	return r;
}

usize
bm_merge_sort(workbench* bench, usize limit)
{
	usize r = rand();

	dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(RANDOM_ARRAY[i]);

	bench->watch.start();
		cpprelude::merge_sort(array.begin(), array.count());
	bench->watch.stop();

	for(usize i = 0; i < array.count(); ++i)
		if((i+r) % 3 == 0)
			r += array[i];

	assert(cpprelude::is_sorted(array.begin(), array.count()));

	return r;
}

usize
bm_std_merge_sort(workbench* bench, usize limit)
{
	usize r = rand();

	std::vector<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push_back(RANDOM_ARRAY[i]);

	bench->watch.start();
		std::stable_sort(array.begin(), array.end());
	bench->watch.stop();

	for(usize i = 0; i < array.size(); ++i)
		if((i+r) % 3 == 0)
			r += array[i];

	assert(std::is_sorted(array.begin(), array.end()));

	return r;
}


usize
bm_v5_quick_sort(workbench* bench, usize limit)
{
	usize r = rand();

	Dynamic_Array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(RANDOM_ARRAY[i]);

	bench->watch.start();
		cpprelude::quick_sort(array.all());
	bench->watch.stop();
	for(usize i = 0; i < array.count(); ++i)
		if((i+r) % 3 == 0)
			r += array[i];

	assert(cpprelude::is_sorted(array.all()));

	return r;
}

usize
bm_quick_sort(workbench* bench, usize limit)
{
	usize r = rand();

	dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(RANDOM_ARRAY[i]);

	bench->watch.start();
		cpprelude::quick_sort(array.begin(), array.count());
	bench->watch.stop();

	for(usize i = 0; i < array.count(); ++i)
		if((i+r) % 3 == 0)
			r += array[i];

	assert(cpprelude::is_sorted(array.begin(), array.count()));

	return r;
}

usize
bm_std_quick_sort(workbench* bench, usize limit)
{
	usize r = rand();

	std::vector<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push_back(RANDOM_ARRAY[i]);

	bench->watch.start();
		std::sort(array.begin(), array.end());
	bench->watch.stop();

	for(usize i = 0; i < array.size(); ++i)
		if((i+r) % 3 == 0)
			r += array[i];

	assert(std::is_sorted(array.begin(), array.end()));

	return r;
}

usize
bm_v5_Tree_Map(workbench* bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		Tree_Map<usize, usize> map;
		for(usize i = 0; i < limit; ++i)
			map[i] = i+r;
		for(usize i = 0; i < limit; ++i)
		{
			auto it = map.lookup(i);
			if(it != map.end())
			{
				if((it->key + it->value) % 2 == 0)
					map.remove(it);
			}
		}
	bench->watch.stop();

	return r;
}

usize
bm_std_map(workbench* bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
		std::map<usize, usize> map;
		for(usize i = 0; i < limit; ++i)
			map[i] = i+r;
		for(usize i = 0; i < limit; ++i)
		{
			auto it = map.find(i);
			if(it != map.end())
			{
				if((it->first + it->second) % 2 == 0)
					map.erase(it);
			}
		}
	bench->watch.stop();

	return r;
}


char* random_str = nullptr;

void
gen_random_str(const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	random_str = new char[len + 1];

	for (int i = 0; i < len; ++i) {
		random_str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	random_str[len] = 0;
}

std::string
bm_std_string_create(workbench *bench, usize limit)
{
	bench->watch.start();
		std::string str(random_str);
	bench->watch.stop();
	return str;
}

String
bm_v5_String(workbench *bench, usize limit)
{
	bench->watch.start();
		String str(random_str);
	bench->watch.stop();
	return str;
}

std::string
bm_std_string_concat(workbench *bench, usize limit)
{
	bench->watch.start();
		std::string str;
		for(usize i = 0; i < limit; ++i)
			str += "Mostafa";
	bench->watch.stop();
	return str;
}

String
bm_v5_String_concat(workbench *bench, usize limit)
{
	bench->watch.start();
		String str;
		for(usize i = 0; i < limit; ++i)
			str.concat("Mostafa");
	bench->watch.stop();
	return str;
}


usize
bm_std_unordered_set(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
	{
		std::unordered_set<usize> s;
		for(usize i = 0; i < limit; ++i)
			s.insert(r + i);
		for(usize i = r; i < limit + r; ++i)
			if(i % 2 == 0)
				s.erase(i);
		for(const auto& num: s)
			r += num;
	}
	bench->watch.stop();
	return r;
}

usize
bm_v5_Hash_Set(workbench *bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
	{
		Hash_Set<usize> s;
		for(usize i = 0; i < limit; ++i)
			s.insert(r + i);
		for(usize i = r; i < limit + r; ++i)
			if(i % 2 == 0)
				s.remove(i);
		for(const auto& num: s)
			r += num;
	}
	bench->watch.stop();
	return r;
}


usize
bm_std_unordered_map(workbench* bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
	{
		std::unordered_map<usize, usize> m;
		for(usize i = 0; i < limit; ++i)
			m[i] = r + i;
		for(usize i = 0; i < limit; ++i)
		{
			auto it = m.find(i);
			if(it->second % 2 == 0)
				m.erase(it);
		}
		for (const auto& it : m)
			r += it.second;
	}
	bench->watch.stop();
	return r;
}

usize
bm_v5_Hash_Map(workbench* bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
	{
		Hash_Map<usize, usize> m;
		for (usize i = 0; i < limit; ++i)
			m[i] = r + i;
		for (usize i = 0; i < limit; ++i)
		{
			auto it = m.lookup(i);
			if (it->value % 2 == 0)
				m.remove(it);
		}
		for (const auto& it : m)
			r += it.value;
	}
	bench->watch.stop();
	return r;
}

usize
bm_hash_array(workbench* bench, usize limit)
{
	usize r = rand();

	bench->watch.start();
	{
		hash_array<usize, usize> m;
		for (usize i = 0; i < limit; ++i)
			m[i] = r + i;
		for (usize i = 0; i < limit; ++i)
		{
			auto it = m.lookup(i);
			if (it.value() % 2 == 0)
				m.remove(it);
		}
		for (auto it = m.begin(); it != m.end(); ++it)
			r += it.value();
	}
	bench->watch.stop();
	return r;
}

//double linked list container
// usize
// bm_dlinked_list(workbench* bench, usize limit)
// {
// 	dlinked_list<usize> array;
// 	bench->watch.start();

// 	for(usize i = 0; i < limit; ++i)
// 		array.insert_front(rand());

// 	bench->watch.stop();

// 	return *array.begin();
// }

// usize
// bm_custom_dlinked_list(workbench* bench, usize limit)
// {
// 	arena.free_all();

// 	dlinked_list<usize> array(arena.context());
// 	bench->watch.start();

// 	for (usize i = 0; i < limit; ++i)
// 		array.insert_front(rand());

// 	bench->watch.stop();

// 	return *array.begin();
// }

// //usize
// //bm_list(workbench* bench, usize limit)
// //{
// //	std::list<usize> array;
// //	bench->watch.start();
// //
// //	for(usize i = 0; i < limit; ++i)
// //		array.push_front(rand());
// //
// //	bench->watch.stop();
// //
// //	return *array.begin();
// //}

// //stack containers
// usize
// bm_stack_array(workbench* bench, usize limit)
// {
// 	stack_array<usize> array;
// 	bench->watch.start();

// 	for(usize i = 0; i < limit; ++i)
// 		array.push(rand());

// 	bench->watch.stop();

// 	return array.top();
// }

// usize
// bm_custom_stack_array(workbench* bench, usize limit)
// {
// 	arena.free_all();

// 	stack_array<usize> array(arena.context());
// 	bench->watch.start();

// 	for (usize i = 0; i < limit; ++i)
// 		array.push(rand());

// 	bench->watch.stop();

// 	return array.top();
// }

// usize
// bm_stack_list(workbench* bench, usize limit)
// {
// 	stack_list<usize> array;
// 	bench->watch.start();

// 	for(usize i = 0; i < limit; ++i)
// 		array.push(rand());

// 	bench->watch.stop();

// 	return array.top();
// }

// usize
// bm_custom_stack_list(workbench* bench, usize limit)
// {
// 	arena.free_all();

// 	stack_list<usize> array(arena.context());
// 	bench->watch.start();

// 	for (usize i = 0; i < limit; ++i)
// 		array.push(rand());

// 	bench->watch.stop();

// 	return array.top();
// }

// usize
// bm_stack(workbench* bench, usize limit)
// {
// 	std::stack<usize> array;
// 	bench->watch.start();

// 	for(usize i = 0; i < limit; ++i)
// 		array.push(rand());

// 	bench->watch.stop();

// 	return array.top();
// }

// //queue containers
// usize
// bm_queue_array(workbench* bench, usize limit)
// {
// 	queue_array<usize> array;

// 	bench->watch.start();

// 	for(usize i = 0; i < limit; ++i)
// 		array.enqueue(rand());

// 	bench->watch.stop();

// 	return array.front();
// }

// usize
// bm_custom_queue_array(workbench* bench, usize limit)
// {
// 	arena.free_all();

// 	queue_array<usize> array(arena.context());

// 	bench->watch.start();

// 	for (usize i = 0; i < limit; ++i)
// 		array.enqueue(rand());

// 	bench->watch.stop();

// 	return array.front();
// }

// usize
// bm_queue_list(workbench* bench, usize limit)
// {
// 	queue_list<usize> array;

// 	bench->watch.start();
// 	for(usize i = 0; i < limit; ++i)
// 		array.enqueue(rand());

// 	bench->watch.stop();

// 	return array.front();
// }

// usize
// bm_custom_queue_list(workbench* bench, usize limit)
// {
// 	arena.free_all();

// 	queue_list<usize> array(arena.context());

// 	bench->watch.start();
// 	for (usize i = 0; i < limit; ++i)
// 		array.enqueue(rand());

// 	bench->watch.stop();

// 	return array.front();
// }

// usize
// bm_queue(workbench* bench, usize limit)
// {
// 	std::queue<usize> array;

// 	bench->watch.start();
// 	for(usize i = 0; i < limit; ++i)
// 		array.push(rand());

// 	bench->watch.stop();

// 	return array.front();
// }

// //priority queue containers
// usize
// bm_priority_queue(workbench* bench, usize limit)
// {
// 	priority_queue<usize> array;

// 	bench->watch.start();
// 	for(usize i = 0; i < limit; ++i)
// 		array.enqueue(rand());

// 	bench->watch.stop();

// 	return array.front();
// }

// usize
// bm_custom_priority_queue(workbench* bench, usize limit)
// {
// 	arena.free_all();

// 	priority_queue<usize> array(arena.context());

// 	bench->watch.start();
// 	for (usize i = 0; i < limit; ++i)
// 		array.enqueue(rand());

// 	bench->watch.stop();

// 	return array.front();
// }

// usize
// bm_std_priority_queue(workbench* bench, usize limit)
// {
// 	std::priority_queue<usize> array;

// 	bench->watch.start();
// 	for(usize i = 0; i < limit; ++i)
// 		array.push(rand());

// 	bench->watch.stop();

// 	return array.top();
// }

// //deque containers
// usize
// bm_bucket_array(workbench* bench, usize limit)
// {
// 	bucket_array<usize> array;

// 	bench->watch.start();
// 	for(usize i = 0; i < limit; ++i)
// 		array.insert_back(rand());

// 	bench->watch.stop();

// 	return *array.begin();
// }

// usize
// bm_custom_bucket_array(workbench* bench, usize limit)
// {
// 	arena.free_all();

// 	bucket_array<usize> array(arena.context());

// 	bench->watch.start();
// 	for (usize i = 0; i < limit; ++i)
// 		array.insert_back(rand());

// 	bench->watch.stop();

// 	return *array.begin();
// }

// usize
// bm_deque(workbench* bench, usize limit)
// {
// 	std::deque<usize> array;

// 	bench->watch.start();
// 	for(usize i = 0; i < limit; ++i)
// 		array.push_back(rand());

// 	bench->watch.stop();

// 	return *array.begin();
// }

// //merge_sort
// void
// bm_merge_sort(workbench* bench, usize limit)
// {
// 	std::random_device device;
// 	std::mt19937 generator(device());
// 	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

// 	cpprelude::dynamic_array<cpprelude::usize> array;

// 	for(cpprelude::usize i = 0; i < limit; ++i)
// 		array.insert_back(distribution(generator));

// 	bench->watch.start();
// 		cpprelude::merge_sort(array.begin(), array.count());
// 	bench->watch.stop();
// }

// void
// bm_std_stable_sort(workbench* bench, usize limit)
// {
// 	std::random_device device;
// 	std::mt19937 generator(device());
// 	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

// 	std::vector<cpprelude::usize> array;

// 	for(cpprelude::usize i = 0; i < limit; ++i)
// 		array.push_back(distribution(generator));

// 	bench->watch.start();
// 		std::stable_sort(array.begin(), array.end());
// 	bench->watch.stop();
// }

// //quick sort
// void
// bm_quick_sort(workbench* bench, usize limit)
// {
// 	std::random_device device;
// 	std::mt19937 generator(device());
// 	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

// 	cpprelude::dynamic_array<cpprelude::usize> array;

// 	for(cpprelude::usize i = 0; i < limit; ++i)
// 		array.insert_back(distribution(generator));

// 	bench->watch.start();
// 		cpprelude::quick_sort(array.begin(), array.count());
// 	bench->watch.stop();
// }

// void
// bm_std_sort(workbench* bench, usize limit)
// {
// 	std::random_device device;
// 	std::mt19937 generator(device());
// 	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

// 	std::vector<cpprelude::usize> array;

// 	for(cpprelude::usize i = 0; i < limit; ++i)
// 		array.push_back(distribution(generator));

// 	bench->watch.start();
// 		std::sort(array.begin(), array.end());
// 	bench->watch.stop();
// }

// void
// bm_heap_sort(workbench* bench, usize limit)
// {
// 	std::random_device device;
// 	std::mt19937 generator(device());
// 	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

// 	cpprelude::dynamic_array<cpprelude::usize> array;

// 	for(cpprelude::usize i = 0; i < limit; ++i)
// 		array.insert_back(distribution(generator));

// 	bench->watch.start();
// 		cpprelude::heap_sort(array.begin(), array.count());
// 	bench->watch.stop();
// }

// void
// bm_std_heap_sort(workbench* bench, usize limit)
// {
// 	std::random_device device;
// 	std::mt19937 generator(device());
// 	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

// 	std::vector<cpprelude::usize> array;

// 	for(cpprelude::usize i = 0; i < limit; ++i)
// 		array.push_back(distribution(generator));

// 	bench->watch.start();
// 		std::make_heap(array.begin(), array.end());
// 		std::sort_heap(array.begin(), array.end());
// 	bench->watch.stop();
// }

// void
// bm_hash_array(workbench* bench, usize limit)
// {
// 	hash_array<usize, usize> array;

// 	bench->watch.start();
// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		array.insert(i, i+9);
// 	}

// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		auto it = array.lookup(i);
// 		array.remove(it);
// 	}
// 	bench->watch.stop();
// 	return;
// }

// void
// bm_custom_hash_array(workbench* bench, usize limit)
// {
// 	arena.free_all();

// 	hash_array<usize, usize> array(arena.context());

// 	bench->watch.start();
// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		array.insert(i, i + 9);
// 	}

// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		auto it = array.lookup(i);
// 		array.remove(it);
// 	}
// 	bench->watch.stop();
// 	return;
// }

// void
// bm_unordered_map(workbench* bench, usize limit)
// {
// 	std::unordered_map<usize, usize> array;

// 	bench->watch.start();
// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		array.emplace(i, i+9);
// 	}

// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		auto it = array.find(i);
// 		array.erase(it);
// 	}
// 	bench->watch.stop();
// 	return;
// }

// void
// bm_hash_map(workbench* bench, usize limit)
// {
	
// 	hash_array<usize, usize> array;

// 	bench->watch.start();
// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		array.insert(i, i + 9);
// 	}

// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		auto it = array.lookup(i);
// 		array.remove(it);
// 	}
// 	bench->watch.stop();
// 	return;
// }

// void
// bm_tree_map(workbench* bench, usize limit)
// {
// 	tree_map<usize, usize> array;

// 	bench->watch.start();
// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		array.insert(i, i+9);
// 	}

// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		auto it = array.lookup(i);
// 		array.remove(it);
// 	}
// 	bench->watch.stop();
// 	return;
// }

// void
// bm_custom_tree_map(workbench* bench, usize limit)
// {
// 	arena.free_all();

// 	tree_map<usize, usize> array(arena.context());

// 	bench->watch.start();
// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		array.insert(i, i + 9);
// 	}

// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		auto it = array.lookup(i);
// 		array.remove(it);
// 	}
// 	bench->watch.stop();
// 	return;
// }

// void
// bm_map(workbench* bench, usize limit)
// {
// 	std::map<usize, usize> array;

// 	bench->watch.start();
// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		array.emplace(i, i+9);
// 	}

// 	for (cpprelude::usize i = 0; i < limit; ++i)
// 	{
// 		auto it = array.find(i);
// 		array.erase(it);
// 	}
// 	bench->watch.stop();
// 	return;
// }

// template<typename semaphore_t>
// void
// semaphore_thread_take_func(semaphore_t* sema, cpprelude::usize limit)
// {
// 	for(cpprelude::usize i = 0; i < limit; ++i)
// 		sema->wait_take();
// }

// template<typename semaphore_t>
// void
// semaphore_thread_give_func(semaphore_t* sema, cpprelude::usize limit)
// {
// 	for(cpprelude::usize i = 0; i < limit; ++i)
// 		sema->wait_give();
// }

// void
// check_binary_semaphore(cpprelude::usize limit)
// {
// 	cpprelude::binary_semaphore sema;
// 	auto give_func = semaphore_thread_give_func<cpprelude::binary_semaphore>;
// 	auto take_func = semaphore_thread_take_func<cpprelude::binary_semaphore>;

// 	std::thread g1(give_func, &sema, limit);
// 	std::thread g2(give_func, &sema, limit*3);
// 	std::thread g3(give_func, &sema, limit*2);

// 	std::thread t1(take_func, &sema, limit);
// 	std::thread t2(take_func, &sema, limit);
// 	std::thread t3(take_func, &sema, limit);
// 	std::thread t4(take_func, &sema, limit);
// 	std::thread t5(take_func, &sema, limit);
// 	std::thread t6(take_func, &sema, limit);

// 	g1.join();
// 	g2.join();
// 	g3.join();
// 	t1.join();
// 	t2.join();
// 	t3.join();
// 	t4.join();
// 	t5.join();
// 	t6.join();

// 	std::cout << "check binary_semaphore" << std::endl;
// 	std::cout << "counter: " << sema._counter << std::endl;
// }

// void
// check_count_semaphore(cpprelude::usize limit)
// {
// 	cpprelude::count_semaphore<1024> sema;
// 	auto give_func = semaphore_thread_give_func<cpprelude::count_semaphore<1024>>;
// 	auto take_func = semaphore_thread_take_func<cpprelude::count_semaphore<1024>>;

// 	std::thread g1(give_func, &sema, limit);
// 	std::thread g2(give_func, &sema, limit*3);
// 	std::thread g3(give_func, &sema, limit*2);

// 	std::thread t1(take_func, &sema, limit);
// 	std::thread t2(take_func, &sema, limit);
// 	std::thread t3(take_func, &sema, limit);
// 	std::thread t4(take_func, &sema, limit);
// 	std::thread t5(take_func, &sema, limit);
// 	std::thread t6(take_func, &sema, limit);

// 	g1.join();
// 	g2.join();
// 	g3.join();
// 	t1.join();
// 	t2.join();
// 	t3.join();
// 	t4.join();
// 	t5.join();
// 	t6.join();

// 	std::cout << "check count_semaphore" << std::endl;
// 	std::cout << "counter: " << sema._counter << std::endl;
// }

// void
// thread_unique_dequeue_func(
// 	cpprelude::thread_unique<cpprelude::queue_array<int>>* job_queue,
// 	cpprelude::usize limit)
// {
// 	auto context = job_queue->new_context();

// 	while(limit)
// 	{
// 		if(job_queue->read(context))
// 		{
// 			if(!job_queue->value.empty())
// 			{
// 				job_queue->value.dequeue();
// 				--limit;
// 				job_queue->read_release(context);
// 			}
// 			else
// 			{
// 				job_queue->read_release(context);
// 				continue;
// 			}
// 		}
// 	}

// }

// void
// thread_unique_enqueue_func(
// 	cpprelude::thread_unique<cpprelude::queue_array<int>>* job_queue,
// 	cpprelude::usize limit)
// {
// 	auto context = job_queue->new_context();

// 	while(limit)
// 	{
// 		if(job_queue->write(context))
// 		{
// 			job_queue->value.enqueue(1);
// 			--limit;
// 			job_queue->write_release(context);
// 		}
// 	}
// }

// void
// check_thread_unique(cpprelude::usize limit)
// {
// 	cpprelude::thread_unique<cpprelude::queue_array<int>> job_queue;
// 	auto enqueue_func = thread_unique_enqueue_func;
// 	auto dequeue_func = thread_unique_dequeue_func;

// 	std::thread g1(dequeue_func, &job_queue, limit);
// 	std::thread g2(dequeue_func, &job_queue, limit*3);
// 	std::thread g3(dequeue_func, &job_queue, limit*2);

// 	std::thread t1(enqueue_func, &job_queue, limit);
// 	std::thread t2(enqueue_func, &job_queue, limit);
// 	std::thread t3(enqueue_func, &job_queue, limit);
// 	std::thread t4(enqueue_func, &job_queue, limit);
// 	std::thread t5(enqueue_func, &job_queue, limit);
// 	std::thread t6(enqueue_func, &job_queue, limit);

// 	g1.join();
// 	g2.join();
// 	g3.join();
// 	t1.join();
// 	t2.join();
// 	t3.join();
// 	t4.join();
// 	t5.join();
// 	t6.join();

// 	std::cout << "check thread_unique" << std::endl;
// 	std::cout << "empty: " << job_queue.value.empty() << std::endl;
// 	std::cout << "count: " << job_queue.value.count() << std::endl;
// }

// void
// thread_multi_reader_read_func(
// 	cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>* array,
// 	cpprelude::usize limit)
// {
// 	auto context = array->new_context();
// 	while(true)
// 	{
// 		if(array->read(context))
// 		{
// 			if(array->value.empty())
// 			{
// 				array->read_release(context);
// 				std::this_thread::sleep_for(std::chrono::duration<usize, std::micro>(10));
// 				continue;
// 			}

// 			//std::cout << "thread_multi_reader_read_func.count = " << array->value.count() << std::endl;

// 			for(const auto& x: array->value)
// 				if(x != 1)
// 					std::cout << "ERROR!" << std::endl;
// 			array->read_release(context);
// 			return;
// 		}
// 	}
// }

// void
// thread_multi_reader_insert_func(
// 	cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>* array,
// 	cpprelude::usize limit)
// {
// 	auto context = array->new_context();
// 	while(true)
// 	{
// 		if(array->write(context))
// 		{
// 			//std::cout << "thread_multi_reader_insert_func" << std::endl;
// 			while(limit--)
// 				array->value.insert_back(1);
// 			array->write_release(context);
// 			return;
// 		}

// 	}
// }

// void
// thread_multi_reader_remove_func(
// 	cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>* array,
// 	cpprelude::usize limit)
// {
// 	auto context = array->new_context();
// 	while(true)
// 	{
// 		if(array->write(context))
// 		{
// 			//std::cout << "thread_multi_reader_remove_func" << std::endl;

// 			if(array->value.count() >= limit)
// 			{
// 				array->value.remove_back(limit);
// 				array->write_release(context);
// 				return;
// 			}
// 			else
// 			{
// 				array->write_release(context);
// 				std::this_thread::sleep_for(std::chrono::duration<usize, std::micro>(10));

// 				continue;
// 			}
// 		}
// 	}
// }

// cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>
// thread_multi_reader_job_queue;

// void
// check_thread_multi_reader(cpprelude::usize limit)
// {
// 	static bool read_done = false;

// 	auto insert_func = thread_multi_reader_insert_func;
// 	auto remove_func = thread_multi_reader_remove_func;
// 	auto read_func = thread_multi_reader_read_func;

// 	std::thread g1(remove_func, &thread_multi_reader_job_queue, limit);
// 	std::thread g2(remove_func, &thread_multi_reader_job_queue, limit*3);
// 	std::thread g3(remove_func, &thread_multi_reader_job_queue, limit*2);

// 	if(!read_done)
// 	{
// 		std::thread r1(read_func, &thread_multi_reader_job_queue, limit);
// 		std::thread r2(read_func, &thread_multi_reader_job_queue, limit);
// 		std::thread r3(read_func, &thread_multi_reader_job_queue, limit);
// 		r1.detach();
// 		r2.detach();
// 		r3.detach();
// 		read_done = true;
// 	}

// 	std::thread t1(insert_func, &thread_multi_reader_job_queue, limit);
// 	std::thread t2(insert_func, &thread_multi_reader_job_queue, limit);
// 	std::thread t3(insert_func, &thread_multi_reader_job_queue, limit);
// 	std::thread t4(insert_func, &thread_multi_reader_job_queue, limit);
// 	std::thread t5(insert_func, &thread_multi_reader_job_queue, limit);
// 	std::thread t6(insert_func, &thread_multi_reader_job_queue, limit);

// 	g1.join();
// 	g2.join();
// 	g3.join();

// 	t1.join();
// 	t2.join();
// 	t3.join();
// 	t4.join();
// 	t5.join();
// 	t6.join();

// 	std::cout << "check thread_multi_reader" << std::endl;
// 	std::cout << "empty: " << thread_multi_reader_job_queue.value.empty() << std::endl;
// 	std::cout << "count: " << thread_multi_reader_job_queue.value.count() << std::endl;
// }

// void gen_random(char *s, const int len) {
//     static const char alphanum[] =
//         "0123456789"
//         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//         "abcdefghijklmnopqrstuvwxyz";

//     for (int i = 0; i < len; ++i) {
//         s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
//     }

//     s[len] = 0;
// }

// std::string
// bm_std_string_create(workbench* bench, usize limit)
// {
// 	usize arr_size = (rand()%1000) + 1000;
// 	char* arr = new char[arr_size];
// 	gen_random(arr, arr_size-1);

// 	bench->watch.start();
// 		std::string str;
// 		str = arr;
// 	bench->watch.stop();

// 	delete[] arr;
// 	return str;
// }

// string
// bm_string_create(workbench* bench, usize limit)
// {
// 	usize arr_size = (rand()%1000) + 1000;
// 	char* arr = new char[arr_size];
// 	gen_random(arr, arr_size-1);

// 	bench->watch.start();
// 		string str;
// 		str = arr;
// 	bench->watch.stop();

// 	delete[] arr;
// 	return str;
// }

// string
// bm_custom_string_create(workbench* bench, usize limit)
// {
// 	arena.free_all();

// 	usize arr_size = (rand() % 1000) + 1000;
// 	char* arr = new char[arr_size];
// 	gen_random(arr, arr_size - 1);

// 	bench->watch.start();
// 	string str(arena.context());
// 	str = arr;
// 	bench->watch.stop();

// 	delete[] arr;
// 	return str;
// }

// std::string
// bm_std_stream(workbench* bench, usize limit)
// {
// 	usize arr_size = (rand() % 1000) + 1000;
// 	char* arr = new char[arr_size];
// 	gen_random(arr, arr_size - 1);
// 	std::string arr_str(arr, arr_size);

// 	bench->watch.start();
// 	std::stringstream str;
// 	str << arr_str;
// 	bench->watch.stop();

// 	delete[] arr;
// 	return str.str();
// }

// usize
// bm_stream(workbench* bench, usize limit)
// {
// 	usize arr_size = (rand() % 1000) + 1000;
// 	char* arr = new char[arr_size];
// 	gen_random(arr, arr_size - 1);
// 	auto arr_slice = make_slice(arr, arr_size);
// 	string arr_str(arr_slice);

// 	bench->watch.start();
// 	memory_stream str;
// 	vprints(str, arr_str);
// 	bench->watch.stop();

// 	delete[] arr;
// 	return str.size();
// }

// struct Screamer
// {
// 	Screamer()
// 	{
// 		println("Screamer");
// 	}

// 	Screamer(const Screamer& other)
// 	{
// 		println("Copy Screamer");
// 	}

// 	Screamer(Screamer&& other)
// 	{
// 		println("Move Screamer");
// 	}

// 	Screamer&
// 	operator=(const Screamer& other)
// 	{
// 		println("copy assign screamer");
// 		return *this;
// 	}

// 	Screamer&
// 	operator=(Screamer&& other)
// 	{
// 		println("move assign screamer");
// 		return *this;
// 	}

// 	~Screamer()
// 	{
// 		println("~Screamer");
// 	}
// };

// struct Special_Screamer
// {
// 	Special_Screamer()
// 	{
// 		println("Special_Screamer");
// 	}

// 	Special_Screamer(const Special_Screamer& other)
// 	{
// 		println("Copy Special_Screamer");
// 	}

// 	Special_Screamer(Special_Screamer&& other)
// 	{
// 		println("Move Special_Screamer");
// 	}

// 	Special_Screamer&
// 	operator=(const Special_Screamer& other)
// 	{
// 		println("copy assign Special_Screamer");
// 		return *this;
// 	}

// 	Special_Screamer&
// 	operator=(Special_Screamer&& other)
// 	{
// 		println("move assign Special_Screamer");
// 		return *this;
// 	}

// 	~Special_Screamer()
// 	{
// 		println("~Special_Screamer");
// 	}
// };

// template<typename T1, typename T2>
// struct Two
// {
// 	T1 first;
// 	T2 second;
// };

// Screamer
// optimal_screamer()
// {
// 	return Screamer();
// }

// Special_Screamer
// optimal_special_screamer()
// {
// 	return Special_Screamer();
// }

// Result<Screamer, usize>
// result_screamer()
// {
// 	return Screamer();
// }

// Result<Special_Screamer, usize>
// result_speical_screamer()
// {
// 	return Special_Screamer();
// }

// std::optional<Screamer>
// optional_screamer()
// {
// 	return Screamer();
// }

// std::optional<Special_Screamer>
// optional_special_screamer()
// {
// 	return Special_Screamer();
// }

// std::variant<Screamer, usize>
// variant_screamer()
// {
// 	return Screamer();
// }

// std::variant<Special_Screamer, usize>
// variant_special_screamer()
// {
// 	return Special_Screamer();
// }

// Two<Screamer, usize>
// sb_screamer()
// {
// 	return { Screamer(), 0 };
// }

// Two<Special_Screamer, usize>
// sb_special_screamer()
// {
// 	return { Special_Screamer(), 0 };
// }

template<typename T>
void
print_stuff(void* data)
{
	T* p = (T*)data;
	std::cout << *p;
}

struct generic_data
{
	using print_func = void(*)(void*);
	void* ptr;
	print_func _print;

	template<typename T>
	generic_data(const T& data)
	{
		ptr = (void*)(&data);
		_print = print_stuff<T>;
	}
};

template<typename ... TArgs>
void
do_stuff(TArgs&& ... args)
{
	generic_data arr[] = { args... };
	println(sizeof...(args));
	for (usize i = 0; i < sizeof...(args); ++i)
	{
		arr[i]._print(arr[i].ptr);
	}
}


void
do_benchmark()
{
	#ifdef DEBUG
		cpprelude::usize limit = 1000;
	#else
		cpprelude::usize limit = 10000;
	#endif

	// println(vprintb(os->unbuf_stdout, "abc"));
	// println(vprints(os->unbuf_stdout, 1, 2, 3, const_str("Mostafa")));
	// println(print_str(os->unbuf_stdout, 1.21f));
	// println();
	// println(vprintf(os->unbuf_stdout, "Mo{{{0}ka {1:.3f}", 1213, 1.2134f));
	println(vprintf(os->unbuf_stdout, "{:0>+4d}\n{:0>+4d}\n", 110, 1));
	//do_stuff("sdfdsf", 34234, 234234);
	return;
	auto handle = os->file_open("My_Stuff.txt").value;
	os->file_close(handle);

	auto sfd = const_str("mostafa");
	os->unbuf_stdout->write(sfd.bytes.convert<byte>());
	auto sfd2 = const_str("مصطفى");
	os->unbuf_stdout->write(sfd2.bytes.convert<byte>());

	std::cout << "\nBENCHMARK START\n" << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_vector, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Dynamic_Array, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_forward_list, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Single_List, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_list, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Double_List, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_stack, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Stack_Array, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Stack_List, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_queue, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Ring_Array, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Queue_List, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_priority_queue, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Priority_Queue, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_map, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Tree_Map, limit)
	});

	std::cout << std::endl << std::endl;

	gen_random_str(limit);

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_string_create, limit),
		CPPRELUDE_BENCHMARK(bm_v5_String, limit)
	});

	delete[] random_str;

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_string_concat, limit),
		CPPRELUDE_BENCHMARK(bm_v5_String_concat, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_unordered_set, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Hash_Set, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_unordered_map, limit),
		CPPRELUDE_BENCHMARK(bm_v5_Hash_Map, limit),
		CPPRELUDE_BENCHMARK(bm_hash_array, limit)
	});

	std::cout << std::endl << std::endl;

	generate_random_data(limit);

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_heap_sort, limit),
		CPPRELUDE_BENCHMARK(bm_v5_heap_sort, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_insertion_sort, limit),
		CPPRELUDE_BENCHMARK(bm_v5_insertion_sort, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_merge_sort, limit),
		CPPRELUDE_BENCHMARK(bm_merge_sort, limit),
		CPPRELUDE_BENCHMARK(bm_v5_merge_sort, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_quick_sort, limit),
		CPPRELUDE_BENCHMARK(bm_quick_sort, limit),
		CPPRELUDE_BENCHMARK(bm_v5_quick_sort, limit)
	});

	std::cout << "\nBENCHMARK END\n" << std::endl;
}