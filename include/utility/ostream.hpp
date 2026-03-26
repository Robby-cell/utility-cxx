#ifndef UTILITY_OSTREAM_HPP_
#define UTILITY_OSTREAM_HPP_ 1

#include <utility/internal/detail.hpp>
#include <utility/utility.hpp>

#include <ostream>

namespace utility {
namespace detail {
template <class Container, class Char, class Traits>
inline enable_if_t<
    detail::is_ostreamable<Container, std::basic_ostream<Char, Traits>>::value,
    std::basic_ostream<Char, Traits>&>
write_exact_container_type_to_stream(std::basic_ostream<Char, Traits>& os,
                                     type_identity_t<Container> container) {
    bool first = true;
    for (const auto& item : container) {
        if (!first) {
            static constexpr const Char sep[]{',', ' '};
            os.write(sep, 2);
        }
        os << item;
        first = false;
    }
    return os;
}
} // namespace detail
template <class Char, class Traits, class Container>
inline enable_if_t<
    detail::is_ostreamable<Container, std::basic_ostream<Char, Traits>>::value,
    std::basic_ostream<Char, Traits>&>
write_container_to_stream(std::basic_ostream<Char, Traits>& os,
                          const Container& container) {
    return detail::write_exact_container_type_to_stream<const Container&>(
        os, container);
}
} // namespace utility

#endif // UTILITY_OSTREAM_HPP_
