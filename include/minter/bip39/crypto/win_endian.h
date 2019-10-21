#ifndef    _I386_ENDIAN_H_
#define    _I386_ENDIAN_H_

/* All the headers include this file. */
#ifdef __MINGW32__
#include <_mingw.h>
#endif
#include <sys/types.h>
#include <cstddef>

#define __LITTLE_ENDIAN_BITFIELD


/* Note that these macros evaluate their arguments several times.  */
#define __swap16gen(x)                            \
    (__uint16_t)(((__uint16_t)(x) & 0xff) << 8 | ((__uint16_t)(x) & 0xff00) >> 8)

#define __swap32gen(x)                            \
    (uint32_t)(((uint32_t)(x) & 0xff) << 24 |            \
    ((uint32_t)(x) & 0xff00) << 8 | ((uint32_t)(x) & 0xff0000) >> 8 |\
    ((uint32_t)(x) & 0xff000000) >> 24)

#define __swap64gen(x)                            \
    (uint64_t)((((uint64_t)(x) & 0xff) << 56) |            \
        ((uint64_t)(x) & 0xff00ULL) << 40 |            \
        ((uint64_t)(x) & 0xff0000ULL) << 24 |            \
        ((uint64_t)(x) & 0xff000000ULL) << 8 |            \
        ((uint64_t)(x) & 0xff00000000ULL) >> 8 |            \
        ((uint64_t)(x) & 0xff0000000000ULL) >> 24 |        \
        ((uint64_t)(x) & 0xff000000000000ULL) >> 40 |        \
        ((uint64_t)(x) & 0xff00000000000000ULL) >> 56)

#define __swap16 __swap16gen
#define __swap32 __swap32gen
#define __swap64 __swap64gen

#define swap16 __swap16
#define swap32 __swap32
#define swap64 __swap64

#endif    /* _I386_ENDIAN_H_ */