#ifndef UTILITYCXX_STRING_VIEW_OSTREAM_HPP_
#define UTILITYCXX_STRING_VIEW_OSTREAM_HPP_ 1

#include <utilitycxx/string_view.hpp>

#include <ostream>

namespace utilitycxx {

template <class Char, class CharTraits, class StreamTraits>
std::basic_ostream<Char, StreamTraits>&
operator<<(std::basic_ostream<Char, StreamTraits>& os,
           utilitycxx::basic_string_view<Char, CharTraits> s) {
    os.write(s.data(), s.size());
    return os;
}

} // namespace utilitycxx

#endif // UTILITYCXX_STRING_VIEW_OSTREAM_HPP_
