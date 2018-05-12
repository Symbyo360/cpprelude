#include "catch.hpp"
#include <cpprelude/Queue_List.h>

using namespace cppr;


TEST_CASE("queue_list", "[queue_list]")
{
	SECTION("Case 01")
	{
		Queue_List<usize> stack;
		CHECK(stack.empty() == true);
		CHECK(stack.count() == 0);
	}

	SECTION("Case 02")
	{
		Queue_List<usize> queue;
		queue.emplace(1);
		queue.enqueue(2);
		queue.enqueue(3);

		CHECK(queue.empty() == false);
		CHECK(queue.count() == 3);

		CHECK(queue.front() == 1);
		CHECK(queue.dequeue() == true);
		CHECK(queue.front() == 2);
		CHECK(queue.dequeue() == true);
		CHECK(queue.front() == 3);
		CHECK(queue.dequeue() == true);
		CHECK(queue.dequeue() == false);

		CHECK(queue.empty() == true);
		CHECK(queue.count() == 0);
	}

	SECTION("Case 03")
	{
		Queue_List<usize> queue;
		queue.emplace(1);
		queue.enqueue(2);
		queue.enqueue(3);

		Queue_List<usize> other(queue);

		//queue checks
		CHECK(queue.empty() == false);
		CHECK(queue.count() == 3);

		CHECK(queue.front() == 1);
		CHECK(queue.dequeue() == true);
		CHECK(queue.front() == 2);
		CHECK(queue.dequeue() == true);
		CHECK(queue.front() == 3);
		CHECK(queue.dequeue() == true);
		CHECK(queue.dequeue() == false);

		CHECK(queue.empty() == true);
		CHECK(queue.count() == 0);

		//other checks
		CHECK(other.empty() == false);
		CHECK(other.count() == 3);

		CHECK(other.front() == 1);
		CHECK(other.dequeue() == true);
		CHECK(other.front() == 2);
		CHECK(other.dequeue() == true);
		CHECK(other.front() == 3);
		CHECK(other.dequeue() == true);
		CHECK(other.dequeue() == false);

		CHECK(other.empty() == true);
		CHECK(other.count() == 0);
	}

	SECTION("Case 04")
	{
		Queue_List<usize> queue;
		queue.emplace(1);
		queue.enqueue(2);
		queue.enqueue(3);

		Queue_List<usize> other(std::move(queue));

		//queue checks
		CHECK(queue.empty() == true);
		CHECK(queue.count() == 0);

		//other checks
		CHECK(other.empty() == false);
		CHECK(other.count() == 3);

		CHECK(other.front() == 1);
		CHECK(other.dequeue() == true);
		CHECK(other.front() == 2);
		CHECK(other.dequeue() == true);
		CHECK(other.front() == 3);
		CHECK(other.dequeue() == true);
		CHECK(other.dequeue() == false);

		CHECK(other.empty() == true);
		CHECK(other.count() == 0);
	}

	SECTION("Case 05")
	{
		Queue_List<usize> queue;
		queue.emplace(1);
		queue.enqueue(2);
		queue.enqueue(3);

		Queue_List<usize> other = queue;

		//queue checks
		CHECK(queue.empty() == false);
		CHECK(queue.count() == 3);

		CHECK(queue.front() == 1);
		CHECK(queue.dequeue() == true);
		CHECK(queue.front() == 2);
		CHECK(queue.dequeue() == true);
		CHECK(queue.front() == 3);
		CHECK(queue.dequeue() == true);
		CHECK(queue.dequeue() == false);

		CHECK(queue.empty() == true);
		CHECK(queue.count() == 0);

		//other checks
		CHECK(other.empty() == false);
		CHECK(other.count() == 3);

		CHECK(other.front() == 1);
		CHECK(other.dequeue() == true);
		CHECK(other.front() == 2);
		CHECK(other.dequeue() == true);
		CHECK(other.front() == 3);
		CHECK(other.dequeue() == true);
		CHECK(other.dequeue() == false);

		CHECK(other.empty() == true);
		CHECK(other.count() == 0);
	}

	SECTION("Case 06")
	{
		Queue_List<usize> queue;
		queue.emplace(1);
		queue.enqueue(2);
		queue.enqueue(3);

		Queue_List<usize> other = std::move(queue);

		//queue checks
		CHECK(queue.empty() == true);
		CHECK(queue.count() == 0);

		//other checks
		CHECK(other.empty() == false);
		CHECK(other.count() == 3);

		CHECK(other.front() == 1);
		CHECK(other.dequeue() == true);
		CHECK(other.front() == 2);
		CHECK(other.dequeue() == true);
		CHECK(other.front() == 3);
		CHECK(other.dequeue() == true);
		CHECK(other.dequeue() == false);

		CHECK(other.empty() == true);
		CHECK(other.count() == 0);
	}

	SECTION("Case 07")
	{
		Queue_List<usize> queue;
		queue.emplace(1);
		queue.enqueue(2);
		queue.enqueue(3);

		CHECK(queue.empty() == false);
		CHECK(queue.count() == 3);
		queue.clear();
		CHECK(queue.empty() == true);
		CHECK(queue.count() == 0);
	}

	SECTION("Case 08")
	{
		Queue_List<usize> queue;
		queue.emplace(1);
		queue.enqueue(2);
		queue.enqueue(3);

		CHECK(queue.empty() == false);
		CHECK(queue.count() == 3);
		queue.reset();
		CHECK(queue.empty() == true);
		CHECK(queue.count() == 0);
	}
}