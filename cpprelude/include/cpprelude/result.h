#pragma once

#include <cassert>
#include <type_traits>

namespace cpprelude
{
	template<typename T, typename E>
	struct result
	{
		//wierd names because of macros
		enum class CPPR_RESULT_STATUS : byte {CPPR_RESULT_NONE, CPPR_RESULT_VALUE, CPPR_RESULT_ERROR};

		union
		{
			T value;
			E error;
		};
		CPPR_RESULT_STATUS _status = CPPR_RESULT_STATUS::CPPR_RESULT_NONE;

		template<typename TCond = T>
		result(const T& val, typename std::enable_if<std::is_copy_constructible<TCond>::value>::type* = nullptr)
			:value(val), _status(CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
		{}

		template<typename TCond = T>
		result(T&& val, typename std::enable_if<std::is_move_constructible<TCond>::value>::type* = nullptr)
			:value(std::move(val)), _status(CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
		{}

		template<typename ECond = E>
		result(const E& err, typename std::enable_if<std::is_copy_constructible<ECond>::value>::type* = nullptr)
			:error(err), _status(CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
		{}

		template<typename ECond = E>
		result(E&& err, typename std::enable_if<std::is_move_constructible<ECond>::value>::type* = nullptr)
			:error(std::move(err)), _status(CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
		{}

		template<typename TCond = T, typename ECond = E>
		result (const result& other,
				typename std::enable_if<
				(std::is_copy_constructible<TCond>::value && std::is_copy_constructible<ECond>::value)
				>::type* = nullptr)
			:_status(other._status)
		{
			if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
				::new (&value) T(other.value);
			else if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
				::new (&error) E(other.error);
		}

		template<typename TCond = T, typename ECond = E>
		typename std::enable_if<
			(std::is_copy_assignable<TCond>::value && std::is_copy_assignable<ECond>::value),
			result<TCond, ECond>>::type&
		operator=(const result& other)
		{
			if(other._status == _status)
			{
				if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
					value = other.value;
				else if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
					error = other.error;
			}
			else
			{
				reset();

				_status = other._status;
				if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
					::new (&value) T(other.value);
				else if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
					::new (&error) E(other.error);
			}

			return *this;
		}

		template<typename TCond = T, typename ECond = E>
		result (result&& other,
				typename std::enable_if<
				(std::is_move_constructible<TCond>::value && std::is_move_constructible<ECond>::value)
				>::type* = nullptr)
			:_status(other._status)
		{
			if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
				::new (&value) T(std::move(other.value));
			else if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
				::new (&error) E(std::move(other.error));
		}

		template<typename TCond = T, typename ECond = E>
		typename std::enable_if<
			(std::is_move_assignable<TCond>::value && std::is_move_assignable<ECond>::value),
			result<TCond, ECond>>::type&
		operator=(result&& other)
		{
			if(other._status == _status)
			{
				if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
					value = std::move(other.value);
				else if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
					error = std::move(other.error);
			}
			else
			{
				reset();

				_status = other._status;
				if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
					::new (&value) T(std::move(other.value));
				else if(_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
					::new (&error) E(std::move(other.error));
			}

			return *this;
		}

		~result()
		{
			reset();
		}

		operator T&&()
		{
			assert(_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE);
			_status = CPPR_RESULT_STATUS::CPPR_RESULT_NONE;
			return std::move(value);
		}

		operator E&&()
		{
			assert(_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR);
			_status = CPPR_RESULT_STATUS::CPPR_RESULT_NONE;
			return std::move(error);
		}

		bool
		ok() const
		{
			return _status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE;
		}

		bool
		has_result() const
		{
			return _status != CPPR_RESULT_STATUS::CPPR_RESULT_NONE;
		}

		E&&
		unwrap_error()
		{
			assert(_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR);
			_status = CPPR_RESULT_STATUS::CPPR_RESULT_NONE;
			return std::move(error);
		}

		T&&
		unwrap_value()
		{
			assert(_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE);
			_status = CPPR_RESULT_STATUS::CPPR_RESULT_NONE;
			return std::move(value);
		}

		void
		reset()
		{
			if (_status == CPPR_RESULT_STATUS::CPPR_RESULT_VALUE)
				value.~T();
			else if (_status == CPPR_RESULT_STATUS::CPPR_RESULT_ERROR)
				error.~E();
			_status = CPPR_RESULT_STATUS::CPPR_RESULT_NONE;
		}
	};

	template<typename T, typename E>
	inline static result<T, E>
	result_success(const T& value)
	{
		return result<T, E>(value);
	}

	template<typename T, typename E>
	inline static result<T, E>
	result_success(T&& value)
	{
		return result<T, E>(std::move(value));
	}

	template<typename T, typename E>
	inline static result<T, E>
	result_error(const E& err)
	{
		return result<T, E>(err);
	}

	template<typename T, typename E>
	inline static result<T, E>
	result_error(E&& err)
	{
		return result<T, E>(std::move(err));
	}
}