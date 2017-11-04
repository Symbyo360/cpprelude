#define CATCH_CONFIG_RUNNER  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <cpprelude/platform.h>

int main(int argc, char* argv[])
{
	int result = Catch::Session().run(argc, argv);
	cpprelude::platform.print_memory_report();
	return (result < 0xff ? result : 0xff);
}
