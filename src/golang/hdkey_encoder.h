/*!
 * bip39.
 * hdkey_encoder.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef BIP39_HDKEY_ENCODER_H
#define BIP39_HDKEY_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stddef.h>
#include <stdlib.h>
#include "bip39_config.h"

typedef struct minter_hdkey {
  minter_data33 public_key;
  minter_data32 private_key;
  minter_data32 chain_code;
  minter_bip32_key ext_private_key;
  minter_bip32_key ext_public_key;
  uint8_t depth;
  uint32_t index;
  uint32_t fingerprint;
} minter_hdkey;

GOEXPORT struct minter_hdkey *encoder_make_bip32_root_key(const struct minter_data64 *seed);
GOEXPORT struct minter_hdkey *encoder_make_ext_key(const struct minter_hdkey *root_key, const char *derivation_path);
GOEXPORT void free_hdkey(struct minter_hdkey *key);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //BIP39_HDKEY_ENCODER_H
