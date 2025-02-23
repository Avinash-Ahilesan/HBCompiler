#include <gtest/gtest.h>
#include "../src/lexer/lexer.h"
#include "../src/fileio/file_interface.h"

std::string testString = "int x = 5";

TEST(FirstTest, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    // Expect equality
    EXPECT_EQ(7 * 6, 42);
}

TEST(LexerOneLine, BasicAssertions) {
    TestFileHandler testFileHandler(testString);
    FileHandlerInterface* file_handler = new FileHandler();
    std::string fileName = "test";
    Lexer l("empty", &testFileHandler);
    EXPECT_EQ(testFileHandler.gcount(), 9);
}