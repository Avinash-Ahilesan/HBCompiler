#include <string>
#include <vector>


enum class TokenType {
    INTEGER
};

class lexer {

    

    public:
        std::vector<TokenType> tokenize(std::string input);
};
