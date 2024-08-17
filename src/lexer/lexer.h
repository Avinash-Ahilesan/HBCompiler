#include <string>
#include <vector>
#include <fstream>


/*
    IDENTIFIER = character followed by numbers, characters, dashes, or underscores REGEX: ^[a-zA-Z_][a-zA-Z0-9_-]*$

    TYPES: int, float, string, char

    

*/

enum class TokenType {
    // identifier types
    VAL_INTEGER, VAL_FLOAT, VAL_CHAR, VAL_STRING,

    // variable
    IDENTIFIER,

    // comparison operators
    EQUALS, NOT_EQUALS, GREATER_THAN, LESS_THAN, GREAT_THAN_EQUALS, LESS_THAN_EQUALS,

    // LOGICAL OPERATOR
    AND, OR, NOT, NEGATE

};

class Lexer {

    public:
        std::vector<TokenType> tokenize();
        Lexer(std::string source_file);
    private:
        char input_buffer[4096];
        std::ifstream f;
        char next_character();
        TokenType lex_numerical_value();
        int curr_char;   // Current Character in Input Buffer
        std::streamsize bytes_read; // Bytes read by file scanner

};
