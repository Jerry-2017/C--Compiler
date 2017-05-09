%x STRING COMMENT CHAR

%{
    #include "string.h"
    #include "../include/syntax.tab.h"
    #include "../include/common.h"


    int yycolumn=1;
    YYLTYPE yylloc;
    extern int errorrec;
    #define YY_USER_ACTION {\
        yylloc.first_line=yylloc.last_line=yylineno;\
        yylloc.first_column=yycolumn;\
        yylloc.last_column=yycolumn+yyleng-1;\
        yycolumn+=yyleng;\
    }
%}

%option yylineno

DOT "."
COMMA ","
QUESTION "?"
SEMI ";"
ASSIGNOP "="
BIGGER ">"
BIGGEREQUAL ">="
EQUAL "=="
LESS "<"
LESSEQUAL "<="
NOTEQUAL "!="
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
NOT "!"
TYPE "int"|"float"
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT "struct"
CASE "case"
SWITCH "switch"
RETURN "return"
IF "if"
ELSE "else"
WHILE "while"

RELOP {BIGGER}|{BIGGEREQUAL}|{EQUAL}|{LESS}|{LESSEQUAL}|{NOTEQUAL}
DIGITS_10_NLZ 0|([1-9][0-9]*)
ALPHABET [a-zA-Z]
DIGITS_10 [0-9]+
DIGITS_8 0([0-7]*)
DIGITS_16 0(X|x)([0-9ABCDEFabcdef]*)
BLANKS [ \r\n\t]


FLOAT [+-]?{DIGITS_10_NLZ}({DOT}{DIGITS_10})?([eE]([+-]?){DIGITS_10_NLZ})?
INT_10 [+-]?{DIGITS_10_NLZ}
INT_8 [+-]?{DIGITS_8}
INT_16 [+-]?{DIGITS_16}

CHAR \'[^\'\n]\'
ID ({ALPHABET}|_)({ALPHABET}|_|{DIGITS_10})*
COMMENT1 "//"[^\r\n]*

%%

{DOT} { yylval.a=add_sym_seq(S_DOT); return DOT; };
{COMMA} { yylval.a=add_sym_seq(S_COMMA); return COMMA; };
{QUESTION} { yylval.a=add_sym_seq(S_QUESTION); return QUESTION; };
{SEMI} { yylval.a=add_sym_seq(S_SEMI); return SEMI; };
{ASSIGNOP} { yylval.a=add_sym_seq(S_ASSIGNOP); return ASSIGNOP; };
{PLUS} { yylval.a=add_sym_seq(S_PLUS); return PLUS; };
{MINUS} { yylval.a=add_sym_seq(S_MINUS); return MINUS; };
{STAR} { yylval.a=add_sym_seq(S_STAR); return STAR; };
{DIV} { yylval.a=add_sym_seq(S_DIV); return DIV; };
{AND} { yylval.a=add_sym_seq(S_AND); return AND; };
{OR} { yylval.a=add_sym_seq(S_OR); return OR; };
{NOT} { yylval.a=add_sym_seq(S_NOT); return NOT; };
{EQUAL} { yylval.a=add_sym_seq(S_EQUAL); return EQUAL; };
{LESS} { yylval.a=add_sym_seq(S_LESS); return LESS; };
{BIGGER} { yylval.a=add_sym_seq(S_BIGGER); return BIGGER; };
{LESSEQUAL} { yylval.a=add_sym_seq(S_LESSEQUAL); return LESSEQUAL; };
{BIGGEREQUAL} { yylval.a=add_sym_seq(S_BIGGEREQUAL); return BIGGEREQUAL; };
{NOTEQUAL} { yylval.a=add_sym_seq(S_NOTEQUAL); return NOTEQUAL; };
{LP} { yylval.a=add_sym_seq(S_LP); return LP; };
{RP} { yylval.a=add_sym_seq(S_RP); return RP; };
{LB} { yylval.a=add_sym_seq(S_LB); return LB; };
{RB} { yylval.a=add_sym_seq(S_RB); return RB; };
{LC} { yylval.a=add_sym_seq(S_LC); return LC; };
{RC} { yylval.a=add_sym_seq(S_RC); return RC; };
{STRUCT} { yylval.a=add_sym_seq(S_STRUCT); return STRUCT; };
{CASE} { yylval.a=add_sym_seq(S_CASE); return CASE; };
{IF} { yylval.a=add_sym_seq(S_IF); return IF; };
{ELSE} { yylval.a=add_sym_seq(S_ELSE); return ELSE; };
{WHILE} { yylval.a=add_sym_seq(S_WHILE); return WHILE; };
{RETURN} { yylval.a=add_sym_seq(S_RETURN); return RETURN; };

{TYPE} { yylval.a=add_sym_seqx(S_TYPE,add_str_tbl(yytext)); return TYPE; }; 
{ID} { yylval.a=add_sym_seqx(S_ID,add_str_tbl(yytext)); return ID; }; 

{INT_10} { yylval.a=add_sym_seqx(S_INT_10,add_str_tbl(yytext)); sscanf(yytext,"%d",&yylval.a->value.ival); return INT_10; };
{INT_8} { yylval.a=add_sym_seqx(S_INT_8,add_str_tbl(yytext)); sscanf(yytext,"%o",&yylval.a->value.ival); return INT_8; };
{INT_16} { yylval.a=add_sym_seqx(S_INT_16,add_str_tbl(yytext)); sscanf(yytext,"%x",&yylval.a->value.ival); return INT_16; };
{FLOAT} { yylval.a=add_sym_seqx(S_FLOAT,add_str_tbl(yytext)); sscanf(yytext,"%f",&yylval.a->value.fval); return FLOAT; };

\" { strbuff=p_str_tbl; BEGIN(STRING); };
<STRING>\\. { strcpy(string_table+strbuff,yytext); strbuff+=strlen(yytext); };
<STRING>\" { 
            string_table[strbuff++]='\0'; 
            symbol_sequence[symbols].sym_type=S_STRING; 
            symbol_sequence[symbols].value.pstr=string_table+p_str_tbl;
            p_str_tbl=strbuff;
            symbols++; 
            BEGIN(INITIAL);
            return STRING;
};
<STRING>[^\r\n"] { strcpy(string_table+strbuff,yytext); strbuff+=strlen(yytext);  };

{CHAR} { yylval.a=add_sym_seqx(S_CHAR,add_str_tbl(yytext)); return CHAR; }; 

{COMMENT1} { printf("line %d :COMMENT1 \n",yylineno); };

\/\* { printf("line %d : COMMENT2 \n",yylineno); BEGIN(COMMENT); };
<COMMENT>\*\/ { BEGIN(INITIAL); };
<COMMENT>.|\n { };

"\n" { }; 
{BLANKS} { }; 

. { printf("\n****Error Type A occured at line %d, dealing lexema %s\n",yylineno,yytext); errorrec=1; return ERROR; }


%%

//printf("\n%d:    ",++lines); }
//printf("%s",yytext); }
int yywrap()
{
    _isend=true;
}

/*

int main(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            perror(argv[1]);
            return 1;
        }
    }
    printf("FileName : %s\n",argv[1]);
    printf("0:  ");
    _isend=false;
    while (!_isend && yylex()!=ERROR);
    printf("\n-------------------\n");
    return 0;
}*/
