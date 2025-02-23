#include <string>
#include <vector>
#include <fstream>
#include "../fileio/file_interface.h"

/*
    IDENTIFIER = character followed by numbers, characters, dashes, or underscores REGEX: ^[a-zA-Z_][a-zA-Z0-9_-]*$

    TYPES: int, float, string, char

    COMPARISON:

    LOGICAL:

*/

enum class TokenType {

    // identifier types
    TYPE_INTEGER, TYPE_FLOAT, TYPE_CHAR, TYPE_STRING,

    // value types
    VAL_INTEGER, VAL_FLOAT, VAL_CHAR, VAL_STRING,

    // variable
    IDENTIFIER,

    // comparison operators
    EQUALS, NOT_EQUALS, GREATER_THAN, LESS_THAN, GREAT_THAN_EQUALS, LESS_THAN_EQUALS, ASSIGNMENT,

    // arithmetic
    PLUS, MINUS, MULTIPLY, DIVIDE, PLUS_EQUALS, MINUS_EQUALS, MULTIPLY_EQUALS, DIVIDE_EQUALS, MODULUS,

    // syntactic symnols
    OPEN_CURLY_BRACKET, CLOSE_CURLY_BRACKET, OPEN_ROUND_BRACKET, CLOSE_ROUND_BRACKET, END_STATEMENT,

    // LOGICAL OPERATOR
    AND, OR, NOT, NEGATE

};

struct Token {
    TokenType token_type;
    std::string token_value;
};

class Lexer {
    public:
        std::vector<Token> tokenize();
        Lexer(std::string source_file, FileHandlerInterface* file_handler);
    private:
        char input_buffer[4096];
        // std::ifstream f;
        FileHandlerInterface* file_handler_interface;
        bool match_keyword(std::string keyword, bool rollback_on_no_trailing_space);
        char next_character();
        void rollback();
        Token lex_numerical_value(char first_char);
        Token lex_starting_alphabetically(char first_char);
        Token lex_symbol(char first_char);
        int curr_char;   // Current Character in Input Buffer
        std::streamsize bytes_read; // Bytes read by file scanner

};
