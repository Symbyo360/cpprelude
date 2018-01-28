#include "benchmark.h"
#include "stopwatch.h"

#include <cpprelude/dynamic_array.h>
#include <vector>

#include <cpprelude/stack_array.h>
#include <cpprelude/stack_list.h>
#include <stack>

#include <cpprelude/slinked_list.h>
#include <cpprelude/dlinked_list.h>
#include <cpprelude/bucket_array.h>
#include <list>
#include <deque>
#include <forward_list>

#include <cpprelude/queue_list.h>
#include <cpprelude/queue_array.h>
#include <queue>
#include <cpprelude/priority_queue.h>

#include <cpprelude/algorithm.h>
#include <algorithm>

#include <cpprelude/threading.h>
#include <functional>

#include <cpprelude/string.h>
#include <string>
#include <sstream>
#include <cpprelude/stream.h>
#include <cpprelude/fmt.h>

#include <cpprelude/hash_array.h>
#include <unordered_map>
#include <cpprelude/tree_map.h>
#include <map>

#include <cpprelude/allocator.h>

#include <iostream>
#include <cpprelude/micro_benchmark.h>
#include <cpprelude/memory_watcher.h>

using namespace cpprelude;

cpprelude::arena_t arena(MEGABYTES(100));

//Vector like container
usize
bm_dynamic_array(workbench* bench, usize limit)
{
	memory_watcher w("dynamic_array");

	dynamic_array<usize> array;
	
	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.insert_back(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
bm_custom_dynamic_array(workbench* bench, usize limit)
{
	memory_watcher w;

	arena.free_all();
	dynamic_array<usize> array(arena.context());

	bench->watch.start();
	for (usize i = 0; i < limit; ++i)
		array.insert_back(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
bm_vector(workbench* bench, usize limit)
{
	std::vector<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push_back(rand());

	bench->watch.stop();

	return *array.begin();
}

//single linked list containers
usize
bm_slinked_list(workbench* bench, usize limit)
{
	slinked_list<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.insert_front(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
bm_custom_slinked_list(workbench* bench, usize limit)
{
	arena.free_all();

	slinked_list<usize> array(arena.context());
	bench->watch.start();

	for (usize i = 0; i < limit; ++i)
		array.insert_front(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
bm_forward_list(workbench* bench, usize limit)
{
	std::forward_list<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push_front(rand());

	bench->watch.stop();

	return *array.begin();
}

//double linked list container
usize
bm_dlinked_list(workbench* bench, usize limit)
{
	dlinked_list<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.insert_front(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
bm_custom_dlinked_list(workbench* bench, usize limit)
{
	arena.free_all();

	dlinked_list<usize> array(arena.context());
	bench->watch.start();

	for (usize i = 0; i < limit; ++i)
		array.insert_front(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
bm_list(workbench* bench, usize limit)
{
	std::list<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push_front(rand());

	bench->watch.stop();

	return *array.begin();
}

//stack containers
usize
bm_stack_array(workbench* bench, usize limit)
{
	stack_array<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.top();
}

usize
bm_custom_stack_array(workbench* bench, usize limit)
{
	arena.free_all();

	stack_array<usize> array(arena.context());
	bench->watch.start();

	for (usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.top();
}

usize
bm_stack_list(workbench* bench, usize limit)
{
	stack_list<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.top();
}

usize
bm_custom_stack_list(workbench* bench, usize limit)
{
	arena.free_all();

	stack_list<usize> array(arena.context());
	bench->watch.start();

	for (usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.top();
}

usize
bm_stack(workbench* bench, usize limit)
{
	std::stack<usize> array;
	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.top();
}

//queue containers
usize
bm_queue_array(workbench* bench, usize limit)
{
	queue_array<usize> array;

	bench->watch.start();

	for(usize i = 0; i < limit; ++i)
		array.enqueue(rand());

	bench->watch.stop();

	return array.front();
}

usize
bm_custom_queue_array(workbench* bench, usize limit)
{
	arena.free_all();

	queue_array<usize> array(arena.context());

	bench->watch.start();

	for (usize i = 0; i < limit; ++i)
		array.enqueue(rand());

	bench->watch.stop();

	return array.front();
}

usize
bm_queue_list(workbench* bench, usize limit)
{
	queue_list<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.enqueue(rand());

	bench->watch.stop();

	return array.front();
}

usize
bm_custom_queue_list(workbench* bench, usize limit)
{
	arena.free_all();

	queue_list<usize> array(arena.context());

	bench->watch.start();
	for (usize i = 0; i < limit; ++i)
		array.enqueue(rand());

	bench->watch.stop();

	return array.front();
}

usize
bm_queue(workbench* bench, usize limit)
{
	std::queue<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.front();
}

//priority queue containers
usize
bm_priority_queue(workbench* bench, usize limit)
{
	priority_queue<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.enqueue(rand());

	bench->watch.stop();

	return array.front();
}

usize
bm_custom_priority_queue(workbench* bench, usize limit)
{
	arena.free_all();

	priority_queue<usize> array(arena.context());

	bench->watch.start();
	for (usize i = 0; i < limit; ++i)
		array.enqueue(rand());

	bench->watch.stop();

	return array.front();
}

usize
bm_std_priority_queue(workbench* bench, usize limit)
{
	std::priority_queue<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.push(rand());

	bench->watch.stop();

	return array.top();
}

//deque containers
usize
bm_bucket_array(workbench* bench, usize limit)
{
	bucket_array<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.insert_back(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
bm_custom_bucket_array(workbench* bench, usize limit)
{
	arena.free_all();

	bucket_array<usize> array(arena.context());

	bench->watch.start();
	for (usize i = 0; i < limit; ++i)
		array.insert_back(rand());

	bench->watch.stop();

	return *array.begin();
}

usize
bm_deque(workbench* bench, usize limit)
{
	std::deque<usize> array;

	bench->watch.start();
	for(usize i = 0; i < limit; ++i)
		array.push_back(rand());

	bench->watch.stop();

	return *array.begin();
}

//merge_sort
void
bm_merge_sort(workbench* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	cpprelude::dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(distribution(generator));

	bench->watch.start();
		cpprelude::merge_sort(array.begin(), array.count());
	bench->watch.stop();
}
void
bm_std_stable_sort(workbench* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	std::vector<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push_back(distribution(generator));

	bench->watch.start();
		std::stable_sort(array.begin(), array.end());
	bench->watch.stop();
}

void
bm_cppr_stable_sort(workbench* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	cpprelude::dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(distribution(generator));

	bench->watch.start();
		std::stable_sort(array.begin(), array.end());
	bench->watch.stop();
}

//quick sort
void
bm_quick_sort(workbench* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	cpprelude::dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(distribution(generator));

	bench->watch.start();
		cpprelude::quick_sort(array.begin(), array.count());
	bench->watch.stop();
}

void
bm_std_sort(workbench* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	std::vector<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push_back(distribution(generator));

	bench->watch.start();
		std::sort(array.begin(), array.end());
	bench->watch.stop();
}

void
bm_cppr_sort(workbench* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	cpprelude::dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(distribution(generator));

	bench->watch.start();
		std::sort(array.begin(), array.end());
	bench->watch.stop();
}

void
bm_heap_sort(workbench* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	cpprelude::dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(distribution(generator));

	bench->watch.start();
		cpprelude::heap_sort(array.begin(), array.count());
	bench->watch.stop();
}

void
bm_std_heap_sort(workbench* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	std::vector<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.push_back(distribution(generator));

	bench->watch.start();
		std::make_heap(array.begin(), array.end());
		std::sort_heap(array.begin(), array.end());
	bench->watch.stop();
}

void
bm_cppr_heap_sort(workbench* bench, usize limit)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<cpprelude::usize> distribution(0, limit);

	cpprelude::dynamic_array<cpprelude::usize> array;

	for(cpprelude::usize i = 0; i < limit; ++i)
		array.insert_back(distribution(generator));

	bench->watch.start();
		std::make_heap(array.begin(), array.end());
		std::sort_heap(array.begin(), array.end());
	bench->watch.stop();
}

void
bm_hash_array(workbench* bench, usize limit)
{
	hash_array<usize, usize> array;

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.insert(i, i+9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.lookup(i);
		array.remove(it);
	}
	bench->watch.stop();
	return;
}

void
bm_custom_hash_array(workbench* bench, usize limit)
{
	arena.free_all();

	hash_array<usize, usize> array(arena.context());

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.insert(i, i + 9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.lookup(i);
		array.remove(it);
	}
	bench->watch.stop();
	return;
}

void
bm_unordered_map(workbench* bench, usize limit)
{
	std::unordered_map<usize, usize> array;

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.emplace(i, i+9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.find(i);
		array.erase(it);
	}
	bench->watch.stop();
	return;
}

void
bm_hash_map(workbench* bench, usize limit)
{
	hash_array<usize, usize> array;

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.insert(i, i + 9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.lookup(i);
		array.remove(it);
	}
	bench->watch.stop();
	return;
}

void
bm_tree_map(workbench* bench, usize limit)
{
	tree_map<usize, usize> array;

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.insert(i, i+9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.lookup(i);
		array.remove(it);
	}
	bench->watch.stop();
	return;
}

void
bm_custom_tree_map(workbench* bench, usize limit)
{
	arena.free_all();

	tree_map<usize, usize> array(arena.context());

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.insert(i, i + 9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.lookup(i);
		array.remove(it);
	}
	bench->watch.stop();
	return;
}

void
bm_map(workbench* bench, usize limit)
{
	std::map<usize, usize> array;

	bench->watch.start();
	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		array.emplace(i, i+9);
	}

	for (cpprelude::usize i = 0; i < limit; ++i)
	{
		auto it = array.find(i);
		array.erase(it);
	}
	bench->watch.stop();
	return;
}

template<typename semaphore_t>
void
semaphore_thread_take_func(semaphore_t* sema, cpprelude::usize limit)
{
	for(cpprelude::usize i = 0; i < limit; ++i)
		sema->wait_take();
}

template<typename semaphore_t>
void
semaphore_thread_give_func(semaphore_t* sema, cpprelude::usize limit)
{
	for(cpprelude::usize i = 0; i < limit; ++i)
		sema->wait_give();
}

void
check_binary_semaphore(cpprelude::usize limit)
{
	cpprelude::binary_semaphore sema;
	auto give_func = semaphore_thread_give_func<cpprelude::binary_semaphore>;
	auto take_func = semaphore_thread_take_func<cpprelude::binary_semaphore>;

	std::thread g1(give_func, &sema, limit);
	std::thread g2(give_func, &sema, limit*3);
	std::thread g3(give_func, &sema, limit*2);

	std::thread t1(take_func, &sema, limit);
	std::thread t2(take_func, &sema, limit);
	std::thread t3(take_func, &sema, limit);
	std::thread t4(take_func, &sema, limit);
	std::thread t5(take_func, &sema, limit);
	std::thread t6(take_func, &sema, limit);

	g1.join();
	g2.join();
	g3.join();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	std::cout << "check binary_semaphore" << std::endl;
	std::cout << "counter: " << sema._counter << std::endl;
}

void
check_count_semaphore(cpprelude::usize limit)
{
	cpprelude::count_semaphore<1024> sema;
	auto give_func = semaphore_thread_give_func<cpprelude::count_semaphore<1024>>;
	auto take_func = semaphore_thread_take_func<cpprelude::count_semaphore<1024>>;

	std::thread g1(give_func, &sema, limit);
	std::thread g2(give_func, &sema, limit*3);
	std::thread g3(give_func, &sema, limit*2);

	std::thread t1(take_func, &sema, limit);
	std::thread t2(take_func, &sema, limit);
	std::thread t3(take_func, &sema, limit);
	std::thread t4(take_func, &sema, limit);
	std::thread t5(take_func, &sema, limit);
	std::thread t6(take_func, &sema, limit);

	g1.join();
	g2.join();
	g3.join();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	std::cout << "check count_semaphore" << std::endl;
	std::cout << "counter: " << sema._counter << std::endl;
}

void
thread_unique_dequeue_func(
	cpprelude::thread_unique<cpprelude::queue_array<int>>* job_queue,
	cpprelude::usize limit)
{
	auto context = job_queue->new_context();

	while(limit)
	{
		if(job_queue->read(context))
		{
			if(!job_queue->value.empty())
			{
				job_queue->value.dequeue();
				--limit;
				job_queue->read_release(context);
			}
			else
			{
				job_queue->read_release(context);
				continue;
			}
		}
	}

}

void
thread_unique_enqueue_func(
	cpprelude::thread_unique<cpprelude::queue_array<int>>* job_queue,
	cpprelude::usize limit)
{
	auto context = job_queue->new_context();

	while(limit)
	{
		if(job_queue->write(context))
		{
			job_queue->value.enqueue(1);
			--limit;
			job_queue->write_release(context);
		}
	}
}

void
check_thread_unique(cpprelude::usize limit)
{
	cpprelude::thread_unique<cpprelude::queue_array<int>> job_queue;
	auto enqueue_func = thread_unique_enqueue_func;
	auto dequeue_func = thread_unique_dequeue_func;

	std::thread g1(dequeue_func, &job_queue, limit);
	std::thread g2(dequeue_func, &job_queue, limit*3);
	std::thread g3(dequeue_func, &job_queue, limit*2);

	std::thread t1(enqueue_func, &job_queue, limit);
	std::thread t2(enqueue_func, &job_queue, limit);
	std::thread t3(enqueue_func, &job_queue, limit);
	std::thread t4(enqueue_func, &job_queue, limit);
	std::thread t5(enqueue_func, &job_queue, limit);
	std::thread t6(enqueue_func, &job_queue, limit);

	g1.join();
	g2.join();
	g3.join();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	std::cout << "check thread_unique" << std::endl;
	std::cout << "empty: " << job_queue.value.empty() << std::endl;
	std::cout << "count: " << job_queue.value.count() << std::endl;
}

void
thread_multi_reader_read_func(
	cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>* array,
	cpprelude::usize limit)
{
	auto context = array->new_context();
	while(true)
	{
		if(array->read(context))
		{
			if(array->value.empty())
			{
				array->read_release(context);
				std::this_thread::sleep_for(std::chrono::duration<usize, std::micro>(10));
				continue;
			}

			//std::cout << "thread_multi_reader_read_func.count = " << array->value.count() << std::endl;

			for(const auto& x: array->value)
				if(x != 1)
					std::cout << "ERROR!" << std::endl;
			array->read_release(context);
			return;
		}
	}
}

void
thread_multi_reader_insert_func(
	cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>* array,
	cpprelude::usize limit)
{
	auto context = array->new_context();
	while(true)
	{
		if(array->write(context))
		{
			//std::cout << "thread_multi_reader_insert_func" << std::endl;
			while(limit--)
				array->value.insert_back(1);
			array->write_release(context);
			return;
		}

	}
}

void
thread_multi_reader_remove_func(
	cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>* array,
	cpprelude::usize limit)
{
	auto context = array->new_context();
	while(true)
	{
		if(array->write(context))
		{
			//std::cout << "thread_multi_reader_remove_func" << std::endl;

			if(array->value.count() >= limit)
			{
				array->value.remove_back(limit);
				array->write_release(context);
				return;
			}
			else
			{
				array->write_release(context);
				std::this_thread::sleep_for(std::chrono::duration<usize, std::micro>(10));

				continue;
			}
		}
	}
}

cpprelude::thread_multi_reader<cpprelude::dynamic_array<int>>
thread_multi_reader_job_queue;

void
check_thread_multi_reader(cpprelude::usize limit)
{
	static bool read_done = false;

	auto insert_func = thread_multi_reader_insert_func;
	auto remove_func = thread_multi_reader_remove_func;
	auto read_func = thread_multi_reader_read_func;

	std::thread g1(remove_func, &thread_multi_reader_job_queue, limit);
	std::thread g2(remove_func, &thread_multi_reader_job_queue, limit*3);
	std::thread g3(remove_func, &thread_multi_reader_job_queue, limit*2);

	if(!read_done)
	{
		std::thread r1(read_func, &thread_multi_reader_job_queue, limit);
		std::thread r2(read_func, &thread_multi_reader_job_queue, limit);
		std::thread r3(read_func, &thread_multi_reader_job_queue, limit);
		r1.detach();
		r2.detach();
		r3.detach();
		read_done = true;
	}

	std::thread t1(insert_func, &thread_multi_reader_job_queue, limit);
	std::thread t2(insert_func, &thread_multi_reader_job_queue, limit);
	std::thread t3(insert_func, &thread_multi_reader_job_queue, limit);
	std::thread t4(insert_func, &thread_multi_reader_job_queue, limit);
	std::thread t5(insert_func, &thread_multi_reader_job_queue, limit);
	std::thread t6(insert_func, &thread_multi_reader_job_queue, limit);

	g1.join();
	g2.join();
	g3.join();

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	std::cout << "check thread_multi_reader" << std::endl;
	std::cout << "empty: " << thread_multi_reader_job_queue.value.empty() << std::endl;
	std::cout << "count: " << thread_multi_reader_job_queue.value.count() << std::endl;
}

void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

std::string
bm_std_string_create(workbench* bench, usize limit)
{
	usize arr_size = (rand()%1000) + 1000;
	char* arr = new char[arr_size];
	gen_random(arr, arr_size-1);

	bench->watch.start();
		std::string str;
		str = arr;
	bench->watch.stop();

	delete[] arr;
	return str;
}

string
bm_string_create(workbench* bench, usize limit)
{
	usize arr_size = (rand()%1000) + 1000;
	char* arr = new char[arr_size];
	gen_random(arr, arr_size-1);

	bench->watch.start();
		string str;
		str = arr;
	bench->watch.stop();

	delete[] arr;
	return str;
}

string
bm_custom_string_create(workbench* bench, usize limit)
{
	arena.free_all();

	usize arr_size = (rand() % 1000) + 1000;
	char* arr = new char[arr_size];
	gen_random(arr, arr_size - 1);

	bench->watch.start();
	string str(arena.context());
	str = arr;
	bench->watch.stop();

	delete[] arr;
	return str;
}

std::string
bm_std_stream(workbench* bench, usize limit)
{
	usize arr_size = (rand() % 1000) + 1000;
	char* arr = new char[arr_size];
	gen_random(arr, arr_size - 1);
	std::string arr_str(arr, arr_size);

	bench->watch.start();
	std::stringstream str;
	str << arr_str;
	bench->watch.stop();

	delete[] arr;
	return str.str();
}

usize
bm_stream(workbench* bench, usize limit)
{
	usize arr_size = (rand() % 1000) + 1000;
	char* arr = new char[arr_size];
	gen_random(arr, arr_size - 1);
	auto arr_slice = make_slice(arr, arr_size);
	string arr_str(arr_slice);

	bench->watch.start();
	memory_stream str;
	vprints(str, arr_str);
	bench->watch.stop();

	delete[] arr;
	return str.size();
}

void
do_benchmark()
{
	cpprelude::usize limit = 100;

	std::cout << "\nBENCHMARK START\n" << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_vector, limit),
		CPPRELUDE_BENCHMARK(bm_dynamic_array, limit),
		CPPRELUDE_BENCHMARK(bm_custom_dynamic_array, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_forward_list, limit),
		CPPRELUDE_BENCHMARK(bm_slinked_list, limit),
		CPPRELUDE_BENCHMARK(bm_custom_slinked_list, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_list, limit),
		CPPRELUDE_BENCHMARK(bm_dlinked_list, limit),
		CPPRELUDE_BENCHMARK(bm_custom_dlinked_list, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_stack, limit),
		CPPRELUDE_BENCHMARK(bm_stack_array, limit),
		CPPRELUDE_BENCHMARK(bm_stack_list, limit),
		CPPRELUDE_BENCHMARK(bm_custom_stack_array, limit),
		CPPRELUDE_BENCHMARK(bm_custom_stack_list, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_queue, limit),
		CPPRELUDE_BENCHMARK(bm_queue_array, limit),
		CPPRELUDE_BENCHMARK(bm_queue_list, limit),
		CPPRELUDE_BENCHMARK(bm_custom_queue_array, limit),
		CPPRELUDE_BENCHMARK(bm_custom_queue_list, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_priority_queue, limit),
		CPPRELUDE_BENCHMARK(bm_priority_queue, limit),
		CPPRELUDE_BENCHMARK(bm_custom_priority_queue, limit),
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_deque, limit),
		CPPRELUDE_BENCHMARK(bm_bucket_array, limit),
		CPPRELUDE_BENCHMARK(bm_custom_bucket_array, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_stable_sort, limit),
		CPPRELUDE_BENCHMARK(bm_cppr_stable_sort, limit),
		CPPRELUDE_BENCHMARK(bm_merge_sort, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_sort, limit),
		CPPRELUDE_BENCHMARK(bm_cppr_sort, limit),
		CPPRELUDE_BENCHMARK(bm_quick_sort, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_heap_sort, limit),
		CPPRELUDE_BENCHMARK(bm_cppr_heap_sort, limit),
		CPPRELUDE_BENCHMARK(bm_heap_sort, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_unordered_map, limit),
		CPPRELUDE_BENCHMARK(bm_hash_array, limit),
		CPPRELUDE_BENCHMARK(bm_custom_hash_array, limit)
	});

	std::cout << std::endl << std::endl;
	
	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_map, limit),
		CPPRELUDE_BENCHMARK(bm_tree_map, limit),
		CPPRELUDE_BENCHMARK(bm_custom_tree_map, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_string_create, limit),
		CPPRELUDE_BENCHMARK(bm_string_create, limit),
		CPPRELUDE_BENCHMARK(bm_custom_string_create, limit)
	});

	std::cout << std::endl << std::endl;

	compare_benchmark(std::cout, {
		CPPRELUDE_BENCHMARK(bm_std_stream, limit),
		CPPRELUDE_BENCHMARK(bm_stream, limit)
	});

	std::cout << std::endl;

	check_binary_semaphore(limit/10);

	std::cout << std::endl;

	check_count_semaphore(limit/10);

	std::cout << std::endl;

	check_thread_unique(limit/10);

	std::cout << std::endl;

	check_thread_multi_reader(limit/10);

	std::cout << "\nBENCHMARK END\n" << std::endl;
}
