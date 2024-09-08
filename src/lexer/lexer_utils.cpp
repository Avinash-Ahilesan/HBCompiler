#include "lexer_utils.h"

bool LexerUtils::is_digit(char c) {
    if (c >= 48 && c <= 57) {
        return true;
    }

    return false;
}

bool LexerUtils::is_alphabetical(char c) {
    return true;
}

bool LexerUtils::is_underscore(char c) {
    return true;
}