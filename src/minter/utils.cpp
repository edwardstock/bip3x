/**
 * bip39. 2018
 * utils.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#include <limits>
#include "utils.h"

std::vector<uint8_t> minter::hexToBytes(const std::string &hex) {
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        auto byte = (uint8_t) strtol(byteString.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

std::string minter::bytesToHex(const uint8_t *data, size_t len) {
    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < len; ++i) {
        ss << std::setw(2) << std::setfill('0') << (int) data[i];
    }

    return ss.str();
}

std::vector<std::string> minter::splitString(const std::string &source, const std::string &delimiter) {
    if (delimiter.empty()) {
        return std::vector<std::string>(0);
    }

    std::string src = source;

    std::vector<std::string> result;
    size_t current = 0;
    while (current != std::string::npos) {
        current = src.find(delimiter);
        if (current != std::string::npos && src.substr(current, delimiter.length()) == delimiter) {
            result.push_back(src.substr(0, current));
            src = src.substr(current + (delimiter.length()), std::string::npos);
        }
    }
    if (src.length() > 0) {
        result.push_back(src);
    }

    return result;
}
const std::string minter::glueStrings(const std::vector<std::string> &input, const std::string &g) {
    std::stringstream ss;
    for (int i = 0; i < input.size(); i++) {
        ss << input[i];
        if (i + 1 < input.size()) {
            ss << g;
        }
    }

    return ss.str();
}

uint32_t minter::stou(std::string const &str, size_t *idx, int base) {
    unsigned long result = std::stoul(str, idx, base);
    if (result > std::numeric_limits<uint32_t>::max()) {
        throw std::out_of_range("stou");
    }

    return static_cast<uint32_t>(result);
}
 