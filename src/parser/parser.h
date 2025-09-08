#include <vector>
#include <variant>
#include "../lexer/lexer.h"
#include "../ast/node.h"

class Parser {

    public:
        Parser(std::vector<Token> token_list);
        Goal g;
        void parse();
        std::string identifier_and_equals();
        void printTree();
        std::string getTreeString();

    private:
        int curr_index = -1;
        Token* curr_word;
        void next_word();
        std::variant<Factor, std::shared_ptr<Expr>> expr();
        std::variant<Factor, std::shared_ptr<Expr>> expr_prime(std::variant<Factor, std::shared_ptr<Expr>> expr);
        std::variant<Factor, std::shared_ptr<Expr>>  term();
        std::variant<Factor, std::shared_ptr<Expr>> term_prime(std::variant<Factor, std::shared_ptr<Expr>> expr);
        std::variant<Factor, std::shared_ptr<Expr>> factor();
        void check_eof();
        VariableDeclaration variable_decl();
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