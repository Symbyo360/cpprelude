#pragma once

namespace cpprelude
{
	/**
	 * @brief      Result of an operation that can fail
	 *
	 * @tparam     T     Type of the returned value
	 * @tparam     E     Type of the returned error
	 */
	template<typename T, typename E>
	struct Result
	{
		/**
		 * Value of the result
		 */

		T value;
		/**
		 * Error of the result
		 */
		E error;
	};
}