// Copyright (c) 2014-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTO_HMAC_SHA512_H
#define BITCOIN_CRYPTO_HMAC_SHA512_H

#include "bip3x/bip39_core.h"
#include "sha512.h"

#include <bip3x/crypto/hmac.h>
#include <cstdint>
#include <cstdlib>

/** A hasher class for HMAC-SHA-512. */
class BIP39_CORE_API CHMAC_SHA512 {
private:
    HMAC_SHA512_CTX m_ctx;

public:
    static const size_t OUTPUT_SIZE = 64;

    CHMAC_SHA512(const unsigned char* key, size_t keylen);
    CHMAC_SHA512 &Write(const unsigned char *data, size_t len);
    void Finalize(unsigned char hash[OUTPUT_SIZE]);
};

#endif // BITCOIN_CRYPTO_HMAC_SHA512_H
