#include "token.h"
#include "adversary.h"
#include <string.h>
#include <stdlib.h>

struct token* lex(const char* source) {

    const char* YYCURSOR = source;
    const char* YYMARKER;

    int lineno = 1;
    const char* start;
    const char* end;

    /*!stags:re2c format = 'const char *@@;\n'; */

    /*!re2c
    re2c:yyfill:enable = 0;
    re2c:define:YYCTYPE = char;
    re2c:tags = 1;

    [\x00]  { return 0; }


    'skill'     { return new_token(lineno, SKILL); }
    'talent'    { return new_token(lineno, TALENT); }
    'package'   { return new_token(lineno, PACKAGE); }
    'minion'    { return new_token(lineno, MINION); }
    'rival'     { return new_token(lineno, RIVAL); }
    'nemesis'   { return new_token(lineno, NEMESIS); }
    ":"         { return new_token(lineno, COLON); }
    ","         { return new_token(lineno, COMMA); }
    "("         { return new_token(lineno, LPAREN); }
    ")"         { return new_token(lineno, RPAREN); }
    'brawn'|'br'        { return new_token_attribute(lineno, attr_brawn); }
    'agility'|'ag'      { return new_token_attribute(lineno, attr_agility); }
    'intellect'|'int'   { return new_token_attribute(lineno, attr_intellect); }
    'cunning'|'cun'     { return new_token_attribute(lineno, attr_cunning); }
    'willpower'|'will'  { return new_token_attribute(lineno, attr_willpower); }
    'presence'|'pr'     { return new_token_attribute(lineno, attr_presence); }
    ["] @start [^"]* @end ["]       { 
        char* strval = (char*)calloc(1, end - start + 1);
        strncpy(strval, start, end - start);
        return new_token_name(lineno, strval);
    }
    @start "-"? [0-9][0-9]+ @end {
        int n = 0;
        int sign = 1;
        if (*start == '-') {
            sign = -1;
            start++;
        }

        for(; start < end; start++) {
            n = n*10 + (*start - '0');
        }

        n = n * sign;

        return new_token_int(lineno, n);
    }

    "\n" { lineno++; }

    */

    return 0;
}