/**
 * bip39. 2018
 * bip39_core.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#pragma once

#include <cstddef>
#include <cstdint>

#cmakedefine BIP39_SHARED
#cmakedefine BIP39_EXPORTING

#ifdef BIP39_SHARED
#  ifdef BIP3X_EXPORTING
#   if _MSC_VER
#       define BIP3X_CORE_API __declspec(dllexport)
#   else
#       define BIP3X_CORE_API __attribute__((visibility("default")))
#   endif
#  else
#   if _MSC_VER
#       define BIP3X_CORE_API __declspec(dllimport)
#   else
#       define BIP3X_CORE_API
#   endif
#  endif
#else
#define BIP3X_CORE_API
#endif // BIP39_SHARED

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
