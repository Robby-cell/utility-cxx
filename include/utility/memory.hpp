#ifndef UTILITY_MEMORY_HPP_
#define UTILITY_MEMORY_HPP_ 1

#include <utility/internal/version.hpp>
#include <utility/utility.hpp>

#include <memory>
#include <type_traits>

namespace utility {

#if UTILITY_HAS_CPP14
using std::make_unique;
#else
template <class T, class... Args>
enable_if_t<std::is_constructible<T, Args&&...>::value, std::unique_ptr<T>>
make_unique(Args&&... args) {
    auto* ptr = reinterpret_cast<T*>(::operator new(sizeof(T)));
    try {
        ::new (ptr) T(static_cast<Args&&>(args)...);
    } catch (...) {
        ::operator delete(ptr);
        throw;
    }
    return std::unique_ptr<T>(ptr);
}
#endif

} // namespace utility

#endif // UTILITY_MEMORY_HPP_
