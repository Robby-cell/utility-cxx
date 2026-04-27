#include <gtest/gtest.h>

#include <utilitycxx/string_view.hpp>
#include <utilitycxx/string_view_hasher.hpp>
#include <utilitycxx/string_view_ostream.hpp>

#include <sstream>
#include <unordered_map>

// NOLINTNEXTLINE
using namespace utilitycxx::string_view_literals;

TEST(StringViewTest, LiteralAndSize) {
    utilitycxx::string_view sv = "Hello World"_sv;
    EXPECT_EQ(sv.size(), 11);
    EXPECT_FALSE(sv.empty());
}

TEST(StringViewTest, SubstrAndFind) {
    utilitycxx::string_view sv = "Hello World";

    auto sub = sv.substr(6, 5);
    EXPECT_TRUE(sub.starts_with("World"));
    EXPECT_TRUE(sv.ends_with("World"));

    EXPECT_EQ(sv.find("World"), 6);
    EXPECT_EQ(sv.find("xyz"), utilitycxx::string_view::npos);
}

TEST(StringViewTest, RemovePrefixSuffix) {
    utilitycxx::string_view sv = "[[Payload]]";
    sv.remove_prefix(2);
    sv.remove_suffix(2);

    EXPECT_TRUE(sv.starts_with("Payload"));
    EXPECT_EQ(sv.size(), 7);
}

TEST(StringViewTest, HasherWithUnorderedMap) {
    std::unordered_map<utilitycxx::string_view, int> map;
    map["apple"_sv] = 100;
    map["banana"_sv] = 200;

    EXPECT_EQ(map["apple"_sv], 100);
    EXPECT_EQ(map["banana"_sv], 200);
    EXPECT_EQ(map.count("cherry"_sv), 0);
}

TEST(StringViewTest, OutputStream) {
    utilitycxx::string_view sv = "Testing output"_sv;
    std::ostringstream oss;
    oss << sv;
    EXPECT_EQ(oss.str(), "Testing output");
}
