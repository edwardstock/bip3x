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
    Data() : bytes_data() { }
    Data(const bytes_data &other) : bytes_data(other) { }
    Data(bytes_data &&other) : bytes_data(other) { }
    Data(size_t size) : bytes_data(size) { }
    Data(const char *hexString) : bytes_data(hexString) { }
    Data(const std::string &hexString) : bytes_data(hexString) { }
    Data(const std::vector<uint8_t> &data) : bytes_data(data) { }
    Data(const uint8_t *data, size_t len) : bytes_data(data, len) { }
    ~Data() override = default;

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

/// \brief Unresizable bytes buffer. If you're trying to write data more than container N, than data just strips
/// to container N and writes to the begin()
/// \tparam N container maximum size
template<size_t N>
class FixedData : public Data {
 public:
    /// \brief Default: resize data to N forever
    FixedData() : Data() {
        m_data.resize(N);
    }

    /// \brief Write uint8_t* with len
    /// \param data bytes data, have check for NULL
    /// \param len if len > N, than container will copy only from 0 to N data
    FixedData(const uint8_t *data, size_t len) : FixedData() {
        if (data != nullptr) {
            memcpy(m_data.data(), data, std::min(len, N));
        }
    }

    /// \brief Be carefully and make sure input data contains >= N elements, otherwise you'll have at least garbage in a data or UB
    /// \param data
    explicit FixedData(const uint8_t *data) : FixedData(data, N) { }

    /// \brief converts hex string to bytes.
    /// \param hexString String CAN'T have prefixes or odd length, as hex representation - 2 characters is a 1 byte
    FixedData(const std::string &hexString) : FixedData() {
        if (hexString.length() % 2 == 0 && hexString.length() <= N * 2) {
            auto tmp = toolboxpp::data::hexToBytes(hexString);
            std::copy(tmp.begin(), tmp.end(), m_data.begin());
            tmp.clear();
        }
    }

    /// \brief converts hex string to bytes.
    /// \param hexString
    FixedData(const char *hexString) : FixedData(hexString) { }

    explicit FixedData(const std::vector<uint8_t> &data) : FixedData() {
        size_t readLen = std::min(data.size(), N);
        std::copy(data.begin(), data.begin() + readLen, begin());
    }

    explicit FixedData(std::vector<uint8_t> &&data) : FixedData() {
        size_t readLen = std::min(data.size(), N);
        std::copy(data.begin(), data.begin() + readLen, begin());
        data.clear();
    }

    FixedData(const FixedData<N> &other) : FixedData() {
        clear();
        m_data = other.m_data;
    }

    FixedData(FixedData<N> &&other) noexcept {
        m_data = std::move(other.m_data);
        other.clear();
    }

    FixedData<N> &operator=(FixedData<N> other) {
        std::swap(m_data, other.m_data);
        other.clear();
        return *this;
    }

    FixedData<N> &operator=(std::vector<uint8_t> other) {
        std::swap(m_data, other);
        other.clear();
        return *this;
    }

    virtual ~FixedData() = default;

    void resize(size_t) {
        // do nothing
        // client code still can resize m_data in derived class
    }

    size_t size() const {
        return N;
    }

};

using Data64 = FixedData<64>;
using Data32 = FixedData<32>;
using Bip32Key = FixedData<112>;

}

#endif //BIP39_UTILS_HPP
