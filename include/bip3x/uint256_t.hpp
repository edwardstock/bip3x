/*!
 * bip39.
 * uint256_t.hpp
 *
 * \date 2018
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef BIP39_UINT256_T_HPP
#define BIP39_UINT256_T_HPP

#include "utils.h"

#include <bip3x/crypto/bignum.h>
#include <bip3x/crypto/memzero.h>

namespace bip3x {

class uint256_t {
private:
    bignum256 m_val;

public:
    uint256_t()
        : m_val() {
        const auto v = toolbox::data::num_to_bytes(0, 32);
        bn_read_be(v.data(), &m_val);
    }

    uint256_t(uint8_t val): m_val() {
         const auto v = toolbox::data::num_to_bytes(val, 32);
         ;
         bn_read_be(v.data(), &m_val);
     }

     uint256_t(int8_t val): m_val() {
         const auto v = toolbox::data::num_to_bytes(val, 32);
         bn_read_be(v.data(), &m_val);
     }

     uint256_t(uint16_t val): m_val() {
         const auto v = toolbox::data::num_to_bytes(val, 32);
         bn_read_be(v.data(), &m_val);
     }

     uint256_t(int16_t val): m_val() {
        if(val < 0) {
            val = val & 0xFFFF;
        }
        const auto v = toolbox::data::num_to_bytes(val, 32);

        bn_read_be(v.data(), &m_val);
     }

    uint256_t(uint32_t val): m_val() {
         const auto v = toolbox::data::num_to_bytes(val, 32);
         bn_read_be(v.data(), &m_val);
     }

     uint256_t(int32_t val): m_val() {
        if(val < 0) {
            val = val & 0xFFFFFFFF;
        }
        const auto v = toolbox::data::num_to_bytes(val, 32);

        bn_read_be(v.data(), &m_val);
     }

    uint256_t(uint64_t val): m_val() {
         const auto v = toolbox::data::num_to_bytes(val, 32);
         bn_read_be(v.data(), &m_val);
     }

     uint256_t(int64_t val): m_val() {
         if (val < 0) {
             val = val & 0xFFFFFFFFFFFFFFFFULL;
         }
         const auto v = toolbox::data::num_to_bytes(val, 32);
         bn_read_be(v.data(), &m_val);
     }

     uint256_t(uint8_t *data): m_val() {
        bn_read_be(data, &m_val);
    }

    uint256_t(const bignum256 &data): m_val(data) { }
    uint256_t(bignum256&& data)
        : m_val(std::move(data)) {
    }
    uint256_t(const bip3x::bytes_data& d)
        : m_val() {
        if (d.size() < 32) {
            std::vector<uint8_t> out(32);
            for (size_t i = 32 - d.size(), s = 0; i < 32; i++) {
                out[i] = d[s++];
            }
            bn_read_be(&out[0], &m_val);
            return;
        }
        //bn_read_be(I.cdata(), &a.val());
        bn_read_be(d.cdata(), &m_val);
    }

    explicit uint256_t(const std::vector<uint8_t> &data) : m_val() {
        if (data.size() > 32) {
            std::vector<uint8_t> out(32);
            out.insert(out.begin(), data.begin(), data.begin() + 32);
            return;
        } else {
            std::vector<uint8_t> out(32);
            out.insert(out.end() - data.size(), data.begin(), data.end());
            bn_read_be(&out[0], &m_val);
            return;
        }
    }


    inline bool operator==(const uint256_t &v) {
        return bn_is_equal(&m_val, &v.m_val) != 0;
    }

    inline bool operator==(uint64_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) != 0;
    }

    inline bool operator==(int64_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) != 0;
    }

    inline bool operator==(uint32_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) != 0;
    }

    inline bool operator==(int32_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) != 0;
    }

    inline bool operator==(uint16_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) != 0;
    }

    inline bool operator==(int16_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) != 0;
    }

    inline bool operator==(uint8_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) != 0;
    }

    inline bool operator==(int8_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) != 0;
    }

    inline bool operator!=(const uint256_t &v) {
        return bn_is_equal(&m_val, &v.m_val) == 0;
    }

    inline bool operator!=(uint64_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) == 0;
    }

    inline bool operator!=(uint32_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) == 0;
    }

    inline bool operator!=(int32_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) == 0;
    }

    inline bool operator!=(uint16_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) == 0;
    }

    inline bool operator!=(int16_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) == 0;
    }

    inline bool operator!=(uint8_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) == 0;
    }

    inline bool operator!=(int8_t v) {
        const uint256_t vn = v;
        return bn_is_equal(&m_val, &vn.m_val) == 0;
    }

    inline bool operator<(bignum256 v) {
        const uint256_t vn = v;
        return bn_is_less(&m_val, &vn.m_val) == 1;
    }

    inline bool operator>(bignum256 v) {
        return !operator<(v);
    }

    inline bool operator>=(const bignum256 &v) {
        return !bn_is_less(&m_val, &v);
    }

    inline uint256_t &operator+(uint64_t v) {
        const uint256_t vn = v;
        bn_add(&m_val, &vn.m_val);
        return *this;
    }

    inline uint256_t &operator+=(uint64_t v) {
        const uint256_t vn = v;
        bn_add(&m_val, &vn.m_val);
        return *this;
    }

    inline uint256_t &operator+(const uint256_t &v) {
        bn_add(&m_val, &v.m_val);
        return *this;
    }

    inline uint256_t &operator+=(const uint256_t &v) {
        bn_add(&m_val, &v.m_val);
        return *this;
    }

    inline uint256_t &operator%=(const uint256_t &v) {
        bn_mod(&m_val, &v.m_val);
        return *this;
    }

    inline uint256_t &operator%(const uint256_t &v) {
        return operator%=(v);
    }

    inline uint256_t &operator-(uint64_t v) {
        const uint256_t vn = v;
        bn_subtract(&m_val, &vn.m_val, &m_val);
        return *this;
    }

    inline uint256_t &operator-=(uint64_t v) {
        const uint256_t vn = v;
        bn_subtract(&m_val, &vn.m_val, &m_val);
        return *this;
    }

    void clear() {
        memset(m_val.val, 0, sizeof(m_val.val));
    }

    std::string str() const {
//        Data d(256);
//        const uint32_t *m = m_val.val; // 9

        char out[256];
        bn_format(&m_val, "", "", 0, 0, true, out, 256);

        return std::string(out);
    }

    friend std::ostream &operator<<(std::ostream & stream, const uint256_t &rhs) {
        stream << rhs.str();
        return stream;
    }

    bignum256& val() {
        return m_val;
    }
};




}

#endif //BIP39_UINT256_T_HPP
