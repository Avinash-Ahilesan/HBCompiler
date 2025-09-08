#include <string>
#include <vector>
#include <iostream>
#include "lexer.h"
#include "lexer_utils.h"
#include "../fileio/file_interface.h"
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
        file_handler_interface->read(input_buffer, sizeof(input_buffer) - 1);
        bytes_read = file_handler_interface->gcount();
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

/*
    Constructor
    Input: Source file which will be opened. Error thrown in constructor if 
            file can't be opened
*/
Lexer::Lexer(std::string source_file_name, FileHandlerInterface* file_handler) {
    // f.open(source_file_name);
    file_handler->open(source_file_name);

    if (!file_handler->is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    curr_char = -1;
    bytes_read = 0;

    // initialize input buffer
    // f.read(input_buffer, sizeof(input_buffer) - 1);
    file_handler->read(input_buffer, sizeof(input_buffer) - 1);
    bytes_read = file_handler->gcount();
    this->file_handler_interface = file_handler;
    std::cout << "Read in: " << bytes_read << "bytes \n";
    std::cout << "Init Input Buffer: " << input_buffer << "\n";
}

// 
Token Lexer::lex_numerical_value(char first_char) {
    std::cout << "Lexing starting numerically";
    char next_char = next_character();
    Token token;
    token.token_type = TokenType::VAL_INTEGER;
    token.token_value += first_char;
    while (next_char != '\0' && next_char != '\n') {
        std::cout << "LEXING INT: " << next_char;
        if (next_char == '.') {
            token.token_type = TokenType::VAL_FLOAT;
        }
        else if (next_char == ' ' || LexerUtils::is_special_symbol(next_char)) {
            rollback();
            std::cout << "Found token: numerical";
            return token;
        }
        else if (!LexerUtils::is_digit(next_char)) {
            throw std::runtime_error("Error: could not lex integer");
        } 
        token.token_value += next_char;
        next_char = next_character();
    }
    std::cout << "Found token: numerical: " << token.token_value << "\n";
    return token;
}

bool Lexer::match_keyword(std::string keyword, bool rollback_on_no_trailing_space) {
    std::cout << "Attempting to match keyword: " << keyword;
    char next_char = next_character();
    std::cout << "matching, next char is: " << next_char << "\n";
    for (int i = 1; i <= keyword.length(); i++) {
        if (i == keyword.length()) {
            if (next_char != ' ' && rollback_on_no_trailing_space) {
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
            for (int j  = i; j >= 1; j--) {
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
    const std::string intKeyword = "int";
    const std::string floatKeyword = "float";
    const std::string charKeyword = "char";
    const std::string stringKeyword = "string";
    const std::string ifKeyword = "if";
    const std::string elseKeyword = "else";

    Token token;


    // can likely optimize this -> instead of rollback, keep processing if we fail to find a keyword
    switch (first_char) {
        case 'i':
            if (match_keyword(ifKeyword, true)) {
                token.token_type = TokenType::IF;
                std::cout << "Found if keyword\n";
                return token;
            }
            if (match_keyword(intKeyword, true)) {
                token.token_type = TokenType::TYPE_INTEGER;
                std::cout << "Found integer keyword\n";
                return token;
            }
            break;
        case 'f':
            if (match_keyword(floatKeyword, true)) {
                token.token_type = TokenType::TYPE_FLOAT;
            }
            std::cout << "Found float keyword\n";
            return token;
            break;
        case 'c':
            if (match_keyword(charKeyword, true)) {
                token.token_type = TokenType::TYPE_CHAR;
            } 
            return token;
            std::cout << "Found char keyword\n";
            break;
        case 's':
            if (match_keyword(stringKeyword, true)) {
                token.token_type = TokenType::TYPE_STRING;
            }
             std::cout << "Found string keyword\n";
            return token;
            break;
        case 'e':
            if (match_keyword(elseKeyword, true)) {
                token.token_type = TokenType::ELSE;
                return token;
                 std::cout << "Found else keyword\n";
            }
            break;
    }

    token.token_value += first_char;
    char next_char = next_character();

    token.token_type = TokenType::IDENTIFIER;
    std::cout << "Lexing starting alphabeically\n";
    while (next_char != '\0') {
        if (next_char == ' ' || next_char =='\n') {
            // found valid identifier
            // create new identifier + type
            std::cout << "identifier: " << token.token_value;
            return token;
        }
        
        // lets say the it's x- -> next symbol isn't alphabetical, or digit, or underscore, so we've found a word
        // however, I suppose if we have a random symbol that shouldn't be found, like @, then we'll catch on next iteration
        // when we try parse the next value
        if (!LexerUtils::is_alphabetical(next_char) && !LexerUtils::is_digit(next_char) && !LexerUtils::is_underscore(next_char)) {
            rollback();
            std::cout << "identifier: " << token.token_value;
            return token;
        }
        token.token_value += next_char;
        next_char = next_character();
    }
    
    return token;
}

Token Lexer::lex_symbol(char first_char) {
    // is_special symobl finds couple of special symbols: { | } = ! + - / % ( ) ; & * > <
    // however we should also identify some symbols like && and | | and -=, +=, *=, /=
    std::cout << "Lexing symbol";
    Token token;
    switch (first_char) {
        case '*':
            if (match_keyword("*=", false)) {
                token.token_type = TokenType::MULTIPLY_EQUALS;
            } else {
                token.token_type = TokenType::MULTIPLY;
            }
            break;
        case '/':
            if (match_keyword("/=", false)) {
                token.token_type = TokenType::DIVIDE_EQUALS;
            } else {
                token.token_type = TokenType::DIVIDE;
            }
            break;
        case '+':
            if (match_keyword("+=", false)) {
                token.token_type = TokenType::PLUS_EQUALS;
            } else {
                token.token_type = TokenType::PLUS;
            }
            break;
        case '-':
           if (match_keyword("-=", false)) {
                token.token_type = TokenType::MINUS_EQUALS;
           } else {
                token.token_type = TokenType::MINUS;
           }
           break;
        case '!':
            if (match_keyword("!=", false)) {
                token.token_type = TokenType::NOT_EQUALS;
            } else {
                token.token_type = TokenType::NOT;
            }
            break;
        case '{':
            token.token_type = TokenType::OPEN_CURLY_BRACKET;
            break;
        case '}':
            token.token_type = TokenType::CLOSE_CURLY_BRACKET;
            break;
        case '(':
            token.token_type = TokenType::OPEN_ROUND_BRACKET;
            break;
        case ')':
            token.token_type = TokenType::CLOSE_ROUND_BRACKET;
            break;
        case '<':
            if (match_keyword("<=", false)) {
                token.token_type = TokenType::LESS_THAN_EQUALS;
            } else {
                token.token_type = TokenType::LESS_THAN;
            }
            break;
        case '>':
            if (match_keyword(">=", false)) {
                token.token_type = TokenType::GREAT_THAN_EQUALS;
            } else {
                token.token_type = TokenType::GREATER_THAN;
            }
            break;    
        case '%':
            token.token_type = TokenType::MODULUS;
            break;
        case '&':
            if (match_keyword("&&", false)) {
                token.token_type = TokenType::AND;
            }
            break;
        case '|':
            if (match_keyword("||", false)) {
                token.token_type = TokenType::OR;
            }
            break;
        case '=':
            if (match_keyword("==", false)) {
                token.token_type= TokenType::EQUALS;
            } else {
                token.token_type = TokenType::ASSIGNMENT;
            }
            break;
    }
    return token;
}

Token Lexer::lex_string(char next_char) {
    std::string str = "";
    next_char = next_character();
    Token token;
    while (!LexerUtils::is_double_quote(next_char)) {
        if (next_char == '\0') {
            throw std::runtime_error("No closing quote for string");
        }
        str += next_char;
        next_char = next_character();
    }

    token.token_type = TokenType::VAL_STRING;
    token.token_value = str;
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
        std::cout << "main loop next char: " << next_char << "\n";
        if (next_char == ' ' || next_char == '\n') {
            next_char = next_character();
            continue;
        }

        if (LexerUtils::is_digit(next_char)) {
            Token retToken = lex_numerical_value(next_char);
            ret.push_back(retToken);
        }
        else if (LexerUtils::is_alphabetical(next_char)) {
            Token retToken = lex_starting_alphabetically(next_char);
            ret.push_back(retToken);
        }
        else if (LexerUtils::is_special_symbol(next_char)) {
            Token retToken = lex_symbol(next_char);
            ret.push_back(retToken);
        } else if (LexerUtils::is_double_quote(next_char)) {
            Token retToken = lex_string(next_char);
            ret.push_back(retToken);
        }
        else {
            throw std::runtime_error("Failed main lexing loop: error\n");
        }

        next_char = next_character();
    }

    return ret;
}

