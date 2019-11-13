/**
 * bip39. 2018
 * utils.hpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#ifndef BIP39_UTILS_HPP
#define BIP39_UTILS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <minter/crypto/ripemd160.h>
#include <cstring>
#include <minter/crypto/base58.h>
#include <minter/crypto/hasher.h>
#include <minter/crypto/sha3.h>
#include <toolboxpp.hpp>
#include "bip39_core.h"
#include "minter/bip39/crypto/hmac_sha512.h"
#include "minter/bip39/crypto/hmac_sha256.h"
#include "minter/bip39/crypto/sha512.h"
#include "minter/bip39/crypto/sha256.h"

namespace minter {

BIP39_CORE_API uint32_t str_to_uint32_t(std::string const &str, size_t *idx = 0, int base = 10);

class Data : public toolboxpp::data::bytes_data {
 public:
    Data() = default;
    Data(size_t size) : bytes_data(size) { }
    Data(const char *hexString) : bytes_data(hexString) { }
    Data(const std::string &hexString) : bytes_data(hexString) { }
    Data(const std::vector<uint8_t> &data) : bytes_data(data) { }
    Data(std::vector<uint8_t> &&data) : bytes_data(data) { }
    Data(const uint8_t *data, size_t len) : bytes_data(data, len) { }
    Data(const bytes_data &other) : bytes_data(other) { }
    Data(bytes_data &&other) : bytes_data(other) { }

    std::string toBase58() const {
        std::vector<char> out(112);
        base58_encode_check(cdata(), size(), HASHER_SHA3K, out.data(), 112);
        return std::string(out.begin(), out.end());
    }

    Data &toHmac512Mutable(const char *key) {
        std::vector<uint8_t> out(64);
        CHMAC_SHA512 hm(reinterpret_cast<const unsigned char *>(key), strlen(key));
        hm.Write(data(), size());
        hm.Finalize(&out[0]);
        clear();
        m_data = std::move(out);

        return *this;
    }

    Data &toHmac512Mutable(const uint8_t *key, size_t len) {
        std::vector<uint8_t> out(64);
        CHMAC_SHA512 hm(key, len);
        hm.Write(data(), size());
        hm.Finalize(&out[0]);
        m_data.clear();
        m_data = std::move(out);

        return *this;
    }

    std::vector<uint8_t> toHmac512(const char *key) const {
        return toHmac512(reinterpret_cast<const uint8_t *>(key), strlen(key));
    }

    std::vector<uint8_t> toHmac512(const uint8_t *key, size_t len) const {
        std::vector<uint8_t> out(64);
        CHMAC_SHA512 hm(key, len);
        hm.Write(cdata(), size());
        hm.Finalize(&out[0]);

        return out;
    }

    std::vector<uint8_t> toSha3K() const {
        std::vector<uint8_t> output(SHA3_256_DIGEST_LENGTH);

        SHA3_CTX hash_ctx;
        keccak_256_Init(&hash_ctx);
        keccak_Update(&hash_ctx, cdata(), size());
        keccak_Final(&hash_ctx, &output[0]);

        return output;
    }

    std::vector<uint8_t> toHash160() const {
        std::vector<uint8_t> out(20);
        ripemd160(cdata(), size(), &out[0]);
        return out;
    }
};

template<size_t N>
class FixedData : public Data {
 public:
    FixedData() : Data() {
        m_data.resize(N);
    }

    FixedData(const char *hexString) : Data(hexString) {
        m_data.resize(N);
    }
    FixedData(const uint8_t *data) : Data(data, (size_t) N) { }
    FixedData(std::vector<uint8_t> &&data) : Data(std::move(data)) { }
    FixedData(const std::vector<uint8_t> &data) : Data(data) { }

    FixedData(const uint8_t *data, size_t len) : Data(data, len) {
    }
};

using Data64 = FixedData<64>;
using Data32 = FixedData<32>;
using Bip32Key = FixedData<112>;

}

#endif //BIP39_UTILS_HPP
