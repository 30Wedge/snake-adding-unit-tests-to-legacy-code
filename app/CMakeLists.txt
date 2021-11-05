cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)
# set debug symbols for all builds
set(CMAKE_BUILD_TYPE Debug)

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
add_definitions(-DVERSION="1.0.1")
add_library(conio INTERFACE )
target_include_directories(conio INTERFACE src/conio)
add_executable(
  snake
  src/snake.c
)
target_link_libraries(
    snake
    conio
)

## Build the Test executable
add_definitions(-DVERSION="1.0.1")

# Build the entire snake app as a library we can link against
add_library(
  snake_lib
  SHARED
    src/snake.c
)
target_include_directories(snake_lib PRIVATE test/fff test/mock )

# build the test executable
add_executable(
  snake_test
  test/snake_test.cpp
)
target_include_directories(snake_test PUBLIC src test/fff test/mock)
target_link_libraries(
  snake_test
  gtest_main
  snake_lib
)

include(GoogleTest)
gtest_discover_tests(snake_test)