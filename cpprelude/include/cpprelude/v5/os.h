#pragma once
#include "cpprelude/defines.h"
#include "cpprelude/api.h"
#include "cpprelude/v5/memory_context.h"

namespace cpprelude
{
	struct OS
	{
		Memory_Context *global_memory;
		usize allocation_count = 0;
		usize allocation_size = 0;
		bool debug_configured = false;

		API_CPPR ~OS();

		template<typename T>
		Owner<T>
		alloc(usize count = 1, u8 alignment = alignof(T))
		{
			return global_memory->template alloc<T>(count, alignment);
		}

		template<typename T>
		void
		free(Owner<T>& value)
		{
			return global_memory->template free<T>(value);
		}

		template<typename T>
		void
		free(Owner<T>&& value)
		{
			return global_memory->template free<T>(value);
		}

		void
		_print_memory_report() const;
	};

	API_CPPR extern OS *os;
}