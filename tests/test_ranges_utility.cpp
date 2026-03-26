#include <gtest/gtest.h>
#include <utility/ranges.hpp>
#include <vector>

TEST(RangesUtilityTest, ArrayDataAndSize) {
    int arr[5] = {1, 2, 3, 4, 5};

    EXPECT_EQ(utility::ranges::size(arr), 5);
    EXPECT_FALSE(utility::ranges::empty(arr));
    EXPECT_EQ(utility::ranges::data(arr), arr);
}

TEST(RangesUtilityTest, ContainerDataAndSize) {
    std::vector<int> vec;

    EXPECT_TRUE(utility::ranges::empty(vec));
    EXPECT_EQ(utility::ranges::size(vec), 0);

    vec.push_back(42);
    EXPECT_FALSE(utility::ranges::empty(vec));
    EXPECT_EQ(utility::ranges::size(vec), 1);
    EXPECT_EQ(*utility::ranges::begin(vec), 42);
}

TEST(RangesUtilityTest, AdlBeginEnd) {
    std::vector<int> vec = {10, 20};

    auto it = utility::ranges::begin(vec);
    EXPECT_EQ(*it, 10);

    auto end_it = utility::ranges::end(vec);
    EXPECT_EQ(std::distance(it, end_it), 2);
}

TEST(RangesUtilityTest, EmptyRangeStruct) {
    utility::ranges::empty_range er;

    EXPECT_TRUE(er.empty());
    EXPECT_EQ(er.size(), 0);
    EXPECT_EQ(er.data(), nullptr);
    EXPECT_TRUE(er.begin() == er.end());
}
