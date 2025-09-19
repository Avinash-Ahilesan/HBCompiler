
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

enum Comparator {
    GREATER_THAN, LESS_THAN, GREATER_THAN_EQUALS, LESS_THAN_EQUALS, EQUALS, NOT_EQUALS,
    NOT, AND, OR
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

struct Condition {
    enum Comparator comparator;
    std::variant<Factor, std::shared_ptr<Expr>> l_value;
    std::variant<Factor, std::shared_ptr<Expr>> r_value; // TODO: model ! with one value
};

struct Statement;

struct IfStatement {
    Condition condition;
    std::shared_ptr<Statement> then_statement;
    std::shared_ptr<Statement> else_statement;
};

struct WhileStatement {
    Condition condition;
    std::shared_ptr<Statement> then_statement;
    std::shared_ptr<Statement> else_statement;
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