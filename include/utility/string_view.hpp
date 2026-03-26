#ifndef UTILITY_STRING_VIEW_HPP_
#define UTILITY_STRING_VIEW_HPP_ 1

#include <utility/internal/macros.hpp>
#include <utility/internal/version.hpp>
#include <utility/utility.hpp>

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>

#if UTILITY_HAS_CPP17
#include <string_view>
#endif

namespace utility {

namespace detail {
template <class S, class = void>
struct is_string_like_helper : std::false_type {};

template <class S>
struct is_string_like_helper<
    S, void_t<decltype((std::declval<S>().data(), std::declval<S>().size()))>>
    : std::true_type {};

template <class S> struct is_string_like : is_string_like_helper<S> {};
} // namespace detail

template <class Char, class CharTraits = std::char_traits<Char>>
class basic_string_view {
  public:
    using char_type = Char;
    using value_type = Char;
    using traits_type = CharTraits;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using pointer = char_type*;
    using const_pointer = const char_type*;
    using reference = char_type&;
    using const_reference = const char_type&;

    using const_iterator = const_pointer;
    using iterator = const_iterator;

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator = const_reverse_iterator;

    enum : size_type { npos = static_cast<size_type>(-1) };

  public:
    basic_string_view() = default;

    template <class String,
              typename std::enable_if<detail::is_string_like<String>::value,
                                      int>::type = 0>
    basic_string_view(const String& s)
        : basic_string_view(s.data(), s.size()) {}

    template <size_type N>
    constexpr basic_string_view(const char_type (&s)[N])
        : basic_string_view(s, N - 1) {}

    basic_string_view(const char_type* str)
        : basic_string_view(str, traits_type::length(str)) {}

    constexpr basic_string_view(const char_type* str, size_type n)
        : m_Data(str), m_Size(n) {}

    constexpr size_type size() const noexcept {
        return m_Size;
    }

    constexpr const_pointer data() const noexcept {
        return m_Data;
    }

    constexpr const_iterator begin() const noexcept {
        return const_iterator(data());
    }

    constexpr const_iterator end() const noexcept {
        return const_iterator(data() + size());
    }

    constexpr const_iterator cbegin() const noexcept {
        return begin();
    }

    constexpr const_iterator cend() const noexcept {
        return end();
    }

    constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    constexpr const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    constexpr const_reverse_iterator crend() const noexcept {
        return rend();
    }

    constexpr const_reference operator[](size_type pos) const noexcept {
        return data()[pos];
    }

    UTILITY_CONSTEXPR20 const_reference at(size_type pos) const {
        if (pos >= size()) {
            throw std::out_of_range("index out of range");
        }
        return operator[](pos);
    }

    UTILITY_CONSTEXPR20 bool
    starts_with(basic_string_view prefix) const noexcept {
        if (prefix.size() > size()) {
            return false;
        }
        return traits_type::compare(data(), prefix.data(), prefix.size()) == 0;
    }

    UTILITY_CONSTEXPR20
    bool ends_with(basic_string_view suffix) const noexcept {
        if (suffix.size() > size()) {
            return false;
        }
        return traits_type::compare(data() + size() - suffix.size(),
                                    suffix.data(), suffix.size()) == 0;
    }

    UTILITY_CONSTEXPR20 basic_string_view substr(size_type pos,
                                                 size_type count = npos) const {
        if (pos > size()) {
            throw std::out_of_range("index out of range");
        }
        count = (std::min)(count, size() - pos);
        return basic_string_view(data() + pos, count);
    }

    constexpr bool empty() const noexcept {
        return size() == 0;
    }

    constexpr size_type max_size() const noexcept {
        return npos;
    }

    UTILITY_CONSTEXPR20 void remove_prefix(size_type n) {
        if (n > size()) {
            throw std::out_of_range("index out of range");
        }
        m_Data += n;
        m_Size -= n;
    }

    UTILITY_CONSTEXPR20 void remove_suffix(size_type n) {
        if (n > size()) {
            throw std::out_of_range("index out of range");
        }
        m_Size -= n;
    }

    UTILITY_CONSTEXPR14 void swap(basic_string_view& that) noexcept {
        using std::swap;
        if (this != std::addressof(that)) {
            swap(m_Data, that.m_Data);
            swap(m_Size, that.m_Size);
        }
    }

    UTILITY_CONSTEXPR14 void swap(basic_string_view&& that) noexcept {
        this->swap(that);
    }

    UTILITY_CONSTEXPR20 size_type find(basic_string_view sub,
                                       size_type pos = 0) const {
        if (pos > size()) {
            throw std::out_of_range("index out of range");
        }
        if (sub.empty()) {
            return pos;
        }
        if (sub.size() > size() - pos) {
            return npos;
        }
        for (size_type i = pos; i <= size() - sub.size(); ++i) {
            if (traits_type::compare(data() + i, sub.data(), sub.size()) == 0) {
                return i;
            }
        }
        return npos;
    }

    template <class StringCharTraits, class Allocator>
    explicit
    operator std::basic_string<char_type, StringCharTraits, Allocator>() const {
        return std::basic_string<char_type, StringCharTraits, Allocator>(
            data(), size());
    }

#if UTILITY_HAS_CPP17
    template <class StringViewCharTraits>
    operator std::basic_string_view<char_type, StringViewCharTraits>()
        const noexcept {
        return std::basic_string_view<char_type, StringViewCharTraits>(data(),
                                                                       size());
    }
#endif

    UTILITY_CONSTEXPR20 int compare(basic_string_view other) const noexcept {
        return traits_type::compare(data(), other.data(),
                                    (std::min)(size(), other.size()));
    }

    friend UTILITY_CONSTEXPR20 bool
    operator==(basic_string_view self, basic_string_view other) noexcept {
        return (self.size() == other.size()) && (self.compare(other) == 0);
    }

    friend UTILITY_CONSTEXPR20 bool
    operator!=(basic_string_view self, basic_string_view other) noexcept {
        return !(self == other);
    }

    friend UTILITY_CONSTEXPR20 bool
    operator<(basic_string_view self, basic_string_view other) noexcept {
        return self.compare(other) < 0;
    }

    friend UTILITY_CONSTEXPR20 bool
    operator<=(basic_string_view self, basic_string_view other) noexcept {
        return self.compare(other) <= 0;
    }

    friend UTILITY_CONSTEXPR20 bool
    operator>(basic_string_view self, basic_string_view other) noexcept {
        return self.compare(other) > 0;
    }

    friend UTILITY_CONSTEXPR20 bool
    operator>=(basic_string_view self, basic_string_view other) noexcept {
        return self.compare(other) >= 0;
    }

  private:
    const char_type* m_Data = nullptr;
    size_type m_Size = 0;
};

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;
#if UTILITY_HAS_CHAR8_T
using u8string_view = basic_string_view<char8_t>;
#endif
} // namespace utility

namespace utility {
namespace string_view_literals {
constexpr string_view operator""_sv(const char* str, std::size_t n) noexcept {
    return string_view(str, n);
}
constexpr wstring_view operator""_sv(const wchar_t* str,
                                     std::size_t n) noexcept {
    return wstring_view(str, n);
}
constexpr u16string_view operator""_sv(const char16_t* str,
                                       std::size_t n) noexcept {
    return u16string_view(str, n);
}
constexpr u32string_view operator""_sv(const char32_t* str,
                                       std::size_t n) noexcept {
    return u32string_view(str, n);
}
#if UTILITY_HAS_CHAR8_T
constexpr u8string_view operator""_sv(const char8_t* str,
                                      std::size_t n) noexcept {
    return u8string_view(str, n);
}
#endif
} // namespace string_view_literals
} // namespace utility

#endif // UTILITY_STRING_VIEW_HPP_
