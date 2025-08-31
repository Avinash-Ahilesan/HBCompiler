
#include <vector>


struct Name {
    std::string* name;
};

struct Num {
    int num;    // just a basic int for now
};

enum Operator {
    ADD, SUBTRACT, MULTIPLY, DIVIDE
};

using FactorVariant = std::variant<Name*, Num*>;
struct Factor {
    // either ()
    FactorVariant variant;
};

struct Expr {
    enum Operator op;
    std::variant<Factor*, Expr*> lhs;
    std::variant<Factor*, Expr*> rhs;
};

struct Goal {
    std::variant<Factor*, Expr*> expr;
};



struct Node {
    int linenum;
};