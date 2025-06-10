#include <gtest/gtest.h>
#include "../src/lexer/lexer.h"
#include "../src/fileio/file_interface.h"
#include "../src/lexer/lexer_utils.h"


void printVectorTokens(std::vector<Token> vector_tokens) {
        for (auto i: vector_tokens) {
        switch (i.token_type)
        {
        case TokenType::TYPE_INTEGER:
            /* code */
            std::cout<< "type integer" << "\n";
            break;
        case TokenType::VAL_INTEGER:
            std::cout << "val integer: " << i.token_value << " \n";
            break;
        case TokenType::IDENTIFIER:
            /* code */
            std::cout<< "identifier: " << i.token_value << " \n";
            break;
        case TokenType::EQUALS:
            /* code */
            std::cout<< "equals" << "\n";
            break;
        default:
            std::cout<< "other token" << "\n";
            break;
        }
    }
}

std::vector<Token> tokenizeString(std::string s) {
    TestFileHandler testFileHandler(s);
    FileHandlerInterface* file_handler = new FileHandler();
    Lexer l("empty", &testFileHandler); 
    return l.tokenize();
}

TEST(FirstTest, BasicAssertions) {
    EXPECT_STRNE("hello", "world");
    // Expect equality
    EXPECT_EQ(7 * 6, 42);
}

TEST(LexerOneLineIntegerAssignment, BasicAssertions) {
    std::string test_line = "int x = 5";
    TestFileHandler testFileHandler(test_line);
    EXPECT_EQ(testFileHandler.gcount(), 9);

    Lexer l("empty", &testFileHandler);
    std::vector<Token> vector = l.tokenize();
    EXPECT_EQ(vector.size(), 4);
    EXPECT_EQ(vector.at(0).token_type, TokenType::TYPE_INTEGER);
    EXPECT_EQ(vector.at(1).token_type, TokenType::IDENTIFIER);
    EXPECT_EQ(vector.at(2).token_type, TokenType::ASSIGNMENT);
    EXPECT_EQ(vector.at(3).token_type, TokenType::VAL_INTEGER);
    EXPECT_EQ(vector.at(3).token_value, "5");
}

TEST(LexerUtilsTest, BasicAssertions) {
    EXPECT_FALSE(LexerUtils::is_special_symbol('"'));
}

TEST(LexerTestStringEqualsFloat, BasicAssertions) {
    std::string stringEqualsAndFloat = "string == 101.5";
    std::vector<Token> tokens = tokenizeString(stringEqualsAndFloat);
    EXPECT_EQ(tokens.at(0).token_type, TokenType::TYPE_STRING);
    EXPECT_EQ(tokens.at(1).token_type, TokenType::EQUALS);
    EXPECT_EQ(tokens.at(2).token_type, TokenType::VAL_FLOAT);
    EXPECT_EQ(tokens.at(2).token_value, "101.5");
    
}

TEST(LexerTestLargerCode, BasicAssertions) {
    FileHandlerInterface* file_handler = new FileHandler();
    Lexer l("./input/testComplex.hb", file_handler);
    std::vector<Token> tokens = l.tokenize();
    EXPECT_EQ(tokens.at(0).token_type, TokenType::TYPE_INTEGER);
    EXPECT_EQ(tokens.at(1).token_type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens.at(2).token_type, TokenType::ASSIGNMENT);
    EXPECT_EQ(tokens.at(3).token_type, TokenType::VAL_INTEGER);
    EXPECT_EQ(tokens.at(4).token_type, TokenType::IF);
    EXPECT_EQ(tokens.at(5).token_type, TokenType::OPEN_ROUND_BRACKET);
    EXPECT_EQ(tokens.at(6).token_type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens.at(7).token_type, TokenType::EQUALS);
    EXPECT_EQ(tokens.at(8).token_type, TokenType::VAL_INTEGER);
    EXPECT_EQ(tokens.at(8).token_value, "5");
    EXPECT_EQ(tokens.at(9).token_type, TokenType::CLOSE_ROUND_BRACKET);
    EXPECT_EQ(tokens.at(10).token_type, TokenType::OPEN_CURLY_BRACKET);
    EXPECT_EQ(tokens.at(11).token_type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens.at(12).token_type, TokenType::OPEN_ROUND_BRACKET);
    EXPECT_EQ(tokens.at(13).token_type, TokenType::VAL_STRING);
    EXPECT_EQ(tokens.at(14).token_type, TokenType::CLOSE_ROUND_BRACKET);
    EXPECT_EQ(tokens.at(15).token_type, TokenType::CLOSE_CURLY_BRACKET);
    EXPECT_EQ(tokens.at(16).token_type, TokenType::ELSE);
    EXPECT_EQ(tokens.at(17).token_type, TokenType::OPEN_CURLY_BRACKET);
    EXPECT_EQ(tokens.at(18).token_type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens.at(19).token_type, TokenType::OPEN_ROUND_BRACKET);
    EXPECT_EQ(tokens.at(20).token_type, TokenType::VAL_STRING);
    EXPECT_EQ(tokens.at(21).token_type, TokenType::CLOSE_ROUND_BRACKET);
    EXPECT_EQ(tokens.at(22).token_type, TokenType::CLOSE_CURLY_BRACKET);
    
}
