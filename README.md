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

a + b x c + d


     +
  /      \
 a        *
        /   \
       b     c

1 + 2 * 3
              Goal
               |
              Expr
           /         \
       Term            Expr'
       /   \          /    \      \
   Factor   Term'    +     Term     Expr'
  /         \         \     \        \
 num        empty    Factor Term'     empty
 /                       \    \ \
1                        num  x factor term'
                           \     \        \
                            2 .   3      empty


Goal -> Expr EOF 
     | VariableDecl EOF                   a * b + c     a

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

VariableDecl -> Type name = Value

Type -> int
     | float
     | char
     | string

Value -> num
       | name 
       | string


Statement -> IfStatement | WhileStatment

IfStatement -> if (condition) { } else { }
WhileStatement -> while (condition) { }
