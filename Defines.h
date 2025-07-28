#pragma once

// Unsigned int types.

/** @brief Unsigned 8-bit integer */
typedef unsigned char u8;

/** @brief Unsigned 16-bit integer */
typedef unsigned short u16;

/** @brief Unsigned 32-bit integer */
typedef unsigned int u32;

/** @brief Unsigned 64-bit integer */
typedef unsigned long long u64;

// Signed int types.

/** @brief Signed 8-bit integer */
typedef signed char i8;

/** @brief Signed 16-bit integer */
typedef signed short i16;

/** @brief Signed 32-bit integer */
typedef signed int i32;

/** @brief Signed 64-bit integer */
typedef signed long long i64;

// Floating point types

/** @brief 32-bit floating point number */
typedef float f32;

/** @brief 64-bit floating point number */
typedef double f64;

// Boolean types

/** @brief 32-bit boolean type, used for APIs which require it */
typedef int b32;

/** @brief 8-bit boolean type */
typedef bool b8;

// Properly define static assertions.
#if defined(__clang__) || defined(__GNUC__)
/** @brief Static assertion */
#    define STATIC_ASSERT _Static_assert
#else

/** @brief Static assertion */
#    define STATIC_ASSERT static_assert
#endif

// Properly define static assertions.
#if defined(__clang__) || defined(__GNUC__)
/** @brief Static assertion */
#    define ASSERT _assert
#else

/** @brief Static assertion */
#    define ASSERT assert
#endif


// Ensure all types are of the correct size.

/** @brief Assert u8 to be 1 byte.*/
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");

/** @brief Assert u16 to be 2 bytes.*/
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");

/** @brief Assert u32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");

/** @brief Assert u64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

/** @brief Assert i8 to be 1 byte.*/
STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");

/** @brief Assert i16 to be 2 bytes.*/
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");

/** @brief Assert i32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");

/** @brief Assert i64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

/** @brief Assert f32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");

/** @brief Assert f64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

/**
 * @brief Any id set to this should be considered invalid,
 * and not actually pointing to a real object.
 */
#define INVALID_ID_U64 18446744073709551615UL
#define INVALID_ID 4294967295U
#define INVALID_ID_U32 INVALID_ID
#define INVALID_ID_U16 65535U
#define INVALID_ID_U8 255U

#define U64_MAX 18446744073709551615UL
#define U32_MAX 4294967295U
#define U16_MAX 65535U
#define U8_MAX 255U
#define U64_MIN 0UL
#define U32_MIN 0U
#define U16_MIN 0U
#define U8_MIN 0U

#define I8_MAX 127
#define I16_MAX 32767
#define I32_MAX 2147483647
#define I64_MAX 9223372036854775807L
#define I8_MIN (-I8_MAX - 1)
#define I16_MIN (-I16_MAX - 1)
#define I32_MIN (-I32_MAX - 1)
#define I64_MIN (-I64_MAX - 1)