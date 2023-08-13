#pragma once

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

#cmakedefine HAVE_SYS_TYPES_H 1
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct bip3x_data64 { uint8_t data[64]; } bip3x_data64;
typedef struct bip3x_data33 { uint8_t data[33]; } bip3x_data33;
typedef struct bip3x_data32 { uint8_t data[32]; } bip3x_data32;
typedef struct bip3x_bip32_key { uint8_t data[112]; } bip3x_bip32_key;
