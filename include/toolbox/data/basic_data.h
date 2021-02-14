/*!
 * toolbox.
 * basic_data.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#ifndef TOOLBOX_BASIC_DATA_H
#define TOOLBOX_BASIC_DATA_H

#include "utils.h"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace toolbox {
namespace data {

template<typename T>
class basic_data {
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using reverse_iterator = typename std::vector<T>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;
    using size_type = typename std::vector<T>::size_type;

    using map_func_t = std::function<T(const T&)>;
    template<typename To>
    using map_to_func_t = std::function<To(const T&)>;
    using switch_map_func_t = std::function<std::vector<T>(const std::vector<T>&)>;
    template<typename To>
    using switch_map_to_func_t = std::function<std::vector<To>(const std::vector<T>&)>;
    using predicate_func_t = std::function<bool(const T&)>;
    template<typename From, typename To>
    using reduce_func_t = std::function<To(const std::vector<From>&)>;

    template<typename Out>
    struct converter {
        Out operator()(const basic_data<T>&) {
        }
    };

    template<typename Out>
    struct converter_vec {
        Out operator()(const std::vector<T>&) {
        }
    };

    template<typename To>
    using converter_func = std::function<To(const basic_data<T>&)>;
    template<typename To>
    using converter_vec_func = std::function<To(const std::vector<T>&)>;

    basic_data() = default;
    virtual ~basic_data() = default;
    basic_data(const basic_data& other) = default;
    basic_data(basic_data&& other) noexcept = default;
    explicit basic_data(size_t size)
        : m_data(size) {
    }
    basic_data(std::initializer_list<T> ilist) {
        m_data.assign(ilist.begin(), ilist.end());
    }
    basic_data(std::initializer_list<std::vector<T>> ilist) {
        std::for_each(ilist.begin(), ilist.end(),
                      [this](std::vector<T> v) { m_data.insert(m_data.end(), v.begin(), v.end()); });
    }

    basic_data(std::vector<T> data)
        : m_data(std::move(data)) {
    }
    basic_data(const T* data, size_t len)
        : m_data(std::vector<T>(data, data + len)) {
    }

    basic_data& operator=(basic_data other) noexcept {
        if (*this == other) {
            return *this;
        }
        std::swap(m_data, other.m_data);
        return *this;
    }

    const T& operator[](size_type n) const noexcept {
        return at(n);
    }

    T& operator[](size_type n) noexcept {
        return at(n);
    }

    operator bool() const noexcept {
        return !empty();
    }

    bool operator==(const basic_data& other) const noexcept {
        if (m_data.empty() && other.m_data.empty()) {
            return true;
        }
        if (m_data.size() != other.m_data.size()) {
            return false;
        }
        if ((m_data.at(0) != other.m_data.at(0)) && (m_data.at(size() - 1) != other.m_data.at(other.size() - 1))) {
            return false;
        }

        return m_data == other.m_data;
    }

    bool operator!=(const basic_data& other) const noexcept {
        return !(operator==(other));
    }

    /// \brief Make sure that 'other' have at least size() elements, otherwise UB
    /// is your friend
    /// \param other
    /// \return
    bool operator==(const T* other) const noexcept {
        if (!other || !other[0] || !other[m_data.size() - 1]) {
            return false;
        }

        for (size_type i = 0; i < m_data.size(); i++) {
            if (m_data[i] != other[i]) {
                return false;
            }
        }

        return true;
    }
    bool operator!=(const T* other) const noexcept {
        return !(operator==(other));
    }

    const std::vector<T>& get() const {
        return m_data;
    }
    std::vector<T>& get() {
        return m_data;
    }

    const T& at(size_type n) const {
        return m_data.at(n);
    }

    T& at(size_type n) {
        return m_data.at(n);
    }

    T* data() {
        return m_data.data();
    }

    const T* data() const {
        return m_data.data();
    }

    const T* cdata() const {
        return m_data.data();
    }

    size_type size() const {
        return m_data.size();
    }

    constexpr size_type size_data() const {
        return sizeof(T) * size();
    }

    void resize(size_type sz) {
        m_data.resize(sz);
    }

    bool empty() const {
        return m_data.empty();
    }

    void reserve(size_type n) {
        m_data.reserve(n);
    }

    size_type capacity() const {
        return m_data.capacity();
    }

    void clear() {
        m_data.clear();
    }

    virtual basic_data<T>& map(const map_func_t& mapper) {
        std::transform(begin(), end(), begin(), std::bind(mapper, std::placeholders::_1));
        return *this;
    }
    virtual basic_data<T> map_c(const map_func_t& mapper) const {
        basic_data<T> out = m_data;
        out.map(mapper);
        return out;
    }

    template<typename To>
    basic_data<To> map_to(const map_to_func_t<To>& mapper) const {
        basic_data<To> out(m_data.size());
        size_t i = 0;
        for (const auto& t : m_data) {
            out[i++] = mapper(t);
        }
        return out;
    }

    virtual basic_data<T>& switch_map(const switch_map_func_t& mapper) {
        auto res = mapper(m_data);
        m_data.clear();
        m_data = std::move(res);
        return *this;
    }

    virtual basic_data<T> switch_map_c(const switch_map_func_t& mapper) const {
        basic_data<T> out = m_data;
        out.switch_map(mapper);
        return out;
    }

    template<typename To>
    basic_data<To> switch_map_to(const switch_map_to_func_t<To>& mapper) const {
        basic_data<To> out = mapper(m_data);
        return out;
    }

    template<typename To>
    To reduce(const reduce_func_t<T, To>& reduce_func) const {
        return reduce_func(get());
    }

    template<typename To>
    To convert(converter<To> conv) {
        return conv(*this);
    }

    template<typename To>
    To convert_v(converter_vec<To> conv) {
        return conv(get());
    }

    template<typename To>
    To convert(converter_func<To> conv) {
        return conv(*this);
    }

    template<typename To>
    To convert_v(converter_vec_func<To> conv) {
        return conv(get());
    }

    virtual basic_data<T>& filter(const predicate_func_t& filter) {
        switch_map([&filter](std::vector<T> source) {
            std::vector<T> out;
            std::for_each(source.begin(), source.end(), [&filter, &out](T& val) {
                if (filter(val)) {
                    out.push_back(val);
                }
            });
            return out;
        });
        return *this;
    }
    virtual basic_data<T> filter_c(const predicate_func_t& filter) const {
        basic_data<T> d = m_data;
        d.filter(filter);
        return d;
    }

    std::vector<T> take_first(size_t n) const {
        std::vector<T> out;
        out.insert(out.begin(), m_data.begin(), m_data.begin() + n);
        return out;
    }
    basic_data<T> take_first_c(size_t n) const {
        return basic_data<T>(take_first(n));
    }
    virtual basic_data<T>& take_first_m(size_t n) {
        auto cp = take_first(n);
        clear();
        m_data = std::move(cp);
        return *this;
    }

    std::vector<T> take_last(size_t n) const {
        std::vector<T> out;
        out.insert(out.begin(), m_data.end() - n, m_data.end());
        return out;
    }
    basic_data<T> take_last_c(size_t n) const {
        return basic_data<T>(take_last(n));
    }
    virtual basic_data<T>& take_last_m(size_t n) {
        auto cp = take_last(n);
        clear();
        m_data = std::move(cp);
        return *this;
    }

    std::vector<T> take_range_from(size_t from) const {
        return take_range(from, size());
    }
    basic_data<T> take_range_from_c(size_t from) const {
        return basic_data<T>(take_range_from(from));
    }
    virtual basic_data<T>& take_range_from_m(size_t from) {
        auto cp = take_range_from(from);
        clear();
        m_data = std::move(cp);
        return *this;
    }

    std::vector<T> take_range_to(size_t to) const {
        return take_range(0, to);
    }
    basic_data<T> take_range_to_c(size_t to) const {
        return basic_data<T>(take_range_to(to));
    }
    virtual basic_data<T>& take_range_to_m(size_t to) {
        auto cp = take_range_to(to);
        clear();
        m_data = std::move(cp);
        return *this;
    }

    std::vector<T> take_range(size_t from, size_t to) const {
        if (from > to) {
            throw std::out_of_range("'from' can't be more than 'to'");
        } else if (to > size()) {
            throw std::out_of_range("'to' can't be more than size()");
        } else if (from > size()) {
            throw std::out_of_range("'from' can't be more than size()");
        }

        std::vector<T> out;
        out.reserve(to - from);
        out.insert(out.begin(), m_data.begin() + from, m_data.begin() + to);
        return out;
    }
    basic_data<T> take_range_c(size_t from, size_t to) const {
        return basic_data<T>(take_range(from, to));
    }
    virtual basic_data<T>& take_range_m(size_t from, size_t to) {
        auto cp = take_range(from, to);
        clear();
        m_data = cp;
        return *this;
    }

    const_reverse_iterator crbegin() const {
        return m_data.crbegin();
    }
    const_reverse_iterator crend() const {
        return m_data.crend();
    }
    reverse_iterator rbegin() {
        return m_data.rbegin();
    }
    reverse_iterator rend() {
        return m_data.rend();
    }
    const_iterator cbegin() const {
        return m_data.cbegin();
    }
    const_iterator begin() const {
        return m_data.begin();
    }
    iterator begin() {
        return m_data.begin();
    }
    const_iterator cend() const {
        return m_data.cend();
    }
    const_iterator end() const {
        return m_data.end();
    }
    iterator end() {
        return m_data.end();
    }

    virtual void push_back(const std::vector<T>& data) {
        m_data.insert(m_data.end(), data.begin(), data.end());
    }

    virtual void push_back(const T* data, size_t len) {
        size_t alloc_size = sizeof(T) * len;
        m_data.resize(size() + alloc_size);
        memcpy(m_data.data() + size() - alloc_size, data, alloc_size);
    }

    virtual void push_back(const_iterator start, const_iterator end) {
        m_data.insert(m_data.end(), start, end);
    }
    virtual void push_back(iterator start, iterator end) {
        m_data.insert(m_data.end(), start, end);
    }

    virtual void push_back(const basic_data<T>& data) {
        m_data.insert(m_data.end(), data.begin(), data.end());
    }

    virtual void push_back(basic_data&& data) {
        m_data.push_back(std::move(data));
    }

    /// \brief batch reduces allocations
    /// \param vals map of positions -> values
    /// \example write_bach(std::map<size_t, std::vector<uint8_t>> {
    ///     {0, 0x00},
    ///     {1, 0x01},
    ///     {3, 0x03},
    ///     {2, 0x02},
    /// })
    /// If some position will not set, it will be initialized by default, so don't forget to set all positions before using
    /// \return written values count
    virtual size_type write_batch(std::map<size_type, T>&& vals) {
        if (vals.empty()) {
            return 0;
        }

        size_type minPos = SIZE_MAX;
        size_type maxPos = 0;
        for (const auto& it : vals) {
            minPos = std::min(minPos, it.first);
            maxPos = std::max(maxPos, it.first);
        }

        if (maxPos >= size()) {
            size_type alloc;
            if (size() != 0) {
                alloc = minPos + vals.size() - size();
                alloc += size();
            } else {
                alloc = vals.size();
            }
            m_data.resize(alloc);
        }

        size_type n = 0;
        for (auto&& el : vals) {
            m_data[el.first] = std::move(el.second);
            n++;
        }
        vals.clear();

        return n;
    }

    template<typename _InputIterator>
    size_type write(iterator position, _InputIterator start, _InputIterator end) {
        size_type dist = std::distance(begin(), position);
        return write(dist, start, end);
    }

    template<typename _InputIterator>
    size_type write(size_type pos, _InputIterator start, _InputIterator end) {
        std::map<size_type, T> tmp;
        size_type i = pos;
        for (auto it = start; it != end; ++it) {
            tmp[i++] = *it;
        }

        return write_batch(std::move(tmp));
    }

    virtual size_type write(size_type pos, const T* data, size_t length) {
        size_t len = sizeof(T) * length;
        if (pos + len >= size()) {
            size_t alloc = size() + (pos + len - size());
            m_data.resize(alloc);
        }

        memcpy(m_data.data() + pos, data, len);
        return length;
    }

    /// \brief Write method overwrite existing data, and if input data.size() >
    /// buffer.size(), it resize current buffer
    /// \param pos
    /// \param data
    /// \return
    virtual size_type write(size_type pos, const basic_data<T>& data) {
        return write(pos, data.get());
    }
    virtual size_type write(size_type pos, basic_data<T>&& data) {
        std::vector<T> tmp;
        tmp.swap(data.get());
        data.clear();
        return write(pos, tmp);
    }
    virtual size_type write(size_type pos, const std::vector<T>& data) {
        return write(pos, data.begin(), data.end());
    }
    virtual size_type write(size_type pos, std::vector<T>&& data) {
        return write(pos, data.begin(), data.end());
    }
    virtual size_type write(size_type pos, T val) {
        if (pos >= size()) {
            size_type alloc;
            if (size() != 0) {
                alloc = size() + 1;
            } else {
                alloc = 1;
            }
            m_data.resize(alloc);
        }
        m_data[pos] = val;
        return 1;
    }

    virtual size_t write_back(T val) {
        return write(size(), val);
    }

    virtual size_type write_back(const std::vector<T>& data) {
        return write(size(), data);
    }
    virtual size_type write_back(std::vector<T>&& data) {
        return write(size(), data);
    }
    virtual size_type write_back(const basic_data<T>& data) {
        return write(size(), data);
    }
    virtual size_type write_back(basic_data<T>&& data) {
        return write(size(), std::move(data));
    }
    virtual size_type write_back(const T* data, size_t len) {
        return write(size(), data, len);
    }

    virtual size_type write_tail(size_type pos, const T* data, size_t dataLen) {
        std::vector<T> tmp(data, data + dataLen);
        return write_tail(pos, tmp);
    }
    virtual size_type write_tail(size_type pos, const basic_data<T>& data) {
        return write_tail(pos, data.get());
    }
    virtual size_type write_tail(size_type pos, const std::vector<T>& data) {
        std::vector<T> tmp(m_data.begin(), m_data.begin() + pos);
        tmp.insert(tmp.begin() + pos, data.begin(), data.end());
        clear();
        m_data = std::move(tmp);
        return data.size();
    }

protected:
    std::vector<T> m_data;
};

} // namespace data
} // namespace toolbox

#endif // TOOLBOX_BASIC_DATA_H
