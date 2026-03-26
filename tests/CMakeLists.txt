include(FetchContent)

# Fetch latest GTest compatible with older CMake if needed, or widely used release
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)

# Prevent installing gtest when building tests
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(utility_tests
    test_memory.cpp
    test_ranges_utility.cpp
    test_scope.cpp
    test_span.cpp
    test_string_view.cpp
    test_string_view_ranges.cpp
    test_tuple.cpp
)
target_link_libraries(utility_tests PRIVATE utility::utility gtest_main)

include(GoogleTest)
gtest_discover_tests(utility_tests)
