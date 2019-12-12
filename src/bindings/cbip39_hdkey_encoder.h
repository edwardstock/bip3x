/*!
 * bip39.
 * hdkey_encoder.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef BIP39_CBIP39_HDKEY_ENCODER_H
#define BIP39_CBIP39_HDKEY_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stddef.h>
#include <stdlib.h>
#include "cbip39_config.h"

typedef struct minter_hdkey {
    bip3x_data33 public_key;
    bip3x_data32 private_key;
    bip3x_data32 chain_code;
    bip3x_bip32_key ext_private_key;
    bip3x_bip32_key ext_public_key;
    uint8_t depth;
    uint32_t index;
    uint32_t fingerprint;
} minter_hdkey;

BIP39_API void encoder_make_bip32_root_key(const struct bip3x_data64* seed, struct minter_hdkey* hdkey);
BIP39_API void encoder_make_ext_key(struct minter_hdkey* hdkey, const char* derivation_path);
BIP39_API void free_hdkey(struct minter_hdkey* key);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //BIP39_CBIP39_HDKEY_ENCODER_H
