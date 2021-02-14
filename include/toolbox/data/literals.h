//
// Created by Edward on 30.11.2019.
//

#ifndef TOOLBOXPP_LITERALS_H
#define TOOLBOXPP_LITERALS_H

#include <cstdint>

namespace toolbox {
namespace data {
namespace literals {

#if defined(__GNUC__) && (__GNUC__ >= 5)
inline uint8_t operator""_byte(unsigned long long val) {
    return (uint8_t) val;
}
inline uint8_t operator""_bytes(unsigned long long val) {
    return (uint8_t) val;
}
inline uint16_t operator""_dbyte(unsigned long long val) {
    return (uint16_t) val;
}
inline uint16_t operator""_dbytes(unsigned long long val) {
    return (uint16_t) val;
}

#endif

} // namespace literals
} // namespace data
} // namespace toolbox

#endif // TOOLBOXPP_LITERALS_H
