

input ::= .
input ::= input skill
input ::= input talent
input ::= input package
input ::= input character

skill(A) ::= SKILL NAME(B) LPAREN ATTRIBUTE(C) RPAREN { struct skill_t* A = new_skill(B.strval, C.attributeval); }
talent(A) ::= TALENT NAME(B) { struct talent_t* A = new_talent(B.strval); }
attributebonus(A) ::= ATTRIBUTE(B) NUMBER(C) {struct attributebonus_t* A = new_attributebonus(B.attributeval, C.intval); }

package(A) ::= PACKAGE namedlist(B)
character(A) ::= MINION namedlist(B)
character(A) ::= RIVAL namedlist(B)
character(A) ::= NEMESIS namedlist(B)

namedlist(A) ::= NAME(B) COLON NAME(C)
namedlist(A) ::= namedlist(B) COMMA NAME(C)

leveleditem(A) ::= NAME(B) LEVEL(C) { 
    struct skill_t* s = find_skill(world, B.strval);
    if (s != NULL) {
        world_add_skill(world, new_skill_reference())
    }
}

leveleditem(A) ::= NAME(B)

