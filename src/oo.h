#ifndef __OO_OO_H__
#define __OO_OO_H__

#include <stddef.h>
#include <stdint.h>

#if !defined __has_attribute
#define __has_attribute(...) 0
#define __attribute__(...)
#endif /* __has_attribute */

/* attribute deprecated */
#if __has_attribute(deprecated)
#define o_deprecated_o __attribute__((deprecated))
#elif defined(_WIN32) || defined(__CYGWIN__)
#define o_deprecated_o __declspec(deprecated)
#else /* !__has_attribute(deprecated) */
#define o_deprecated_o
#endif /* __has_attribute(deprecated) */

/* attribute always inline */
#if __has_attribute(always_inline)
#define o_always_o static __inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define o_always_o static __forceinline
#else /* !_MSC_VER */
#define o_always_o static __inline
#endif /* _MSC_VER */
#define o_inline_o static __inline
#define o_static_o static

#if defined(_WIN32) || defined(__CYGWIN__)
#define o_export_o __declspec(dllexport)
#define o_import_o __declspec(dllimport)
#define o_hidden_o
#elif defined(__has_attribute)
#define o_export_o __attribute__((visibility("default")))
#define o_import_o __attribute__((visibility("default")))
#define o_hidden_o __attribute__((visibility("hidden")))
#else /* !__has_attribute */
#define o_export_o
#define o_import_o
#define o_hidden_o
#endif /* __has_attribute */
#if defined(A_EXPORTS)
#define o_public_o o_export_o
#elif defined(A_SHARED)
#define o_public_o o_import_o
#else /* !o_public_o */
#define o_public_o
#endif /* o_public_o */

#define o_zero_o 0
#if !defined __cplusplus
#define o_null_o NULL
#define o_cast_c(T, ...) ((T)(__VA_ARGS__))
#define o_cast_s(T, ...) ((T)(__VA_ARGS__))
#define o_cast_d(T, ...) ((T)(__VA_ARGS__))
#define o_cast_r(T, ...) ((T)(__VA_ARGS__))
#else /* !__cplusplus */
#define o_null_o nullptr
#define o_cast_c(T, ...) const_cast<T>(__VA_ARGS__)
#define o_cast_s(T, ...) static_cast<T>(__VA_ARGS__)
#define o_cast_d(T, ...) dynamic_cast<T>(__VA_ARGS__)
#define o_cast_r(T, ...) reinterpret_cast<T>(__VA_ARGS__)
#endif /* __cplusplus */

#define o_success_o 0
#define o_failure_o 1
#define o_invalid_o 2

#define o_ctor_o(_) memset(_, 0, sizeof(*_))
#define o_dtor_o(_) memset(_, 0, sizeof(*_))

typedef union o_T_o
{
    // clang-format off
    char c;
      signed int i;
    unsigned int u;
      signed long li;
    unsigned long lu;
      signed short si;
    unsigned short su;
      signed long long lli;
    unsigned long long llu;
    double f64;
    float  f32;
     int8_t i8;
    uint8_t u8;
     int16_t i16;
    uint16_t u16;
     int32_t i32;
    uint32_t u32;
     int64_t i64;
    uint64_t u64;
       size_t size;
     intmax_t imax;
    uintmax_t umax;
     intptr_t iptr;
    uintptr_t uptr;
    ptrdiff_t diff;
    const void *cptr;
    const char *cstr;
          void *ptr;
          char *str;
    // clang-format on
} o_T_o;

#endif /* __OO_OO_H__ */
