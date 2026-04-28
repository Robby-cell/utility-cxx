#ifndef UTILITYCXX_SPAN_HPP_
#define UTILITYCXX_SPAN_HPP_ 1

#include <utilitycxx/internal/macros.hpp>
#include <utilitycxx/internal/version.hpp>
#include <utilitycxx/types.hpp>
#include <utilitycxx/utility.hpp>

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace utilitycxx {

constexpr std::size_t dynamic_extent = static_cast<std::size_t>(-1);

namespace detail {
template <class, class = void>
struct is_container_like_helper : std::false_type {};

template <class T>
struct is_container_like_helper<
    T, void_t<decltype(std::declval<T&>().data(), std::declval<T&>().size())>>
    : std::true_type {};

template <class T> struct is_container_like : is_container_like_helper<T> {};

template <class T, std::size_t Extent> struct span_storage {
    span_storage() = default;

    constexpr span_storage(T* ptr, std::size_t /* size */) : ptr(ptr) {}

    T* ptr = nullptr;
    static constexpr std::size_t size = Extent;
};

template <class T> struct span_storage<T, dynamic_extent> {
    span_storage() = default;

    constexpr span_storage(T* ptr, std::size_t size) : ptr(ptr), size(size) {}

    T* ptr = nullptr;
    std::size_t size = 0;
};

} // namespace detail

template <class T, std::size_t Extent = dynamic_extent> class span {
  public:
    using element_type = T;
    using value_type = typename std::remove_cv<T>::type;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = pointer;
    using const_iterator = const_pointer;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr std::size_t extent = Extent;

  public:
    span() = default;

    template <std::size_t MyExtent = Extent,
              enable_if_t<MyExtent != dynamic_extent &&
                              (MyExtent == Extent || Extent == dynamic_extent),
                          int> = 0>
    constexpr span(element_type (&array)[MyExtent]) : span(array, MyExtent) {}

    constexpr explicit span(pointer ptr, size_type n) : m_Storage(ptr, n) {}

    template <
        class Container,
        enable_if_t<detail::is_container_like<remove_cvref_t<Container>>::value,
                    int> = 0>
    UTILITYCXX_CONSTEXPR20 span(Container& c) : span(c.data(), c.size()) {}

    constexpr pointer data() const noexcept {
        return m_Storage.ptr;
    }

    constexpr iterator begin() const noexcept {
        return iterator(data());
    }

    constexpr iterator end() const noexcept {
        return begin() + size();
    }

    constexpr reverse_iterator rbegin() const noexcept {
        return reverse_iterator(end());
    }

    constexpr reverse_iterator rend() const noexcept {
        return reverse_iterator(begin());
    }

#pragma region element access
    constexpr reference front() const noexcept {
        return *begin();
    }

    constexpr reference back() const noexcept {
        return *(end() - 1);
    }

    constexpr reference operator[](size_type index) const noexcept {
        return data()[index];
    }
#pragma endregion

#pragma region observers
    constexpr size_type size() const noexcept {
        return m_Storage.size;
    }

    constexpr size_type size_bytes() const noexcept {
        return size() * sizeof(element_type);
    }

    constexpr bool empty() const noexcept {
        return size() == 0;
    }
#pragma endregion

  private:
    detail::span_storage<T, Extent> m_Storage;
};

template <class T, std::size_t Extent>
span<const byte,
     (Extent == dynamic_extent) ? dynamic_extent : (Extent * sizeof(T))>
as_bytes(span<T, Extent> s) noexcept {
    constexpr std::size_t span_extent =
        (Extent == dynamic_extent) ? dynamic_extent : (Extent * sizeof(T));
    return span<const byte, span_extent>(
        reinterpret_cast<const byte*>(s.data()), s.size_bytes());
}

template <class T, std::size_t Extent>
span<byte, (Extent == dynamic_extent) ? dynamic_extent : (Extent * sizeof(T))>
as_writable_bytes(span<T, Extent> s) noexcept {
    static_assert(!std::is_const<T>::value, "T must not be const");
    constexpr std::size_t span_extent =
        (Extent == dynamic_extent) ? dynamic_extent : (Extent * sizeof(T));
    return span<byte, span_extent>(reinterpret_cast<byte*>(s.data()),
                                   s.size_bytes());
}

#if UTILITYCXX_HAS_CPP17
template <class Container>
span(Container& c) -> span<
    conditional_t<std::is_const<remove_reference_t<
                      decltype(*std::declval<Container>().begin())>>::value,
                  const typename remove_cvref_t<Container>::value_type,
                  typename remove_cvref_t<Container>::value_type>,
    dynamic_extent>;

template <class T, std::size_t Extent> span(T (&)[Extent]) -> span<T, Extent>;

template <class T> span(T* ptr, std::size_t n) -> span<T, dynamic_extent>;
#endif // ^^^ UTILITYCXX_HAS_CPP17
} // namespace utilitycxx

#endif // UTILITYCXX_SPAN_HPP_
