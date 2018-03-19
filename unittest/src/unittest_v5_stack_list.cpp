#include "catch.hpp"
#include <cpprelude/v5/Stack_List.h>

using namespace cpprelude;

TEST_CASE("stack_list v5", "[stack_list]")
{
	SECTION("Case 01")
	{
		Stack_List<usize> stack;
		CHECK(stack.empty() == true);
		CHECK(stack.count() == 0);
	}

	SECTION("Case 02")
	{
		Stack_List<usize> stack;
		stack.emplace(1);
		stack.push(2);
		stack.push(3);

		CHECK(stack.empty() == false);
		CHECK(stack.count() == 3);

		CHECK(stack.top() == 3);
		CHECK(stack.pop() == true);
		CHECK(stack.top() == 2);
		CHECK(stack.pop() == true);
		CHECK(stack.top() == 1);
		CHECK(stack.pop() == true);
		CHECK(stack.pop() == false);
	}

	SECTION("Case 03")
	{
		Stack_List<usize> stack;
		stack.emplace(1);
		stack.push(2);
		stack.push(3);


		Stack_List<usize> other(stack);
		CHECK(stack.empty() == false);
		CHECK(stack.count() == 3);
		CHECK(other.empty() == false);
		CHECK(other.count() == 3);

		CHECK(stack.top() == 3);
		CHECK(stack.pop() == true);
		CHECK(stack.top() == 2);
		CHECK(stack.pop() == true);
		CHECK(stack.top() == 1);
		CHECK(stack.pop() == true);
		CHECK(stack.pop() == false);

		CHECK(other.top() == 3);
		CHECK(other.pop() == true);
		CHECK(other.top() == 2);
		CHECK(other.pop() == true);
		CHECK(other.top() == 1);
		CHECK(other.pop() == true);
		CHECK(other.pop() == false);
	}

	SECTION("Case 04")
	{
		Stack_List<usize> stack;
		stack.emplace(1);
		stack.push(2);
		stack.push(3);


		Stack_List<usize> other(std::move(stack));
		CHECK(stack.empty() == true);
		CHECK(stack.count() == 0);
		CHECK(other.empty() == false);
		CHECK(other.count() == 3);

		CHECK(other.top() == 3);
		CHECK(other.pop() == true);
		CHECK(other.top() == 2);
		CHECK(other.pop() == true);
		CHECK(other.top() == 1);
		CHECK(other.pop() == true);
		CHECK(other.pop() == false);
	}

	SECTION("Case 05")
	{
		Stack_List<usize> stack;
		stack.emplace(1);
		stack.push(2);
		stack.push(3);


		Stack_List<usize> other = stack;
		CHECK(stack.empty() == false);
		CHECK(stack.count() == 3);
		CHECK(other.empty() == false);
		CHECK(other.count() == 3);

		CHECK(stack.top() == 3);
		CHECK(stack.pop() == true);
		CHECK(stack.top() == 2);
		CHECK(stack.pop() == true);
		CHECK(stack.top() == 1);
		CHECK(stack.pop() == true);
		CHECK(stack.pop() == false);

		CHECK(other.top() == 3);
		CHECK(other.pop() == true);
		CHECK(other.top() == 2);
		CHECK(other.pop() == true);
		CHECK(other.top() == 1);
		CHECK(other.pop() == true);
		CHECK(other.pop() == false);
	}

	SECTION("Case 06")
	{
		Stack_List<usize> stack;
		stack.emplace(1);
		stack.push(2);
		stack.push(3);


		Stack_List<usize> other = std::move(stack);
		CHECK(stack.empty() == true);
		CHECK(stack.count() == 0);
		CHECK(other.empty() == false);
		CHECK(other.count() == 3);

		CHECK(other.top() == 3);
		CHECK(other.pop() == true);
		CHECK(other.top() == 2);
		CHECK(other.pop() == true);
		CHECK(other.top() == 1);
		CHECK(other.pop() == true);
		CHECK(other.pop() == false);
	}

	SECTION("Case 07")
	{
		Stack_List<usize> stack;
		stack.emplace(1);
		stack.push(2);
		stack.push(3);

		CHECK(stack.empty() == false);
		CHECK(stack.count() == 3);
		stack.clear();
		CHECK(stack.empty() == true);
		CHECK(stack.count() == 0);
	}

	SECTION("Case 08")
	{
		Stack_List<usize> stack;
		stack.emplace(1);
		stack.push(2);
		stack.push(3);

		CHECK(stack.empty() == false);
		CHECK(stack.count() == 3);
		stack.reset();
		CHECK(stack.empty() == true);
		CHECK(stack.count() == 0);
	}

}