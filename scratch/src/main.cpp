#include "benchmark.h"
#include <cpprelude/defines.h>
#include <cpprelude/v5/memory.h>
#include <cpprelude/v5/os.h>
#include <cpprelude/v5/dynamic_array.h>
#include <cpprelude/fmt.h>
using namespace cpprelude;

struct V3
{
	r32 x, y, z;
};

int
main(int argc, char** argv)
{
	auto num = os->alloc<usize>(1);
	os->free(num);
	Dynamic_Array<V3> array;
	do_benchmark();
	return 0;
}
