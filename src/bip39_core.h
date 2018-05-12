/**
 * bip39. 2018
 * bip39_core.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef MINTER_BIP39_CORE_H
#define MINTER_BIP39_CORE_H

#include <cstddef>
#include <cstdint>

#ifndef MINTER_BIP39_CORE_API
# if defined(_WIN32)
#  ifdef WALLY_CORE_BUILD
#   define BIP39_CORE_API __declspec(dllexport)
#  else
#   define BIP39_CORE_API
#  endif
# elif defined(__GNUC__) && defined(WALLY_CORE_BUILD)
#  define BIP39_CORE_API __attribute__ ((visibility ("default")))
# else
#  define MINTER_BIP39_CORE_API
# endif
#endif

/** Return codes */
#define MINTER_OK      0 /** Success */
#define MINTER_ERROR  -1 /** General error */
#define MINTER_EINVAL -2 /** Invalid argument */
#define MINTER_ENOMEM -3 /** malloc() failed */

#endif //MINTER_BIP39_CORE_H
