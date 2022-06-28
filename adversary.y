%extra_argument { struct world_t* thisworld }

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

extern char quoted_string[];

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
%type namedlistitem     {struct namedlistitem_t*}

%token_destructor { free((void*)$$->strval); free($$); }

input ::= world .

world ::= .
world ::= world skill .
world ::= world talent .
world ::= world package .
world ::= world character .
world ::= world weapon .


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

namedlist(A) ::= name(B) COLON . {
    A = new_namedlist(list_MAX, B);
}

namedlist(A) ::= namedlist namedlistitem(B) . {
    namedlist_add_reference(thisworld, A, B);
}

namedlist(A) ::= namedlist COMMA namedlistitem(B) . {
    namedlist_add_reference(thisworld, A, B);
}

namedlistitem(A) ::= leveleditem(B) . { A = new_namedlistitem_leveled(B); }
namedlistitem(A) ::= attributebonus(B) . { A = new_namedlistitem_attribute(B); }

leveleditem(A) ::= name(B) . { A = new_leveleditem(B, 0); }
leveleditem(A) ::= name(B) NUMBER(C) . { A= new_leveleditem(B, C->intval); }
attributebonus(A) ::= ATTRIBUTE(B) NUMBER(C) . { A = new_attributebonus(B->attributeval, C->intval); }

weapon ::= WEAPON name LPAREN name SEMICOLON weaponstats RPAREN .

weaponstats ::= weaponstats SEMICOLON criticalphrase .
weaponstats ::= weaponstats SEMICOLON damagephrase .
weaponstats ::= weaponstats SEMICOLON specials .
weaponstats ::= .

damagephrase ::= DAMAGE NUMBER .
criticalphrase ::= CRITICAL NUMBER .

specials ::= specials COMMA name .
specials ::= name .

name(A) ::= QUOTEDSTRING(B) . { A = strdup(B->strval); }
name(A) ::= WORD(B) .    { A = strdup(B->strval); }

