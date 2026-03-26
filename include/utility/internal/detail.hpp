#ifndef UTILITY_INTERNAL_DETAIL_HPP_
#define UTILITY_INTERNAL_DETAIL_HPP_ 1

#include <utility/utility.hpp>

#include <type_traits>
#include <utility>

namespace utility {
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
} // namespace utility

#endif // UTILITY_INTERNAL_DETAIL_HPP_
