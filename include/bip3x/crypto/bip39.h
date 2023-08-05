/**
 * Copyright (c) 2013-2014 Tomas Dzetkulic
 * Copyright (c) 2013-2014 Pavol Rusnak
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __BIP39_H__
#define __BIP39_H__

#include "bip3x/bip3x_config.h"

#include <cstdint>

#define BIP39_PBKDF2_ROUNDS 2048

BIP39_CORE_API const char *mnemonic_generate(int strength);	// strength in bits
BIP39_CORE_API const uint16_t *mnemonic_generate_indexes(int strength);	// strength in bits

BIP39_CORE_API const char *mnemonic_from_data(const uint8_t *data, int len);
BIP39_CORE_API const uint16_t *mnemonic_from_data_indexes(const uint8_t *data, int len);

BIP39_CORE_API int mnemonic_check(const char *mnemonic);

// passphrase must be at most 256 characters or code may crash
BIP39_CORE_API void mnemonic_to_seed(const char *mnemonic, const char *passphrase, uint8_t seed[512 / 8], void (*progress_callback)(uint32_t current, uint32_t total));

BIP39_CORE_API const char * const *mnemonic_wordlist(void);

#endif
