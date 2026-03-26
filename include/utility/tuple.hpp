#ifndef UTILITY_TUPLE_HPP_
#define UTILITY_TUPLE_HPP_ 1

#include <utility/internal/macros.hpp>
#include <utility/internal/version.hpp>
#include <utility/utility.hpp>

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace utility {
using std::make_tuple;
using std::tie;
using std::tuple;
using std::tuple_element;
using std::tuple_size;
#if UTILITY_HAS_CPP14
using std::tuple_element_t;
#else
template <std::size_t I, class T>
using tuple_element_t = typename tuple_element<I, T>::type;
#endif
} // namespace utility

#if UTILITY_HAS_CPP17
namespace utility {
using std::apply;
using std::invoke;
using std::invoke_result;
using std::invoke_result_t;
using std::result_of;
using std::result_of_t;
} // namespace utility
#else  // ^^^ UTILITY_HAS_CPP17 / VVV !UTILITY_HAS_CPP17
namespace utility {
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
template <class C, class Pointed, class Object, class... Args>
auto invoke_memptr_impl(std::true_type /*is_mem_func*/, Pointed C::* member,
                        Object&& object, Args&&... args)
    -> decltype((static_cast<Object&&>(object).*
                 member)(static_cast<Args&&>(args)...)) {
    using object_t = typename remove_cvref<Object>::type;

    // Logic: derived -> wrapped -> pointer
    if (std::is_base_of<C, object_t>::value) {
        return (static_cast<Object&&>(object).*
                member)(static_cast<Args&&>(args)...);
    } else if (is_reference_wrapper<object_t>::value) {
        return (object.*member)(static_cast<Args&&>(args)...);
    } else {
        return ((static_cast<Object&&>(object)).*
                member)(static_cast<Args&&>(args)...);
    }
}

template <class C, class Pointed, class Object>
auto invoke_memptr_impl(std::false_type /*is_mem_func*/, Pointed C::* member,
                        Object&& object)
    -> decltype(static_cast<Object&&>(object).*member) {
    using object_t = typename remove_cvref<Object>::type;

    if (std::is_base_of<C, object_t>::value) {
        return static_cast<Object&&>(object).*member;
    } else if (is_reference_wrapper<object_t>::value) {
        return object.get().*member;
    } else {
        return (*static_cast<Object&&>(object)).*member;
    }
}
} // namespace detail

template <class F, class... Args>
auto invoke(F&& f, Args&&... args)
    -> enable_if_t<std::is_member_pointer<remove_cvref_t<F>>::value,
                   result_of_t<F(Args...)>> {
    using f_t = remove_cvref_t<F>;

    // Further differentiate between member function and member data
    using is_func = typename std::is_member_function_pointer<f_t>::type;
    return detail::invoke_memptr_impl(is_func(), f,
                                      static_cast<Args&&>(args)...);
}

template <class F, class... Args>
auto invoke(F&& f, Args&&... args)
    -> enable_if_t<!std::is_member_pointer<remove_cvref_t<F>>::value,
                   result_of_t<F(Args...)>> {
    return static_cast<F&&>(f)(static_cast<Args&&>(args)...);
}
} // namespace utility
#endif // ^^^ !UTILITY_HAS_CPP17

#endif // UTILITY_TUPLE_HPP_
