/**
 * bip39. 2018
 * bip39_core.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef LIBWALLY_CORE_BIT39_CORE_H
#define LIBWALLY_CORE_BIT39_CORE_H

#include <stddef.h>
#include <stdint.h>

#ifndef BIP39_CORE_API
# if defined(_WIN32)
#  ifdef WALLY_CORE_BUILD
#   define BIP39_CORE_API __declspec(dllexport)
#  else
#   define BIP39_CORE_API
#  endif
# elif defined(__GNUC__) && defined(WALLY_CORE_BUILD)
#  define BIP39_CORE_API __attribute__ ((visibility ("default")))
# else
#  define BIP39_CORE_API
# endif
#endif

/** Return codes */
#define BIP39_OK      0 /** Success */
#define BIP39_ERROR  -1 /** General error */
#define BIP39_EINVAL -2 /** Invalid argument */
#define BIP39_ENOMEM -3 /** malloc() failed */

#endif //LIBWALLY_CORE_BIT39_CORE_H
