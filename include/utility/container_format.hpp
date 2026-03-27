#ifndef UTILITY_CONTAINER_FORMAT_HPP_
#define UTILITY_CONTAINER_FORMAT_HPP_ 1

#include <utility/format.hpp>

namespace utility {

/**
 * A reusable base class for container formatters.
 * Usage:
 * template<typename T>
 * struct std::formatter<std::vector<T>> :
 * utility::container_formatter<std::vector<T>, utility::StdFormatProxy> {};
 */
template <class Container, class FormatProxy> struct container_formatter {
    template <class ParseContext>
    UTILITY_CONSTEXPR14 auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <class FormatContext>
    auto format(const Container& c, FormatContext& ctx) const
        -> decltype(ctx.out()) {
        return detail::write_exact_container_type_to_formatter<
            const Container&, FormatContext, FormatProxy>(ctx.out(), c);
    }
};

} // namespace utility

#endif // UTILITY_CONTAINER_FORMAT_HPP_
