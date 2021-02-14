/*!
 * toolbox.
 * fixed_bytes_data.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#ifndef TOOLBOX_BYTES_ARRAY_H
#define TOOLBOX_BYTES_ARRAY_H

#include "bytes_data.h"

namespace toolbox {
namespace data {

template<size_t N>
class bytes_array;

/// \brief Unresizable bytes buffer. If you're trying to write data more than container N, otherwise data will be silently striped
/// \tparam N container maximum size
template<size_t N>
class bytes_array : public bytes_data {
public:
    /// \brief Default: resize data to N forever
    bytes_array()
        : bytes_data(N) {
    }

    /// \brief Write uint8_t* with len
    /// \param data bytes data, have check for NULL
    /// \param len if len > N, than container will copy only from 0 to N data
    bytes_array(const uint8_t* data, size_t len)
        : bytes_array() {

        if (data != nullptr) {
            memcpy(m_data.data(), data, std::min(N, len));
        }
    }

    /// \brief Be carefully and make sure input data contains >= N elements, otherwise you'll have at least garbage in a data or UB
    /// \param data
    explicit bytes_array(const uint8_t* data)
        : bytes_array(data, N) {
    }

    /// \brief converts hex string to bytes.
    /// \param hex String CAN'T have prefixes or odd length, as hex representation - 2 characters is a 1 byte
    bytes_array(const std::string& hex)
        : bytes_array() {
        if (hex.length() % 2 == 0 && hex.length() <= N * 2) {
            auto tmp = toolbox::data::hex_to_bytes(hex);

            std::copy(tmp.begin(), tmp.begin() + std::min(N, tmp.size()), m_data.begin());
            tmp.clear();
        }
    }

    /// \brief converts hex string to bytes.
    /// \param hex
    bytes_array(const char* hex)
        : bytes_array(std::string(hex)) {
    }

    bytes_array(const std::vector<uint8_t>& data)
        : bytes_array() {
        size_t readLen = std::min(data.size(), N);
        std::copy(data.begin(), data.begin() + readLen, begin());
    }

    bytes_array(std::vector<uint8_t>&& other)
        : bytes_array() {

        if (other.size() == N) {
            m_data = std::move(other);
            return;
        }

        // copy as much, as possible
        std::copy(other.begin(), other.end(), begin());
        // then if left some gap between old this size and new data, fill it with zeroes
        std::fill(begin() + other.size(), end(), 0);
        other.clear();
    }

    bytes_array(const bytes_array<N>& other)
        : bytes_array() {
        clear();
        m_data = other.m_data;
    }

    bytes_array(const basic_data<uint8_t>& other)
        : bytes_array() {
        clear();
        if (other.size() == N) {
            m_data = other.get();
            return;
        }

        // copy as much, as possible
        std::copy(other.begin(), other.begin() + std::min(N, other.size()), begin());
        // then if left some gap between old this size and new data, fill it with zeroes
        std::fill(begin() + std::min(N, other.size()), end(), 0);
    }

    bytes_array(basic_data<uint8_t>&& other)
        : bytes_array() {
        clear();
        if (other.size() == N) {
            m_data = std::move(other.get());
            return;
        }

        // copy as much, as possible
        std::copy(other.begin(), other.begin() + std::min(N, other.size()), begin());
        // then if left some gap between old this size and new data, fill it with zeroes
        std::fill(begin() + std::min(N, other.size()), end(), 0);

        other.clear();
        other.resize(0);
    }

    bytes_array(bytes_array<N>&& other) noexcept {
        m_data = std::move(other.m_data);
        other.clear();
    }

    bytes_array<N>& operator=(bytes_array<N> other) {
        std::swap(m_data, other.m_data);
        return *this;
    }

    bytes_array<N>& operator=(std::vector<uint8_t> other) {
        if (other.size() == N) {
            std::swap(m_data, other);
            return *this;
        }

        // copy as much, as possible
        std::copy(other.begin(), other.begin() + std::min(N, other.size()), begin());
        // then if left some gap between old this size and new data, fill it with zeroes
        std::fill(begin() + std::min(N, other.size()), end(), 0);
        other.clear();
        other.resize(0);

        return *this;
    }

    ~bytes_array() override = default;

    // leaving it here for compatibility
    void reserve(size_t) {
    }

    size_t capacity() const {
        return N;
    }

    // leaving it here for compatibility
    void resize(size_t) {
        // do nothing
        // client code still can resize m_data in derived class
    }

    constexpr size_t size() const {
        return N;
    }

    /// \brief writes only those elements that have position < N
    /// \param vals
    /// \return
    size_type write_batch(std::map<basic_data::size_type, uint8_t>&& vals) override {
        if (vals.empty()) {
            return 0;
        }

        size_type n = 0;
        for (auto&& el : vals) {
            if (el.first < N) {
                m_data[el.first] = el.second;
            }
            n++;
        }
        vals.clear();

        return n;
    }

    size_t write(size_t pos, uint16_t val) override {
        if (sizeof(val) + pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        return bytes_data::write(pos, val);
    }
    size_t write(size_t pos, int32_t val) override {
        if (sizeof(val) + pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        return bytes_data::write(pos, val);
    }
    size_t write(size_t pos, uint32_t val) override {
        if (sizeof(val) + pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        return bytes_data::write(pos, val);
    }
    size_t write(size_t pos, uint64_t val) override {
        if (pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        if (sizeof(val) + pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        return bytes_data::write(pos, val);
    }
    size_type write(basic_data::size_type pos, const uint8_t* data, size_t dataLen) override {
        if (pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        return bytes_data::write(pos, data, std::min(N - pos, dataLen));
    }
    size_type write(basic_data::size_type pos, const basic_data<uint8_t>& data) override {
        if (pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        return bytes_data::write(pos, data.take_range(0, std::min(N - pos, data.size())));
    }
    size_type write(basic_data::size_type pos, basic_data<uint8_t>&& data) override {
        if (pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        return bytes_data::write(pos, data.take_range(0, std::min(N - pos, data.size())));
    }
    size_type write(basic_data::size_type pos, const std::vector<uint8_t>& data) override {
        if (pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        return bytes_data::write(pos, data.data(), std::min(N - pos, data.size()));
    }
    size_type write(basic_data::size_type pos, std::vector<uint8_t>&& data) override {
        if (pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        return bytes_data::write(pos, data.data(), std::min(N - pos, data.size()));
    }
    size_type write(basic_data::size_type pos, uint8_t val) override {
        if (sizeof(val) + pos > N) {
            throw std::out_of_range("value cannot be fit to passed position");
        }
        return bytes_data::write(pos, val);
    }

private:
    basic_data<uint8_t>& take_first_m(size_t n) override {
        return basic_data::take_first_m(n);
    }
    basic_data<uint8_t>& take_last_m(size_t n) override {
        return basic_data::take_last_m(n);
    }
    basic_data<uint8_t>& take_range_from_m(size_t from) override {
        return basic_data::take_range_from_m(from);
    }
    basic_data<uint8_t>& take_range_to_m(size_t to) override {
        return basic_data::take_range_to_m(to);
    }
    basic_data<uint8_t>& take_range_m(size_t from, size_t to) override {
        return basic_data::take_range_m(from, to);
    }

    basic_data<uint8_t>& filter(const predicate_func_t& filter) override {
        return basic_data::filter(filter);
    }
    void push_back(const std::vector<uint8_t>& data) override {
        basic_data::push_back(data);
    }
    void push_back(const uint8_t* data, size_t len) override {
        basic_data::push_back(data, len);
    }
    void push_back(const basic_data<uint8_t>& data) override {
        basic_data::push_back(data);
    }
    size_t write_back(uint8_t val) override {
        return basic_data::write_back(val);
    }
    size_type write_back(const std::vector<uint8_t>& data) override {
        return basic_data::write_back(data);
    }
    size_type write_back(std::vector<uint8_t>&& data) override {
        return basic_data::write_back(data);
    }
    size_type write_back(const basic_data<uint8_t>& data) override {
        return basic_data::write_back(data);
    }
    size_type write_back(basic_data<uint8_t>&& data) override {
        return basic_data::write_back(data);
    }
    size_type write_back(const uint8_t* data, size_t len) override {
        return basic_data::write_back(data, len);
    }
    void push_back(const_iterator start, const_iterator end) override {
        bytes_data::push_back(start, end);
    }
    void push_back(iterator start, iterator end) override {
        bytes_data::push_back(start, end);
    }
    void push_back(basic_data&& data) override {
        bytes_data::push_back(data);
    }
    size_type write_tail(size_type pos, const uint8_t* data, size_t len) override {
        return basic_data::write_tail(pos, data, len);
    }
    size_type write_tail(size_type pos, const basic_data<uint8_t>& data) override {
        return basic_data::write_tail(pos, data);
    }
    size_type write_tail(size_type pos, const std::vector<uint8_t>& data) override {
        return basic_data::write_tail(pos, data);
    }
};

} // namespace data
} // namespace toolbox

#endif //TOOLBOX_BYTES_ARRAY_H
