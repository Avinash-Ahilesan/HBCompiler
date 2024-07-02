#include <string>
#include <vector>
#include <iostream>
#include "lexer.h"



std::vector<TokenType> lexer::tokenize(std::string input) {
    std::vector<TokenType> ret;
    std::cout << input;
    TokenType type = TokenType::INTEGER;
    ret.push_back(type);
    return ret;
}

