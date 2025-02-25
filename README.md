# Build
`make` to build main `carl` executable

`make tests` for unit tests

`make docs` for Doxygen documentation

## BNF of language

```ebnf
<program>               ::= <command>* EOF

<command>               ::= <showStatement> | <applyStatement> | <defineStatement> | <statement>
<showStatement>         ::= "show" <expression> ";"
<applyStatement>        ::= "apply" IDENTIFIER ":" <expression> ";"
<defineStatement>       ::= "define" IDENTIFIER "->" <expression> "=" <expression> ";"

<statement>             ::= <expressionStatement> ";"
<expressionStatement>   ::= <expression>

<expression>            ::= <equality>
<equality>              ::= <comparison> ( ("!=" | "==") <comparison> )*
<comparison>            ::= <term> ( ( ">" | ">=" | "<" | "<=" ) <term> )*
<term>                  ::= <factor> ( ( "+" | "-" ) <factor> )*
<factor>                ::= ( "!" | "-" ) <unary> | <primary>
<primary>               ::= NUMBER | STRING | IDENTIFIER | false | true
```
