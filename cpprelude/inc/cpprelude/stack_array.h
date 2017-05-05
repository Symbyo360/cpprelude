#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/dynamic_array.h"


namespace cpprelude
{
	template<typename T>
	struct  stack_array
	{
		dynamic_array <T> _array;
		usize _count;

		stack_array() :_count(0)
		{
		}

		stack_array(usize count) :_count(0)
		{
			_array.resize(count);
		}
		
		void
		push(T item) 
		{
			if (_count == _array.count())
				_array.insert_back(item);
			else
				_array[_count] = item;

			_count++;
		}

		T
		pop() 
		{

			T x;
			if (_count > 0) {
				x = _array[_count - 1];
				//Shrinking array size with factor 25%,
				//Most probably it will never enter this case! due to increase factor 
				//in dynamic_array 1.5

				if (_count == _array.count() / 4.0)
					_array.resize(_array.count() / 2);
			}
			

			_count--;
			return x;
		}


		bool
		isEmpty() 
		{
			return (_count == 0);
		}

		usize
		size() 
		{
			return _count;
		}

		usize
		capacity() 
		{
			return _array.capacity();
		}

	};

}
