#include "../parser/parser.h"
#include <iostream>

Parser::Parser(std::vector<Token> token_list)
{
    this->word_list = token_list;
}

void printTokenType(TokenType token_type)
{
    switch (token_type)
    {
    case TokenType::TYPE_INTEGER:
        /* code */
        std::cout << "type integer \n";
        break;
    case TokenType::VAL_INTEGER:
        std::cout << "val integer: \n";
        break;
    case TokenType::IDENTIFIER:
        /* code */
        std::cout << "identifier: \n";
        break;
    case TokenType::EQUALS:
        /* code */
        std::cout << "equals \n";
        break;
    case TokenType::VAL_STRING:
        std::cout << "string \n";
        break;
    default:
        std::cout << "other token \n";
        break;
    }
}

void Parser::next_word()
{
    if (++curr_index >= word_list.size())
    {
        curr_word = NULL; // signify end of list
    }
    curr_word = &word_list[curr_index];
}

void Parser::parse()
{
    // Entry point Goal -> Expr
    std::cout << "IN PARSE";
    this->next_word();
    if (is_one_of(this->curr_word, TokenType::TYPE_INTEGER))
    {
        if (variable_decl())
        {
            if (!is_one_of(this->curr_word, TokenType::END_OF_FILE))
            {
                throw std::runtime_error("Last character is not end of file");
            }
        }
        return;
    }
    else
    {
        auto e = expr();
        if (!is_one_of(this->curr_word, TokenType::END_OF_FILE))
        {
            throw std::runtime_error("Last character is not end of file");
        }
        g.expr = e;
        return;
    }
    throw std::runtime_error("Not an expression");
}

// Expr -> Term Expr'
std::variant<Factor *, Expr *> Parser::expr()
{
    // 5 * 5  
    std::cout << "IN EXPR";
    auto left = term();
    return expr_prime(left);
    std::cout << "Not a term";
}

// Term -> Factor Term'
std::variant<Factor *, Expr *> Parser::term()
{
    std::cout << "IN TERM";
    // Expr
    auto lhs = factor();
    return term_prime(lhs);
}

// Term' -> x Factor Term'
//       -> / Factor Term'
//       -> empty
std::variant<Factor*, Expr*> Parser::term_prime(std::variant<Factor *, Expr *> lhs)
{
    std::cout << "IN TERM'";
    if (is_one_of(this->curr_word, TokenType::MULTIPLY, TokenType::DIVIDE))
    {
        Expr* expr = new Expr();
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
        std::cout << "next word";
        printTokenType(this->curr_word->token_type);
        std::variant<Factor *, Expr *> rhs = factor();
        expr->rhs = rhs;
        return term_prime(expr);
    }
    if (is_one_of(this->curr_word, TokenType::PLUS, TokenType::MINUS, TokenType::CLOSE_ROUND_BRACKET, TokenType::END_OF_FILE))
    {
        // we can eat an epsilon if the term that follows is )
        // FOLLOW(term') = + - )
        std::cout << "term': true from follow";
        return lhs;
    }
    throw std::runtime_error("Not a term prime");
}

//
std::variant<Factor *, Expr *> Parser::expr_prime(std::variant<Factor *, Expr *> lhs)
{
    std::cout << "IN EXPR'";
    if (is_one_of(this->curr_word, TokenType::PLUS, TokenType::MINUS))
    {
        Expr *expr = new Expr();
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

        std::variant<Factor *, Expr *> rhs = term();
        expr->rhs = rhs;
        return expr_prime(expr);
    }
    if (is_one_of(this->curr_word, TokenType::CLOSE_ROUND_BRACKET, TokenType::END_OF_FILE))
    {
        return lhs;
    }
    throw std::runtime_error("not an expression prime");
}

std::variant<Factor *, Expr *> Parser::factor()
{
    std::cout << "IN FACTOR";
    if (is_one_of(this->curr_word, TokenType::OPEN_ROUND_BRACKET))
    {
        this->next_word();
        std::cout << "creating expr for factor, brackets detected";
        auto sub_expr = expr();
        if (is_one_of(this->curr_word, TokenType::CLOSE_ROUND_BRACKET))
        {
            this->next_word();
            std::cout << "returning expr";
            return sub_expr;
        }
        throw std::runtime_error("Unmatched opening bracket");
    }
    else if (is_one_of(this->curr_word, TokenType::VAL_INTEGER, TokenType::IDENTIFIER))
    {
        // create correct node type
        Factor *factor = new Factor();
        if (is_one_of(this->curr_word, TokenType::VAL_INTEGER))
        {
            std::cout << "processing val int";
            Num *num = new Num();
            num->num = std::stoi(this->curr_word->token_value);
            factor->variant = num;
        } 
        else if (is_one_of(this->curr_word, TokenType::IDENTIFIER)) {
            std::cout << "processing identifier";
            Name *name = new Name();
            name->name = new std::string(this->curr_word->token_value);
            factor->variant = name;
        }
        this->next_word();
        return factor;
    }
    throw std::runtime_error("Not a factor");
}

bool Parser::identifier_and_equals()
{
    this->next_word();
    if (is_one_of(this->curr_word, TokenType::IDENTIFIER))
    {
        this->next_word();
        if (is_one_of(this->curr_word, TokenType::EQUALS))
        {
            return true;
        }
        else
        {
            std::cout << "expecting equals, found something else";
        }
    }
    else
    {
        std::cout << "expecting identifier, found something else: ";
        printTokenType(this->curr_word->token_type);
    }

    return false;
}

bool Parser::variable_decl()
{
    if (is_one_of(this->curr_word, TokenType::TYPE_INTEGER))
    {
        if (identifier_and_equals())
        {
            this->next_word();
            if (is_one_of(this->curr_word, TokenType::VAL_INTEGER))
            {
                return true;
            }
            else
            {
                std::cout << "TYPE MISMATCH: INT ASSIGNMENT MISMATCH";
            }
        }
    }
    else if (is_one_of(this->curr_word, TokenType::TYPE_STRING))
    {
        if (identifier_and_equals())
        {
            this->next_word();
            if (is_one_of(this->curr_word, TokenType::VAL_STRING))
            {
                return true;
            }
            else
            {
                std::cout << "TYPE MISMATCH: INT ASSIGNMENT MISMATCH";
            }
        }
    }
    else if (is_one_of(this->curr_word, TokenType::TYPE_CHAR))
    {
        if (identifier_and_equals())
        {
            this->next_word();
            if (is_one_of(this->curr_word, TokenType::VAL_CHAR))
            {
                return true;
            }
            else
            {
                std::cout << "TYPE MISMATCH: INT ASSIGNMENT MISMATCH";
            }
        }
    }
    else if (is_one_of(this->curr_word, TokenType::TYPE_FLOAT))
    {
        if (identifier_and_equals())
        {
            this->next_word();
            if (is_one_of(this->curr_word, TokenType::VAL_FLOAT))
            {
                return true;
            }
            else
            {
                std::cout << "TYPE MISMATCH: INT ASSIGNMENT MISMATCH";
            }
        }
    }
}

template <typename... Ts>
struct Overload : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

struct Visitor
{
    void operator()(Expr *e)
    {
    }

    void operator()(Factor *f)
    {
    }
};

std::string Parser::getTreeString()
{

    std::function<std::string(std::variant<Factor *, Expr *>)> expr_to_string;

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
        return std::visit(Overload{[&](Name *e) -> std::string
                            {
                                return *e->name;
                            },
                            [&](Num *f) -> std::string
                            {
                                return std::to_string(f->num);
                            }},
                   factor);
    };

    expr_to_string = [&](std::variant<Factor *, Expr *> node) -> std::string
    {
        return std::visit(Overload{[&](Expr *e) -> std::string
                            {
                                std::cout << "EXPR TO STRING";
                                std::string lhs = expr_to_string(e->lhs);
                                std::string rhs = expr_to_string(e->rhs);
                                return "(" + op_to_string(e->op) + " " + lhs + " " + rhs + ")";
                            },
                            [&](Factor *f) -> std::string
                            {
                                std::cout << "FACTOR TO STRING";
                                if (f != NULL) {
                                    return factor_to_string(f->variant);
                                }
                                return "F";
                            }},
                   node);
    };

    std::cout << "TO STRING";
    return expr_to_string(g.expr);
}

void Parser::printTree()
{
    std::cout << "\n";
    std::cout << getTreeString();
}
