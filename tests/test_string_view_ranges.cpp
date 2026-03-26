#include <gtest/gtest.h>

#include <utility/string_view_ranges.hpp>

#include <vector>

// NOLINTNEXTLINE
using namespace utility::string_view_literals;

TEST(StringViewRangesTest, SplitByChar) {
    utility::string_view csv = "a,b,c";
    std::vector<utility::string_view> tokens;

    for (auto token : utility::ranges::split(csv, ',')) {
        tokens.push_back(token);
    }

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_TRUE(
        tokens[0] ==
        "a"_sv); // Needs explicit comparison logic or string_view operator==
    EXPECT_EQ(tokens[1].data()[0], 'b');
    EXPECT_EQ(tokens[2].data()[0], 'c');
}

TEST(StringViewRangesTest, SplitByStringView) {
    utility::string_view data = "one<sep>two<sep>three";
    utility::string_view delim = "<sep>"_sv;
    std::vector<utility::string_view> tokens;

    for (auto token : utility::ranges::split(data, delim)) {
        tokens.push_back(token);
    }

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].size(), 3); // "one"
    EXPECT_EQ(tokens[1].size(), 3); // "two"
    EXPECT_EQ(tokens[2].size(), 5); // "three"
}

TEST(StringViewRangesTest, SplitEmpty) {
    utility::string_view empty_sv = ""_sv;
    int count = 0;
    for (auto token : utility::ranges::split(empty_sv, ',')) {
        count++;
    }
    EXPECT_EQ(count, 0); // No tokens for an empty string
}
