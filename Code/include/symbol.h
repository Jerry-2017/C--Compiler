#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "common.h"


#define MAX_SYMBOLNUM 0xfffff

typedef enum SymbolType{
    S_END,
    S_ID, //Identifier
    S_KEY,    //Reserved Keyword    
    S_INT_10, S_INT_8, S_INT_16, S_FLOAT, S_STRING, S_CHAR,
    S_TYPE,
    S_DOT, S_COMMA, S_QUESTION, S_SEMI,
    S_ASSIGNOP,
    S_PLUS, S_MINUS, S_STAR, S_DIV, S_AND, S_OR, S_NOT,
    S_EQUAL, S_LESS, S_BIGGER,  S_LESSEQUAL, S_BIGGEREQUAL, S_NOTEQUAL,
    S_LP, S_RP, S_LB, S_RB, S_LC, S_RC,
    S_DEFINE, S_STRUCT, S_CASE, S_IF, S_ELSE, S_WHILE , S_RETURN,
    S_ERROR,

    S_PROGRAM,
    S_RELOP,
    S_EXTDEFLIST,
    S_EXTDEF,
    S_EXTDECLIST,
    S_SPECIFER,
    S_STRUCTSPECIFER,
    S_OPTTAG,
    S_TAG,
    S_VARDEC,
    S_FUNDEC,
    S_VARLIST,
    S_PARAMDEC,
    S_COMPST,
    S_STMTLIST,
    S_STMT,
    S_DEFLIST,
    S_DEF,
    S_DECLIST,
    S_DEC,
    S_EXP,
    S_INT,
    S_ARGS
} _ST;

typedef struct SymbolItem {

    char *sym_str;
    bool reverse_scan;
    
    int sym_type;
    int sym_affix_type;
    int action_id;

    int compst_func_id;
    int func_id;

    int context_relate_id;

    int val_type_id;
    int var_id;
    bool is_left_val;

    int func_ret_type;

    int lineno;
    int cldno; //Child Number
    struct SymbolItem *lc,*rc;
    union {
        int ival;
        float fval;
        int ID;
        char *pstr;
    }value;
} _SI;

#ifdef __SYMBOL_C__

char string_table[MAX_STRLEN];

_SI symbol_sequence[MAX_SYMBOLNUM];

char* sym_trans_table[0x100];

int symbols=0;
int p_str_tbl=0;
int strbuff;

#else

extern char string_table[MAX_STRLEN];

extern _SI symbol_sequence[MAX_SYMBOLNUM];

extern char* sym_trans_table[0x100];

extern int symbols;
extern int p_str_tbl;
extern int strbuff;

#endif

_SI* get_nth_child(_SI* node,int x);
_SI* add_sym_seq(int sym_type);
_SI* add_sym_seqx(int sym_type,char *val);
char *add_str_tbl(char *p_str);
void travel(_SI* node,int depth);
_SI* add_sym_node(int sym_type,int num , ...);
void init_sym();
char* sym_str(int sym_type);
void init_sym_trans();

#endif
