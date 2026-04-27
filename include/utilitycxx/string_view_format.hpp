#ifndef UTILITYCXX_STRING_VIEW_FORMAT_HPP_
#define UTILITYCXX_STRING_VIEW_FORMAT_HPP_ 1

#include <utilitycxx/internal/macros.hpp>
#include <utilitycxx/internal/std_format_proxy.hpp>
#include <utilitycxx/internal/version.hpp>
#include <utilitycxx/string_view.hpp>

namespace utilitycxx {
template <class StringView, class FormatProxy>
struct basic_string_view_formatter {
    template <class ParseContext>
    UTILITYCXX_CONSTEXPR14 auto parse(ParseContext& ctx) noexcept
        -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <class FormatContext>
    auto format(StringView s, FormatContext& ctx) const -> decltype(ctx.out()) {
        FormatProxy::print(ctx, s);
        return ctx.out();
    }
};
} // namespace utilitycxx

#if UTILITYCXX_HAS_CPP20
#include <format>
#include <string_view>

// Standard injection for utilitycxx::string_view
template <class Char, class Traits>
struct std::formatter<utilitycxx::basic_string_view<Char, Traits>, Char>
    : private std::formatter<std::basic_string_view<Char, Traits>, Char> {
  private:
    using Base = std::formatter<std::basic_string_view<Char, Traits>, Char>;

  public:
    template <class ParseContext>
    constexpr auto parse(ParseContext& ctx) -> decltype(Base::parse(ctx)) {
        // This correctly consumes format specifiers (like `:<10`) and advances
        // the iterator
        return Base::parse(ctx);
    }

    template <class FormatContext>
    auto format(utilitycxx::basic_string_view<Char, Traits> s,
                FormatContext& ctx) const
        -> decltype(Base::format(std::basic_string_view<Char, Traits>(s.data(),
                                                                      s.size()),
                                 ctx)) {
        // Convert utilitycxx::string_view to std::string_view so the underlying
        // formatter can print it
        return Base::format(
            std::basic_string_view<Char, Traits>(s.data(), s.size()), ctx);
    }
};
#endif // ^^^ UTILITYCXX_HAS_CPP20

#endif // UTILITYCXX_STRING_VIEW_FORMAT_HPP_
