#include "../parser/parser.h"
#include <iostream>

Parser::Parser(std::vector<Token> token_list) {
    this->word_list = token_list;
}

void Parser::next_word() {
    if (++curr_index >= word_list.size()) {
        curr_word =  NULL; // signify end of list
    }
    curr_word = &word_list[curr_index];
}

bool Parser::parse() {
    // Entry point Goal -> Expr
    this->next_word();
    if (expr()) {
        if (is_one_of(this->curr_word, TokenType::END_OF_FILE)) {
            return true;
        }
        else {
            std::cout << "Last character is not end of file";
        }
    }
    std::cout << "Not an expression";
    return false;
}

// Expr -> Term Expr'
bool Parser::expr() {
    if (term()) {
        if (expr_prime()) {
            return true;
        }
        std::cout << "Not an Expr'";
        return false;
    }
    std::cout << "Not a term";
    return false;
}

// Term -> Factor Term'
bool Parser::term() {
    if (factor()) {
        return term_prime();
    }
    std::cout << "Not a factor";
    return false;
}

// Term' -> x Factor Term'
//       -> / Factor Term'
//       -> empty
bool Parser::term_prime() {
    if (is_one_of(this->curr_word, TokenType::MULTIPLY, TokenType::DIVIDE)) {
        this->next_word();
        if (factor()) {
            return term_prime();
        }
        else {
            std::cout << "NOT A FACTOR";
            return false;
        }
    } 
    if (is_one_of(this->curr_word, TokenType::PLUS, TokenType::MINUS, TokenType::CLOSE_ROUND_BRACKET, TokenType::END_OF_FILE)) {
        // we can eat an epsilon if the term that follows is )
        // FOLLOW(term') = + - )
        return true;
    }
    std::cout << "Not a term prime";
    return false;
}

//
bool Parser::expr_prime() {
    if (is_one_of(this->curr_word, TokenType::PLUS, TokenType::MINUS)) {
        this->next_word();
        if (factor()) {
            return term_prime();
        }
        else {
            std::cout << "NOT A FACTOR";
            return false;
        }
    }
    if (is_one_of(this->curr_word, TokenType::CLOSE_ROUND_BRACKET, TokenType::END_OF_FILE)) {
        return true;
    }
    std::cout << "not an expression prime";
    return false;
}

bool Parser::factor() {
    if (is_one_of(this->curr_word, TokenType::OPEN_ROUND_BRACKET)) {
        this->next_word();
        if (expr())
            if (is_one_of(this->curr_word, TokenType::CLOSE_ROUND_BRACKET)) {
                this->next_word();
                return true;
            }
        else 
            return false;
    }
    else if (is_one_of(this->curr_word, TokenType::VAL_INTEGER, TokenType::IDENTIFIER)) {
        this->next_word();
        return true;
    }

    return false;
}



