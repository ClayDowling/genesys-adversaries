/* used for getting the name of included files */
%x incl
%option case-insensitive

%{

#include "adversary.h"
#include "token.h"

enum attribute_t yy_adv_attribute = attr_MAX;
int yy_adv_lineno = 1;

%}

DIGIT   [0-9]

%%

\n              { yy_adv_lineno++; }
,               { return COMMA; }
:               { return COLON; }
\(              { return LPAREN; }
\)              { return RPAREN; }

-?{DIGIT}+      { return NUMBER; }

skill           { return SKILL; }
talent          { return TALENT; }
minion          { return MINION; }
rival           { return RIVAL; }
nemesis         { return NEMESIS; }

brawn           { yy_adv_attribute = attr_brawn; return ATTRIBUTE; }
agility         { yy_adv_attribute = attr_agility; return ATTRIBUTE; }
intellect       { yy_adv_attribute = attr_intellect; return ATTRIBUTE; }
cunning         { yy_adv_attribute = attr_cunning; return ATTRIBUTE; }
willpower       { yy_adv_attribute = attr_willpower; return ATTRIBUTE; }
presence        { yy_adv_attribute = attr_presence; return ATTRIBUTE; }

br              { yy_adv_attribute = attr_brawn; return ATTRIBUTE; }
ag              { yy_adv_attribute = attr_agility; return ATTRIBUTE; }
int             { yy_adv_attribute = attr_intellect; return ATTRIBUTE; }
cun             { yy_adv_attribute = attr_cunning; return ATTRIBUTE; }
will            { yy_adv_attribute = attr_willpower; return ATTRIBUTE; }
pr              { yy_adv_attribute = attr_presence; return ATTRIBUTE; }


[A-Za-z][A-Za-z0-9]*  { return WORD; }

\"([^\\\"]|\\.)*\"            { return QUOTEDSTRING; }

[ \t]       // Eat up the whitespace

.               { 
        fprintf(stderr, "Unrecognized symbol \"%s\" in input.\n", yytext);
        yyterminate();
    }


use     BEGIN(incl);

<incl>[ \t]*        /* Eat whitespace before the file name */
<incl>[^ \t\n]+$    { // Filename except trailing whitespace
        yyin = fopen(yytext, "r");

        if (!yyin) {
            fprintf(stderr, "Could not open \"%s\"\n", yytext);
            return 0;
        }

        yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE ));
        BEGIN(INITIAL);
    }

<<EOF>>     {
        yypop_buffer_state();

        if ( !YY_CURRENT_BUFFER ) {
            yyterminate();
        }
    }