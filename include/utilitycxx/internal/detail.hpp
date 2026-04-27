#ifndef UTILITYCXX_INTERNAL_DETAIL_HPP_
#define UTILITYCXX_INTERNAL_DETAIL_HPP_ 1

#include <utilitycxx/utility.hpp>

#include <type_traits>
#include <utility>

namespace utilitycxx {
namespace detail {

template <class, class, class = void>
struct is_ostreamable_helper : std::false_type {};

template <class T, class Os>
struct is_ostreamable_helper<
    T, Os, void_t<decltype(std::declval<Os&>() << std::declval<T>())>>
    : std::true_type {};

template <class T, class Os>
struct is_ostreamable : is_ostreamable_helper<T, Os> {};

} // namespace detail
} // namespace utilitycxx

#endif // UTILITYCXX_INTERNAL_DETAIL_HPP_
