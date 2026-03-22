# C++11 Utility Library Feature List

Since this library aims to be a C++11 compatible header-only library, many features would ideally backport functionality from newer language standards (C++14/17/20) or provide tools commonly found in libraries like Boost or Abseil.

## 1. Memory Utilities (C++14 Backports)
* `utility::make_unique`: C++11 is famously missing `std::make_unique`. Providing this is almost mandatory for a C++11 utility library.

## 2. Type Traits and Metaprogramming (C++14/17 Backports)
* `_t` aliases (e.g., `utility::enable_if_t`, `utility::decay_t`, `utility::conditional_t`) to reduce `typename ...::type` clutter.
* `utility::void_t`: Essential for SFINAE (added in C++17)
## 3. String Utilities
* `utility::string_view` (Wait, might be too heavy? A lightweight `string_view` backport similar to C++17).
* String manipulation algorithms not found in standard C++:
    * `starts_with` and `ends_with`.
    * `split` (e.g., splitting a string by a delimiter into a lazy iterator).
    * `trim`, `trim_left`, `trim_right`.
    * `join` (e.g., joining an iterable of strings with a delimiter).

## 4. Vocabulary Types (C++17/20 Backports)
* `utility::optional`: Much safer than returning pointers or sentinel values.
* `utility::expected`: Error handling without exceptions (C++23 feature).
* `utility::span`: A non-owning view of contiguous memory sequences (C++20 feature).
* `utility::function_ref`: A lightweight, non-owning reference to a callable (avoids the allocation overhead of `std::function`).

## 5. Functional & Algorithm Utilities
* `utility::invoke` and `utility::apply` (C++17).
* Scope guards (`utility::defer` / Go-style defer): Extremely useful for deterministic cleanup, especially if exceptions are disabled.
* Enhanced algorithms: `contains` (for associative containers or ranges), vector erase-remove idiom functions.

## 6. Miscellaneous
* `utility::unreachable()`: Compiler hints for optimization.
* Assertion macros (e.g., `UTILITY_ASSERT` with custom termination).
* `utility::noncopyable`: A convenience base class or macro to delete copy constructors.
