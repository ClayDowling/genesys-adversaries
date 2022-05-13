

input ::= .
input ::= input skill
input ::= input talent
input ::= input package
input ::= input character

skill(A) ::= SKILL NAME(B) LPAREN ATTRIBUTE(C) RPAREN
talent(A) ::= TALENT NAME(B)
attributebonus(A) ::= ATTRIBUTE(B) NUMBER(C)

namedlist(A) ::= NAME(B) COLON NAME(C)
namedlist(A) ::= namedlist(B) COMMA NAME(C)

package(A) ::= PACKAGE namedlist(B)
character(A) ::= MINION namedlist(B)
character(A) ::= RIVAL namedlist(B)
character(A) ::= NEMESIS namedlist(B)
