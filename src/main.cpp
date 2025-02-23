#include <iostream>
#include <string>
#include "./lexer/lexer.h"
#include "./fileio/file_interface.h"

using namespace std;

int main() {
    FileHandlerInterface* file_handler = new FileHandler();
    Lexer l("./input/source.hb", file_handler);
    std::vector<Token> vectorTokens = l.tokenize();

    if (vectorTokens.size() != 4) {
        std::cout << "Expected 4 tokens, found " << vectorTokens.size();
    }
    std::cout << "Printing lexed items. Number of tokens: " << vectorTokens.size() << "\n";
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
        default:
            std::cout<< "other token" << "\n";
            break;
        }
    }
}

