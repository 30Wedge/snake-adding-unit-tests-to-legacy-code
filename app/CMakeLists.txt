cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

## Build the App Executable
add_library(conio INTERFACE )
target_include_directories(conio INTERFACE src/conio)
add_definitions(-DVERSION="1.0.1")
add_executable(
  snake
  src/snake.c
)
target_link_libraries(
    snake
    conio
)

## Test executable
add_executable(
  snake_test
  test/snake_test.cpp
)
target_include_directories(snake_test PRIVATE test/fff)
target_link_libraries(
  snake_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(snake_test)