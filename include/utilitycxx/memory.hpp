#include <type_traits>
#ifndef UTILITYCXX_MEMORY_HPP_
#define UTILITYCXX_MEMORY_HPP_ 1

#include <utilitycxx/internal/version.hpp>
#include <utilitycxx/utility.hpp>

#include <cstddef>
#include <memory>

namespace utilitycxx {

#if UTILITYCXX_HAS_CPP14
using std::make_unique;
#else
namespace detail {
template <class T> struct make_unique_t {
    using single = std::unique_ptr<T>;
};

template <class T> struct make_unique_t<T[]> {
    using array = std::unique_ptr<T[]>;
};

template <class T, std::size_t Extent> struct make_unique_t<T[Extent]> {
    struct invalid {};
};

template <class T> using unique_ptr_t = typename make_unique_t<T>::single;

template <class T> using unique_ptr_array_t = typename make_unique_t<T>::array;

template <class T>
using invalid_unique_ptr_t = typename make_unique_t<T>::invalid;
} // namespace detail

template <class T, class... Args>
detail::unique_ptr_t<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(static_cast<Args&&>(args)...));
}

template <class T>
detail::unique_ptr_array_t<T> make_unique(std::size_t count) {
    return std::unique_ptr<T>(new
                              typename std::remove_extent<T>::type[count]());
}
#endif
} // namespace utilitycxx

#endif // UTILITYCXX_MEMORY_HPP_
