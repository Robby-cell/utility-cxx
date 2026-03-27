#ifndef UTILITY_SPAN_FORMAT_HPP_
#define UTILITY_SPAN_FORMAT_HPP_ 1

#include <utility/format.hpp>
#include <utility/span.hpp>

#if UTILITY_HAS_CPP20
#include <format>
#include <utility/internal/std_format_proxy.hpp> // For StdFormatProxy

namespace utility {
namespace detail {
template <class Context, class Char, class T, std::size_t Extent>
inline void write_span_to_formatter(Context& ctx, span<T, Extent> s) {
    using Proxy = StdFormatProxy<Context, Char>;

    // BUG FIX: Pass the actual span 's', NOT an empty initializer '{}'
    write_exact_container_type_to_formatter<span<T, Extent>, Proxy>(ctx, s);
}
} // namespace detail
} // namespace utility

template <class T, std::size_t Extent, class Char>
struct std::formatter<utility::span<T, Extent>, Char> {
    template <class ParseContext> constexpr auto parse(ParseContext& ctx) {
        // Return the iterator immediately (assuming no complex formatting args
        // inside {})
        return ctx.begin();
    }

    template <class FormatContext>
    auto format(utility::span<T, Extent> s, FormatContext& ctx) const {
        utility::detail::write_span_to_formatter<FormatContext, Char>(ctx, s);
        return ctx.out();
    }
};
#endif

#endif // UTILITY_SPAN_FORMAT_HPP_
