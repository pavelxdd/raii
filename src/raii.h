#pragma once

/* RAII is a single header wrapper around compiler `cleanup` attribute.
 *
 * For GCC it uses nested functions for cleanup handlers.
 * For Clang it uses Blocks language extension.
 */

#ifndef __raii_has_feature
#   ifdef __has_feature
#       define __raii_has_feature(x) __has_feature(x)
#   else
#       define __raii_has_feature(x) (0)
#   endif
#endif

#ifndef __raii_has_attribute
#   ifdef __has_attribute
#       define __raii_has_attribute(x) __has_attribute(x)
#   else
#       define __raii_has_attribute(x) (0)
#   endif
#endif

#ifndef __raii_inline
#   if __raii_has_attribute(__always_inline__)
#       define __raii_inline inline __attribute__((__always_inline__))
#   else
#       define __raii_inline inline
#   endif
#endif

#ifndef __raii_unused
#   if __raii_has_attribute(__unused__)
#       define __raii_unused __attribute__((__unused__))
#   else
#       define __raii_unused
#   endif
#endif

#ifndef __raii_cleanup
#   if __raii_has_attribute(__cleanup__)
#       define __raii_cleanup(x) __attribute__((__cleanup__(x)))
#   else
#       error "attribute cleanup is not supported"
#   endif
#endif

#define __raii_concat(x, y) __raii_concat_helper(x, y)
#define __raii_concat_helper(x, y) x ## y

#ifdef __INTELLISENSE__ // disable RAII for VSCode IntelliSense
#define RAII(__type, __name, __dtor, __init) __type __name = (__init)
#else
#if defined(__clang__)
#if __raii_has_feature(blocks)
typedef void (^__raii_cleanup_block_t)(void);
static __raii_inline __raii_unused \
void __raii_cleanup_block(__raii_cleanup_block_t *b) { (*b)(); }
#define RAII(__type, __name, __dtor, __init) \
    __block __type __name = (__init); \
    __raii_unused __raii_cleanup(__raii_cleanup_block) \
    __raii_cleanup_block_t __raii_concat(__raii_cleanup_, __name) = ^{ { __dtor(__name); } }
#else
#error "blocks feature is not supported"
#endif
#elif defined(__GNUC__)
#define RAII(__type, __name, __dtor, __init) \
    void __raii_concat(__raii_cleanup_, __name) (__type *ptr) { __dtor(*ptr); } \
    __raii_unused __raii_cleanup(__raii_concat(__raii_cleanup_, __name)) \
    __type __name = (__init)
#else
#error "compiler is not supported"
#endif
#endif // __INTELLISENSE__
