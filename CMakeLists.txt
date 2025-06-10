cmake_minimum_required(VERSION 3.14)
project(HBCompiler)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCE_FILES src/main.cpp src/lexer/lexer.cpp src/lexer/lexer_utils.cpp src/parser/parser.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(
    main
    ${SOURCE_FILES}
)
add_executable(
  lexer_test
  tests/lexer_test.cc
)

add_executable(
  parser_test
  tests/parser_test.cc
)

target_sources(lexer_test PRIVATE src/lexer/lexer.cpp src/lexer/lexer_utils.cpp src/parser/parser.cpp)
target_sources(parser_test PRIVATE src/lexer/lexer.cpp src/lexer/lexer_utils.cpp src/parser/parser.cpp)

target_link_libraries(
  lexer_test
  GTest::gtest_main
)

target_link_libraries(
  parser_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lexer_test)
gtest_discover_tests(parser_test)