// Copyright (c) 2014-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "bip3x/crypto/hmac_sha512.h"

CHMAC_SHA512::CHMAC_SHA512(const unsigned char *key, size_t keylen) : m_ctx() {
    hmac_sha512_Init(&m_ctx, key, keylen);
}

CHMAC_SHA512 &CHMAC_SHA512::Write(const unsigned char *data, size_t len) {
    hmac_sha512_Update(&m_ctx, data, len);
    return *this;
}

void CHMAC_SHA512::Finalize(unsigned char hash[OUTPUT_SIZE])
{
    hmac_sha512_Final(&m_ctx, hash);
}


