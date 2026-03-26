#ifndef UTILITY_STRING_VIEW_HASHER_HPP_
#define UTILITY_STRING_VIEW_HASHER_HPP_ 1

#include <utility/string_View.hpp>
#include <utility/utility.hpp>

#include <cstdint>
#include <cstring>

// Forward declaration of char traits
namespace std {
template <class Char> struct char_traits;
template <class T> struct hash;
} // namespace std

namespace utility {

namespace detail {
class fast_hasher {
  private:
    // NOLINTNEXTLINE
    static constexpr std::uint64_t M = 0xC6A4A7935BD1E995ULL;
    static constexpr int R = 47; // NOLINT

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
    static std::uint64_t hash64(const void* data, std::size_t len,
                                std::uint64_t seed = 0) noexcept {
        const auto* p = static_cast<const std::uint8_t*>(data);

        // Incorporate the seed into the initial hash state
        std::uint64_t h =
            (seed ^ seed_init) ^ (static_cast<std::uint64_t>(len) * M);

        const std::uint8_t* end = p + (len & ~7);
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

    static std::uint32_t hash32(const void* data, std::size_t len,
                                std::uint32_t seed = 0) noexcept {
        auto h = hash64(data, len, static_cast<std::uint64_t>(seed));
        return static_cast<std::uint32_t>(h ^ (h >> 32));
    }
};

inline std::size_t
hash_bytes(const void* ptr, std::size_t len,
           std::size_t seed = 0x9E3779B97F4A7BB9ULL) noexcept {
    struct hash32 {
        static std::size_t hash_bytes(const void* ptr, std::size_t len,
                                      std::size_t seed) noexcept {
            return fast_hasher::hash32(ptr, len, seed);
        }
    };
    struct hash64 {
        static std::size_t hash_bytes(const void* ptr, std::size_t len,
                                      std::size_t seed) noexcept {
            return fast_hasher::hash64(ptr, len, seed);
        }
    };
    return conditional_t<sizeof(void*) == sizeof(std::uint64_t), hash64,
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

#endif // UTILITY_STRING_VIEW_HASHER_HPP_
