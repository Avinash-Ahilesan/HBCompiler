#include <iostream>
#include <string>
#include "./lexer/lexer.h"

using namespace std;

int main() {
    //std::ifstream source_file ("../input/source.hb");
    Lexer l("./input/source.hb");
    l.tokenize();
}

