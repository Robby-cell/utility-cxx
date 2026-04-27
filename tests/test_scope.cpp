#include <gtest/gtest.h>

#include <utilitycxx/scope.hpp>

TEST(ScopeTest, DeferExecutesOnDestruction) {
    int counter = 0;
    {
        auto d = utilitycxx::defer([&]() { counter++; });
        EXPECT_EQ(counter, 0); // Not executed yet
    }
    EXPECT_EQ(counter, 1); // Executed when `d` went out of scope
}

TEST(ScopeTest, DeferDisablePreventsExecution) {
    int counter = 0;
    {
        auto d = utilitycxx::defer([&]() { counter++; });
        EXPECT_EQ(counter, 0);
        d.disable(); // Cancel the defer
    }
    EXPECT_EQ(counter, 0); // Never executed
}

TEST(ScopeTest, DeferEnable) {
    int counter = 0;
    {
        auto d = utilitycxx::defer([&]() { counter++; });
        d.disable();
        d.enable(); // Re-enable
    }
    EXPECT_EQ(counter, 1);
}
