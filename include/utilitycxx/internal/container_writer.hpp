#ifndef UTILITYCXX_INTERNAL_CONTAINER_WRITER_HPP_
#define UTILITYCXX_INTERNAL_CONTAINER_WRITER_HPP_ 1

#include <utilitycxx/utility.hpp>

namespace utilitycxx {
namespace detail {
template <class Container, class Traits, class Writer>
inline void
write_exact_container_type_with_traits(Writer& writer,
                                       type_identity_t<Container> container) {
    bool first = true;
    for (const auto& item : container) {
        if (!first) {
            static constexpr const typename Traits::char_type sep[]{',', ' '};
            Traits::write(writer, sep, 2);
        }
        Traits::write(writer, item);
        first = false;
    }
}
} // namespace detail
} // namespace utilitycxx

#endif // UTILITYCXX_INTERNAL_CONTAINER_WRITER_HPP_
