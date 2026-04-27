#include <gtest/gtest.h>

#include <utilitycxx/tuple.hpp>

#include <functional>

int free_func(int a, int b) {
    return a * b;
}

struct Calculator {
    int state = 10;

    int add(int x) {
        return state + x;
    }
    int get_state() const {
        return state;
    }
};

TEST(TupleTest, InvokeFreeFunction) {
    int result = utilitycxx::invoke(free_func, 5, 4);
    EXPECT_EQ(result, 20);
}

TEST(TupleTest, InvokeLambda) {
    auto lambda = [](int x, int y) { return x + y; };
    int result = utilitycxx::invoke(lambda, 10, 20);
    EXPECT_EQ(result, 30);
}

TEST(TupleTest, InvokeMemberFunction) {
    Calculator calc;

    // Direct object
    int res1 = utilitycxx::invoke(&Calculator::add, calc, 5);
    EXPECT_EQ(res1, 15);

    // Pointer
    int res2 = utilitycxx::invoke(&Calculator::add, &calc, 10);
    EXPECT_EQ(res2, 20);

    // std::reference_wrapper
    int res3 = utilitycxx::invoke(&Calculator::add, std::ref(calc), 15);
    EXPECT_EQ(res3, 25);
}

TEST(TupleTest, InvokeMemberData) {
    Calculator calc;
    calc.state = 99;

    int res = utilitycxx::invoke(&Calculator::state, calc);
    EXPECT_EQ(res, 99);
}

TEST(TupleTest, ResultOfT) {
    // Compile-time check to ensure result_of_t works
    bool is_same =
        std::is_same<utilitycxx::result_of_t<decltype(free_func)*(int, int)>,
                     int>::value;
    EXPECT_TRUE(is_same);
}
