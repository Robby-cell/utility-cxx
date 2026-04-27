#ifndef UTILITYCXX_CONTAINER_FORMAT_HPP_
#define UTILITYCXX_CONTAINER_FORMAT_HPP_ 1

#include <utilitycxx/format.hpp>

namespace utilitycxx {

/**
 * A reusable base class for container formatters.
 * Usage:
 * template<typename T>
 * struct std::formatter<std::vector<T>> :
 * utility::container_formatter<std::vector<T>, utility::StdFormatProxy> {};
 */
template <class Container, class FormatProxy> struct container_formatter {
    template <class ParseContext>
    UTILITYCXX_CONSTEXPR14 auto parse(ParseContext& ctx)
        -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <class FormatContext>
    auto format(const Container& c, FormatContext& ctx) const
        -> decltype(ctx.out()) {
        return detail::write_exact_container_type_to_formatter<
            const Container&, FormatContext, FormatProxy>(ctx.out(), c);
    }
};

} // namespace utilitycxx

#endif // UTILITYCXX_CONTAINER_FORMAT_HPP_
