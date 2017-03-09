%x STRING COMMENT CHAR

%{
    #include "string.h"
    #define MAX_STRLEN 0xfffff
    #define MAX_SYMBOLNUM 0xfffff
    typedef struct SymbolItem {
        char *sym_str;
        int sym_type;
    } _SI;
    typedef enum SymbolType{
        ID, //Identifier
        KEY,    //Reserved Keyword    
        INT, FLOAT, STRING, CHAR,
        TYPE,
        DOT, COMMA, QUESTION, SEMI,
        ASSIGNOP,
        PLUS, MINUS, STAR, DIV, AND, OR, NOT,
        EQUAL, LESS, BIGGER,  LESSEQUAL, BIGGEREQUAL, NOTEQUAL,
        LP, RP, LB, RB, LC, RC,
        DEFINE, STRUCT, CASE, IF, ELSE, WHILE 
    } _ST;
    void add_sym_seq(char *p_str, int sym_type);
    char *add_str_tbl(char *p_str);
    int lines=0;
    int symbols=0;
    int p_str_tbl=0;
    int strbuff;
    char string_table[MAX_STRLEN];
    _SI symbol_sequence[MAX_SYMBOLNUM];
%}

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
DEFINE "define"
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
DIGITS_8 0|([1-7][0-7]*)
BLANKS [ \r\n\t]


FLOAT [+-]?{DIGITS_10_NLZ}({DOT}{DIGITS_10})?([eE]([+-]?){DIGITS_10_NLZ})?
INT [+-]?{DIGITS_10_NLZ}

CHAR \'[^\'\n]\'
ID ({ALPHABET}|_)({ALPHABET}|_|{DIGITS_10})*
COMMENT1 "//"[^\r\n]*

%%

{DOT} { printf("DOT "); add_sym_seq(NULL,DOT); };
{COMMA} { printf("COMMA "); add_sym_seq(NULL,COMMA); };
{QUESTION} { printf("QUESTION "); add_sym_seq(NULL,QUESTION); };
{SEMI} { printf("SEMI "); add_sym_seq(NULL,SEMI); };
{ASSIGNOP} { printf("ASSIGNOP "); add_sym_seq(NULL,ASSIGNOP); };
{PLUS} { printf("PLUS "); add_sym_seq(NULL,PLUS); };
{MINUS} { printf("MINUS "); add_sym_seq(NULL,MINUS); };
{STAR} { printf("STAR "); add_sym_seq(NULL,STAR); };
{DIV} { printf("DIV "); add_sym_seq(NULL,DIV); };
{AND} { printf("AND "); add_sym_seq(NULL,AND); };
{OR} { printf("OR "); add_sym_seq(NULL,OR); };
{NOT} { printf("NOT "); add_sym_seq(NULL,NOT); };
{EQUAL} { printf("EQUAL "); add_sym_seq(NULL,EQUAL); };
{LESS} { printf("LESS "); add_sym_seq(NULL,LESS); };
{BIGGER} { printf("BIGGER "); add_sym_seq(NULL,BIGGER); };
{LESSEQUAL} { printf("LESSEQUAL "); add_sym_seq(NULL,LESSEQUAL); };
{BIGGEREQUAL} { printf("BIGGEREQUAL "); add_sym_seq(NULL,BIGGEREQUAL); };
{NOTEQUAL} { printf("NOTEQUAL "); add_sym_seq(NULL,NOTEQUAL); };
{LP} { printf("LP "); add_sym_seq(NULL,LP); };
{RP} { printf("RP "); add_sym_seq(NULL,RP); };
{LB} { printf("LB "); add_sym_seq(NULL,LB); };
{RB} { printf("RB "); add_sym_seq(NULL,RB); };
{LC} { printf("LC "); add_sym_seq(NULL,LC); };
{RC} { printf("RC "); add_sym_seq(NULL,RC); };
{DEFINE} { printf("DEFINE "); add_sym_seq(NULL,DEFINE); };
{STRUCT} { printf("STRUCT "); add_sym_seq(NULL,STRUCT); };
{CASE} { printf("CASE "); add_sym_seq(NULL,CASE); };
{IF} { printf("IF "); add_sym_seq(NULL,IF); };
{ELSE} { printf("ELSE "); add_sym_seq(NULL,ELSE); };
{WHILE} { printf("WHILE "); add_sym_seq(NULL,WHILE); };

{TYPE} { printf("TYPE "); add_sym_seq(add_str_tbl(yytext),TYPE); }; 
{ID} { printf("ID "); add_sym_seq(add_str_tbl(yytext),ID); }; 

{INT} { printf("INT "); add_sym_seq(add_str_tbl(yytext),INT); };
{FLOAT} { printf("FLOAT "); add_sym_seq(add_str_tbl(yytext),FLOAT); };

\" { strbuff=p_str_tbl; BEGIN(STRING); };
<STRING>\\. { strcpy(string_table+strbuff,yytext); strbuff+=strlen(yytext); };
<STRING>\" { 
            string_table[strbuff++]='\0'; 
            symbol_sequence[symbols].sym_type=STRING; 
            symbol_sequence[symbols].sym_str=string_table+p_str_tbl;
            p_str_tbl=strbuff;
            symbols++; 
            BEGIN(INITIAL);
};
<STRING>[^\r\n"] { strcpy(string_table+strbuff,yytext); strbuff+=strlen(yytext); };

{CHAR} { printf("CHAR "); add_sym_seq(add_str_tbl(yytext),CHAR); }; 

{COMMENT1} { printf("COMMENT1 "); };

\/\* { printf("COMMENT2"); BEGIN(COMMENT); };
<COMMENT>\*\/ { BEGIN(INITIAL); };
<COMMENT>.|\n { };

"\n" { printf("\n%d:    ",++lines); }
{BLANKS} { printf("%s",yytext); }

. { printf("\n****Error Type A occured at line %d, dealing lexema %s\n",lines,yytext); }

%%

void add_sym_seq(char *p_str, int sym_type)
{
    symbol_sequence[symbols].sym_str=p_str;
    symbol_sequence[symbols].sym_type=sym_type;
    symbols++;
}

char *add_str_tbl(char *p_str){
    strcpy(string_table+p_str_tbl,p_str);
    int ret_value=p_str_tbl;
    p_str_tbl+=strlen(p_str)+1;
    return string_table+ret_value;
}


int main(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            perror(argv[1]);
            return 1;
        }
    }
    printf("FileName : %s\n",argv[1]);
    printf("0:  ");
    yylex();
    printf("\n-------------------\n");
    return 0;
}
