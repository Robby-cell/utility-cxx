#ifndef UTILITYCXX_INTERNAL_STD_FORMAT_PROXY_HPP_
#define UTILITYCXX_INTERNAL_STD_FORMAT_PROXY_HPP_ 1

#include <utilitycxx/internal/version.hpp>

#if UTILITYCXX_HAS_CPP20
#include <format>

namespace std {
template <class> struct char_traits;
}

namespace utilitycxx {
namespace detail {
// StdFormatProxy acts as the bridge for our container abstraction.
template <class Context, class Char, class CharTraits = std::char_traits<Char>>
struct StdFormatProxy {
    using char_type = Char;
    using traits_type = CharTraits;
    using writer_type = Context&;

    // Formats a generic value (like a container item) and updates the context
    // iterator
    template <class T> static void print(writer_type ctx, const T& value) {
        ctx.advance_to(std::format_to(ctx.out(), "{}", value));
    }
};
} // namespace detail
} // namespace utilitycxx
#endif

#endif // UTILITYCXX_INTERNAL_STD_FORMAT_PROXY_HPP_
