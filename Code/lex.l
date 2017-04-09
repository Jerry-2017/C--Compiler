%x STRING COMMENT CHAR

%{
    #include "string.h"
    #include "syntax.tab.h"
    #include "kernel.h"
  //  #include "syntax.tab.h"

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
INT [+-]?{DIGITS_10_NLZ}
INT_8 [+-]?{DIGITS_8}
INT_16 [+-]?{DIGITS_16}

CHAR \'[^\'\n]\'
ID ({ALPHABET}|_)({ALPHABET}|_|{DIGITS_10})*
COMMENT1 "//"[^\r\n]*

%%

{DOT} { yylval.a=add_sym_seq("DOT",DOT); return DOT; };
{COMMA} { yylval.a=add_sym_seq("COMMA",COMMA); return COMMA; };
{QUESTION} { yylval.a=add_sym_seq("QUESTION",QUESTION); return QUESTION; };
{SEMI} { yylval.a=add_sym_seq("SMEI",SEMI); return SEMI; };
{ASSIGNOP} { yylval.a=add_sym_seq("ASSIGNOP",ASSIGNOP); return ASSIGNOP; };
{PLUS} { yylval.a=add_sym_seq("PLUS",PLUS); return PLUS; };
{MINUS} { yylval.a=add_sym_seq("MINUS",MINUS); return MINUS; };
{STAR} { yylval.a=add_sym_seq("STAR",STAR); return STAR; };
{DIV} { yylval.a=add_sym_seq("DIV",DIV); return DIV; };
{AND} { yylval.a=add_sym_seq("AND",AND); return AND; };
{OR} { yylval.a=add_sym_seq("OR",OR); return OR; };
{NOT} { yylval.a=add_sym_seq("NOT",NOT); return NOT; };
{EQUAL} { yylval.a=add_sym_seq("EQUAL",EQUAL); return EQUAL; };
{LESS} { yylval.a=add_sym_seq("LESS",LESS); return LESS; };
{BIGGER} { yylval.a=add_sym_seq("BIGGER",BIGGER); return BIGGER; };
{LESSEQUAL} { yylval.a=add_sym_seq("LESSEQUAL",LESSEQUAL); return LESSEQUAL; };
{BIGGEREQUAL} { yylval.a=add_sym_seq("BIGGEREQUAL",BIGGEREQUAL); return BIGGEREQUAL; };
{NOTEQUAL} { yylval.a=add_sym_seq("NOTEQUAL",NOTEQUAL); return NOTEQUAL; };
{LP} { yylval.a=add_sym_seq("LP",LP); return LP; };
{RP} { yylval.a=add_sym_seq("RP",RP); return RP; };
{LB} { yylval.a=add_sym_seq("LB",LB); return LB; };
{RB} { yylval.a=add_sym_seq("RB",RB); return RB; };
{LC} { yylval.a=add_sym_seq("LC",LC); return LC; };
{RC} { yylval.a=add_sym_seq("RC",RC); return RC; };
{STRUCT} { yylval.a=add_sym_seq("STRUCT",STRUCT); return STRUCT; };
{CASE} { yylval.a=add_sym_seq("CASE",CASE); return CASE; };
{IF} { yylval.a=add_sym_seq("IF",IF); return IF; };
{ELSE} { yylval.a=add_sym_seq("ELSE",ELSE); return ELSE; };
{WHILE} { yylval.a=add_sym_seq("WHILE",WHILE); return WHILE; };

{TYPE} { yylval.a=add_sym_seqx("TYPE",TYPE,add_str_tbl(yytext)); return TYPE; }; 
{ID} { yylval.a=add_sym_seqx("ID",ID,add_str_tbl(yytext)); return ID; }; 

{INT} { yylval.a=add_sym_seqx("INT",INT,add_str_tbl(yytext)); sscanf(yytext,"%d",&yylval.a->value.ival); return INT; };
{INT_8} { yylval.a=add_sym_seqx("INT_8",INT_8,add_str_tbl(yytext)); sscanf(yytext,"%o",&yylval.a->value.ival); return INT_8; };
{INT_16} { yylval.a=add_sym_seqx("INT_16",INT_16,add_str_tbl(yytext)); sscanf(yytext,"%x",&yylval.a->value.ival); return INT_16; };
{FLOAT} { yylval.a=add_sym_seqx("FLOAT",FLOAT,add_str_tbl(yytext)); sscanf(yytext,"%f",&yylval.a->value.fval); return FLOAT; };

\" { strbuff=p_str_tbl; BEGIN(STRING); };
<STRING>\\. { strcpy(string_table+strbuff,yytext); strbuff+=strlen(yytext); };
<STRING>\" { 
            string_table[strbuff++]='\0'; 
            symbol_sequence[symbols].sym_type=STRING; 
            symbol_sequence[symbols].sym_str="STRING";
            symbol_sequence[symbols].value.pstr=string_table+p_str_tbl;
            p_str_tbl=strbuff;
            symbols++; 
            BEGIN(INITIAL);
            return STRING;
};
<STRING>[^\r\n"] { strcpy(string_table+strbuff,yytext); strbuff+=strlen(yytext);  };

{CHAR} { yylval.a=add_sym_seqx("CHAR",CHAR,add_str_tbl(yytext)); return CHAR; }; 

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
