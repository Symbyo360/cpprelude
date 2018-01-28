# Types

**CPPrelude** provides a simple primitive types that's concise and simple.

## Primitives

| Type  | Description                              |
| ----- | ---------------------------------------- |
| ubyte | unsigned char                            |
| byte  | char                                     |
| i8    | int8_t, signed 8-bit integer             |
| i16   | int16_t, signed 16-bit integer           |
| i32   | int32_t, signed 32-bit integer           |
| i64   | int64_t, signed 64-bit integer           |
| u8    | uint8_t, unsigned 8-bit integer          |
| u16   | uint16_t, unsigned 16-bit integer        |
| u32   | uint32_t, unsigned 32-bit integer        |
| u64   | uint64_t, unsigned 64-bit integer        |
| r32   | float, a 32-bit floating point real number |
| r64   | double, a 64-bit floating point real number |
| isize | a signed size type that depending on your architecture 32-bit integer in the case of 86-arch and 64-bit in case of 64-arch |
| usize | an unsigned size type that depending on your architecture 32-bit integer in the case of 86-arch and 64-bit in case of 64-arch |



## Strings

You can find [string](String.md) documentation [here](String.md)

## Misc

**BYTES**: macro defines an amount of bytes.

**KILOBYTES**: macro defines an amount of kilobytes.

**MEGABYTES**: macro defines an amount of megabytes.

**GIGABYTES**: macro defines an amount of gigabytes.

```c++
#define BYTES(amount) amount
#define KILOBYTES(amount) BYTES(amount) * 1024LL
#define MEGABYTES(amount) KILOBYTES(amount) * 1024LL
#define GIGABYTES(amount) MEGABYTES(amount) * 1024LL
```

