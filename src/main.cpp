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
    close_round.token_type = TokenType::CLOSE_ROUND_BRACKET;
    eof.token_type = TokenType::END_OF_FILE;

    std::vector<Token> tokens;
    tokens.push_back(open_round);
    tokens.push_back(num);
    tokens.push_back(close_round);
    tokens.push_back(eof);
    Parser p(tokens);

    std::cout << "PARSED: " << p.parse();
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

    main2();
}



