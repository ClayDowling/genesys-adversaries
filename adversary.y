%extra_argument { struct world_t* thisworld }

%parse_accept {
    printf("World loaded!\n");
}
%parse_failure {
    fprintf(stderr, "We're Forked!  Abandoning parse.\n");
}

%syntax_error {
    fprintf(stderr, "Unexpected token type %s near line %d\n", tag_to_name(yymajor), yyminor->lineno);
    switch(yymajor) {
        case NAME:
            fprintf(stderr, "name %s\n", yyminor->strval);
            break;
        case NUMBER:
            fprintf(stderr, "number %d\n", yyminor->intval);
            break;
    }
}

%include {

#include "ast.h"
#include "terminal_tags.h"

}

%token_type {struct token*}

%type world             {struct world_t*}
%type skill             {struct skill_t*}
%type talent            {struct talent_t*}
%type attributebonus    {struct attributebonus_t*}
%type package           {struct namedlist_t*}
%type character         {struct namedlist_t*}
%type leveleditem	{struct leveleditem_t*}
%type namedlist         {struct namedlist_t*}

input ::= world .

world ::= .
world ::= world skill .
world ::= world talent .
world ::= world package .
world ::= world character .


skill ::= SKILL NAME(B) LPAREN ATTRIBUTE(C) RPAREN . { world_add_skill(thisworld, new_skill(B->strval, C->attributeval)); }
talent ::= TALENT NAME(B) . { world_add_talent(thisworld, new_talent(B->strval)); }

package(A) ::= PACKAGE namedlist(B) . {
    A = B;
    A->type = list_package;
    world_add_package(thisworld, A);
}

character(A) ::= MINION namedlist(B) . {
    A = B;
    A->type = list_minion;
    world_add_character(thisworld, A);
}
character(A) ::= RIVAL namedlist(B) . {
    A = B;
    A->type = list_rival;
    world_add_character(thisworld, A);
}
character(A) ::= NEMESIS namedlist(B) . {
    A = B;
    A->type = list_nemesis;
    world_add_character(thisworld, A);
}

namedlist(A) ::= NAME(B) COLON leveleditem(C) . { 
    A = new_namedlist(list_MAX, B->strval); 
    struct listitem_t* li = world_add_reference(thisworld, C->name, C->level);
    if (li != NULL) {
        node_append(A->TOP, (void*)li);
    } else {
        fprintf(stderr, "Item %s is not defined.\n", C->name);
    }    
}

namedlist(A) ::= namedlist leveleditem(B) . {
    struct listitem_t* li = world_add_reference(thisworld, B->name, B->level);
    if (li != NULL) {
        node_append(A->TOP, (void*)li);
    } else {
        fprintf(stderr, "Item %s is not defined.\n", B->name);
    }
}

namedlist(A) ::= namedlist COMMA leveleditem(B) . {
    struct listitem_t* li = world_add_reference(thisworld, B->name, B->level);
    if (li != NULL) {
        node_append(A->TOP, (void*)li);
    } else {
        fprintf(stderr, "Item %s is not defined.\n", B->name);
    }
}

namedlist(A) ::= namedlist COMMA attributebonus(B) . {
	node_append(A->TOP, (void*)B);
}

leveleditem(A) ::= NAME(B) . { A = new_leveleditem(B->strval, 0); }
leveleditem(A) ::= NAME(B) NUMBER(C) . { A= new_leveleditem(B->strval, C->intval); }
attributebonus(A) ::= ATTRIBUTE(B) NUMBER(C) . { A = new_attributebonus(B->attributeval, C->intval); }
