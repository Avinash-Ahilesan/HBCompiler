# HBCompiler

## Basic Syntactic Rules

### Identifier
An Identifier must start with an alphabetical character (a-zA-Z) and can be followed by a number or underscores.
An identifier is used as name for variables, function names, etc. Anything you name yourself is an identifier.

Example
int x = 5

x is an identifier.

### Numerical
There are two types of numericals:
An integer, is a number without any decimal values
A float is a decimal number with a decimal value.

More to follow.

### Chars
characters are single bytes, defined using single quotes.
Example:

char x = 'a'
### String
Strings are a series of chars. 
Example:

string myText = "hello"

# Grammar Rules
The implementation of my parser uses a top-down recursive descent approach. An LL(1) grammar.

Goal -> Expr

Expr -> Term Expr'

Expr' -> + Term Expr'
      | - Term Expr'
      | empty

Term -> Factor Term'
Term' -> x Factor Term'
       | / Factor Term'
       | empty
Factor -> ( Expr )
        | num
        | name

VariableDecl -> name = Value

Value -> num
       | string
       | name 

Statement -> IfStatement | WhileStatment | ForStatement

