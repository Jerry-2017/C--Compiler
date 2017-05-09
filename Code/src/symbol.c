#define __SYMBOL_C__

#include "../include/common.h"

void init_sym()
{
    memset(symbol_sequence,0,sizeof(symbol_sequence));
    memset(string_table,0,sizeof(string_table));
    init_sym_trans();
    p_str_tbl=0;
    symbols=0;
}

void init_sym_trans()
{
    sym_trans_table[S_END]="End";
    sym_trans_table[S_ID]="Id";
    sym_trans_table[S_KEY]="Key";
    sym_trans_table[S_INT_10]="Int_10";
    sym_trans_table[S_INT_8]="Int_8";
    sym_trans_table[S_INT_16]="Int_16";
    sym_trans_table[S_FLOAT]="Float";
    sym_trans_table[S_STRING]="String";
    sym_trans_table[S_CHAR]="Char";
    sym_trans_table[S_TYPE]="Type";
    sym_trans_table[S_DOT]="Dot";
    sym_trans_table[S_COMMA]="Comma";
    sym_trans_table[S_QUESTION]="Question";
    sym_trans_table[S_SEMI]="Semi";
    sym_trans_table[S_ASSIGNOP]="Assignop";
    sym_trans_table[S_PLUS]="Plus";
    sym_trans_table[S_MINUS]="Minus";
    sym_trans_table[S_STAR]="Star";
    sym_trans_table[S_DIV]="Div";
    sym_trans_table[S_AND]="And";
    sym_trans_table[S_OR]="Or";
    sym_trans_table[S_NOT]="Not";
    sym_trans_table[S_LP]="Lp";
    sym_trans_table[S_RP]="Rp";
    sym_trans_table[S_LB]="Lb";
    sym_trans_table[S_RB]="Rb";
    sym_trans_table[S_LC]="Lc";
    sym_trans_table[S_RC]="Rc";
    sym_trans_table[S_DEFINE]="Define";
    sym_trans_table[S_STRUCT]="Struct";
    sym_trans_table[S_RETURN]="Return";
    sym_trans_table[S_CASE]="Case";
    sym_trans_table[S_IF]="If";
    sym_trans_table[S_ELSE]="Else";
    sym_trans_table[S_WHILE]="While";
    sym_trans_table[S_ERROR]="Error";
    sym_trans_table[S_PROGRAM]="Program";
    sym_trans_table[S_EXTDEFLIST]="Extdeflist";
    sym_trans_table[S_EXTDEF]="Extdef";
    sym_trans_table[S_EXTDECLIST]="Extdeclist";
    sym_trans_table[S_SPECIFER]="Specifer";
    sym_trans_table[S_STRUCTSPECIFER]="Structspecifer";
    sym_trans_table[S_OPTTAG]="Opttag";
    sym_trans_table[S_TAG]="Tag";
    sym_trans_table[S_VARDEC]="Vardec";
    sym_trans_table[S_FUNDEC]="Fundec";
    sym_trans_table[S_VARLIST]="Varlist";
    sym_trans_table[S_PARAMDEC]="Paramdec";
    sym_trans_table[S_COMPST]="Compst";
    sym_trans_table[S_STMTLIST]="Stmtlist";
    sym_trans_table[S_STMT]="Stmt";
    sym_trans_table[S_DEFLIST]="Deflist";
    sym_trans_table[S_DEF]="Def";
    sym_trans_table[S_DECLIST]="Declist";
    sym_trans_table[S_DEC]="Dec";
    sym_trans_table[S_EXP]="Exp";
    sym_trans_table[S_INT]="Int";
    sym_trans_table[S_ARGS]="Args";
    sym_trans_table[S_BIGGER]=">";
    sym_trans_table[S_LESS]="<";
    sym_trans_table[S_EQUAL]="==";
    sym_trans_table[S_NOTEQUAL]="!=";
    sym_trans_table[S_BIGGEREQUAL]=">=";
    sym_trans_table[S_LESSEQUAL]="<=";
    sym_trans_table[S_RELOP]="Relop";

}

char* sym_str(int sym_type)
{
    return sym_trans_table[sym_type];
}

_SI* add_sym_seq(int sym_type)
{
    return add_sym_seqx(sym_type,NULL);
}

_SI* add_sym_seqx(int sym_type,char *val)
{
    //printf("*x%s\n ",sym_str(sym_type));
    //symbol_sequence[symbols].sym_str=p_str;
    symbol_sequence[symbols].lineno=yylineno;
    symbol_sequence[symbols].sym_type=sym_type;
    symbol_sequence[symbols].value.pstr=val;
    symbol_sequence[symbols].cldno=0;
    symbol_sequence[symbols].action_id=-1;
    symbol_sequence[symbols].reverse_scan=false;
    symbol_sequence[symbols].var_id=-1;
    symbol_sequence[symbols].compst_func_id=-1;
    symbol_sequence[symbols].func_id=-1;
    return &symbol_sequence[symbols++];
}

char *add_str_tbl(char *p_str){
    strcpy(string_table+p_str_tbl,p_str);
    int ret_value=p_str_tbl;
    p_str_tbl+=strlen(p_str)+1;
    return string_table+ret_value;
}

_SI* get_nth_child(_SI* node,int x)
{
    int cnt=node->cldno;
    if (x>cnt || x<=0) {
        printf("Overflow in get nth child\n");
        return node;
    }

    _SI* p=node->lc;
    x--;
    while (x>0)
    {
        p=p->rc;
        x--;
    }
    return p;
}

_SI* add_sym_node(int sym_type, int num, ...)
{
    va_list valist;
    //printf("%s ",sym_str);
    //printf("%s\n",sym_str(sym_type));
    _SI *psi,*cpsi=&symbol_sequence[symbols++];  
    cpsi->action_id=-1;  
    cpsi->reverse_scan=false;
    cpsi->sym_type=sym_type;
    cpsi->var_id=-1;
    cpsi->func_id=-1;
    cpsi->compst_func_id=-1;
    cpsi->sym_str=sym_str(sym_type);
    cpsi->lineno=yylineno;
    va_start(valist,num);
    
    cpsi->lc=NULL; cpsi->rc=NULL;
    int i;
    if(num>0)
    {
        psi=va_arg(valist, _SI*);
        cpsi->lc=psi;
        cpsi->lineno=psi->lineno;
        if(num>1)
        {
            for(i=0; i<num-1; ++i)
            {
                psi->rc=va_arg(valist,_SI*);
                psi=psi->rc;
            }
        }
    }
    cpsi->cldno=num;
    return cpsi;
}
