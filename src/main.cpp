#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"

using namespace std;

int main() {
    std::ifstream source_file ("../input/source.hb");
    std::string fileString;

    if (source_file.is_open()) {
        std::string nextLine;
        while (source_file) {
         std::getline(source_file, nextLine);
         fileString.append(nextLine);
         fileString.append("\n");
        }
    }
    std::cout << fileString;
    return 0;
}

