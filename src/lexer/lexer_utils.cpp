#include "lexer_utils.h"

bool LexerUtils::is_digit(char c) {
    if (c >= 48 && c <= 57) {
        return true;
    }

    return false;
}

bool LexerUtils::is_alphabetical(char c) {
    if (c >= 65 && c <= 90) {
        return true;
    }

    if (c >= 97 && c <= 122) {
        return true;
    }

    return false;
}

bool LexerUtils::is_underscore(char c) {
    return c == '_';
}

bool LexerUtils::is_special_symbol(char c) {
    // couple of special symbols: { | } = ! + - / % ( ) ; & * > <
    if (c == '{' || c == '|' || c == '}' || c == '&') {
        // { | & }
        return true;
    }

    if (c == '(' || c == ')') { // ( )
        return true;
    }

    if (c == '=' || c == '<' || c == '>' || c == '!') { // = 
        return true;
    }

    if (c == '+' || c == '-' || c == '*' || c == '\\' || c == '%') {
        return true;
    }
    
    return false;
}

bool LexerUtils::is_double_quote(char c) {
    return c == '"';
}

bool LexerUtils::is_single_quote(char c) {
    return c == '\'';
}