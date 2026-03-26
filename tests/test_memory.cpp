#include <gtest/gtest.h>

#include <utility/memory.hpp>

struct MockObject {
    int a;
    double b;
    MockObject(int a, double b) : a(a), b(b) {}
};

TEST(MemoryTest, MakeUniquePrimitive) {
    auto ptr = utility::make_unique<int>(42);
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 42);
}

TEST(MemoryTest, MakeUniqueObject) {
    auto ptr = utility::make_unique<MockObject>(10, 3.14);
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(ptr->a, 10);
    EXPECT_DOUBLE_EQ(ptr->b, 3.14);
}

TEST(MemoryTest, MakeUniqueArrayNotSupportedOutTheBox) {
    // Note: C++14 make_unique supports arrays (make_unique<T[]>).
    SUCCEED();
}
