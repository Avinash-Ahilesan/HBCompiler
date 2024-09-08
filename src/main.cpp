#include <iostream>
#include <string>
#include "./lexer/lexer.h"

using namespace std;

int main() {
    Lexer l("./input/source.hb");
    l.tokenize();
}

