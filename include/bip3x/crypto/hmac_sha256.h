// Copyright (c) 2014-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTO_HMAC_SHA256_H
#define BITCOIN_CRYPTO_HMAC_SHA256_H

#include "bip3x/bip39_core.h"
#include "bip3x/crypto/hmac.h"
#include "sha256.h"

#include <stdint.h>
#include <stdlib.h>

/** A hasher class for HMAC-SHA-256. */
class BIP39_CORE_API CHMAC_SHA256 {
private:
    HMAC_SHA256_CTX m_ctx;

public:
    static const size_t OUTPUT_SIZE = 32;

    CHMAC_SHA256(const unsigned char* key, size_t keylen);
    CHMAC_SHA256 &Write(const unsigned char *data, size_t len);
    void Finalize(unsigned char hash[OUTPUT_SIZE]);
};

#endif // BITCOIN_CRYPTO_HMAC_SHA256_H
