#include "benchmark.h"
//#include <cpprelude/fmt.h>
//#include <Windows.h>

//void
//func(int x)
//{
//	cpprelude::println(x);
//}
//
//template<typename ... TArgs>
//DWORD WINAPI
//_thread_start(LPVOID data)
//{
//	func((int)data);
//	return 0;
//}

int
main(int argc, char** argv)
{
	/*DWORD thread_id;
	int thread_data = 32;
	CreateThread(NULL, 0, _thread_start, &thread_data, 0, &thread_id);*/

	do_benchmark();
	return 0;
}
