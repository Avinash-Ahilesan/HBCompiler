#include <vector>
#include "../lexer/lexer.h"

class Parser {

    public:
        Parser(std::vector<Token> token_list);
        bool parse();

    private:
        int curr_index = -1;
        Token* curr_word;
        void next_word();
        bool expr();
        bool expr_prime();
        bool term();
        bool term_prime();
        bool factor();
        template<typename... Args>
        bool is_one_of(Token* t, Args... args) {
            for (TokenType token_type : {args...}) {
                if (t->token_type == token_type) {
                    return true;
                }
            }
            return false;
        }
        std::vector<Token> word_list;
};