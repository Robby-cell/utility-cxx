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

add_library(utility_tests_cpp20 OBJECT test_format_cpp20.cpp)

# Force C++20 just for this target
set_target_properties(utility_tests_cpp20 PROPERTIES 
    CXX_STANDARD 20 
    CXX_STANDARD_REQUIRED ON
)
target_link_libraries(utility_tests_cpp20 PRIVATE utility::utility gtest)

add_executable(utility_tests
    test_memory.cpp
    test_ranges_utility.cpp
    test_scope.cpp
    test_span.cpp
    test_string_view.cpp
    test_string_view_ranges.cpp
    test_tuple.cpp
)
set_target_properties(utility_tests PROPERTIES 
    CXX_STANDARD 11 
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(utility_tests PRIVATE 
    utility::utility 
    gtest_main
    utility_tests_cpp20 # Links the C++20 object files safely
)

include(GoogleTest)
gtest_discover_tests(utility_tests)
