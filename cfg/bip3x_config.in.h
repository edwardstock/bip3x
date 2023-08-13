#pragma once

#include <cstddef>
#include <cstdint>

#define BIP3X_C extern "C"

#ifdef BIP3X_BUILT_AS_STATIC
#define BIP3X_CORE_API
#define BIP3X_NO_EXPORT
#else
#ifndef BIP3X_CORE_API
#if _MSC_VER
#ifdef BIP3X_EXPORTS
#define BIP3X_CORE_API __declspec(dllexport)
#else
#define BIP3X_CORE_API __declspec(dllimport)
#endif
#else
#ifdef BIP3X_EXPORTS
#define BIP3X_CORE_API __attribute__((visibility("default")))
#else
#define BIP3X_CORE_API
#endif
#endif
#endif

#ifndef BIP3X_NO_EXPORT
#define BIP3X_NO_EXPORT
#endif
#endif

#if defined(WIN32) && !defined(bzero)
#include <cstring>
#define bzero(d,l) memset(d, 0, l)
#endif

/** Return codes */
#define BIP3X_OK      0 /** Success */
#define BIP3X_ERROR   -1 /** General error */
#define BIP3X_EINVAL  -2 /** Invalid argument */
#define BIP3X_ENOMEM  -3 /** malloc() failed */

/** Valid entropy lengths */
#define BIP3X_ENTROPY_LEN_128 16
#define BIP3X_ENTROPY_LEN_160 20
#define BIP3X_ENTROPY_LEN_192 24
#define BIP3X_ENTROPY_LEN_224 28
#define BIP3X_ENTROPY_LEN_256 32
#define BIP3X_ENTROPY_LEN_288 36
#define BIP3X_ENTROPY_LEN_320 40

#cmakedefine USE_OPENSSL_RANDOM
