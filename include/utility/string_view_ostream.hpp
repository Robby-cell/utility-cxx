#ifndef UTILITY_STRING_VIEW_OSTREAM_HPP_
#define UTILITY_STRING_VIEW_OSTREAM_HPP_ 1

#include <utility/string_view.hpp>

#include <ostream>

namespace utility {

template <class Char, class CharTraits, class StreamTraits>
std::basic_ostream<Char, StreamTraits> &
operator<<(std::basic_ostream<Char, StreamTraits> &os,
           utility::basic_string_view<Char, CharTraits> s) {
  os.write(s.data(), s.size());
  return os;
}

} // namespace utility

#endif // UTILITY_STRING_VIEW_OSTREAM_HPP_
