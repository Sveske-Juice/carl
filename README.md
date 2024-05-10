# Carl

## BNF of language

```ebnf
<program>               ::= <statement>* EOF

<ruleApply>             ::= <applyStatement> | <definition>
<applyStatement>        ::= "apply" IDENTIFIER ":" <expression> ";"

<definition>            ::= <defineStatement> | <statement>
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
