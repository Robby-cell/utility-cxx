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

add_executable(test_utility test_utility.cpp)
target_link_libraries(test_utility PRIVATE utility::utility gtest_main)

include(GoogleTest)
gtest_discover_tests(test_utility)
