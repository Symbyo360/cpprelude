#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"
#include "cpprelude/v5/Owner.h"
#include "cpprelude/v5/Hash.h"
#include <cstring>

namespace cpprelude
{
	namespace internal
	{
		enum HASH_FLAGS : u8
		{
			EMPTY = 0,
			USED = 1,
			DELETED = 2
		};

		template<typename TKey, typename TValue>
		struct Hash_Pair
		{
			TKey key;
			TValue value;

			Hash_Pair(){}

			Hash_Pair(const TKey& k)
				:key(k)
			{}

			Hash_Pair(TKey&& k)
				:key(std::move(k))
			{}

			Hash_Pair(const TKey& k, const TValue& v)
				:key(k), value(v)
			{}

			Hash_Pair(const TKey& k, TValue&& v)
				:key(k), value(std::move(v))
			{}

			Hash_Pair(TKey&& k, const TValue& v)
				:key(std::move(k)), value(v)
			{}

			Hash_Pair(TKey&& k, TValue&& v)
				:key(std::move(k)), value(std::move(v))
			{}

			bool
			operator==(const Hash_Pair& other) const
			{
				return key == other.key;
			}

			bool
			operator!=(const Hash_Pair& other) const
			{
				return !operator==(other);
			}
		};

		template<typename TKey, typename TValue>
		struct Pair_Hash_Functor
		{
			inline usize
			operator()(const internal::Hash_Pair<const TKey, TValue>& pair) const
			{
				Hash<TKey> hasher;
				return hasher(pair.key);
			}
		};
	}

	template<typename T, typename THash = Hash<T>>
	struct Hash_Set
	{

		using Data_Type = T;
		using Range_Type = Hash_Range<const T, internal::HASH_FLAGS>;
		using Const_Range_Type = Hash_Range<const T, internal::HASH_FLAGS>;
		using iterator = Hash_Iterator<const T, internal::HASH_FLAGS>;
		using const_iterator = iterator;

		Memory_Context mem_context;
		Owner<Data_Type> _values;
		Owner<internal::HASH_FLAGS> _flags;
		THash _hasher;
		usize _count;

		Hash_Set(const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _hasher(THash()),
			 _count(0)
		{}

		Hash_Set(const THash& hasher, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _hasher(hasher),
			 _count(0)
		{}

		Hash_Set(const Hash_Set& other)
			:mem_context(other.memory_context),
			 _hasher(other._hasher),
			 _count(other._count)
		{
			_flags = mem_context.template alloc<internal::HASH_FLAGS>(other._flags.count());
			move(_flags, other._flags);

			_values = mem_context.template alloc<Data_Type>(other._values.count());
			for(usize i = 0; i < _flags.count(); ++i)
				if(_flags[i] == internal::HASH_FLAGS::USED)
					::new (_values.ptr + i) Data_Type(other._values[i]);
		}

		Hash_Set(const Hash_Set& other, const Memory_Context& context)
			:mem_context(context),
			 _hasher(other._hasher),
			 _count(other._count)
		{
			_flags = mem_context.template alloc<u8>(other._flags.count());
			move(_flags, other._flags);

			_values = mem_context.template alloc<Data_Type>(other._values.count());
			for(usize i = 0; i < other._flags.count(); ++i)
				if(other._flags[i] == internal::HASH_FLAGS::USED)
					::new (_values.ptr + i) Data_Type(other._values[i]);
		}

		Hash_Set(Hash_Set&& other)
			:mem_context(std::move(other.mem_context)),
			 _values(std::move(other._values)),
			 _flags(std::move(other._flags)),
			 _hasher(std::move(other._hasher)),
			 _count(other._count)
		{
			other._count = 0;
		}

		Hash_Set&
		operator=(const Hash_Set& other)
		{
			if(_flags.count() < other._flags.count())
			{
				reset();
				_flags = mem_context.template alloc<u8>(other._flags.count());
				_values = mem_context.template alloc<Data_Type>(other._values.count());
			}
			else
			{
				for(usize i = 0; i < _flags.count(); ++i)
				{
					if(_flags[i] == internal::HASH_FLAGS::USED)
					{
						_values[i].~Data_Type();
						_flags[i] = internal::HASH_FLAGS::EMPTY;
					}
				}
			}

			_hasher = other._hasher;
			mem_context = other.mem_context;

			move(_flags, other._flags);
			for(usize i = 0; i < _flags.count(); ++i)
			{
				if(_flags[i] == internal::HASH_FLAGS::USED)
					::new (_values.ptr + i) Data_Type(other._values[i]);
			}

			_count = other._count;
			return *this;
		}

		Hash_Set&
		operator=(Hash_Set&& other)
		{
			reset();
			mem_context = std::move(other.mem_context);
			_values = std::move(other._values);
			_flags = std::move(other._flags);
			_hasher = std::move(other._hasher);
			_count = other._count;

			other._count = 0;
			return *this;
		}

		~Hash_Set()
		{
			reset();
		}

		usize
		capacity() const
		{
			return _flags.count();
		}

		usize
		count() const
		{
			return _count;
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		void
		reserve(usize expected_count)
		{
			if(_flags.count() - _count >= expected_count)
				return;

			usize double_cap = (_flags.count() * 2);
			usize fit = _count + expected_count;
			usize new_cap = double_cap > fit ? double_cap : fit;

			auto new_flags = mem_context.template alloc<internal::HASH_FLAGS>(new_cap);
			auto new_values = mem_context.template alloc<Data_Type>(new_cap);

			std::memset(new_flags.ptr, 0, new_flags.size);

			if (_count != 0)
			{
				for (usize i = 0; i < _flags.count(); ++i)
				{
					if (_flags[i] == internal::HASH_FLAGS::USED)
					{
						usize new_index = _find_insertion_index(new_flags, new_values, _values[i]);
						new_flags[new_index] = internal::HASH_FLAGS::USED;
						::new (new_values.ptr + new_index) Data_Type(std::move(_values[i]));
					}
				}
			}

			mem_context.template free<internal::HASH_FLAGS>(_flags);
			mem_context.template free<Data_Type>(_values);

			_flags = std::move(new_flags);
			_values = std::move(new_values);
		}

		iterator
		insert(const Data_Type& value)
		{
			_maintain_space_complexity();

			usize index = _find_insertion_index(value);

			if(_flags[index] == internal::HASH_FLAGS::EMPTY)
			{
				_flags[index] = internal::HASH_FLAGS::USED;
				::new (_values.ptr + index) Data_Type(value);
				++_count;
			}

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		iterator
		insert(Data_Type&& value)
		{
			_maintain_space_complexity();

			usize index = _find_insertion_index(value);

			if(_flags[index] == internal::HASH_FLAGS::EMPTY)
			{
				_flags[index] = internal::HASH_FLAGS::USED;
				::new (_values.ptr + index) Data_Type(std::move(value));
				++_count;
			}

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		iterator
		lookup(const Data_Type& value)
		{
			auto index = _lookup_index(value);

			if(index == _flags.count())
				return end();

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		const_iterator
		lookup(const Data_Type& value) const
		{
			auto index = _lookup_index(value);

			if(index == _flags.count())
				return end();

			return const_iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		bool
		remove(const Data_Type& value)
		{
			auto index = _lookup_index(value);

			if(index == _flags.count())
				return false;

			_flags[index] = internal::HASH_FLAGS::DELETED;
			_values[index].~Data_Type();
			--_count;
			return true;
		}

		bool
		remove(const iterator& it)
		{
			usize index = it._flag - _flags.ptr;

			if(index == _flags.count())
				return false;

			_flags[index] = internal::HASH_FLAGS::DELETED;
			_values[index].~Data_Type();
			--_count;
			return true;
		}

		void
		clear()
		{
			if(_count == 0)
				return;
			for(usize i = 0; i < _flags.count(); ++i)
			{
				if(_flags[i] == internal::HASH_FLAGS::USED)
				{
					_flags[i] = internal::HASH_FLAGS::EMPTY;
					_values[i].~Data_Type();
				}
			}
			_count = 0;
		}

		void
		reset()
		{
			clear();
			mem_context.template free<internal::HASH_FLAGS>(_flags);
			mem_context.template free<Data_Type>(_values);
		}

		Range_Type
		all()
		{
			return Range_Type(begin(), end(), _count);
		}

		Const_Range_Type
		all() const
		{
			return Const_Range_Type(begin(), end(), _count);
		}

		Range_Type
		range(usize start, usize end_count)
		{
			auto it = begin();
			for(usize i = 0; i < start; ++i)
				++it;
			return Range_Type(it, end(), end_count - start);
		}

		Const_Range_Type
		range(usize start, usize end_count) const
		{
			auto it = begin();
			for(usize i = 0; i < start; ++i)
				++it;
			return Const_Range_Type(it, end(), end_count - start);
		}

		iterator
		begin()
		{
			if(_count == 0)
				return end();

			usize ix = 0;
			for(ix = 0; ix < _flags.count(); ++ix)
				if(_flags[ix] == internal::HASH_FLAGS::USED)
					break;
			return iterator(_values.ptr + ix, _flags.ptr + ix, _flags.count() - ix);
		}

		const_iterator
		begin() const
		{
			if(_count == 0)
				return end();

			usize ix = 0;
			for(ix = 0; ix < _flags.count(); ++ix)
				if(_flags[ix] == internal::HASH_FLAGS::USED)
					break;
			return const_iterator(_values.ptr + ix, _flags.ptr + ix, _flags.count() - ix);
		}

		const_iterator
		cbegin() const
		{
			if(_count == 0)
				return end();

			usize ix = 0;
			for(ix = 0; ix < _flags.count(); ++ix)
				if(_flags[ix] == internal::HASH_FLAGS::USED)
					break;
			return const_iterator(_values.ptr + ix, _flags.ptr + ix, _flags.count() - ix);
		}

		iterator
		end()
		{
			usize ix = _flags.count();
			return iterator(_values.ptr + ix, _flags.ptr + ix, 0);
		}

		const_iterator
		end() const
		{
			usize ix = _flags.count();
			return const_iterator(_values.ptr + ix, _flags.ptr + ix, 0);
		}

		const_iterator
		cend() const
		{
			usize ix = _flags.count();
			return const_iterator(_values.ptr + ix, _flags.ptr + ix, 0);
		}

		usize
		_lookup_index(const Owner<internal::HASH_FLAGS>& flags, const Owner<Data_Type>& values,
					  const Data_Type& value) const
		{
			usize cap = flags.count();
			if(cap == 0) return cap;

			usize hash_value = _hasher(value);
			usize index = hash_value % cap;
			usize ix = index;

			//linear probing goes here
			while(true)
			{
				//if the cell is empty then we didn't find the value
				if(flags[ix] == internal::HASH_FLAGS::EMPTY)
					return cap;

				//if the cell is used and it's the same value then we found it
				if (flags[ix] == internal::HASH_FLAGS::USED &&
					values[ix] == value)
					return ix;

				//increment the index
				++ix;
				ix %= cap;

				//check if we went full circle
				if(ix == index)
					return cap;
			}

			return cap;
		}

		usize
		_lookup_index(const Data_Type& value) const
		{
			return _lookup_index(_flags, _values, value);
		}

		usize
		_find_insertion_index(const Owner<internal::HASH_FLAGS>& flags, const Owner<Data_Type>& values,
							  const Data_Type& value) const
		{
			usize cap = flags.count();
			if(cap == 0) return cap;

			usize hash_value = _hasher(value);
			usize index = hash_value % cap;
			usize ix = index;

			//linear probing goes here
			while(true)
			{
				//this is an empty spot or deleted spot then we can use it
				if (flags[ix] == internal::HASH_FLAGS::EMPTY ||
					flags[ix] == internal::HASH_FLAGS::DELETED)
					return ix;

				//the position is not empty but if it's the same value then we return it
				if(values[ix] == value)
					return ix;

				//the position is not empty and the value is not the same
				++ix;
				ix %= cap;

				//if we went a full circle then we just return the cap to signal no index found
				if(ix == index)
					return cap;
			}
			return cap;
		}

		usize
		_find_insertion_index(const Data_Type& value) const
		{
			return _find_insertion_index(_flags, _values, value);
		}

		void
		_maintain_space_complexity()
		{
			constexpr usize STARTING_COUNT = 8;
			if (_flags.count() == 0)
			{
				reserve(STARTING_COUNT);
			}
			else if (_count > (_flags.count() / 2))
			{
				//reserve another capacity to double it
				reserve(_flags.count());
			}
		}
	};

	template<typename TKey, typename TValue,
			 typename THash = internal::Pair_Hash_Functor<TKey, TValue>>
	struct Hash_Map
	{

		using Data_Type = internal::Hash_Pair<const TKey, TValue>;
		using Range_Type = Hash_Range<Data_Type, internal::HASH_FLAGS>;
		using Const_Range_Type = Hash_Range<const Data_Type, internal::HASH_FLAGS>;
		using iterator = Hash_Iterator<Data_Type, internal::HASH_FLAGS>;
		using const_iterator = Hash_Iterator<const Data_Type, internal::HASH_FLAGS>;

		Memory_Context mem_context;
		Owner<Data_Type> _values;
		Owner<internal::HASH_FLAGS> _flags;
		THash _hasher;
		usize _count;

		Hash_Map(const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _hasher(THash()),
			 _count(0)
		{}

		Hash_Map(const THash& hasher, const Memory_Context& context = os->global_memory)
			:mem_context(context),
			 _hasher(hasher),
			 _count(0)
		{}

		Hash_Map(const Hash_Map& other)
			:mem_context(other.memory_context),
			 _hasher(other._hasher),
			 _count(other._count)
		{
			_flags = mem_context.template alloc<internal::HASH_FLAGS>(other._flags.count());
			move(_flags, other._flags);

			_values = mem_context.template alloc<Data_Type>(other._values.count());
			for(usize i = 0; i < _flags.count(); ++i)
				if(_flags[i] == internal::HASH_FLAGS::USED)
					::new (_values.ptr + i) Data_Type(other._values[i]);
		}

		Hash_Map(const Hash_Map& other, const Memory_Context& context)
			:mem_context(context),
			 _hasher(other._hasher),
			 _count(other._count)
		{
			_flags = mem_context.template alloc<u8>(other._flags.count());
			move(_flags, other._flags);

			_values = mem_context.template alloc<Data_Type>(other._values.count());
			for(usize i = 0; i < other._flags.count(); ++i)
				if(other._flags[i] == internal::HASH_FLAGS::USED)
					::new (_values.ptr + i) Data_Type(other._values[i]);
		}

		Hash_Map(Hash_Map&& other)
			:mem_context(std::move(other.mem_context)),
			 _values(std::move(other._values)),
			 _flags(std::move(other._flags)),
			 _hasher(std::move(other._hasher)),
			 _count(other._count)
		{
			other._count = 0;
		}

		Hash_Map&
		operator=(const Hash_Map& other)
		{
			if(_flags.count() < other._flags.count())
			{
				reset();
				_flags = mem_context.template alloc<u8>(other._flags.count());
				_values = mem_context.template alloc<Data_Type>(other._values.count());
			}
			else
			{
				for(usize i = 0; i < _flags.count(); ++i)
				{
					if(_flags[i] == internal::HASH_FLAGS::USED)
					{
						_values[i].~Data_Type();
						_flags[i] = internal::HASH_FLAGS::EMPTY;
					}
				}
			}

			_hasher = other._hasher;
			mem_context = other.mem_context;

			move(_flags, other._flags);
			for(usize i = 0; i < _flags.count(); ++i)
			{
				if(_flags[i] == internal::HASH_FLAGS::USED)
					::new (_values.ptr + i) Data_Type(other._values[i]);
			}

			_count = other._count;
			return *this;
		}

		Hash_Map&
		operator=(Hash_Map&& other)
		{
			reset();
			mem_context = std::move(other.mem_context);
			_values = std::move(other._values);
			_flags = std::move(other._flags);
			_hasher = std::move(other._hasher);
			_count = other._count;

			other._count = 0;
			return *this;
		}

		~Hash_Map()
		{
			reset();
		}

		usize
		capacity() const
		{
			return _flags.count();
		}

		usize
		count() const
		{
			return _count;
		}

		bool
		empty() const
		{
			return _count == 0;
		}

		void
		reserve(usize expected_count)
		{
			if(_flags.count() - _count >= expected_count)
				return;

			usize double_cap = (_flags.count() * 2);
			usize fit = _count + expected_count;
			usize new_cap = double_cap > fit ? double_cap : fit;

			auto new_flags = mem_context.template alloc<internal::HASH_FLAGS>(new_cap);
			auto new_values = mem_context.template alloc<Data_Type>(new_cap);

			std::memset(new_flags.ptr, 0, new_flags.size);

			if (_count != 0)
			{
				for (usize i = 0; i < _flags.count(); ++i)
				{
					if (_flags[i] == internal::HASH_FLAGS::USED)
					{
						usize new_index = _find_insertion_index(new_flags, new_values, _values[i].key);
						new_flags[new_index] = internal::HASH_FLAGS::USED;
						::new (new_values.ptr + new_index) Data_Type(std::move(_values[i]));
					}
				}
			}

			mem_context.template free<internal::HASH_FLAGS>(_flags);
			mem_context.template free<Data_Type>(_values);

			_flags = std::move(new_flags);
			_values = std::move(new_values);
		}

		iterator
		insert(const Data_Type& value)
		{
			_maintain_space_complexity();

			usize index = _find_insertion_index(value.key);

			if(_flags[index] == internal::HASH_FLAGS::EMPTY)
			{
				_flags[index] = internal::HASH_FLAGS::USED;
				::new (_values.ptr + index) Data_Type(value);
				++_count;
			}

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		iterator
		insert(Data_Type&& value)
		{
			_maintain_space_complexity();

			usize index = _find_insertion_index(value.key);

			if(_flags[index] == internal::HASH_FLAGS::EMPTY)
			{
				_flags[index] = internal::HASH_FLAGS::USED;
				::new (_values.ptr + index) Data_Type(std::move(value));
				++_count;
			}

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		iterator
		insert(const TKey& key)
		{
			_maintain_space_complexity();

			usize index = _find_insertion_index(key);

			if(_flags[index] == internal::HASH_FLAGS::EMPTY)
			{
				_flags[index] = internal::HASH_FLAGS::USED;
				::new (_values.ptr + index) Data_Type(key);
				++_count;
			}

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		iterator
		insert(TKey&& key)
		{
			_maintain_space_complexity();

			usize index = _find_insertion_index(key);

			if(_flags[index] == internal::HASH_FLAGS::EMPTY)
			{
				_flags[index] = internal::HASH_FLAGS::USED;
				::new (_values.ptr + index) Data_Type(std::move(key));
				++_count;
			}

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		iterator
		insert(const TKey& key, const TValue& value)
		{
			_maintain_space_complexity();

			usize index = _find_insertion_index(key);

			if(_flags[index] == internal::HASH_FLAGS::EMPTY)
			{
				_flags[index] = internal::HASH_FLAGS::USED;
				::new (_values.ptr + index) Data_Type(key, value);
				++_count;
			}

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		iterator
		insert(const TKey& key, TValue&& value)
		{
			_maintain_space_complexity();

			usize index = _find_insertion_index(key);

			if(_flags[index] == internal::HASH_FLAGS::EMPTY)
			{
				_flags[index] = internal::HASH_FLAGS::USED;
				::new (_values.ptr + index) Data_Type(key, std::move(value));
				++_count;
			}

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		iterator
		insert(TKey&& key, const TValue& value)
		{
			_maintain_space_complexity();

			usize index = _find_insertion_index(key);

			if(_flags[index] == internal::HASH_FLAGS::EMPTY)
			{
				_flags[index] = internal::HASH_FLAGS::USED;
				::new (_values.ptr + index) Data_Type(std::move(key), value);
				++_count;
			}

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		iterator
		insert(TKey&& key, TValue&& value)
		{
			_maintain_space_complexity();

			usize index = _find_insertion_index(key);

			if(_flags[index] == internal::HASH_FLAGS::EMPTY)
			{
				_flags[index] = internal::HASH_FLAGS::USED;
				::new (_values.ptr + index) Data_Type(std::move(key), std::move(value));
				++_count;
			}

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		TValue&
		operator[](const TKey& key)
		{
			_maintain_space_complexity();

			usize index = _lookup_index(key);
			if(index != _flags.count())
				return _values[index].value;

			index = _find_insertion_index(key);
			_flags[index] = internal::HASH_FLAGS::USED;
			::new (_values.ptr + index) Data_Type(key);
			++_count;

			return _values[index].value;
		}

		TValue&
		operator[](TKey&& key)
		{
			_maintain_space_complexity();

			usize index = _lookup_index(key);
			if(index != _flags.count())
				return _values[index].value;

			index = _find_insertion_index(key);
			_flags[index] = internal::HASH_FLAGS::USED;
			::new (_values.ptr + index) Data_Type(std::move(key));
			++_count;

			return _values[index].value;
		}

		iterator
		lookup(const Data_Type& value)
		{
			auto index = _lookup_index(value.key);

			if(index == _flags.count())
				return end();

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		const_iterator
		lookup(const Data_Type& value) const
		{
			auto index = _lookup_index(value.key);

			if(index == _flags.count())
				return end();

			return const_iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		iterator
		lookup(const TKey& key)
		{
			auto index = _lookup_index(key);

			if(index == _flags.count())
				return end();

			return iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		const_iterator
		lookup(const TKey& key) const
		{
			auto index = _lookup_index(key);

			if(index == _flags.count())
				return end();

			return const_iterator(_values.ptr + index, _flags.ptr + index, _flags.count() - index);
		}

		bool
		remove(const Data_Type& value)
		{
			auto index = _lookup_index(value.key);

			if(index == _flags.count())
				return false;

			_flags[index] = internal::HASH_FLAGS::DELETED;
			_values[index].~Data_Type();
			--_count;
			return true;
		}

		bool
		remove(const TKey& key)
		{
			auto index = _lookup_index(key);

			if(index == _flags.count())
				return false;

			_flags[index] = internal::HASH_FLAGS::DELETED;
			_values[index].~Data_Type();
			--_count;
			return true;
		}

		bool
		remove(const iterator& it)
		{
			usize index = it._flag - _flags.ptr;

			if(index == _flags.count())
				return false;

			_flags[index] = internal::HASH_FLAGS::DELETED;
			_values[index].~Data_Type();
			--_count;
			return true;
		}

		bool
		remove(const const_iterator& it)
		{
			usize index = it._flag - _flags.ptr;

			if(index == _flags.count())
				return false;

			_flags[index] = internal::HASH_FLAGS::DELETED;
			_values[index].~Data_Type();
			--_count;
			return true;
		}

		void
		clear()
		{
			if(_count == 0)
				return;
			for(usize i = 0; i < _flags.count(); ++i)
			{
				if(_flags[i] == internal::HASH_FLAGS::USED)
				{
					_flags[i] = internal::HASH_FLAGS::EMPTY;
					_values[i].~Data_Type();
				}
			}
			_count = 0;
		}

		void
		reset()
		{
			clear();
			mem_context.template free<internal::HASH_FLAGS>(_flags);
			mem_context.template free<Data_Type>(_values);
		}

		Range_Type
		all()
		{
			return Range_Type(begin(), end(), _count);
		}

		Const_Range_Type
		all() const
		{
			return Const_Range_Type(begin(), end(), _count);
		}

		Range_Type
		range(usize start, usize end_count)
		{
			auto it = begin();
			for(usize i = 0; i < start; ++i)
				++it;
			return Range_Type(it, end(), end_count - start);
		}

		Const_Range_Type
		range(usize start, usize end_count) const
		{
			auto it = begin();
			for(usize i = 0; i < start; ++i)
				++it;
			return Const_Range_Type(it, end(), end_count - start);
		}

		iterator
		begin()
		{
			if(_count == 0)
				return end();

			usize ix = 0;
			for(ix = 0; ix < _flags.count(); ++ix)
				if(_flags[ix] == internal::HASH_FLAGS::USED)
					break;
			return iterator(_values.ptr + ix, _flags.ptr + ix, _flags.count() - ix);
		}

		const_iterator
		begin() const
		{
			if(_count == 0)
				return end();

			usize ix = 0;
			for(ix = 0; ix < _flags.count(); ++ix)
				if(_flags[ix] == internal::HASH_FLAGS::USED)
					break;
			return const_iterator(_values.ptr + ix, _flags.ptr + ix, _flags.count() - ix);
		}

		const_iterator
		cbegin() const
		{
			if(_count == 0)
				return end();

			usize ix = 0;
			for(ix = 0; ix < _flags.count(); ++ix)
				if(_flags[ix] == internal::HASH_FLAGS::USED)
					break;
			return const_iterator(_values.ptr + ix, _flags.ptr + ix, _flags.count() - ix);
		}

		iterator
		end()
		{
			usize ix = _flags.count();
			return iterator(_values.ptr + ix, _flags.ptr + ix, 0);
		}

		const_iterator
		end() const
		{
			usize ix = _flags.count();
			return const_iterator(_values.ptr + ix, _flags.ptr + ix, 0);
		}

		const_iterator
		cend() const
		{
			usize ix = _flags.count();
			return const_iterator(_values.ptr + ix, _flags.ptr + ix, 0);
		}

		usize
		_lookup_index(const Owner<internal::HASH_FLAGS>& flags, const Owner<Data_Type>& values,
					  const TKey& key) const
		{
			usize cap = flags.count();
			if(cap == 0) return cap;

			usize hash_value = _hasher(key);
			usize index = hash_value % cap;
			usize ix = index;

			//linear probing goes here
			while(true)
			{
				//if the cell is empty then we didn't find the key
				if(flags[ix] == internal::HASH_FLAGS::EMPTY)
					return cap;

				//if the cell is used and it's the same key then we found it
				if (flags[ix] == internal::HASH_FLAGS::USED &&
					values[ix] == key)
					return ix;

				//increment the index
				++ix;
				ix %= cap;

				//check if we went full circle
				if(ix == index)
					return cap;
			}

			return cap;
		}

		usize
		_lookup_index(const TKey& key) const
		{
			return _lookup_index(_flags, _values, key);
		}

		usize
		_find_insertion_index(const Owner<internal::HASH_FLAGS>& flags, const Owner<Data_Type>& values,
							  const TKey& key) const
		{
			usize cap = flags.count();
			if(cap == 0) return cap;

			usize hash_value = _hasher(key);
			usize index = hash_value % cap;
			usize ix = index;

			//linear probing goes here
			while(true)
			{
				//this is an empty spot or deleted spot then we can use it
				if (flags[ix] == internal::HASH_FLAGS::EMPTY ||
					flags[ix] == internal::HASH_FLAGS::DELETED)
					return ix;

				//the position is not empty but if it's the same key then we return it
				if(values[ix] == key)
					return ix;

				//the position is not empty and the key is not the same
				++ix;
				ix %= cap;

				//if we went a full circle then we just return the cap to signal no index found
				if(ix == index)
					return cap;
			}
			return cap;
		}

		usize
		_find_insertion_index(const TKey& key) const
		{
			return _find_insertion_index(_flags, _values, key);
		}

		void
		_maintain_space_complexity()
		{
			constexpr usize STARTING_COUNT = 8;
			if (_flags.count() == 0)
			{
				reserve(STARTING_COUNT);
			}
			else if (_count > (_flags.count() / 2))
			{
				//reserve another capacity to double it
				reserve(_flags.count());
			}
		}
	};
}