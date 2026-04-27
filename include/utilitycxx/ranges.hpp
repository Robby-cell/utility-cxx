#ifndef UTILITYCXX_RANGES_HPP_
#define UTILITYCXX_RANGES_HPP_ 1

#include <utilitycxx/internal/macros.hpp>
#include <utilitycxx/internal/version.hpp>

#include <cstddef>
#include <iterator>

namespace utilitycxx {

namespace ranges {
struct sentinel_iterator {
    friend constexpr bool operator==(sentinel_iterator /* first */,
                                     sentinel_iterator /* last */) noexcept {
        return true;
    }

    friend constexpr bool operator!=(sentinel_iterator /* first */,
                                     sentinel_iterator /* last */) noexcept {
        return false;
    }

    UTILITYCXX_CONSTEXPR14 sentinel_iterator& operator++() noexcept {
        return *this;
    }

    UTILITYCXX_CONSTEXPR14 sentinel_iterator operator++(int) noexcept {
        return *this;
    }

    UTILITYCXX_CONSTEXPR14 sentinel_iterator& operator--() noexcept {
        return *this;
    }

    UTILITYCXX_CONSTEXPR14 sentinel_iterator operator--(int) noexcept {
        return *this;
    }
};

struct empty_range {
    constexpr sentinel_iterator begin() const noexcept {
        return sentinel_iterator{};
    }

    constexpr sentinel_iterator end() const noexcept {
        return sentinel_iterator{};
    }

    constexpr size_t size() const noexcept {
        return 0;
    }

    constexpr bool empty() const noexcept {
        return true;
    }

    constexpr std::nullptr_t data() const noexcept {
        return nullptr;
    }
};

namespace detail {
using std::begin;
using std::end;

template <typename T>
constexpr auto adl_begin(T&& t) -> decltype(begin(std::forward<T>(t))) {
    return begin(std::forward<T>(t));
}

template <typename T>
constexpr auto adl_end(T&& t) -> decltype(end(std::forward<T>(t))) {
    return end(std::forward<T>(t));
}
} // namespace detail

template <typename T>
constexpr auto begin(T&& t) -> decltype(detail::adl_begin(std::forward<T>(t))) {
    return detail::adl_begin(std::forward<T>(t));
}

template <typename T>
constexpr auto end(T&& t) -> decltype(detail::adl_end(std::forward<T>(t))) {
    return detail::adl_end(std::forward<T>(t));
}

template <typename T>
constexpr auto cbegin(const T& t) -> decltype(detail::adl_begin(t)) {
    return detail::adl_begin(t);
}

template <typename T>
constexpr auto cend(const T& t) -> decltype(detail::adl_end(t)) {
    return detail::adl_end(t);
}

template <typename T> constexpr auto size(const T& t) -> decltype(t.size()) {
    return t.size();
}

template <typename T, std::size_t N>
constexpr std::size_t size(const T (& /* array */)[N]) noexcept {
    return N;
}

template <typename T> constexpr auto empty(const T& t) -> decltype(t.empty()) {
    return t.empty();
}

template <typename T, std::size_t N>
constexpr bool empty(const T (& /* array */)[N]) noexcept {
    return false;
}

template <typename T> constexpr auto data(T& t) -> decltype(t.data()) {
    return t.data();
}

template <typename T> constexpr auto data(const T& t) -> decltype(t.data()) {
    return t.data();
}

template <typename T, std::size_t N> constexpr T* data(T (&array)[N]) noexcept {
    return array;
}

} // namespace ranges
} // namespace utilitycxx

#endif // UTILITYCXX_RANGES_HPP_
