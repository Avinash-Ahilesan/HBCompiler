#include <string>
#include <vector>
#include <fstream>


class LexerUtils {

    public:
        static bool is_digit(char c);
        static bool is_alphabetical(char c);
        static bool is_underscore(char c);
        static bool is_special_symbol(char c);
        static bool is_double_quote(char c);
        static bool is_single_quote(char c);
    private:

};
