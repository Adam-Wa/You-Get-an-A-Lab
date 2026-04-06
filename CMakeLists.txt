cmake_minimum_required(VERSION 3.24)

project(you_get_an_a)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(hello_test tests/hello_test.cpp src/hello.cpp)
add_executable(main main.cpp src/hello.cpp)

target_link_libraries(hello_test PRIVATE Catch2::Catch2WithMain)
