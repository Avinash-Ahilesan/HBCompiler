#include "../parser/parser.h"
#include <iostream>

Parser::Parser(std::vector<Token> token_list)
{
    this->word_list = token_list;
}

std::string getTokenType(TokenType token_type)
{
    switch (token_type)
    {
        case TokenType::TYPE_INTEGER:
            /* code */
            return "type integer \n";
            break;
        case TokenType::VAL_INTEGER:
            return "val integer: \n";
            break;
        case TokenType::IDENTIFIER:
            /* code */
            return "identifier: \n";
            break;
        case TokenType::EQUALS:
            /* code */
            return "equals \n";
            break;
        case TokenType::VAL_STRING:
            return "string \n";
            break;
        default:
            return "other token \n";
            break;
    }
}

void printTokenType(TokenType token_type) {
    std::cout << getTokenType(token_type);
}

void Parser::next_word()
{
    if (++curr_index >= word_list.size())
    {
        curr_word = NULL; // signify end of list
    }
    curr_word = &word_list[curr_index];
}

void Parser::check_eof() {
    if (!is_one_of(this->curr_word, TokenType::END_OF_FILE)) {
        throw std::runtime_error("Last character is not end of file");
    }
}

template <typename ToVariant, typename FromVariant>
ToVariant ConvertVariant(const FromVariant& from) {
    ToVariant to = std::visit([](auto&& arg) -> ToVariant {return arg ; }, from);
    return to;
}

void Parser::parse()
{
    // Entry point Goal -> Expr
    this->next_word();
    if (is_one_of(this->curr_word, TokenType::TYPE_INTEGER, TokenType::TYPE_CHAR, TokenType::TYPE_FLOAT, TokenType::TYPE_STRING))
    {
        auto var_decl = variable_decl();
        check_eof();
        Statement s{
            var_decl
        };
        g.statement_list.push_back(s);
        return;
    }
    else
    {
        auto expression = expr();
        check_eof();
        Statement s {
            ConvertVariant<decltype(s.statement), decltype(expression)>(expression)
        };
        g.statement_list.push_back(s);
    }
}

// Expr -> Term Expr'
std::variant<Factor, std::shared_ptr<Expr>> Parser::expr()
{
    auto left = term();
    return expr_prime(left);
}

// Term -> Factor Term'
std::variant<Factor, std::shared_ptr<Expr>> Parser::term()
{
    auto lhs = factor();
    return term_prime(lhs);
}

// Term' -> x Factor Term'
//       -> / Factor Term'
//       -> empty
std::variant<Factor, std::shared_ptr<Expr>> Parser::term_prime(std::variant<Factor, std::shared_ptr<Expr>> lhs)
{
    if (is_one_of(this->curr_word, TokenType::MULTIPLY, TokenType::DIVIDE))
    {
        std::shared_ptr<Expr> expr(new Expr);
        expr->lhs = lhs;
        if (is_one_of(this->curr_word, TokenType::MULTIPLY))
        {
            expr->op = Operator::MULTIPLY;
        }
        if (is_one_of(this->curr_word, TokenType::DIVIDE))
        {
            expr->op = Operator::DIVIDE;
        }
        this->next_word();
        printTokenType(this->curr_word->token_type);
        std::variant<Factor , std::shared_ptr<Expr>> rhs = factor();
        expr->rhs = rhs;
        return term_prime(expr);
    }
    if (is_one_of(this->curr_word, TokenType::PLUS, TokenType::MINUS, TokenType::CLOSE_ROUND_BRACKET, TokenType::END_OF_FILE))
    {
        // we can eat an epsilon if the term that follows is )
        // FOLLOW(term') = + - )
        return lhs;
    }
    throw std::runtime_error("Not a term prime");
}

//
std::variant<Factor, std::shared_ptr<Expr>> Parser::expr_prime(std::variant<Factor, std::shared_ptr<Expr>> lhs)
{
    if (is_one_of(this->curr_word, TokenType::PLUS, TokenType::MINUS))
    {
        std::shared_ptr<Expr> expr(new Expr);
        expr->lhs = lhs;
        if (is_one_of(this->curr_word, TokenType::PLUS))
        {
            expr->op = Operator::ADD;
        }
        if (is_one_of(this->curr_word, TokenType::MINUS))
        {
            expr->op = Operator::SUBTRACT;
        }

        this->next_word();

        std::variant<Factor, std::shared_ptr<Expr>> rhs = term();
        expr->rhs = rhs;
        return expr_prime(expr);
    }
    if (is_one_of(this->curr_word, TokenType::CLOSE_ROUND_BRACKET, TokenType::END_OF_FILE))
    {
        return lhs;
    }
    throw std::runtime_error("not an expression prime");
}

std::variant<Factor, std::shared_ptr<Expr>> Parser::factor()
{
    if (is_one_of(this->curr_word, TokenType::OPEN_ROUND_BRACKET))
    {
        this->next_word();
        auto sub_expr = expr();
        if (is_one_of(this->curr_word, TokenType::CLOSE_ROUND_BRACKET))
        {
            this->next_word();
            return sub_expr;
        }
        throw std::runtime_error("Unmatched opening bracket");
    }
    else if (is_one_of(this->curr_word, TokenType::VAL_INTEGER, TokenType::IDENTIFIER))
    {
        // create correct node type
        Factor factor;
        if (is_one_of(this->curr_word, TokenType::VAL_INTEGER))
        {
            Num num;
            num.num = std::stoi(this->curr_word->token_value);
            factor.variant = num;
        } 
        else if (is_one_of(this->curr_word, TokenType::IDENTIFIER)) {
            Name name;
            name.name =  this->curr_word->token_value;
            factor.variant = name;
        }
        this->next_word();
        return factor;
    }
    throw std::runtime_error("Not a factor");
}

std::string Parser::identifier_and_equals()
{
    this->next_word();
    std::string ident_name;
    if (is_one_of(this->curr_word, TokenType::IDENTIFIER))
    {
        ident_name = this->curr_word->token_value;
        this->next_word();
        if (is_one_of(this->curr_word, TokenType::EQUALS))
        {
            return ident_name;
        }
    }
    throw std::runtime_error("expecting identifier, instead found: " + getTokenType(this->curr_word->token_type));
}

VariableDeclaration Parser::variable_decl()
{
    VariableDeclaration variable_decl;
    if (is_one_of(this->curr_word, TokenType::TYPE_INTEGER))
    {
        std::string variable_identifier = identifier_and_equals();

        variable_decl.var_type = VariableType::INTEGER;
        variable_decl.name = variable_identifier;
        this->next_word();
        if (is_one_of(this->curr_word, TokenType::VAL_INTEGER))
        {
            variable_decl.value = std::stoi(this->curr_word->token_value);
            this->next_word();
            return variable_decl;       
        } else if (is_one_of(this->curr_word, TokenType::IDENTIFIER)) {
            Name name;
            name.name = this->curr_word->token_value;
            variable_decl.value = name;
            this->next_word();
            return variable_decl;
        }
        throw std::runtime_error("TYPE MISMATCH: INT ASSIGNMENT MISMATCH");
    }
    else if (is_one_of(this->curr_word, TokenType::TYPE_STRING))
    {
        std::string variable_identifier = identifier_and_equals();

        variable_decl.var_type = VariableType::STRING;
        variable_decl.name = variable_identifier;
        this->next_word();
        if (is_one_of(this->curr_word, TokenType::VAL_STRING)) 
        {
            variable_decl.value = this->curr_word->token_value;
            this->next_word();
            return variable_decl;
        } else if (is_one_of(this->curr_word, TokenType::IDENTIFIER)) {
            Name name;
            name.name = this->curr_word->token_value;
            variable_decl.value = name;
            this->next_word();
            return variable_decl;
        }
        throw std::runtime_error("TYPE MISMATCH: STRING ASSIGNMENT MISMATCH");
    }
}

template <typename... Ts>
struct Overload : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

std::string Parser::getTreeString()
{

    std::function<std::string(std::vector<Statement>)> goal_to_string;
    std::function<std::string(std::variant<Factor, std::shared_ptr<Expr>, VariableDeclaration, IfStatement, WhileStatement>)> statement_to_string;

    std::function<std::string(std::variant<Factor, std::shared_ptr<Expr>>)> expr_to_string;

    std::function<std::string(Operator)> op_to_string;

    op_to_string = [&](Operator op) -> std::string {
        if (op == Operator::ADD) return "+";
        if (op == Operator::DIVIDE) return "/";
        if (op == Operator::MULTIPLY) return "*";
        if (op == Operator::SUBTRACT) return "-";
        return "B";
    };

    std::function<std::string(FactorVariant)> factor_to_string;

    factor_to_string = [&](FactorVariant factor) -> std::string
    {
        return std::visit(Overload{[&](Name e) -> std::string
                            {
                                return e.name;
                            },
                            [&](Num f) -> std::string
                            {
                                return std::to_string(f.num);
                            }},
                   factor);
    };

    
    std::function<std::string(VariableDeclaration)> variable_decl_to_string = [&](VariableDeclaration node) -> std::string {
        return std::visit(
            Overload{[&](int i) -> std::string
                {
                    return std::to_string(i);
                },
                [&](std::string str) -> std::string
                {
                    return "\"" + str + "\"";
                },
                [&](Name n) -> std::string
                {
                    return n.name;
                }, 
            }
            , 
            node.value
        );
    };

    statement_to_string = [&](std::variant<Factor, std::shared_ptr<Expr>, VariableDeclaration, IfStatement, WhileStatement> node) -> std::string
    {
        return std::visit(Overload{[&](std::shared_ptr<Expr> e) -> std::string
                            {
                                return expr_to_string(e);
                            },
                            [&](Factor f) -> std::string
                            {
                                return expr_to_string(f);
                            },
                            [&](VariableDeclaration d) -> std::string
                            {
                                return "(decl " + d.name + " " + variable_decl_to_string(d) + ")";
                            },
                            [&](IfStatement d) -> std::string
                            {
                                return "test";
                            },
                            [&](WhileStatement d) -> std::string
                            {
                                return "test2";
                            }  
                        },
                   node);
    };

    expr_to_string = [&](std::variant<Factor, std::shared_ptr<Expr>> node) -> std::string
    {
        return std::visit(Overload{[&](std::shared_ptr<Expr> e) -> std::string
                            {
                                std::cout << "EXPR TO STRING";
                                std::string lhs = expr_to_string(e->lhs);
                                std::string rhs = expr_to_string(e->rhs);
                                return "(" + op_to_string(e->op) + " " + lhs + " " + rhs + ")";
                            },
                            [&](Factor f) -> std::string
                            {
                                std::cout << "FACTOR TO STRING";
                                return factor_to_string(f.variant);
                                return "F";
                            }},
                   node);
    };

    goal_to_string = [&](std::vector<Statement> statement_list) -> std::string
    {
        std::string strs;
        for (Statement stmt: statement_list) {
            strs += statement_to_string(stmt.statement);
        }
        return strs;
    };

    std::cout << "TO STRING";
    return goal_to_string(g.statement_list);
}

void Parser::printTree()
{
    std::cout << "\n";
    std::cout << getTreeString();
}
