#ifndef UTILITYCXX_FORMAT_HPP_
#define UTILITYCXX_FORMAT_HPP_ 1

#include <utilitycxx/internal/container_writer.hpp>
#include <utilitycxx/internal/version.hpp>
#include <utilitycxx/string_view.hpp>
#include <utilitycxx/string_view_format.hpp>

#include <ios>

namespace utilitycxx {
namespace detail {
template <class FormatProxy> struct format_writer_traits {
    using char_type = typename FormatProxy::char_type;
    using traits_type = typename FormatProxy::traits_type;
    using writer_type =
        typename FormatProxy::writer_type; // Usually FormatContext&

    static void write(writer_type writer, const char_type* s,
                      std::streamsize n) {
        FormatProxy::print(
            writer, utilitycxx::basic_string_view<char_type, traits_type>{
                        s, static_cast<size_t>(n)});
    }

    static void write(writer_type writer, const char_type& c) {
        FormatProxy::print(writer, c);
    }

    // Matches what `container_writer.hpp` will call for items:
    // Traits::write(writer, item)
    template <class T> static void write(writer_type writer, const T& value) {
        FormatProxy::print(writer, value);
    }

    // Provided for symmetry with ostream_writer_traits
    template <class T> static void put(writer_type writer, const T& value) {
        FormatProxy::print(writer, value);
    }

    static void flush(writer_type writer) {}
};

/**
 * Core internal function to format a container.
 * This matches the pattern in ostream.hpp
 */
template <class Container, class FormatProxy>
inline void write_exact_container_type_to_formatter(
    typename FormatProxy::writer_type writer,
    type_identity_t<Container> container) {
    write_exact_container_type_with_traits<Container,
                                           format_writer_traits<FormatProxy>>(
        writer, container);
}
} // namespace detail
} // namespace utilitycxx

#endif // UTILITYCXX_FORMAT_HPP_
