#ifndef UTILITY_SCOPE_HPP_
#define UTILITY_SCOPE_HPP_ 1

#include <utility/internal/macros.hpp>
#include <utility/internal/version.hpp>
#include <utility/utility.hpp>

#include <memory>
#include <type_traits>
#include <utility>

namespace utility {

namespace detail {
template <class F, class = void> struct defer_scope_storage {
  public:
    static_assert(std::is_same<void, decltype(std::declval<F&>()())>::value,
                  "Must return void");

    defer_scope_storage(defer_scope_storage&&) noexcept = default;
    defer_scope_storage& operator=(defer_scope_storage&&) noexcept = default;

    template <class Func>
    explicit defer_scope_storage(Func&& func, bool enabled)
        : m_Function(static_cast<Func&&>(func)), m_Enabled(enabled) {}

    UTILITY_CONSTEXPR20 void operator()() {
        m_Function();
    }

    constexpr bool is_enabled() const noexcept {
        return m_Enabled;
    }

    UTILITY_CONSTEXPR14 void disable() noexcept {
        m_Enabled = false;
    }

    UTILITY_CONSTEXPR14 void enable() noexcept {
        m_Enabled = true;
    }

  private:
    F m_Function;
    bool m_Enabled = true;
};

template <class F>
struct defer_scope_storage<F, enable_if_t<std::is_empty<F>::value, void>>
    : private F {
  public:
    static_assert(std::is_same<void, decltype(std::declval<F&>()())>::value,
                  "Must return void");

    defer_scope_storage(defer_scope_storage&&) noexcept = default;
    defer_scope_storage& operator=(defer_scope_storage&&) noexcept = default;

    template <class Func>
    explicit defer_scope_storage(Func&& func, bool enabled)
        : F(static_cast<Func&&>(func)), m_Enabled(enabled) {}

    UTILITY_CONSTEXPR20 void operator()() {
        F::operator()();
    }

    constexpr bool is_enabled() const noexcept {
        return m_Enabled;
    }

    UTILITY_CONSTEXPR14 void disable() noexcept {
        m_Enabled = false;
    }

    UTILITY_CONSTEXPR14 void enable() noexcept {
        m_Enabled = true;
    }

  private:
    bool m_Enabled = true;
};

namespace _assertions {
static const auto _empty_lambda = []() {}; // NOLINT
static_assert(
    sizeof(defer_scope_storage<decltype(_empty_lambda)>) == 1,
    "defer_scope_storage with empty lambda should take 1 byte of storage");

inline void _dummy_func() {}
static_assert(sizeof(defer_scope_storage<decltype(_dummy_func)*>) == 16,
              "defer_scope_storage with function pointer should take 16 bytes "
              "of storage");
} // namespace _assertions
} // namespace detail

template <class F> class defer_scope {
  public:
    template <class Func,
              enable_if_t<std::is_constructible<F, Func&&>::value, int> = 0>
    explicit defer_scope(Func&& func, bool enabled = true)
        : m_Storage(static_cast<Func&&>(func), enabled) {}

    defer_scope(const defer_scope&) = delete;
    defer_scope& operator=(const defer_scope&) = delete;

    defer_scope(defer_scope&& other) noexcept
        : m_Storage(std::move(other.m_Storage)) {
        other.disable();
    }

    defer_scope& operator=(defer_scope&& other) noexcept {
        if (this != std::addressof(other)) {
            m_Storage = std::move(other.m_Storage);
            other.disable();
        }
        return *this;
    }

    ~defer_scope() {
        if (is_enabled()) {
            m_Storage();
            m_Storage.disable();
        }
    }

    UTILITY_CONSTEXPR14 void disable() noexcept {
        m_Storage.disable();
    }

    UTILITY_CONSTEXPR14 void enable() noexcept {
        m_Storage.enable();
    }

    constexpr bool is_enabled() const noexcept {
        return m_Storage.is_enabled();
    }

  private:
    detail::defer_scope_storage<F> m_Storage;
};

template <class F> defer_scope<remove_cvref_t<F>> defer(F&& f) {
    return defer_scope<remove_cvref_t<F>>(static_cast<F&&>(f));
}

} // namespace utility

#endif // UTILITY_SCOPE_HPP_
