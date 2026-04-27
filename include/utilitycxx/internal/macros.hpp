#ifndef UTILITYCXX_INTERNAL_MACROS_HPP_
#define UTILITYCXX_INTERNAL_MACROS_HPP_ 1

#include <utilitycxx/internal/version.hpp>

#if UTILITYCXX_HAS_CPP14
#define UTILITYCXX_CONSTEXPR14 constexpr
#else
#define UTILITYCXX_CONSTEXPR14 inline
#endif

#if UTILITYCXX_HAS_CPP17
#define UTILITYCXX_CONSTEXPR17 constexpr
#else
#define UTILITYCXX_CONSTEXPR17 inline
#endif

#if UTILITYCXX_HAS_CPP20
#define UTILITYCXX_CONSTEXPR20 constexpr
#else
#define UTILITYCXX_CONSTEXPR20 inline
#endif

#if UTILITYCXX_HAS_CPP23
#define UTILITYCXX_CONSTEXPR23 constexpr
#else
#define UTILITYCXX_CONSTEXPR23 inline
#endif

#if UTILITYCXX_HAS_CPP17
#define UTILITYCXX_NODISCARD [[nodiscard]]
#else
#define UTILITYCXX_NODISCARD
#endif

#if __cpp_lib_char8_t >= 201811L
#define UTILITYCXX_HAS_CHAR8_T 1
#else
#define UTILITYCXX_HAS_CHAR8_T 0
#endif

#endif // UTILITYCXX_INTERNAL_MACROS_HPP_
