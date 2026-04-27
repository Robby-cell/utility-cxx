#ifndef UTILITYCXX_SPAN_OSTREAM_HPP_
#define UTILITYCXX_SPAN_OSTREAM_HPP_ 1

#include <utilitycxx/ostream.hpp>
#include <utilitycxx/span.hpp>

namespace utilitycxx {

namespace detail {
template <class Char, class T, std::size_t Extent>
inline enable_if_t<
    detail::is_ostreamable<span<T, Extent>, std::basic_ostream<Char>>::value,
    void>
write_span_to_stream(std::basic_ostream<Char>& os, span<T, Extent> s) {
    return write_exact_container_type_to_stream<span<T, Extent>>(os, s);
}
} // namespace detail

template <class T, std::size_t Extent>
inline std::ostream& operator<<(std::ostream& os, span<T, Extent> s) {
    detail::write_span_to_stream(os, s);
    return os;
}

template <class T, std::size_t Extent>
inline std::wostream& operator<<(std::wostream& os, span<T, Extent> s) {
    detail::write_span_to_stream(os, s);
    return os;
}
} // namespace utilitycxx

#endif // UTILITYCXX_SPAN_OSTREAM_HPP_
