#ifndef UTILITY_INTERNAL_CONTAINER_WRITER_HPP_
#define UTILITY_INTERNAL_CONTAINER_WRITER_HPP_ 1

#include <utility/utility.hpp>

namespace utility {
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
} // namespace utility

#endif // UTILITY_INTERNAL_CONTAINER_WRITER_HPP_
