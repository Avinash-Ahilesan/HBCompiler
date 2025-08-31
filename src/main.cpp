#include <iostream>
#include <string>
#include "./lexer/lexer.h"
#include "./fileio/file_interface.h"
#include "./lexer/lexer_utils.h"
#include "parser/parser.h"

using namespace std;

void printVectorTokens(std::vector<Token> vectorTokens) {
    for (auto i: vectorTokens) {
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
            case TokenType::VAL_STRING:
                std::cout << "string" << "\n";
                break;
            default:
                std::cout<< "other token" << "\n";
                break;
            }
        }
}

int main2() {
    Token open_round;
    Token num;
    Token close_round;
    Token eof;

    open_round.token_type = TokenType::OPEN_ROUND_BRACKET;
    num.token_type = TokenType::VAL_INTEGER;
    num.token_value = "21";
    close_round.token_type = TokenType::CLOSE_ROUND_BRACKET;
    eof.token_type = TokenType::END_OF_FILE;

    std::vector<Token> tokens;
    tokens.push_back(open_round);
    tokens.push_back(num);
    tokens.push_back(close_round);
    tokens.push_back(eof);
    Parser p(tokens);

    //std::cout << "PARSED: " << p.parse() << "\n";
    //std::cout << "\n";
    p.parse();
    std::cout << "\nPRINTING\n";
    p.printTree();
}

int main3() {
        // Goal -> Expr -> Term + Term Expr'-> Term + Term empty -> num + num
    Token num1;
    Token plus;
    Token num2;
    Token eof;

    num1.token_type = TokenType::VAL_INTEGER;
    num1.token_value = "3";
    plus.token_type = TokenType::PLUS;
    num2.token_type = TokenType::VAL_INTEGER;
    num2.token_value = "6";
    eof.token_type = TokenType::END_OF_FILE;

    std::vector<Token> tokens;
    tokens.push_back(num1);
    tokens.push_back(plus);
    tokens.push_back(num2);
    tokens.push_back(eof);
    Parser p(tokens);

    p.parse();
    p.printTree();
}

int main4() {
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

    
    // std::vector<Token> tokens = {num, multiply, num, eof};
    std::vector<Token> tokens = {open_rnd_brkt, num, add, name, close_rnd_brkt, multiply, open_rnd_brkt, name, divide, num, close_rnd_brkt, eof};    Parser p1(tokens);
    p1.parse();

    std::cout << p1.getTreeString();
}


int main() {
    // FileHandlerInterface* file_handler = new FileHandler();
    // Lexer l("./input/source.hb", file_handler);
    // std::vector<Token> vectorTokens = l.tokenize();

    // if (vectorTokens.size() != 4) {
    //     std::cout << "Expected 4 tokens, found " << vectorTokens.size();
    // }
    // std::cout << "Printing lexed items. Number of tokens: " << vectorTokens.size() << "\n";
    // printVectorTokens(vectorTokens);

    // FileHandlerInterface* file_handler2 = new FileHandler();
    // Lexer l2("./input/testComplex.hb", file_handler2);
    // std::vector<Token> vectorTokens2 = l2.tokenize();
    // std::cout << "NEW FILE \n";
    // printVectorTokens(vectorTokens2);

    //main3();
    main4();
}



