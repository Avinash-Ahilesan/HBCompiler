#include <gtest/gtest.h>
#include "../src/parser/parser.h"

TEST(FirstTest, BasicAssertions) {
    Token open_round;
    Token num;
    Token close_round;
    Token eof;

    open_round.token_type = TokenType::OPEN_ROUND_BRACKET;
    num.token_type = TokenType::VAL_INTEGER;
    close_round.token_type = TokenType::CLOSE_ROUND_BRACKET;
    eof.token_type = TokenType::END_OF_FILE;

    std::vector<Token> tokens;
    tokens.push_back(open_round);
    tokens.push_back(num);
    tokens.push_back(close_round);
    tokens.push_back(eof);
    Parser p(tokens);

    EXPECT_TRUE(p.parse());
}