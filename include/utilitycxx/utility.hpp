#ifndef UTILITYCXX_UTILITY_HPP_
#define UTILITYCXX_UTILITY_HPP_ 1

#include <type_traits>

namespace utilitycxx {
[[noreturn]] inline void unreachable() {
    // Uses compiler specific extensions if possible.
    // Even if no extension is used, undefined behavior is still raised by
    // an empty function body and the noreturn attribute.
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
    __assume(false);
#else // GCC, Clang
    __builtin_unreachable();
#endif
}

template <class T> struct type_identity {
    using type = T;
};

template <class T> using type_identity_t = typename type_identity<T>::type;

using std::enable_if;

template <bool Cond, class T = void>
using enable_if_t = typename enable_if<Cond, T>::type;

using std::conditional;

template <bool Cond, class T, class U>
using conditional_t = typename conditional<Cond, T, U>::type;

using std::remove_cv;
using std::remove_reference;

template <class T> using remove_cv_t = typename remove_cv<T>::type;
template <class T>
using remove_reference_t = typename remove_reference<T>::type;

template <class T> using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

template <class T> struct remove_cvref : type_identity<remove_cvref_t<T>> {};

using std::decay;
template <class T> using decay_t = typename std::decay<T>::type;

template <class...> using void_t = void;

struct non_copyable {
    non_copyable() = default;
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;
    non_copyable(non_copyable&&) noexcept = default;
    non_copyable& operator=(non_copyable&&) noexcept = default;
    ~non_copyable() = default;
};

struct non_movable {
    non_movable() = default;
    non_movable(const non_movable&) = default;
    non_movable& operator=(const non_movable&) = default;
    non_movable(non_movable&&) noexcept = delete;
    non_movable& operator=(non_movable&&) noexcept = delete;
    ~non_movable() = default;
};

struct non_copyable_non_movable : non_copyable, non_movable {};

using std::integral_constant;
} // namespace utilitycxx

#endif // UTILITYCXX_UTILITY_HPP_
