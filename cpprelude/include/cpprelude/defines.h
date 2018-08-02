#pragma once
#include <stdint.h>
#include <stddef.h>

#define BYTES(amount) (amount)
#define KILOBYTES(amount) (BYTES(amount) * 1024ULL)
#define MEGABYTES(amount) (KILOBYTES(amount) * 1024ULL)
#define GIGABYTES(amount) (MEGABYTES(amount) * 1024ULL)


//type definitions
#ifdef USE_CPPR_NAMESPACE
namespace cppr{
#endif

typedef unsigned char ubyte;
typedef char byte;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float r32;
typedef double r64;

typedef size_t usize;
typedef intptr_t isize;

#ifdef USE_CPPR_NAMESPACE
}
#endif
