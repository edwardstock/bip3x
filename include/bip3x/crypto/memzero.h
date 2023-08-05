#ifndef __MEMZERO_H__
#define __MEMZERO_H__

#include "bip3x/bip3x_config.h"

#include <cstddef>

BIP3X_CORE_API void memzero(void *s, size_t n);

#endif
