/**
 * bip39. 2018
 * utils.hpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef BIP39_UTILS_HPP
#define BIP39_UTILS_HPP

#include "bip39_core.h"
#include "bip3x/crypto/hmac_sha256.h"
#include "bip3x/crypto/hmac_sha512.h"
#include "bip3x/crypto/sha256.h"
#include "bip3x/crypto/sha512.h"

#include <bip3x/crypto/base58.h>
#include <bip3x/crypto/hasher.h>
#include <bip3x/crypto/ripemd160.h>
#include <bip3x/crypto/sha3.h>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <string>
#include <toolbox/data/bytes_array.h>
#include <toolbox/data/bytes_data.h>
#include <toolbox/data/utils.h>
#include <vector>

namespace bip3x {

BIP39_CORE_API uint32_t str_to_uint32_t(std::string const& str, size_t* idx = 0, int base = 10);

struct to_hmac_sha512 {
public:
    to_hmac_sha512(const char* key)
        : m_key(reinterpret_cast<const unsigned char*>(key)),
          m_key_len(strlen(key)) {
    }

    to_hmac_sha512(const uint8_t* key, size_t keylen)
        : m_key(key),
          m_key_len(keylen) {
    }

    std::vector<uint8_t> operator()(const std::vector<uint8_t>& source) {
        std::vector<uint8_t> out(64);
        CHMAC_SHA512 hm(m_key, m_key_len);
        hm.Write(source.data(), source.size());
        hm.Finalize(&out[0]);
        return out;
    }

private:
    const uint8_t* m_key;
    size_t m_key_len;
};

using bytes_data = toolbox::data::bytes_data;
template<size_t N>
using bytes_array = toolbox::data::bytes_array<N>;
using bytes_64 = toolbox::data::bytes_array<64>;
using bytes_32 = toolbox::data::bytes_array<32>;
using bip32_key = toolbox::data::bytes_array<112>;

} // namespace bip3x

#endif //BIP39_UTILS_HPP
