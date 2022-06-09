/* used for getting the name of included files */
%x incl
%x QS

%option case-insensitive noyywrap yylineno
%option nodefault
%option header-file="lexer.h"

%{

#include "adversary.h"
#include "token.h"

enum attribute_t yy_adv_attribute = attr_MAX;
int yy_adv_lineno = 1;

#define BUFFER_MAX 60
char quoted_string[BUFFER_MAX];

struct bufstack {
    struct bufstack *prev;
    YY_BUFFER_STATE bs;
    int lineno;
    char *filename;
    FILE* f;
} *curbs = 0;

char* curfilename;

int newfile(char *fn);
int popfile();


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

use[ \t]+\"          { BEGIN(incl); }

<incl>[^\n\"]+  { // Filename except trailing whitespace
        {
            int c;
            while((c = input()) && c != '\n');
        }
        yylineno++;
        if (!newfile(yytext)) {
            yyterminate();
        }
        BEGIN(INITIAL);
    }
<incl>.|\n      {
        fprintf(stderr, "Bad \"use\" file on line %d", yylineno);
        yyterminate();
    }

[A-Za-z][A-Za-z0-9]*  { return WORD; }

\"              { BEGIN(QS); }
<QS>[^\n\"]*    { 
                    char* yyptr = yytext;
                    int i = 0;
                    memset(quoted_string, 0, BUFFER_MAX);
                    while(*yyptr && i < BUFFER_MAX) {
                        quoted_string[i] = *yyptr++;
                        ++i;
                    }
                }
<QS>\"          { BEGIN(INITIAL); return QUOTEDSTRING; }

[ \t]       // Eat up the whitespace

<<EOF>>     { if (!popfile) yyterminate();  }


%%

/* Open a new file and make it the current buffer, saving the old buffer to the stack */
int newfile(char* fn) {
    FILE *f = fopen(fn, "r");
    struct bufstack *bs = (struct bufstack*)calloc(1, sizeof(struct bufstack));

    if (!f) {
        perror(fn);
        return 0;
    }
    if (!bs) {
        perror("malloc");
        return 0;
    }

    if (curbs) curbs->lineno = yylineno;
    bs->prev = curbs;

    bs->bs = yy_create_buffer(f, YY_BUF_SIZE);
    bs->f = f;
    bs->filename = strdup(fn);
    yy_switch_to_buffer(bs->bs);
    curbs = bs;
    yylineno = 1;
    curfilename = bs->filename;
    return 1;
}

int popfile() {
    struct bufstack *bs = curbs;
    struct bufstack* prevbs;

    if (!bs) return 0;

    /* Clean up current entry */
    fclose(bs->f);
    free(bs->filename);
    yy_delete_buffer(bs->bs);

    /* Switch to previous buffer */
    prevbs = bs->prev;
    free(bs);

    /* Out of buffers and therefore out of input */
    if (!prevbs) return 0;

    yy_switch_to_buffer(prevbs->bs);
    curbs = prevbs;
    yylineno = curbs->lineno;
    curfilename = curbs->filename;
    return 1;
}