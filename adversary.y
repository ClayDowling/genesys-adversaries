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
        case QUOTEDSTRING:
        case WORD:
            fprintf(stderr, "name %s\n", yyminor->strval);
            break;
        case NUMBER:
            fprintf(stderr, "number %d\n", yyminor->intval);
            break;
    }
}

%include {

#include <string.h>
#include <stdlib.h>

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
%type name              {const char*}
%type builtname         {char*}

input ::= world .

world ::= .
world ::= world skill .
world ::= world talent .
world ::= world package .
world ::= world character .


skill ::= SKILL name(B) LPAREN ATTRIBUTE(C) RPAREN . { world_add_skill(thisworld, new_skill(B, C->attributeval)); }
talent ::= TALENT name(B) . { world_add_talent(thisworld, new_talent(B)); }

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

namedlist(A) ::= name(B) COLON leveleditem(C) . { 
    A = new_namedlist(list_MAX, B); 
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

leveleditem(A) ::= name(B) . { A = new_leveleditem(B, 0); }
leveleditem(A) ::= name(B) NUMBER(C) . { A= new_leveleditem(B, C->intval); }
attributebonus(A) ::= ATTRIBUTE(B) NUMBER(C) . { A = new_attributebonus(B->attributeval, C->intval); }

name(A) ::= QUOTEDSTRING(B) . { A = B->strval; }


name(A) ::= builtname(B) .    { A = B; }


builtname(A) ::= builtname(B) WORD(C) . { 
    int len = strlen(B) + strlen(C->strval) + 2; 
    A = (char*)calloc(1, len); 
    snprintf(A, len, "%s %s", B, C->strval);
    free(B);
    } 
