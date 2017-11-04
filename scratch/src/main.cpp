#include <iostream>
#include "benchmark.h"
#include <cpprelude/platform.h>

int
main(int argc, char** argv)
{
	std::cout << "Hello, World!" << std::endl;
	std::cout << cpprelude::platform.RAM_SIZE << std::endl;
	//return 0;
	do_benchmark();
	cpprelude::platform.print_memory_report();
	return 0;
}
