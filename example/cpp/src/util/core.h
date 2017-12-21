/*
 * core.h
 *
 *  Created on: Aug 15, 2017
 *      Author: hanchen
 */

#ifndef CORE_H_
#define CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* from linux/compiler.h */
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

/* from linux/compiler-gcc.h */
/*
 * kernel style, complex and cool
 * used int[] != &(int *)[0] and int * == &(int *)[0]
 */
#ifdef __cplusplus
#define __must_be_array(a) 0
#else
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#endif

/* from linux/kernel.h */
#define BUILD_BUG_ON_NOT_POWER_OF_2(n) BUILD_BUG_ON((n) == 0 || (((n) & ((n) - 1)) != 0))

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))

#if !defined BUILD_BUG_ON
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
#endif

/* speedup if */
#if __GNUC__ >= 3
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#define ISTYPE(x, y) __same_type((x), (y))
#define SAMETYPE(x, y) __same_type((x), (y))

/*
 * if not array, will be fail when compile
 * if is array, will return 0
 */
#define MUST_BE_ARRAY(arr) __must_be_array(arr)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

/*
 * the length of the array, int[3] is 3
 * these is two method to avoid ARRLEN(int*), the param is must be array
 * 1.used anonymous array ((int[]) { })[0]
 *   #define ARRLEN(arr) (sizeof(arr) / sizeof(((typeof(arr)){})[0]))

 * 2.kernel style and is complex and cool
 *   used int[] != &(int *)[0] and int * == &(int *)[0]
 *   #define ARRLEN(arr) \
 *       (sizeof(arr) / sizeof((arr)[0]) \
 *       + sizeof(typeof(int[1 - 2 * !!__builtin_types_compatible_p(typeof(arr), typeof(&arr[0]))]) * 0)
 */
#define ARRLEN(arr) ARRAY_SIZE(arr)

#ifdef __cplusplus
}
#endif

#endif /* CORE_H_ */
