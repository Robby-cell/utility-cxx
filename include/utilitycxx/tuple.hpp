#ifndef UTILITYCXX_TUPLE_HPP_
#define UTILITYCXX_TUPLE_HPP_ 1

#include <utilitycxx/internal/macros.hpp>
#include <utilitycxx/internal/version.hpp>
#include <utilitycxx/utility.hpp>

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace utilitycxx {
using std::make_tuple;
using std::tie;
using std::tuple;
using std::tuple_element;
using std::tuple_size;
#if UTILITYCXX_HAS_CPP14
using std::tuple_element_t;
#else
template <std::size_t I, class T>
using tuple_element_t = typename tuple_element<I, T>::type;
#endif
} // namespace utilitycxx

#if UTILITYCXX_HAS_CPP17
namespace utilitycxx {
using std::apply;
using std::invoke;
using std::invoke_result;
using std::invoke_result_t;
using std::result_of;
using std::result_of_t;
} // namespace utilitycxx
#else  // ^^^ UTILITYCXX_HAS_CPP17 / VVV !UTILITYCXX_HAS_CPP17
namespace utilitycxx {
namespace detail {
template <class T> struct is_reference_wrapper : std::false_type {};
template <class U>
struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};

template <class T> struct invoke_impl {
    template <class F, class... Args>
    static auto call(F&& f, Args&&... args)
        -> decltype(static_cast<F&&>(f)(static_cast<Args&&>(args)...));
};

template <class B, class MT> struct invoke_impl<MT B::*> {
    template <class T, class Td = decay_t<T>,
              class = enable_if_t<std::is_base_of<B, Td>::value>>
    static auto get(T&& t) -> T&&;

    template <class T, class Td = decay_t<T>,
              class = enable_if_t<is_reference_wrapper<Td>::value>>
    static auto get(T&& t) -> decltype(t.get());

    template <class T, class Td = decay_t<T>,
              class = enable_if_t<!std::is_base_of<B, Td>::value>,
              class = enable_if_t<!is_reference_wrapper<Td>::value>>
    static auto get(T&& t) -> decltype(*static_cast<T&&>(t));

    template <class T, class... Args, class MT1,
              class = enable_if_t<std::is_function<MT1>::value>>
    static auto call(MT1 B::* pmf, T&& t, Args&&... args)
        -> decltype((invoke_impl::get(static_cast<T&&>(t)).*
                     pmf)(static_cast<Args&&>(args)...));

    template <class T>
    static auto call(MT B::* pmd, T&& t)
        -> decltype(invoke_impl::get(static_cast<T&&>(t)).*pmd);
};

template <class F, class... Args, class Fd = decay_t<F>>
auto INVOKE(F&& f, Args&&... args)
    -> decltype(invoke_impl<Fd>::call(static_cast<F&&>(f),
                                      static_cast<Args&&>(args)...));
} // namespace detail

namespace detail {
template <typename AlwaysVoid, typename, typename...> struct invoke_result {};
template <typename F, typename... Args>
struct invoke_result<decltype(void(detail::INVOKE(std::declval<F>(),
                                                  std::declval<Args>()...))),
                     F, Args...> {
    using type =
        decltype(detail::INVOKE(std::declval<F>(), std::declval<Args>()...));
};
} // namespace detail

template <class F, class... ArgTypes>
struct invoke_result : detail::invoke_result<void, F, ArgTypes...> {};

template <class F, class... Args>
using invoke_result_t = typename invoke_result<F, Args...>::type;

template <class> struct result_of;
template <class F, class... ArgTypes> struct result_of<F(ArgTypes...)> {
    using type = decltype(detail::INVOKE(std::declval<F>(),
                                         std::declval<ArgTypes>()...));
};

template <class F> using result_of_t = typename result_of<F>::type;

namespace detail {
// 1. Member function on object or reference
template <class C, class Pointed, class Object, class... Args>
inline enable_if_t<
    std::is_base_of<C, decay_t<Object>>::value &&
        std::is_function<Pointed>::value,
    decltype((std::declval<Object>().*
              std::declval<Pointed C::*>())(std::declval<Args>()...))>
invoke_memptr_impl(Pointed C::* member, Object&& object, Args&&... args) {
    return (static_cast<Object&&>(object).*
            member)(static_cast<Args&&>(args)...);
}

// 2. Member function on reference_wrapper
template <class C, class Pointed, class Object, class... Args>
inline enable_if_t<
    detail::is_reference_wrapper<decay_t<Object>>::value &&
        std::is_function<Pointed>::value,
    decltype((std::declval<Object>().get().*
              std::declval<Pointed C::*>())(std::declval<Args>()...))>
invoke_memptr_impl(Pointed C::* member, Object&& object, Args&&... args) {
    return (object.get().*member)(static_cast<Args&&>(args)...);
}

// 3. Member function on pointer or smart pointer
template <class C, class Pointed, class Object, class... Args>
inline enable_if_t<
    !std::is_base_of<C, decay_t<Object>>::value &&
        !detail::is_reference_wrapper<decay_t<Object>>::value &&
        std::is_function<Pointed>::value,
    decltype(((*std::declval<Object>()).*
              std::declval<Pointed C::*>())(std::declval<Args>()...))>
invoke_memptr_impl(Pointed C::* member, Object&& object, Args&&... args) {
    return ((*static_cast<Object&&>(object)).*
            member)(static_cast<Args&&>(args)...);
}

// 4. Member data on object or reference
template <class C, class Pointed, class Object>
inline enable_if_t<std::is_base_of<C, decay_t<Object>>::value &&
                       !std::is_function<Pointed>::value,
                   decltype(std::declval<Object>().*
                            std::declval<Pointed C::*>())>
invoke_memptr_impl(Pointed C::* member, Object&& object) {
    return static_cast<Object&&>(object).*member;
}

// 5. Member data on reference_wrapper
template <class C, class Pointed, class Object>
inline enable_if_t<detail::is_reference_wrapper<decay_t<Object>>::value &&
                       !std::is_function<Pointed>::value,
                   decltype(std::declval<Object>().get().*
                            std::declval<Pointed C::*>())>
invoke_memptr_impl(Pointed C::* member, Object&& object) {
    return object.get().*member;
}

// 6. Member data on pointer or smart pointer
template <class C, class Pointed, class Object>
inline enable_if_t<!std::is_base_of<C, decay_t<Object>>::value &&
                       !detail::is_reference_wrapper<decay_t<Object>>::value &&
                       !std::is_function<Pointed>::value,
                   decltype((*std::declval<Object>()).*
                            std::declval<Pointed C::*>())>
invoke_memptr_impl(Pointed C::* member, Object&& object) {
    return (*static_cast<Object&&>(object)).*member;
}
} // namespace detail

template <class F, class... Args>
inline enable_if_t<std::is_member_pointer<decay_t<F>>::value,
                   result_of_t<F(Args...)>>
invoke(F&& f, Args&&... args) {
    return detail::invoke_memptr_impl(f, static_cast<Args&&>(args)...);
}

template <class F, class... Args>
inline enable_if_t<!std::is_member_pointer<decay_t<F>>::value,
                   result_of_t<F(Args...)>>
invoke(F&& f, Args&&... args) {
    return static_cast<F&&>(f)(static_cast<Args&&>(args)...);
}
} // namespace utilitycxx
#endif // ^^^ !UTILITYCXX_HAS_CPP17

#endif // UTILITYCXX_TUPLE_HPP_
