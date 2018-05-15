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
#include <ripemd160.h>
#include <cstring>
#include "base58.h"
#include "hasher.h"
#include "crypto/hmac_sha512.h"
#include "crypto/hmac_sha256.h"
#include "crypto/sha512.h"
#include "crypto/sha256.h"

namespace minter {

std::vector<uint8_t> hexToBytes(const std::string &hex);
std::string bytesToHex(const uint8_t *data, size_t len);
std::vector<std::string> splitString(const std::string &source, const std::string &delimiter);
const std::string glueStrings(const std::vector<std::string> &input, const std::string &g);
uint32_t stou(std::string const &str, size_t *idx = 0, int base = 10);

template<typename NumT>
void numToBytes(const NumT num, std::vector<uint8_t> &out) {
    static_assert(std::is_integral<NumT>::value, "Only integral types can be passed");

    size_t sz = sizeof(num);
    for (int i = 0; i < sz; i++) {
        out[(out.size() - 1) - i] = (num >> (i * 8));
    }
}

class Data {
 protected:
    std::vector<uint8_t> m_data;

 public:
    template<typename NumT>
    static Data readNumber(NumT num) {
        Data out(sizeof(num));
        numToBytes<NumT>(num, out.get());
        return out;
    }

    template<typename NumT>
    static Data readNumber(NumT num, size_t outSize) {
        assert(outSize >= sizeof(num));
        Data out(outSize);
        numToBytes<NumT>(num, out.get());
        return out;
    }

    Data() = default;
    Data(std::size_t size) {
        m_data.resize(size);
    }

    Data(const char *hexString) : m_data(hexToBytes(hexString)) { }
    Data(const std::vector<uint8_t> &data) : m_data(data) { }
    Data(std::vector<uint8_t> &&data) : m_data(std::move(data)) { }
    Data(const uint8_t *data, size_t len) {
        m_data.resize(len);
        memcpy(this->data(), data, len);
    }

    Data(const Data &other) = default;
    Data(Data &&other) = default;
    Data &operator=(const Data &other) = default;
    Data &operator=(Data &&other) = default;
    const std::vector<uint8_t> cget() const {
        return m_data;
    }

    std::vector<uint8_t> &get() {
        return m_data;
    }

    const std::string toHex() const {
        return bytesToHex(&m_data[0], size());
    }

    const size_t size() const {
        return m_data.size();
    }

    bool empty() const {
        return size() != 0;
    }

    operator bool() const noexcept {
        return !empty();
    }

    uint8_t *data() {
        return &m_data[0];
    }

    const uint8_t *cdata() const {
        return &m_data[0];
    }

    const std::string toString() {
        char out[size()];
        memcpy(out, data(), size());
        return std::string(out);
    }

    std::vector<uint8_t> takeFirstBytes(size_t n) const {
        std::vector<uint8_t> out;
        out.insert(out.begin(), m_data.begin(), m_data.begin() + n);
        return out;
    }

    std::vector<uint8_t> takeLastBytes(size_t n) const {
        std::vector<uint8_t> out;
        out.insert(out.begin(), m_data.end() - n, m_data.end());
        return out;
    }

    const std::string toBase58() {
        std::vector<char> out(112);
        size_t sz = size();
        base58_encode_check(cdata(), size(), HASHER_SHA3K, &out[0], 112);
        const char *res = &out[0];
        return std::string(res);
    }

    Data &toHmac512Mutable(const char *key) {
        std::vector<uint8_t> out(64);
        CHMAC_SHA512 hm(reinterpret_cast<const unsigned char *>(key), strlen(key));
        hm.Write(data(), size());
        hm.Finalize(&out[0]);
        clearReset();
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

    const std::vector<uint8_t> toHmac512(const char *key) const {
        return toHmac512(reinterpret_cast<const uint8_t *>(key), strlen(key));
    }

    const std::vector<uint8_t> toHmac512(const uint8_t *key, size_t len) const {
        std::vector<uint8_t> out(64);
        CHMAC_SHA512 hm(key, len);
        hm.Write(cdata(), size());
        hm.Finalize(&out[0]);

        return out;
    }

    const std::vector<uint8_t> toHash160() const {
        std::vector<uint8_t> out(20);
        ripemd160(cdata(), size(), &out[0]);
        return out;
    }

    void write(size_t pos, const uint8_t *data, size_t dataLen) {
        for (int i = 0; i < dataLen; i++) {
            m_data[i + pos] = data[i];
        }
    }

    void write(size_t pos, uint8_t data) {
        m_data[pos] = data;
    }

    void write(size_t pos, uint32_t data) {
        writeUint32BE(pos, data);
    }

    void write(size_t pos, const Data &data) {
        insert(pos, data.cget());
    }

    void insert(size_t pos, const std::vector<uint8_t> &data) {
        for (int i = 0; i < data.size(); i++) {
            m_data[pos + i] = data[i];
        }
    }

    inline uint8_t &operator[](std::size_t idx) noexcept {
        return m_data[idx];
    }

    inline uint8_t operator[](std::size_t idx) const noexcept {
        return m_data[idx];
    }

    bool operator==(const Data &other) const noexcept {
        return m_data == other.m_data;
    }

    bool operator!=(const Data &other) const noexcept {
        return m_data != other.m_data;
    }

    bool operator==(const uint8_t *other) const noexcept {
        if (!other || !other[0] || !other[m_data.size() - 1]) {
            return false;
        }

        for (size_t i = 0; i < m_data.size(); i++) {
            if (m_data[i] != other[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const uint8_t *other) const noexcept {
        return !operator==(other);
    }

    void writeUint32BE(size_t pos, uint32_t val) {
        m_data[pos] = val >> 24;
        m_data[pos + 1] = val >> 16;
        m_data[pos + 2] = val >> 8;
        m_data[pos + 3] = val;
    }

    void writeUint8(size_t pos, uint8_t *ptr, uint8_t val) {
        ptr[pos] = val;
    }

    void clear() {
        std::fill(m_data.begin(), m_data.end(), 0);
        m_data.clear();
    }

    void clearReset() {
        size_t sz = size();
        clear();
        resize(sz);
    }

    void resize(size_t sz) {
        m_data.resize(sz);
    }

    template<typename T>
    T to() {
        size_t len = sizeof(T);
        if (len == 1) {
            return m_data[0];
        } else if (len == 2) {
            uint16_t out = m_data[0] << 8u;
            out <<= 8;
            out |= m_data[1];
            return out;
        } else if (len == 4) {
            uint32_t out = (m_data[0] << 24ul) | (m_data[1] << 16u) | (m_data[2] << 8u) | (m_data[3] & 0xFFu);
            return out;
        } else if (sizeof(uint64_t) == len) {
            uint64_t out = (m_data[0] << 56ul)
                | (m_data[1] << 48ul)
                | (m_data[2] << 40ul)
                | (m_data[3] << 32ul)
                | (m_data[4] << 24ul)
                | (m_data[5] << 16ul)
                | (m_data[6] << 8ul)
                | (m_data[7] << 0xFFul);

            return out;
        }

        return 0;
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
    template<size_t NN>
    FixedData<NN> takeFirstData() const {
        return FixedData<NN>(takeFirstBytes(NN));
    }

    template<size_t NN>
    FixedData<NN> takeLastData() const {
        return FixedData<NN>(takeLastBytes(NN));
    }
};

using Data64 = FixedData<64>;
using Data32 = FixedData<32>;
using Bip32Key = FixedData<112>;

}

#endif //BIP39_UTILS_HPP
