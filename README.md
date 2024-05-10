# Carl

## BNF of language
```
<program>               ::= <statement>* EOF

<statement>             ::= <expressionStatement> ";"
<expressionStatement>   ::= <expression>

<expression>            ::= <equality>
<equality>              ::= <comparison> ( ("!=" | "==") comparison )*
<comparison>            ::= <term> ( ( ">" | ">=" | "<" | "<=" ) <term> )*
<term>                  ::= <factor> ( ( "+" | "-" ) <factor> )*
<factor>                ::= ( "!" | "-" ) <unary> | <primary>
<primary>               ::= NUMBER | STRING | false | true
```
