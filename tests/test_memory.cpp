#include <gtest/gtest.h>

#include <utilitycxx/memory.hpp>

struct MockObject {
    int a;
    double b;
    MockObject(int a, double b) : a(a), b(b) {}
};

TEST(MemoryTest, MakeUniquePrimitive) {
    auto ptr = utilitycxx::make_unique<int>(42);
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 42);
}

TEST(MemoryTest, MakeUniqueObject) {
    auto ptr = utilitycxx::make_unique<MockObject>(10, 3.14);
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(ptr->a, 10);
    EXPECT_DOUBLE_EQ(ptr->b, 3.14);
}

TEST(MemoryTest, MakeUniqueArray) {
    auto ptr = utilitycxx::make_unique<int[]>(5);
    ASSERT_NE(ptr, nullptr);
    for (std::size_t i = 0; i < 5; ++i) {
        ptr[i] = i;
    }
    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(ptr[i], i);
    }
}

TEST(Memorytest, MakeUniqueArrayLifeTime) {
    static int count = 0;
    struct Lifetime {
        Lifetime() {
            ++count;
        }
        ~Lifetime() {
            --count;
        }
    };
    {
        auto ptr = utilitycxx::make_unique<Lifetime[]>(5);
        EXPECT_EQ(count, 5);
    }
    EXPECT_EQ(count, 0);
}
