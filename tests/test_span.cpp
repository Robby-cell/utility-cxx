#include <gtest/gtest.h>
#include <sstream>
#include <utility/span.hpp>
#include <utility/span_ostream.hpp>
#include <vector>

TEST(SpanTest, ConstructFromStdVector) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    // C++11 requires explicit type: utility::span<int>
    utility::span<int> s(vec);

    EXPECT_EQ(s.size(), 5);
    EXPECT_EQ(s.front(), 1);
    EXPECT_EQ(s.back(), 5);
}

TEST(SpanTest, ConstructFromCArray) {
    int arr[] = {10, 20, 30};
    utility::span<int> s(arr);

    EXPECT_EQ(s.size(), 3);
    EXPECT_EQ(s[1], 20);
}

TEST(SpanTest, ModifyUnderlyingData) {
    std::vector<int> vec = {0, 0, 0};
    utility::span<int> s(vec);

    s[1] = 42;
    EXPECT_EQ(vec[1], 42);
}

TEST(SpanTest, AsBytes) {
    int arr[] = {0x11223344};
    utility::span<int> s(arr);

    auto bytes = utility::as_bytes(s);
    EXPECT_EQ(bytes.size(), sizeof(int));
}

TEST(SpanTest, OutputStream) {
    std::vector<int> vec = {1, 2, 3};
    utility::span<int> s(vec);

    std::ostringstream oss;
    oss << s;
    EXPECT_EQ(oss.str(), "1, 2, 3");
}
