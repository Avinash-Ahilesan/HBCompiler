#include <string>
#include <vector>
#include <iostream>
#include "lexer.h"
#include "lexer_utils.h"
#include <fstream>




/*
    Input: None
    Output: Returns next character in buffer. returns \0 if at end of file
    TODO: make use of double buffer
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

void Lexer::rollback() {
    curr_char--;
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


Token Lexer::lex_numerical_value(char first_char) {
    char next_char = next_character();
    Token token;
    std::cout << "Lexing starting numerically";
    // while (next_char != '\0') {
    //     std::cout << "LEXING INT: " << next_char;
    //     if (next_char == '.') {
    //         token.token_type = TokenType::VAL_FLOAT;
    //     }
    //     if (!LexerUtils::is_digit(next_char)) {
    //         throw std::runtime_error("Error: could not lex integer");
    //     }
    // }
    
    return token;
}

bool Lexer::match_keyword(std::string keyword) {
    std::cout << "Attempting to match keyword: " << keyword;
    char next_char = next_character();
    for (int i = 1; i <= keyword.length(); i++) {
        if (i == keyword.length()) {
            if (next_char != ' ') {
                std::cout << "Failed match 1";
                for (int j  = i; j > 1; j--) {
                    rollback();
                } 
                return false;
            } else {
                return true;
            }
        } 
        if (keyword.at(i) == next_char) {
            next_char = next_character();
        } else {
            std::cout << "Failed match 2";
            // rollback if not a match() 
            for (int j  = i; j > 1; j--) {
                rollback();
            }
            return false;
        }
    }

    return true;
}

// i -> n -> t
Token Lexer::lex_starting_alphabetically(char first_char) {
    std::vector<std::string> identifierName;
    std::string intKeyword = "int";
    std::string floatKeyword = "float";
    std::string charKeyword = "char";
    std::string stringKeyword = "string";

    Token token;


    // can likely optimize this -> instead of rollback, keep processing if we fail to find a keyword
    switch (first_char) {
        case 'i':
            if (match_keyword(intKeyword)) {
                token.token_type = TokenType::TYPE_INTEGER;
                std::cout << "Found integer keyword\n";
            }
            return token;
            break;
        case 'f':
            if (match_keyword(floatKeyword)) {
                token.token_type = TokenType::TYPE_FLOAT;
            }
            std::cout << "Found float keyword\n";
            return token;
            break;
        case 'c':
            if (match_keyword(charKeyword)) {
                token.token_type = TokenType::TYPE_CHAR;
            } 
            return token;
            std::cout << "Found char keyword\n";
            break;
        case 's':
            if (match_keyword(stringKeyword)) {
                token.token_type = TokenType::TYPE_STRING;
            }
             std::cout << "Found string keyword\n";
            return token;
            break;
    }

    std::string identifier = "";
    identifier += first_char;
    char next_char = next_character();

    std::cout << "Lexing starting alphabeically\n";
    while (next_char != '\0') {

        if (next_char == ' ') {
            // found valid identifier
            // create new identifier + type
            token.token_type = TokenType::IDENTIFIER;
            std::cout << "identifier: " << identifier;
            return token;
            // token.token_value = malloc()
        }

        // abcd
        
        if (!LexerUtils::is_alphabetical(next_char) && !LexerUtils::is_digit(next_char) && !LexerUtils::is_underscore(next_char)) {
            throw std::runtime_error("Error: could not lex identifier");
        }
        identifier += next_char;
        next_char = next_character();
    }
    
    return token;
}

//
//  Read from file and create tokens 
//
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> ret;
    std::cout << "STARTING TOKENIZE\n";

    char next_char = next_character();
    while (next_char != '\0') {
        //std::cout << "main loop next char: " << next_char << "\n";
        if (next_char == ' ') {
            next_char = next_character();
            continue;
        }

        if (LexerUtils::is_digit(next_char)) {
            ret.push_back(lex_numerical_value(next_char));
        }

        if (LexerUtils::is_alphabetical(next_char)) {
            ret.push_back(lex_starting_alphabetically(next_char));
             
        }

        next_char = next_character();
    }

    Token placeholder_token;
    placeholder_token.token_type = TokenType::VAL_INTEGER;
    ret.push_back(placeholder_token);
    return ret;
}

