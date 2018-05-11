#include "cpprelude/IO.h"
#include <mutex>

namespace cpprelude
{
	std::recursive_mutex*
	_print_lock()
	{
		static std::recursive_mutex _internal_print_lock;
		return &_internal_print_lock;
	}

	std::recursive_mutex*
	_print_err_lock()
	{
		static std::recursive_mutex _internal_print_err_lock;
		return &_internal_print_err_lock;
	}

	void
	_acquire_print_lock()
	{
		_print_lock()->lock();
	}

	void
	_release_print_lock()
	{
		_print_lock()->unlock();
	}

	void
	_acquire_print_err_lock()
	{
		_print_err_lock()->lock();
	}

	void
	_release_print_err_lock()
	{
		_print_err_lock()->unlock();
	}
}