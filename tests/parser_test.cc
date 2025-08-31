#include <gtest/gtest.h>
#include "../src/parser/parser.h"


Token num = {TokenType::VAL_INTEGER, "5"};
Token num_2 = {TokenType::VAL_INTEGER, "12"};
Token name = {TokenType::IDENTIFIER, "my_name"};
Token open_rnd_brkt = {TokenType::OPEN_ROUND_BRACKET};
Token close_rnd_brkt = {TokenType::CLOSE_ROUND_BRACKET};
Token add = {TokenType::PLUS};
Token subtract = {TokenType::MINUS};
Token multiply = {TokenType::MULTIPLY};
Token divide = {TokenType::DIVIDE};
Token eof = {TokenType::END_OF_FILE};

Token type_integer = {TokenType::TYPE_INTEGER};
Token type_float = {TokenType::TYPE_INTEGER};
Token type_string = {TokenType::TYPE_INTEGER};
Token type_char = {TokenType::TYPE_INTEGER};

Token equals = {TokenType::EQUALS};

void assert_parse_tree_equals(std::vector<Token> tokens, std::string expected) {
    Parser p(tokens);
    p.parse();

    std::string parseTreeString = p.getTreeString();
    std::cout << parseTreeString;
    EXPECT_EQ(parseTreeString, expected);
}

TEST(ParserTestExpr, BasicAssertions) {

    std::vector<Token> tokens = {open_rnd_brkt, num, close_rnd_brkt, eof};
    assert_parse_tree_equals(tokens, "5");
}

TEST(ParserTestExprMissingEOF, BasicAssertions) {
    std::vector<Token> tokens = {open_rnd_brkt, num, close_rnd_brkt};
    Parser p(tokens);
    EXPECT_ANY_THROW(p.parse());
}

TEST(ParserTestNumPlusNum, BasicAssertions) {
    // Goal -> Expr -> Term + Term Expr'-> Term + Term empty -> num + num

    std::vector<Token> tokens = {num, add, num_2, eof};

    assert_parse_tree_equals(tokens, "(+ 5 12)");

}

TEST(ParserTestAdvancedExpressions, BasicAssertions) {

    std::vector<Token> tokens = {num, add, name, multiply, num, eof};

    assert_parse_tree_equals(tokens, "(+ 5 (* my_name 5))");
}

TEST(AllExprRelatedOps, BasicAssertions) {
    std::vector<Token> tokens = {open_rnd_brkt, num, add, name, close_rnd_brkt, multiply, open_rnd_brkt, name, divide, num, close_rnd_brkt, eof};
    assert_parse_tree_equals(tokens, "(* (+ 5 my_name) (/ my_name 5))");
}

TEST(DeeperBrackets, BasicAssertions) {
    std::vector<Token> tokens = {open_rnd_brkt, open_rnd_brkt, num, add, name, subtract, num_2, close_rnd_brkt, add, name, close_rnd_brkt, multiply, num, eof};
    assert_parse_tree_equals(tokens, "(* (+ (- (+ 5 my_name) 12) my_name) 5)");
}

TEST(MissingSymbolAddSubtractMultiplyDivide, BasicAssertions) {
    std::vector<Token> tokens = {num, num, eof};

    Parser p(tokens);

    EXPECT_ANY_THROW(p.parse());

    tokens = {name, num, eof};

    Parser p2(tokens);
    
    EXPECT_ANY_THROW(p.parse());
}

TEST(MissingBracketOpenClose, BasicAssertions) {
    std::vector<Token> tokens = {open_rnd_brkt, num, eof};

    Parser p(tokens);
    EXPECT_ANY_THROW(p.parse());    

    tokens = {num, close_rnd_brkt, eof};

    Parser p2(tokens);
    EXPECT_ANY_THROW(p.parse());
}

TEST(VariableDeclInteger, BasicAssertions) {
    std::vector<Token> tokens = {type_integer, name, equals, num, eof};

    Parser p(tokens);

}