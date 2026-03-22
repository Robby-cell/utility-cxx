#include "utility.hpp"
#include <cstdint>
#include <cstring>
#ifndef UTILITY_STRING_VIEW_HPP_
#define UTILITY_STRING_VIEW_HPP_ 1

#include <utility/internal/macros.hpp>
#include <utility/internal/version.hpp>

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>

// Forward declaration of char traits
namespace std {
template <class Char> struct char_traits;
template <class T> struct hash;
} // namespace std

namespace utility {

namespace detail {
template <class S, class = void>
struct is_string_like_helper : std::false_type {};

template <class S>
struct is_string_like_helper<
    S, void_t<decltype((std::declval<S>().data(), std::declval<S>().size()))>>
    : std::true_type {};

template <class S>
struct is_string_like
    : std::integral_constant<bool, is_string_like_helper<S>::value> {};
} // namespace detail

template <class Char, class CharTraits = std::char_traits<Char>>
class basic_string_view {
public:
  using char_type = Char;
  using value_type = Char;
  using traits_type = CharTraits;

  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  using pointer = char_type *;
  using const_pointer = const char_type *;
  using reference = char_type &;
  using const_reference = const char_type &;

  using const_iterator = const_pointer;
  using iterator = const_iterator;

  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;

  static constexpr size_type npos = static_cast<size_type>(-1);

public:
  basic_string_view() = default;

  template <class String,
            typename std::enable_if<detail::is_string_like<String>::value,
                                    int>::type = 0>
  basic_string_view(const String &s) : basic_string_view(s.data(), s.size()) {}

  template <size_type N>
  constexpr basic_string_view(const char_type (&s)[N])
      : basic_string_view(s, N - 1) {}

  basic_string_view(const char_type *str)
      : basic_string_view(str, traits_type::length(str)) {}

  constexpr basic_string_view(const char_type *str, size_type n)
      : m_Data(str), m_Size(n) {}

  constexpr size_type size() const noexcept { return m_Size; }

  constexpr const_pointer data() const noexcept { return m_Data; }

  constexpr const_iterator begin() const noexcept {
    return const_iterator(data());
  }

  constexpr const_iterator end() const noexcept {
    return const_iterator(data() + size());
  }

  constexpr const_iterator cbegin() const noexcept { return begin(); }

  constexpr const_iterator cend() const noexcept { return end(); }

  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }

  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }

  constexpr const_reverse_iterator crend() const noexcept { return rend(); }

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
    return traits_type::compare(data() + size() - suffix.size(), suffix.data(),
                                suffix.size()) == 0;
  }

  UTILITY_CONSTEXPR20 basic_string_view substr(size_type pos,
                                               size_type count = npos) const {
    if (pos > size()) {
      throw std::out_of_range("index out of range");
    }
    count = (std::min)(count, size() - pos);
    return basic_string_view(data() + pos, count);
  }

  constexpr bool empty() const noexcept { return size() == 0; }

  constexpr size_type max_size() const noexcept { return npos; }

  UTILITY_CONSTEXPR20 void remove_prefix(size_type n) {
    if (n > size()) {
      throw std::out_of_range("index out of range");
    }
    m_Data += n;
    m_Size -= n;
  }

  UTILITY_CONSTEXPR20 void remove_suffix(size_type n) noexcept {
    if (n > size()) {
      throw std::out_of_range("index out of range");
    }
    m_Size -= n;
  }

  UTILITY_CONSTEXPR14 void swap(basic_string_view &that) noexcept {
    using std::swap;
    if (this != std::addressof(that)) {
      swap(m_Data, that.m_Data);
      swap(m_Size, that.m_Size);
    }
  }

  UTILITY_CONSTEXPR14 void swap(basic_string_view &&that) noexcept {
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

private:
  const char_type *m_Data = nullptr;
  size_type m_Size = 0;
};

using string_view = basic_string_view<char>;

namespace detail {
class fast_hasher {
private:
  static constexpr std::uint64_t M = 0xC6A4A7935BD1E995ULL;
  static constexpr int R = 47;

  // A constant to help "initialize" the hash with the provided seed
  static constexpr std::uint64_t seed_init = 0x9747B28C863640B7ULL;

  static inline std::uint64_t mix(std::uint64_t k) {
    k *= M;
    k ^= k >> R;
    k *= M;
    return k;
  }

public:
  /**
   * @param data Pointer to the bytes
   * @param len Number of bytes
   * @param seed User-provided seed (default: 0)
   */
  static std::uint64_t hash64(const void *data, std::size_t len,
                              std::uint64_t seed = 0) noexcept {
    const std::uint8_t *p = static_cast<const std::uint8_t *>(data);

    // Incorporate the seed into the initial hash state
    std::uint64_t h =
        (seed ^ seed_init) ^ (static_cast<std::uint64_t>(len) * M);

    const std::uint8_t *end = p + (len & ~7);
    for (; p != end; p += 8) {
      std::uint64_t k;
      std::memcpy(&k, p, 8);
      h ^= mix(k);
      h *= M;
    }

    std::uint64_t remaining = 0;
    switch (len & 7) {
    case 7:
      remaining |= static_cast<std::uint64_t>(p[6]) << 48;
    case 6:
      remaining |= static_cast<std::uint64_t>(p[5]) << 40;
    case 5:
      remaining |= static_cast<std::uint64_t>(p[4]) << 32;
    case 4:
      remaining |= static_cast<std::uint64_t>(p[3]) << 24;
    case 3:
      remaining |= static_cast<std::uint64_t>(p[2]) << 16;
    case 2:
      remaining |= static_cast<std::uint64_t>(p[1]) << 8;
    case 1:
      remaining |= static_cast<std::uint64_t>(p[0]);
    }

    h ^= remaining;
    h *= M;

    // Finalizer (Avalanche effect)
    h ^= h >> 33;
    h *= 0xFF51AFD7ED558CCDULL;
    h ^= h >> 33;
    h *= 0xC4CEB9FE1A85EC53ULL;
    h ^= h >> 33;

    return h;
  }

  static std::uint32_t hash32(const void *data, std::size_t len,
                              std::uint32_t seed = 0) noexcept {
    auto h = hash64(data, len, static_cast<std::uint64_t>(seed));
    return static_cast<std::uint32_t>(h ^ (h >> 32));
  }
};

inline std::size_t
hash_bytes(const void *ptr, std::size_t len,
           std::size_t seed = 0x9E3779B97F4A7BB9ULL) noexcept {
  struct hash32 {
    static std::size_t hash_bytes(const void *ptr, std::size_t len,
                                  std::size_t seed) noexcept {
      return fast_hasher::hash32(ptr, len, seed);
    }
  };
  struct hash64 {
    static std::size_t hash_bytes(const void *ptr, std::size_t len,
                                  std::size_t seed) noexcept {
      return fast_hasher::hash64(ptr, len, seed);
    }
  };
  return conditional_t<sizeof(void *) == sizeof(std::uint64_t), hash64,
                       hash32>::hash_bytes(ptr, len, seed);
}
} // namespace detail

template <class> struct basic_string_view_hash;

template <class Char, class CharTraits>
struct basic_string_view_hash<basic_string_view<Char, CharTraits>> {
  size_t operator()(basic_string_view<Char, CharTraits> s) const noexcept {
    return detail::hash_bytes(s.data(), s.size() * sizeof(Char));
  }
};
} // namespace utility

template <class Char, class CharTraits>
struct std::hash<utility::basic_string_view<Char, CharTraits>>
    : public utility::basic_string_view_hash<
          utility::basic_string_view<Char, CharTraits>> {};

namespace utility {
constexpr string_view operator""_sv(const char *str, size_t n) noexcept {
  return string_view(str, n);
}
} // namespace utility

#endif // UTILITY_STRING_VIEW_HPP_
