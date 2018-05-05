#include "benchmark.h"

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

#include <cpprelude/v5/Panic.h>
#include <cpprelude/v5/Memory_Stream.h>
#include <cpprelude/v5/Buffered_Stream.h>
#include <cpprelude/v5/File.h>
#include <cpprelude/v5/Benchmark.h>

using namespace cpprelude;

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
bm_Dynamic_Array(Stopwatch& watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Dynamic_Array<usize> array;

		for(usize i = 0; i < limit; ++i)
			array.insert_back(i + r);

		for(const auto& number: array)
			if(number % 3 == 0)
				r += number;
	}
	watch.stop();

	return r;
}

usize
bm_vector(Stopwatch& watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::vector<usize> array;

		for(usize i = 0; i < limit; ++i)
			array.push_back(i + r);

		for (const auto& number : array)
			if (number % 3 == 0)
				r += number;
	}
	watch.stop();

	return r;
}


usize
bm_Single_List(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Single_List<usize> list;
		for(usize i = 0; i < limit; ++i)
			list.insert_front(i + r);

		for (const auto& number : list)
			if (number % 2 == 0)
				r += number;
	}
	watch.stop();
	return r;
}


usize
bm_forward_list(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::forward_list<usize> list;
		for(usize i = 0; i < limit; ++i)
			list.push_front(i + r);

		for(auto& number: list)
			if(number % 2 == 0)
				r += number;
	}
	watch.stop();
	return r;
}

usize
bm_Double_List(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Double_List<usize> list;
		for(usize i = 0; i < limit; ++i)
			if((i + r) % 2 == 0)
				list.emplace_back(i + r);
			else
				list.emplace_front(i + r);

		for(const auto& number: list)
			if(number % 2 == 0)
				r += number;
	}
	watch.stop();
	return r;
}


usize
bm_list(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::list<usize> list;
		for(usize i = 0; i < limit; ++i)
			if((i + r) % 2 == 0)
				list.push_back(i + r);
			else
				list.push_front(i + r);

		for(const auto& number: list)
			if(number % 2 == 0)
				r += number;
	}
	watch.stop();
	return r;
}


usize
bm_Stack_Array(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Stack_Array<usize> stack;
		for (usize i = 0; i < limit; ++i)
			if ((i + r) % 2 == 0)
				stack.push(i + r);

		while (!stack.empty())
		{
			r += stack.top();
			stack.pop();
		}
	}
	watch.stop();
	return r;
}

usize
bm_Stack_List(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Stack_List<usize> stack;
		for (usize i = 0; i < limit; ++i)
			if ((i + r) % 2 == 0)
				stack.push(i + r);

		while (!stack.empty())
		{
			r += stack.top();
			stack.pop();
		}
	}
	watch.stop();
	return r;
}

usize
bm_stack(Stopwatch &watch, usize limit)
{
	usize r = rand();
	watch.start();
	{
		std::stack<usize> stack;
		for (usize i = 0; i < limit; ++i)
			if ((i + r) % 2 == 0)
				stack.push(i + r);

		while (!stack.empty())
		{
			r += stack.top();
			stack.pop();
		}
	}
	watch.stop();
	return r;
}


usize
bm_Ring_Array(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Ring_Array<usize> que;
		que.reserve(limit);
		for (usize i = 0; i < limit; ++i)
			que.insert_front(i + r);

		while(!que.empty())
		{
			r += que.front();
			que.remove_front();
		}
	}
	watch.stop();
	return r;
}

usize
bm_Queue_List(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Queue_List<usize> que;
		for (usize i = 0; i < limit; ++i)
			que.enqueue(i + r);

		while (!que.empty())
		{
			r += que.front();
			que.dequeue();
		}
	}
	watch.stop();
	return r;
}

usize
bm_queue(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::queue<usize> que;
		for (usize i = 0; i < limit; ++i)
			que.push(i + r);

		while (!que.empty())
		{
			r += que.front();
			que.pop();
		}
	}
	watch.stop();
	return r;
}


usize
bm_Priority_Queue(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		Priority_Queue<usize> array;
		for(usize i = 0; i < limit; ++i)
			array.enqueue(i + r);

		while(!array.empty())
		{
			if(array.front() % 2 == 0)
				r += array.front();
			array.dequeue();
		}
	}
	watch.stop();
	return r;
}

usize
bm_priority_queue(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
		std::priority_queue<usize> array;
		for(usize i = 0; i < limit; ++i)
			array.push(i + r);

		while(!array.empty())
		{
			if(array.top() % 2 == 0)
				r += array.top();
			array.pop();
		}
	}
	watch.stop();
	return r;
}


usize
bm_Tree_Map(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
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
	}
	watch.stop();

	return r;
}

usize
bm_map(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
	{
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
	}
	watch.stop();

	return r;
}


usize
bm_Hash_Set(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
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
	watch.stop();
	return r;
}

usize
bm_unordered_set(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
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
	watch.stop();
	return r;
}


usize
bm_Hash_Map(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
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
	watch.stop();
	return r;
}

usize
bm_unordered_map(Stopwatch &watch, usize limit)
{
	usize r = rand();

	watch.start();
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
	watch.stop();
	return r;
}


void
bm_String(Stopwatch &watch, usize limit)
{
	watch.start();
	{
		String str;
		for(usize i = 0; i < limit; ++i)
			str.concat("Mostafa");
	}
	watch.stop();
}

void
bm_string(Stopwatch &watch, usize limit)
{
	watch.start();
	{
		std::string str;
		for(usize i = 0; i < limit; ++i)
			str += "Mostafa";
	}
	watch.stop();
}


bool
bm_heap_sort(Stopwatch &watch, usize limit)
{
	Dynamic_Array<cpprelude::usize> array = RANDOM_ARRAY;

	watch.start();
		cpprelude::heap_sort(array.all());
	watch.stop();

	bool result = cpprelude::is_sorted(array.all());
	assert(result);

	return result;
}

bool
bm_std_heap_sort(Stopwatch &watch, usize limit)
{
	std::vector<cpprelude::usize> array(RANDOM_ARRAY.begin(), RANDOM_ARRAY.end());

	watch.start();
		std::make_heap(array.begin(), array.end());
		std::sort_heap(array.begin(), array.end());
	watch.stop();

	bool result = std::is_sorted(array.begin(), array.end());
	assert(result);

	return result;
}


bool
bm_merge_sort(Stopwatch &watch, usize limit)
{
	Dynamic_Array<cpprelude::usize> array = RANDOM_ARRAY;

	watch.start();
		cpprelude::merge_sort(array.all());
	watch.stop();

	bool result = cpprelude::is_sorted(array.all());
	assert(result);

	return result;
}

bool
bm_std_merge_sort(Stopwatch &watch, usize limit)
{
	std::vector<cpprelude::usize> array(RANDOM_ARRAY.begin(), RANDOM_ARRAY.end());

	watch.start();
		std::stable_sort(array.begin(), array.end());
	watch.stop();

	bool result = std::is_sorted(array.begin(), array.end());
	assert(result);

	return result;
}


bool
bm_quick_sort(Stopwatch &watch, usize limit)
{
	Dynamic_Array<cpprelude::usize> array = RANDOM_ARRAY;

	watch.start();
		cpprelude::quick_sort(array.all());
	watch.stop();

	bool result = cpprelude::is_sorted(array.all());
	assert(result);

	return result;
}

bool
bm_std_quick_sort(Stopwatch &watch, usize limit)
{
	std::vector<cpprelude::usize> array(RANDOM_ARRAY.begin(), RANDOM_ARRAY.end());

	watch.start();
		std::sort(array.begin(), array.end());
	watch.stop();

	bool result = std::is_sorted(array.begin(), array.end());
	assert(result);

	return result;
}




void
do_benchmark()
{
	cpprelude::usize limit = 1000;
	
	//Memory_Stream stream;
	//vprintf(stream, "Hello!, My name is {}, and my age is {}.", "Mostafa", 25);

	/*File file = unwrap(File::open("leak.txt"_const_str, IO_MODE2::READ, OPEN_MODE2::OPEN_ONLY), OS_ERROR::OK, "My File didn't open");
	
	while(true)
	{
		String value;
		usize read_size = readln(value);
		cpprelude::printf("read_size: {}, word: ({})\n", read_size, value);
		if (value == "close" ||
			value == "اغلق")
			break;
	}
	return;*/

	compare_benchmarks(
		summary("std::vector", [&](Stopwatch& watch)
		{
			bm_vector(watch, limit);
		}),

		summary("Dynamic_Array", [&](Stopwatch& watch)
		{
			bm_Dynamic_Array(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::string", [&](Stopwatch& watch)
		{
			bm_string(watch, limit);
		}),

		summary("String", [&](Stopwatch& watch)
		{
			bm_String(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::forward_list", [&](Stopwatch& watch)
		{
			bm_forward_list(watch, limit);
		}),

		summary("Single_List", [&](Stopwatch& watch)
		{
			bm_Single_List(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::list", [&](Stopwatch& watch)
		{
			bm_list(watch, limit);
		}),

		summary("Double_List", [&](Stopwatch& watch)
		{
			bm_Double_List(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::stack", [&](Stopwatch& watch)
		{
			bm_stack(watch, limit);
		}),

		summary("Stack_Array", [&](Stopwatch& watch)
		{
			bm_Stack_Array(watch, limit);
		}),

		summary("Stack_List", [&](Stopwatch& watch)
		{
			bm_Stack_List(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::queue", [&](Stopwatch& watch)
		{
			bm_queue(watch, limit);
		}),

		summary("Ring_Array", [&](Stopwatch& watch)
		{
			bm_Ring_Array(watch, limit);
		}),

		summary("Queue_List", [&](Stopwatch& watch)
		{
			bm_Queue_List(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::priority_queue", [&](Stopwatch& watch)
		{
			bm_priority_queue(watch, limit);
		}),

		summary("Priority_Queue", [&](Stopwatch& watch)
		{
			bm_Priority_Queue(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::map", [&](Stopwatch& watch)
		{
			bm_map(watch, limit);
		}),

		summary("Tree_Map", [&](Stopwatch& watch)
		{
			bm_Tree_Map(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::unordered_set", [&](Stopwatch& watch)
		{
			bm_unordered_set(watch, limit);
		}),

		summary("Hash_Set", [&](Stopwatch& watch)
		{
			bm_Hash_Set(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::unordered_map", [&](Stopwatch& watch)
		{
			bm_unordered_map(watch, limit);
		}),

		summary("Hash_Map", [&](Stopwatch& watch)
		{
			bm_Hash_Map(watch, limit);
		})
	);

	generate_random_data(limit);

	println();

	compare_benchmarks(
		summary("std::heap_sort", [&](Stopwatch& watch)
		{
			bm_std_heap_sort(watch, limit);
		}),

		summary("heap_sort", [&](Stopwatch& watch)
		{
			bm_heap_sort(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::stable_sort", [&](Stopwatch& watch)
		{
			bm_std_merge_sort(watch, limit);
		}),

		summary("merge_sort", [&](Stopwatch& watch)
		{
			bm_merge_sort(watch, limit);
		})
	);

	println();

	compare_benchmarks(
		summary("std::sort", [&](Stopwatch& watch)
		{
			bm_std_quick_sort(watch, limit);
		}),

		summary("quick_sort", [&](Stopwatch& watch)
		{
			bm_quick_sort(watch, limit);
		})
	);
}