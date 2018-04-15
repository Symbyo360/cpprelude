#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/v5/Ranges.h"

namespace cpprelude
{
	namespace internal
	{
		//MurmurHashUnaligned2
		template<usize ptr_size>
		struct _Hash_Bytes
		{
			inline usize
			operator()(const void* ptr, usize len, usize seed)
			{
				static_assert(ptr_size == 0, "there's no hash function defined for this architecture pointer size");
				return 0;
			}
		};

		//4 byte pointer size architectures
		template<>
		struct _Hash_Bytes<4>
		{
			inline usize
			operator()(const void* ptr, usize len, usize seed)
			{
				const usize m = 0x5bd1e995;
				usize hash = seed ^ len;
				const byte* buffer = static_cast<const byte*>(ptr);

				while(len >= 4)
				{
					usize k = *reinterpret_cast<const usize*>(buffer);
					k *= m;
					k ^= k >> 24;
					k *= m;
					hash *= m;
					hash ^= k;
					buffer += 4;
					len -= 4;
				}

				if(len == 3)
				{
					hash ^= static_cast<ubyte>(buffer[2]) << 16;
					--len;
				}

				if(len == 2)
				{
					hash ^= static_cast<ubyte>(buffer[1]) << 8;
					--len;
				}

				if(len == 1)
				{
					hash ^= static_cast<ubyte>(buffer[0]);
					hash *= m;
					--len;
				}

				hash ^= hash >> 13;
				hash *= m;
				hash ^= hash >> 15;
				return hash;
			}
		};

		//8 byte pointer size architectures
		template<>
		struct _Hash_Bytes<8>
		{
			inline usize
			load_bytes(const byte* p, isize n) const
			{
				usize result = 0;
				--n;
				do
					result = (result << 8) + static_cast<ubyte>(p[n]);
				while(--n >= 0);
				return result;
			}

			inline usize
			shift_mix(usize v) const
			{
				return v ^ (v >> 47);
			}

			inline usize
			operator()(const void* ptr, usize len, usize seed)
			{
				static const usize mul = (static_cast<usize>(0xc6a4a793UL) << 32UL) + static_cast<usize>(0x5bd1e995UL);

				const byte* const buffer = static_cast<const byte*>(ptr);

				const i32 len_aligned = len & ~0x7;
				const byte* const end = buffer + len_aligned;

				usize hash = seed ^ (len * mul);
				for(const byte* p = buffer;
					p != end;
					p += 8)
				{
					const usize data = shift_mix((*reinterpret_cast<const usize*>(p)) * mul) * mul;
					hash ^= data;
					hash *= mul;
				}

				if((len & 0x7) != 0)
				{
					const usize data = load_bytes(end, len & 0x7);
					hash ^= data;
					hash *= mul;
				}

				hash = shift_mix(hash) * mul;
				hash = shift_mix(hash);
				return hash;
			}
		};
	}

	template<typename T>
	struct Hash
	{
		inline usize
		operator()(T value) const
		{
			static_assert(sizeof(T) == 0, "there's no hash function defined for this type");
			return 0;
		}
	};

	//pointer hash
	template<typename T>
	struct Hash<T*>
	{
		inline usize
		operator()(T* ptr) const
		{
			return reinterpret_cast<usize>(ptr);
		}
	};

	#define TRIVIAL_HASH(TYPE)\
	template<>\
	struct Hash<TYPE>\
	{\
		inline usize\
		operator()(TYPE value) const\
		{\
			return static_cast<usize>(value);\
		}\
	}

	TRIVIAL_HASH(byte);
	TRIVIAL_HASH(i8);
	TRIVIAL_HASH(i16);
	TRIVIAL_HASH(i32);
	TRIVIAL_HASH(i64);
	TRIVIAL_HASH(u8);
	TRIVIAL_HASH(u16);
	TRIVIAL_HASH(u32);
	TRIVIAL_HASH(u64);

	#undef TRIVIAL_HASH

	inline static usize
	murmur_hash(const void* ptr, usize len, usize seed = 0xc70f6907UL)
	{
		internal::_Hash_Bytes<sizeof(void*)> hasher;
		return hasher(ptr, len, seed);
	}

	template<typename T>
	struct Hash<Slice<T>>
	{
		inline usize
		operator()(const Slice<T>& data) const
		{
			return murmur_hash(data.ptr, data.size);
		}
	};

	template<>
	struct Hash<r32>
	{
		inline usize
		operator()(r32 value) const
		{
			return value != 0.0f ? murmur_hash(&value, sizeof(r32)) : 0;
		}
	};

	template<>
	struct Hash<r64>
	{
		inline usize
		operator()(r64 value) const
		{
			return value != 0.0f ? murmur_hash(&value, sizeof(r64)) : 0;
		}
	};

	template<>
	struct Hash<Rune>
	{
		inline usize
		operator()(Rune value) const
		{
			return static_cast<usize>(value.data);
		}
	};
}