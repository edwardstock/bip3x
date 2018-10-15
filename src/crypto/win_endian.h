/*
 * Endianness - Detected Files and Settings
 *
 * This file is parsed by CMake during makefile generation
 */

#ifndef ENDIANNESS_GENERATED_CONFIG_H_
#define ENDIANNESS_GENERATED_CONFIG_H_

/* If a non-system implementation is forced, do not define any headers as available */
/* #undef ENDIANNESS_CONFIG_FORCE_NON_SYSTEM_IMPLEMENTATION */

#ifndef ENDIANNESS_CONFIG_FORCE_NON_SYSTEM_IMPLEMENTATION

/* Whether endian.h is available */
/* #undef ENDIANNESS_CONFIG_HAVE_ENDIAN_H */

/* Whether sys/endian.h is available */
/* #undef ENDIANNESS_CONFIG_HAVE_SYS_ENDIAN_H */

/* Whether sys/param.h is available */
#define ENDIANNESS_CONFIG_HAVE_SYS_PARAM_H

/* Whether inttypes.h is available */
#define ENDIANNESS_CONFIG_HAVE_INTTYPES_H

/* Whether features.h is available */
/* #undef ENDIANNESS_CONFIG_HAVE_FEATURES_H */

/* Whether libkern/OSByteOrder.h is available */
/* #undef ENDIANNESS_CONFIG_HAVE_LIBKERN_OSBYTEORDER_H */

/* Whether arpa/inet.h is available */
/* #undef ENDIANNESS_CONFIG_HAVE_ARPA_INET_H */

/* Whether netinet/in.h is available */
/* #undef ENDIANNESS_CONFIG_HAVE_NETINET_IN_H */

/* Whether winsock2.h is available */
#define ENDIANNESS_CONFIG_HAVE_WINSOCK2_H

/* Whether the endian.h or sys/endian.h functions use OpenBSD style (letoh16) */
/* #undef ENDIANNESS_CONFIG_HAVE_OPENBSD_STYLE_ENDIAN_FUNCTIONS */

/* Whether the endian.h or sys/endian.h functions use Non-OpenBSD style (le16toh) */
/* #undef ENDIANNESS_CONFIG_HAVE_NON_OPENBSD_STYLE_ENDIAN_FUNCTIONS */

/* Whether the compiler supports builtin byteswap intrinsics (__builtin_bswap16) */
#define ENDIANNESS_CONFIG_HAVE_GCC_STYLE_BSWAP_FUNCTIONS

/* Whether the compiler supports builtin byteswap intrinsics (_byteswap_ushort) */
#define ENDIANNESS_CONFIG_HAVE_MSVC_STYLE_BSWAP_FUNCTIONS

/* Whether the 16bit hton functions are available (htons, ntohs) */
/* #undef ENDIANNESS_CONFIG_HAVE_HTONS_FUNCTIONS */

/* Whether the 32bit hton functions are available (htonl, ntohl) */
/* #undef ENDIANNESS_CONFIG_HAVE_HTONL_FUNCTIONS */

/* Whether the 64bit hton functions are available (htonll, ntohll) */
/* #undef ENDIANNESS_CONFIG_HAVE_HTONLL_FUNCTIONS */

#endif /* ENDIANNESS_CONFIG_FORCE_NON_SYSTEM_IMPLEMENTATION */



/* ------------------------------------------------------------------- */
/* Function declarations, depending on the macro values above. */

#include <stddef.h>
#include <stdint.h>

/* Necessary for some systems to expose functions */
#if !(defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(ENDIANNESS_CONFIG_FORCE_NON_SYSTEM_IMPLEMENTATION)
#	ifndef __USE_BSD
#		define __USE_BSD
#	endif
#	ifndef _BSD_SOURCE
#		define _BSD_SOURCE
#	endif
#	ifndef _SVID_SOURCE
#		define _SVID_SOURCE
#	endif
#	ifndef _DEFAULT_SOURCE
#		define _DEFAULT_SOURCE
#	endif
#endif

#ifdef ENDIANNESS_CONFIG_HAVE_ENDIAN_H
#	include <endian.h>
#endif

#ifdef ENDIANNESS_CONFIG_HAVE_SYS_ENDIAN_H
#	include <sys/endian.h>
#endif

#ifdef ENDIANNESS_CONFIG_HAVE_SYS_PARAM_H
#	include <sys/param.h>
#endif

#ifdef ENDIANNESS_CONFIG_HAVE_INTTYPES_H
#	include <inttypes.h>
#endif

/*
	Using features.h we can test for the GLIBC version.
	If it is below 2.9 "endian.h" will be missing the required functions.
	See http://linux.die.net/man/3/endian
 */
#ifdef ENDIANNESS_CONFIG_HAVE_FEATURES_H
#	include <features.h>
#	if !defined(__GLIBC__) || !defined(__GLIBC_MINOR__) || ((__GLIBC__ < 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ < 9)))
#		if defined(ENDIANNESS_CONFIG_HAVE_ENDIAN_H)
#			undef ENDIANNESS_CONFIG_HAVE_ENDIAN_H
#		endif
#		if defined(ENDIANNESS_CONFIG_HAVE_SYS_ENDIAN_H)
#			undef ENDIANNESS_CONFIG_HAVE_SYS_ENDIAN_H
#		endif
#	endif
#endif

#ifdef ENDIANNESS_CONFIG_HAVE_ARPA_INET_H
#	include <arpa/inet.h>
#endif

#ifdef ENDIANNESS_CONFIG_HAVE_NETINET_IN_H
#	include <netinet/in.h>
#endif

#ifdef ENDIANNESS_CONFIG_HAVE_WINSOCK2_H
#	include <WinSock2.h>
#endif

#if defined(ENDIANNESS_CONFIG_HAVE_MSVC_STYLE_BSWAP_FUNCTIONS) && !defined(ENDIANNESS_CONFIG_HAVE_GCC_STYLE_BSWAP_FUNCTIONS)
#	define __builtin_bswap16(x) _byteswap_ushort((x))
#	define __builtin_bswap32(x) _byteswap_ulong((x))
#	define __builtin_bswap64(x) _byteswap_uint64((x))
#	define ENDIANNESS_CONFIG_HAVE_GCC_STYLE_BSWAP_FUNCTIONS
#endif

#ifdef ENDIANNESS_CONFIG_HAVE_LIBKERN_OSBYTEORDER_H
/* Apple Systems, Mac OS */
#	include <libkern/OSByteOrder.h>

#	define htobe16(x) OSSwapHostToBigInt16(x)
#	define htole16(x) OSSwapHostToLittleInt16(x)
#	define betoh16(x) OSSwapBigToHostInt16(x)
#	define letoh16(x) OSSwapLittleToHostInt16(x)

#	define htobe32(x) OSSwapHostToBigInt32(x)
#	define htole32(x) OSSwapHostToLittleInt32(x)
#	define betoh32(x) OSSwapBigToHostInt32(x)
#	define letoh32(x) OSSwapLittleToHostInt32(x)

#	define htobe64(x) OSSwapHostToBigInt64(x)
#	define htole64(x) OSSwapHostToLittleInt64(x)
#	define betoh64(x) OSSwapBigToHostInt64(x)
#	define letoh64(x) OSSwapLittleToHostInt64(x)

#elif defined(ENDIANNESS_CONFIG_HAVE_NON_OPENBSD_STYLE_ENDIAN_FUNCTIONS) && !defined(ENDIANNESS_CONFIG_HAVE_OPENBSD_STYLE_ENDIAN_FUNCTIONS)

/* Define the methods using those that are in the Non-OpenBSD style */
#	define betoh16(x) be16toh((x))
#	define letoh16(x) le16toh((x))
#	define betoh32(x) be32toh((x))
#	define letoh32(x) le32toh((x))
#	define betoh64(x) be64toh((x))
#	define letoh64(x) le64toh((x))

#elif !defined(ENDIANNESS_CONFIG_HAVE_NON_OPENBSD_STYLE_ENDIAN_FUNCTIONS) && !defined(ENDIANNESS_CONFIG_HAVE_OPENBSD_STYLE_ENDIAN_FUNCTIONS) && (ENDIANNESS_CONFIG_ENDIAN_TYPE == ENDIANNESS_VAL_LITTLE_ENDIAN)

/* 16bit functions */
#	if defined(ENDIANNESS_CONFIG_HAVE_HTONS_FUNCTIONS)
#		ifndef htobe16
#			define htobe16(x) htons(x)
#		endif
#		define htole16(x) (x)
#		define betoh16(x) ntohs(x)
#		define letoh16(x) (x)
#	elif defined(ENDIANNESS_CONFIG_HAVE_MSVC_STYLE_BSWAP_FUNCTIONS) || defined(ENDIANNESS_CONFIG_HAVE_GCC_STYLE_BSWAP_FUNCTIONS)
#		define htobe16(x) __builtin_bswap16(x)
#		define htole16(x) (x)
#		define betoh16(x) __builtin_bswap16(x)
#		define letoh16(x) (x)
#	else
#		define htobe16(x) to_big_endian16(x)
#		define htole16(x) (x)
#		define betoh16(x) from_big_endian16(x)
#		define letoh16(x) (x)
#	endif
/* 32bit functions */
#	if defined(ENDIANNESS_CONFIG_HAVE_HTONL_FUNCTIONS)
#		ifndef htobe32
#			define htobe32(x) htonl(x)
#		endif
#		define htole32(x) (x)
#		define betoh32(x) ntohl(x)
#		define letoh32(x) (x)
#	elif defined(ENDIANNESS_CONFIG_HAVE_MSVC_STYLE_BSWAP_FUNCTIONS) || defined(ENDIANNESS_CONFIG_HAVE_GCC_STYLE_BSWAP_FUNCTIONS)
#		define htobe32(x) __builtin_bswap32(x)
#		define htole32(x) (x)
#		define betoh32(x) __builtin_bswap32(x)
#		define letoh32(x) (x)
#	else
#		define htobe32(x) to_big_endian32(x)
#		define htole32(x) (x)
#		define betoh32(x) from_big_endian32(x)
#		define letoh32(x) (x)
#	endif
/* 64bit functions */
#	if defined(ENDIANNESS_CONFIG_HAVE_HTONLL_FUNCTIONS)
#		define htobe64(x) htonll(x)
#		define htole64(x) (x)
#		define betoh64(x) ntohll(x)
#		define letoh64(x) (x)
#	elif defined(ENDIANNESS_CONFIG_HAVE_MSVC_STYLE_BSWAP_FUNCTIONS) || defined(ENDIANNESS_CONFIG_HAVE_GCC_STYLE_BSWAP_FUNCTIONS)
#		ifndef htobe64
#			define htobe64(x) __builtin_bswap64(x)
#		endif
#		define htole64(x) (x)
#		define betoh64(x) __builtin_bswap64(x)
#		define letoh64(x) (x)
#	else
#		define htobe64(x) to_big_endian64(x)
#		define htole64(x) (x)
#		define betoh64(x) from_big_endian64(x)
#		define letoh64(x) (x)
#	endif

#elif !defined(ENDIANNESS_CONFIG_HAVE_NON_OPENBSD_STYLE_ENDIAN_FUNCTIONS) && !defined(ENDIANNESS_CONFIG_HAVE_OPENBSD_STYLE_ENDIAN_FUNCTIONS) && (ENDIANNESS_CONFIG_ENDIAN_TYPE == ENDIANNESS_VAL_BIG_ENDIAN)

/* 16bit functions */
#	if defined(ENDIANNESS_CONFIG_HAVE_MSVC_STYLE_BSWAP_FUNCTIONS) || defined(ENDIANNESS_CONFIG_HAVE_GCC_STYLE_BSWAP_FUNCTIONS)
#		define htobe16(x) (x)
#		define htole16(x) __builtin_bswap16(x)
#		define betoh16(x) (x)
#		define letoh16(x) __builtin_bswap16(x)
#	else
#		define htobe16(x) (x)
#		define htole16(x) to_little_endian16(x)
#		define betoh16(x) (x)
#		define letoh16(x) from_little_endian16(x)
#	endif
/* 32bit functions */
#	if defined(ENDIANNESS_CONFIG_HAVE_MSVC_STYLE_BSWAP_FUNCTIONS) || defined(ENDIANNESS_CONFIG_HAVE_GCC_STYLE_BSWAP_FUNCTIONS)
#		define htobe32(x) (x)
#		define htole32(x) __builtin_bswap32(x)
#		define betoh32(x) (x)
#		define letoh32(x) __builtin_bswap32(x)
#	else
#		define htobe32(x) (x)
#		define htole32(x) to_little_endian32(x)
#		define betoh32(x) (x)
#		define letoh32(x) from_little_endian32(x)
#	endif
/* 64bit functions */
#	if defined(ENDIANNESS_CONFIG_HAVE_MSVC_STYLE_BSWAP_FUNCTIONS) || defined(ENDIANNESS_CONFIG_HAVE_GCC_STYLE_BSWAP_FUNCTIONS)
#		define htobe64(x) (x)
#		define htole64(x) __builtin_bswap64(x)
#		define betoh64(x) (x)
#		define letoh64(x) __builtin_bswap64(x)
#	else
#		define htobe64(x) (x)
#		define htole64(x) to_little_endian64(x)
#		define betoh64(x) (x)
#		define letoh64(x) from_little_endian64(x)
#	endif

#else
/* fallback if no system-provided functions are available */
#	define htobe16(x) to_big_endian16(x)
#	define htole16(x) to_little_endian16(x)
#	define betoh16(x) from_big_endian16(x)
#	define letoh16(x) from_little_endian16(x)
#	define htobe32(x) to_big_endian32(x)
#	define htole32(x) to_little_endian32(x)
#	define betoh32(x) from_big_endian32(x)
#	define letoh32(x) from_little_endian32(x)
#	define htobe64(x) to_big_endian64(x)
#	define htole64(x) to_little_endian64(x)
#	define betoh64(x) from_big_endian64(x)
#	define letoh64(x) from_little_endian64(x)
#endif

#ifndef ENDIANNESS_CONFIG_FORCE_SYSTEM_IMPLEMENTATION
/*
	Datatypes for accessing the bytes of uintNN_t
 */
union Endian_Data_16 {
  uint16_t integer_value;
  unsigned char char_values[2];
};

union Endian_Data_32 {
  uint32_t integer_value;
  unsigned char char_values[4];
};

union Endian_Data_64 {
  uint64_t integer_value;
  unsigned char char_values[8];
};

#ifndef ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_0_INDEX
#define ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_0_INDEX 1
#define ENDIANNESS_2_BYTE_TYPE_LSB_PLUS_1_INDEX 0
#define ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_0_INDEX 3
#define ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_1_INDEX 2
#define ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_2_INDEX 1
#define ENDIANNESS_4_BYTE_TYPE_LSB_PLUS_3_INDEX 0
#define ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_0_INDEX 7
#define ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_1_INDEX 6
#define ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_2_INDEX 5
#define ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_3_INDEX 4
#define ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_4_INDEX 3
#define ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_5_INDEX 2
#define ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_6_INDEX 1
#define ENDIANNESS_8_BYTE_TYPE_LSB_PLUS_7_INDEX 0
#endif

/*
	Custom, non-system dependent implementations of the endian-switching functions.
	These depend on the correct findings of the CMake endian test.
 */
uint16_t to_little_endian16(uint16_t value);

uint32_t to_little_endian32(uint32_t value);
uint64_t to_little_endian64(uint64_t value);

uint16_t to_big_endian16(uint16_t value);
uint32_t to_big_endian32(uint32_t value);
uint64_t to_big_endian64(uint64_t value);

uint16_t from_little_endian16(uint16_t value);
uint32_t from_little_endian32(uint32_t value);
uint64_t from_little_endian64(uint64_t value);

uint16_t from_big_endian16(uint16_t value);
uint32_t from_big_endian32(uint32_t value);
uint64_t from_big_endian64(uint64_t value);

#	if BYTE_ORDER == LITTLE_ENDIAN
#		define be16toh(x) from_big_endian16(x)
#		define le16toh(x) (x)
#		define be32toh(x) from_big_endian32(x)
#		define le32toh(x) (x)
#		define be64toh(x) from_big_endian64(x)
#		define le64toh(x) (x)
#	elif BYTE_ORDER == BIG_ENDIAN

/* that would be xbox 360 */
#		define htobe16(x) (x)
#		define htole16(x) __builtin_bswap16(x)
#		define be16toh(x) (x)
#		define le16toh(x) __builtin_bswap16(x)

#		define htobe32(x) (x)
#		define htole32(x) __builtin_bswap32(x)
#		define be32toh(x) (x)
#		define le32toh(x) __builtin_bswap32(x)

#		define htobe64(x) (x)
#		define htole64(x) __builtin_bswap64(x)
#		define be64toh(x) (x)
#		define le64toh(x) __builtin_bswap64(x)

#	else

#		error byte order not supported

#	endif

#endif /* ENDIANNESS_CONFIG_FORCE_SYSTEM_IMPLEMENTATION */

#endif /* ENDIANNESS_GENERATED_CONFIG_H_ */
