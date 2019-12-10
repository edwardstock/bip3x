/*! 
 * bip39. 2018
 *
 * Copyright (c) 2018 Arvid Gerstmann.
 * This code is licensed under MIT license.
 *
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#ifndef BIP39_PCGRandRAND_H
#define BIP39_PCGRandRAND_H

#include <functional>
#include <random>

class PCGRand {
public:
    using result_type = uint32_t;
    static constexpr result_type(min)() {
        return 0;
    }
    static constexpr result_type(max)() {
        return UINT32_MAX;
    }
    //    friend bool operator==(PCGRand const &, PCGRand const &);
    //    friend bool operator!=(PCGRand const &, PCGRand const &);

    PCGRand()
        : m_state(0x853c49e6748fea9bULL), m_inc(0xda3e39cb94b95bdbULL) {
    }
    explicit PCGRand(std::random_device& rd) {
        seed(rd);
    }

    void seed(std::random_device& rd) {
        uint64_t s0 = uint64_t(rd()) << 31u | uint64_t(rd());
        uint64_t s1 = uint64_t(rd()) << 31u | uint64_t(rd());

        m_state = 0;
        m_inc = (s1 << 1u) | 1u;
        (void) operator()();
        m_state += s0;
        (void) operator()();
    }

    result_type operator()() {
        uint64_t oldstate = m_state;
        m_state = oldstate * 6364136223846793005ULL + m_inc;
        auto xorshifted = uint32_t(((oldstate >> 18u) ^ oldstate) >> 27u);
        uint64_t rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31u));
    }

    void discard(unsigned long long n) {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

private:
    uint64_t m_state;
    uint64_t m_inc;
};

//bool operator==(PCGRand const &lhs, PCGRand const &rhs) {
//    return lhs.m_state == rhs.m_state
//        && lhs.m_inc == rhs.m_inc;
//}
//bool operator!=(PCGRand const &lhs, PCGRand const &rhs) {
//    return lhs.m_state != rhs.m_state
//        || lhs.m_inc != rhs.m_inc;
//}

#endif //BIP39_PCGRandRAND_H
