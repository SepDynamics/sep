include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# The gtest_discover_tests function is now available, so we can enable testing
enable_testing()

# The GTest::gtest and GTest::gtest_main targets are now available to link against
