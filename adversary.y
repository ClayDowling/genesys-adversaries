%extra_argument { struct world_t* world }

%parse_accept {
    printf("World loaded!\n");
}
%parse_failure {
    printf("Forked!\n");
}

%include {

#include "ast.h"

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
world(A) ::= world skill(B) . { world_add_skill(A, B); }
world(A) ::= world talent(B) . { world_add_talent(A, B); }
world(A) ::= world package(B) . { world_add_package(A, B); }
world(A) ::= world character(B) . { world_add_character(A, B); }

skill(A) ::= SKILL NAME(B) LPAREN ATTRIBUTE(C) RPAREN . { A = new_skill(B->strval, C->attributeval); }
talent(A) ::= TALENT NAME(B) . { A = new_talent(B->strval); }

package(A) ::= PACKAGE namedlist(B) . {
    A = B;
    A->type = list_package;
}
character(A) ::= MINION namedlist(B) . {
    A = B;
    A->type = list_minion;
}
character(A) ::= RIVAL namedlist(B) . {
    A = B;
    A->type = list_rival;
}
character(A) ::= NEMESIS namedlist(B) . {
    A = B;
    A->type = list_nemesis;
}

namedlist(A) ::= NAME(B) COLON leveleditem(C) . { 
    A = new_namedlist(list_MAX, B->strval); 
    struct listitem_t* li = world_add_reference(world, C->name, C->level);
    if (li != NULL) {
        node_append(A->TOP, (void*)li);
    } else {
        fprintf(stderr, "Item %s is not defined.\n", C->name);
    }    
}

namedlist(A) ::= namedlist leveleditem(B) . {
    struct listitem_t* li = world_add_reference(world, B->name, B->level);
    if (li != NULL) {
        node_append(A->TOP, (void*)li);
    } else {
        fprintf(stderr, "Item %s is not defined.\n", B->name);
    }
}

namedlist(A) ::= namedlist COMMA leveleditem(B) . {
    struct listitem_t* li = world_add_reference(world, B->name, B->level);
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
