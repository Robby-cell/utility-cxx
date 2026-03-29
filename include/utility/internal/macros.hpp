#ifndef UTILITY_INTERNAL_MACROS_HPP_
#define UTILITY_INTERNAL_MACROS_HPP_ 1

#include <utility/internal/version.hpp>

#if UTILITY_HAS_CPP14
#define UTILITY_CONSTEXPR14 constexpr
#else
#define UTILITY_CONSTEXPR14 inline
#endif

#if UTILITY_HAS_CPP17
#define UTILITY_CONSTEXPR17 constexpr
#else
#define UTILITY_CONSTEXPR17 inline
#endif

#if UTILITY_HAS_CPP20
#define UTILITY_CONSTEXPR20 constexpr
#else
#define UTILITY_CONSTEXPR20 inline
#endif

#if UTILITY_HAS_CPP23
#define UTILITY_CONSTEXPR23 constexpr
#else
#define UTILITY_CONSTEXPR23 inline
#endif

#if UTILITY_HAS_CPP17
#define UTILITY_NODISCARD [[nodiscard]]
#else
#define UTILITY_NODISCARD
#endif

#if __cpp_lib_char8_t >= 201811L
#define UTILITY_HAS_CHAR8_T 1
#else
#define UTILITY_HAS_CHAR8_T 0
#endif

#endif // UTILITY_INTERNAL_MACROS_HPP_
