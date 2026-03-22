#ifndef UTILITY_STRING_VIEW_FORMAT_HPP_
#define UTILITY_STRING_VIEW_FORMAT_HPP_ 1

#include <utility/internal/version.hpp>

#if UTILITY_HAS_CPP20
#include <utility/string_view.hpp>

#include <format>

template <class Char, class CharTraits>
struct std::formatter<utility::basic_string_view<Char, CharTraits>> {
  constexpr auto parse(auto &ctx) noexcept { return ctx.begin(); }

  auto format(utility::basic_string_view<Char, CharTraits> s, auto &ctx) const {
    return std::format_to_n(ctx.out(), s.size(), "{}", s.data());
  }
};
#endif

#endif // UTILITY_STRING_VIEW_FORMAT_HPP_
