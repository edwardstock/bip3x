#ifndef __MEMZERO_H__
#define __MEMZERO_H__

#include <cstddef>
#include "minter/bip39/bip39_core.h"

BIP39_CORE_API void memzero(void *s, size_t n);

#endif
