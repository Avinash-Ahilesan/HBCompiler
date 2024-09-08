#include <string>
#include <vector>
#include <fstream>


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
    EQUALS, NOT_EQUALS, GREATER_THAN, LESS_THAN, GREAT_THAN_EQUALS, LESS_THAN_EQUALS,

    // LOGICAL OPERATOR
    AND, OR, NOT, NEGATE

};

struct Token {
    TokenType token_type;
    unsigned char **token_value;
};

class Lexer {

    public:
        std::vector<Token> tokenize();
        Lexer(std::string source_file);
    private:
        char input_buffer[4096];
        std::ifstream f;
        bool match_keyword(std::string keyword);
        char next_character();
        void rollback();
        Token lex_numerical_value(char first_char);
        Token lex_starting_alphabetically(char first_char);
        int curr_char;   // Current Character in Input Buffer
        std::streamsize bytes_read; // Bytes read by file scanner

};
