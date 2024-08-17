#include <string>
#include <vector>
#include <iostream>
#include "lexer.h"
#include "lexer_utils.h"
#include <fstream>




/*
[ i | n | t |  | E | | | ]
    Input: 
    Output: returns \0 if at end of file
*/
char Lexer::next_character() {
    curr_char++;
    if ((unsigned)curr_char > sizeof(input_buffer)- 1) {
        curr_char = -1;

        // read and re-fill buffer
        f.read(input_buffer, sizeof(input_buffer) - 1);
        bytes_read = f.gcount();
        std::cout << "Next Character: refilled buffer, read " << bytes_read << " bytes"; 
    }

    if (curr_char > bytes_read - 1 || bytes_read == 0) {
        std::cout << "Next Character: end of file";
        return '\0';   
    }

    return input_buffer[curr_char];
}

Lexer::Lexer(std::string source_file_name) {
    f.open(source_file_name);
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    curr_char = -1;
    bytes_read = 0;

    // initialize input buffer
    f.read(input_buffer, sizeof(input_buffer) - 1);
    bytes_read = f.gcount();
    std::cout << "Read in: " << bytes_read << "bytes \n";
    std::cout << "Init Input Buffer: " << input_buffer << "\n";
}


TokenType Lexer::lex_numerical_value() {
    char next_char = next_character();
    TokenType token_type = TokenType::VAL_INTEGER;
    while (next_char != '\0') {
        std::cout << "LEXING INT: " << next_char;
        if (next_char == '.') {
            token_type = TokenType::VAL_FLOAT;
        }
        if (!LexerUtils::is_digit(next_char)) {
            throw std::runtime_error("Error: could not lex integer");
        }
    }
    
    return token_type;
}

//
//  Read from file and create tokens 
//
std::vector<TokenType> Lexer::tokenize() {
    std::vector<TokenType> ret;


    TokenType type = TokenType::VAL_INTEGER;
    char next_char = next_character();
    while (next_char != '\0') {
        std::cout << next_char;
        if (next_char == ' ') {
            continue;
        }

        if (LexerUtils::is_digit(next_char)) {
            ret.push_back(lex_numerical_value());
        }

        if (LexerUtils::is_alphabetical(next_char)) {
            ret.push_back(lex_starting_alphabetical());
        }

        next_char = next_character();
    }
    ret.push_back(type);
    return ret;
}

