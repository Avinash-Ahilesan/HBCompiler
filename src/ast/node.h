
#include <vector>


struct Name {
    std::string name;
};

struct Num {
    int num;    // just a basic int for now
};

enum Operator {
    ADD, SUBTRACT, MULTIPLY, DIVIDE
};

enum VariableType {
    INTEGER, STRING, FLOAT, CHAR
};

using FactorVariant = std::variant<Name, Num>;
struct Factor {
    // either ()
    FactorVariant variant;
};

struct Expr {
    enum Operator op;
    std::variant<Factor, std::shared_ptr<Expr>> lhs;
    std::variant<Factor, std::shared_ptr<Expr>> rhs;
};


struct VariableDeclaration {
    enum VariableType var_type;
    std::string name;
    std::variant<int, std::string, Name> value;
};

struct IfStatement {

};

struct WhileStatement {

};

struct Statement {
    std::variant<Factor, std::shared_ptr<Expr>, VariableDeclaration, IfStatement, WhileStatement> statement;
};

struct Goal {
    std::vector<Statement> statement_list;
};


struct Node {
    int linenum;
};