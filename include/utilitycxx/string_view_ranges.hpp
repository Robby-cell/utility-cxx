#ifndef UTILITYCXX_RANGES_STRING_VIEW_HPP_
#define UTILITYCXX_RANGES_STRING_VIEW_HPP_ 1

#include <utilitycxx/string_view.hpp>

#include <iterator>

namespace utilitycxx {
namespace ranges {

class split_view {
  public:
    class iterator {
      private:
        void advance() {
            if (m_IsEnd) {
                return;
            }

            if (m_Remainder.empty()) {
                m_IsEnd = true;
                return;
            }

            if (m_Delimiter.empty()) {
                if (!m_Remainder.empty()) {
                    m_Current = m_Remainder.substr(0, 1);
                    m_Remainder.remove_prefix(1);
                } else {
                    m_Current = m_Remainder;
                    m_Remainder = utilitycxx::string_view(nullptr, 0);
                }
                return;
            }

            auto pos = m_Remainder.find(m_Delimiter);
            if (pos == utilitycxx::string_view::npos) {
                m_Current = m_Remainder;
                m_Remainder = utilitycxx::string_view(nullptr, 0);
            } else {
                m_Current = m_Remainder.substr(0, pos);
                m_Remainder.remove_prefix(pos + m_Delimiter.size());
            }
        }

      public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = utilitycxx::string_view;
        using difference_type = std::ptrdiff_t;
        using pointer = const utilitycxx::string_view*;
        using reference = const utilitycxx::string_view&;

        iterator() = default;

        iterator(utilitycxx::string_view s, utilitycxx::string_view d)
            : m_Remainder(s), m_Delimiter(d), m_IsEnd(false) {
            advance();
        }

        reference operator*() const {
            return m_Current;
        }

        pointer operator->() const {
            return &m_Current;
        }

        iterator& operator++() {
            if (m_Remainder.data() == nullptr && m_Remainder.size() == 0) {
                m_IsEnd = true;
            } else {
                advance();
            }
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const iterator& a, const iterator& b) {
            if (a.m_IsEnd && b.m_IsEnd) {
                return true;
            }
            if (a.m_IsEnd != b.m_IsEnd) {
                return false;
            }
            return a.m_Remainder.data() == b.m_Remainder.data() &&
                   a.m_Remainder.size() == b.m_Remainder.size() &&
                   a.m_Current.data() == b.m_Current.data() &&
                   a.m_Current.size() == b.m_Current.size();
        }

        friend bool operator!=(const iterator& a, const iterator& b) {
            return !(a == b);
        }

      private:
        utilitycxx::string_view m_Remainder;
        utilitycxx::string_view m_Delimiter;
        utilitycxx::string_view m_Current;
        bool m_IsEnd = true;
    };

  private:
    utilitycxx::string_view str_;
    utilitycxx::string_view delim_;

  public:
    split_view() = default;
    split_view(utilitycxx::string_view str, utilitycxx::string_view delim)
        : str_(str), delim_(delim) {}

    iterator begin() const {
        return iterator(str_, delim_);
    }
    iterator end() const {
        return iterator();
    }
};

class split_char_view {
  public:
    class iterator {
      private:
        void advance() {
            if (m_IsEnd) {
                return;
            }
            if (m_Remainder.empty()) {
                m_IsEnd = true;
                return;
            }

            char d_arr[1] = {m_Delimiter};
            utilitycxx::string_view d(d_arr, 1);

            auto pos = m_Remainder.find(d);
            if (pos == utilitycxx::string_view::npos) {
                m_Current = m_Remainder;
                m_Remainder = utilitycxx::string_view(nullptr, 0);
            } else {
                m_Current = m_Remainder.substr(0, pos);
                m_Remainder.remove_prefix(pos + 1);
            }
        }

      public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = utilitycxx::string_view;
        using difference_type = std::ptrdiff_t;
        using pointer = const utilitycxx::string_view*;
        using reference = const utilitycxx::string_view&;

        iterator() = default;

        iterator(utilitycxx::string_view s, char d)
            : m_Remainder(s), m_Delimiter(d), m_IsEnd(false) {
            advance();
        }

        reference operator*() const {
            return m_Current;
        }
        pointer operator->() const {
            return &m_Current;
        }

        iterator& operator++() {
            if (m_Remainder.data() == nullptr && m_Remainder.size() == 0) {
                m_IsEnd = true;
            } else {
                advance();
            }
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const iterator& a, const iterator& b) {
            if (a.m_IsEnd && b.m_IsEnd) {
                return true;
            }
            if (a.m_IsEnd != b.m_IsEnd) {
                return false;
            }
            return a.m_Remainder.data() == b.m_Remainder.data() &&
                   a.m_Remainder.size() == b.m_Remainder.size() &&
                   a.m_Current.data() == b.m_Current.data() &&
                   a.m_Current.size() == b.m_Current.size();
        }

        friend bool operator!=(const iterator& a, const iterator& b) {
            return !(a == b);
        }

      private:
        utilitycxx::string_view m_Remainder;
        utilitycxx::string_view m_Current;
        char m_Delimiter = '\0';
        bool m_IsEnd = true;
    };

  public:
    split_char_view() = default;
    split_char_view(utilitycxx::string_view str, char delim)
        : m_Str(str), m_Delimiter(delim) {}

    iterator begin() const {
        return iterator(m_Str, m_Delimiter);
    }
    iterator end() const {
        return iterator();
    }

  private:
    utilitycxx::string_view m_Str;
    char m_Delimiter = '\0';
};

inline split_view split(utilitycxx::string_view str,
                        utilitycxx::string_view delim) {
    return split_view(str, delim);
}

inline split_char_view split(utilitycxx::string_view str, char delim) {
    return split_char_view(str, delim);
}

} // namespace ranges
} // namespace utilitycxx

#endif // UTILITYCXX_RANGES_STRING_VIEW_HPP_
