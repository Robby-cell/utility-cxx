#include <gtest/gtest.h>

#include <utility/internal/version.hpp>

#if !UTILITY_HAS_CPP20
#error "test_format.cpp must be compiled with C++20 or higher!"
#endif

#include <utility/span.hpp>
#include <utility/span_format.hpp>
#include <utility/string_view.hpp>
#include <utility/string_view_format.hpp>

#include <format>
#include <vector>

// NOLINTNEXTLINE
using namespace utility::string_view_literals;

TEST(FormatTest, StringViewFormatBasic) {
    auto sv = "Hello Format"_sv;

    auto result = std::format("Message: {}", sv);
    EXPECT_EQ(result, "Message: Hello Format");
}

TEST(FormatTest, StringViewFormatAlignmentAndWidth) {
    auto sv = "C++20"_sv;

    // Test that std::format's native formatting specs still work correctly
    auto left_align = std::format(">{:<10}<", sv);
    EXPECT_EQ(left_align, ">C++20     <");

    auto right_align = std::format(">{:>10}<", sv);
    EXPECT_EQ(right_align, ">     C++20<");
}

TEST(FormatTest, SpanFormatBasic) {
    std::vector<int> vec = {10, 20, 30};
    utility::span<int> s(vec);

    auto result = std::format("{}", s);
    EXPECT_EQ(result, "10, 20, 30");
}
