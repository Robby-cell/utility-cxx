#ifndef UTILITY_OSTREAM_HPP_
#define UTILITY_OSTREAM_HPP_ 1

#include <utility/internal/container_writer.hpp>
#include <utility/internal/detail.hpp>
#include <utility/utility.hpp>

#include <ios>
#include <ostream>

namespace utility {
namespace detail {
template <class Char, class Traits> struct ostream_writer_traits {
    using char_type = Char;
    using traits_type = Traits;
    using writer_type = std::basic_ostream<Char, Traits>&;

    static void write(writer_type writer, const char_type* s,
                      std::streamsize n) {
        writer.write(s, n);
    }

    // Handles raw character output
    static void write(writer_type writer, const char_type& c) {
        writer.put(c);
    }

    // Handles generic formatted output (like ints, strings, etc.)
    // Matches Traits::write(writer, item) in container_writer.hpp
    template <class T> static void write(writer_type writer, const T& value) {
        writer << value;
    }

    // Keep put for symmetry or if used elsewhere
    template <class T> static void put(writer_type writer, const T& value) {
        writer << value;
    }

    static void flush(writer_type writer) {
        writer.flush();
    }
};

template <class Container, class Char, class Traits>
inline enable_if_t<
    detail::is_ostreamable<Container, std::basic_ostream<Char, Traits>>::value,
    void>
write_exact_container_type_to_stream(std::basic_ostream<Char, Traits>& os,
                                     type_identity_t<Container> container) {
    write_exact_container_type_with_traits<Container,
                                           ostream_writer_traits<Char, Traits>>(
        os, container);
}
} // namespace detail

template <class Char, class Traits, class Container>
inline enable_if_t<
    detail::is_ostreamable<Container, std::basic_ostream<Char, Traits>>::value,
    std::basic_ostream<Char, Traits>&>
write_container_to_stream(std::basic_ostream<Char, Traits>& os,
                          const Container& container) {
    detail::write_exact_container_type_to_stream<const Container&>(os,
                                                                   container);
    return os;
}
} // namespace utility

#endif // UTILITY_OSTREAM_HPP_
