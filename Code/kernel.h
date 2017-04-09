#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

extern int yylineno;
extern char* yytext;
typedef struct YYLTYPE  {  
    int first_line;  
    int first_column;  
    int last_line;  
    int last_column;  
} YYLTYPE;

//#define NULL (void*)0
#define MAX_STRLEN 0xfffffff
#define MAX_SYMBOLNUM 0xfffff
#define true 1
#define false 0
typedef struct SymbolItem {
    char *sym_str;
    int sym_type;
    int cldno;
    struct SymbolItem *lc,*rc;
    union {
        int ival;
        float fval;
        int ID;
        char *pstr;
    }value;
} _SI;

/*
    typedef enum SymbolType{
    END,
    ID, //Identifier
    KEY,    //Reserved Keyword    
    INT, INT_8, INT_16, FLOAT, STRING, CHAR,
    TYPE,
    DOT, COMMA, QUESTION, SEMI,
    ASSIGNOP,
    PLUS, MINUS, STAR, DIV, AND, OR, NOT,
    EQUAL, LESS, BIGGER,  LESSEQUAL, BIGGEREQUAL, NOTEQUAL,
    LP, RP, LB, RB, LC, RC,
    DEFINE, STRUCT, CASE, IF, ELSE, WHILE ,
    ERROR
} _ST;*/


_SI* add_sym_seq(char *p_str, int sym_type);
_SI* add_sym_seqx(char *p_str, int sym_type,char *val);
char *add_str_tbl(char *p_str);
void travel(_SI* node,int depth);
_SI* add_sym_node(char *sym_str,int num , ...);

#endif

#ifndef __GLOBAL__
#define __GLOBAL__
extern _SI* root;
extern int _isend;
extern int lines;
extern int symbols;
extern int p_str_tbl;
extern int strbuff;
extern int errorrec;
extern char string_table[MAX_STRLEN];
extern _SI symbol_sequence[MAX_SYMBOLNUM];
#endif
