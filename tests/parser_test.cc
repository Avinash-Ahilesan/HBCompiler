#include <gtest/gtest.h>
#include "../src/parser/parser.h"


Token num = {TokenType::VAL_INTEGER};
Token name = {TokenType::IDENTIFIER};
Token open_rnd_brkt = {TokenType::OPEN_ROUND_BRACKET};
Token close_rnd_brkt = {TokenType::CLOSE_ROUND_BRACKET};
Token add = {TokenType::PLUS};
Token subtract = {TokenType::MINUS};
Token multiply = {TokenType::MULTIPLY};
Token divide = {TokenType::DIVIDE};
Token eof = {TokenType::END_OF_FILE};

TEST(ParserTestExpr, BasicAssertions) {

    std::vector<Token> tokens = {open_rnd_brkt, num, close_rnd_brkt, eof};

    Parser p(tokens);
    EXPECT_TRUE(p.parse());
}

TEST(ParserTestExprMissingEOF, BasicAssertions) {
    std::vector<Token> tokens = {open_rnd_brkt, num, close_rnd_brkt};

    Parser p(tokens);
    EXPECT_FALSE(p.parse());
}

TEST(ParserTestNumPlusNum, BasicAssertions) {
    // Goal -> Expr -> Term + Term Expr'-> Term + Term empty -> num + num

    std::vector<Token> tokens = {num, add, num, eof};

    Parser p(tokens);
    EXPECT_TRUE(p.parse());
}

TEST(ParserTestAdvancedExpressions, BasicAssertions) {
    Token num1 = {TokenType::VAL_INTEGER};
    Token plus = {TokenType::PLUS};
    Token name1 = {TokenType::IDENTIFIER};
    Token multiply = {TokenType::MULTIPLY};
    Token num2 = {TokenType::VAL_INTEGER};
    Token eof = {TokenType::END_OF_FILE};

    std::vector<Token> tokens = {num, add, name, multiply, num, eof};

    Parser p(tokens);
    EXPECT_TRUE(p.parse());
}

TEST(AllExprRelatedOps, BasicAssertions) {
    std::vector<Token> tokens = {open_rnd_brkt, num, add, name, close_rnd_brkt, multiply, open_rnd_brkt, name, divide, num, close_rnd_brkt, eof};

    Parser p(tokens);
    EXPECT_TRUE(p.parse());
}

TEST(MissingSymbolAddSubtractMultiplyDivide, BasicAssertions) {
    std::vector<Token> tokens = {num, num, eof};

    Parser p(tokens);
    EXPECT_FALSE(p.parse());

    tokens = {name, num, eof};

    Parser p2(tokens);
    EXPECT_FALSE(p2.parse());
    
}

TEST(MissingBracketOpenClose, BasicAssertions) {
    std::vector<Token> tokens = {open_rnd_brkt, num, eof};

    Parser p(tokens);
    EXPECT_FALSE(p.parse());

    tokens = {num, close_rnd_brkt, eof};

    Parser p2(tokens);
    EXPECT_FALSE(p2.parse());
}