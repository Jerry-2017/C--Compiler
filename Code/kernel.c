#define __GLOBAL__

#include "kernel.h"
#include "syntax.tab.h"

_SI* root;
int _isend=false;
int lines=0;
int symbols=0;
int p_str_tbl=0;
int errorrec=0;
int strbuff;
char string_table[MAX_STRLEN];
_SI symbol_sequence[MAX_SYMBOLNUM];

_SI* add_sym_seq(char *p_str, int sym_type)
{
    return add_sym_seqx(p_str,sym_type,NULL);
}

_SI* add_sym_seqx(char *p_str, int sym_type,char *val)
{
    //printf("*x%s\n ",p_str);
    symbol_sequence[symbols].sym_str=p_str;
    symbol_sequence[symbols].sym_type=sym_type;
    symbol_sequence[symbols].value.pstr=val;
    symbol_sequence[symbols].cldno=0;
    return &symbol_sequence[symbols++];
}

char *add_str_tbl(char *p_str){
    strcpy(string_table+p_str_tbl,p_str);
    int ret_value=p_str_tbl;
    p_str_tbl+=strlen(p_str)+1;
    return string_table+ret_value;
}

_SI* add_sym_node(char * sym_str, int num, ...)
{
    va_list valist;
    //printf("%s ",sym_str);
    _SI *psi,*cpsi=&symbol_sequence[symbols++];    
    cpsi->sym_str=sym_str;
    va_start(valist,num);
    
    cpsi->lc=NULL; cpsi->rc=NULL;
    int i;
    if(num>0)
    {
        psi=va_arg(valist, _SI*);
        cpsi->lc=psi;
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

void init()
{
    memset(symbol_sequence,0,sizeof(symbol_sequence));
    memset(string_table,0,sizeof(string_table));
    p_str_tbl=0;
    symbols=0;

}

void travel(_SI* node,int depth)
{
    int i;
    for (i=0;i<depth*2;i++)
        printf(" ");
    printf("%s",node->sym_str);
    if (node->sym_type==ID)
    {
        printf (" : %s",node->value.pstr);
    }
    if (node->sym_type==FLOAT)
    {
        printf (" : %f",node->value.fval);
    }
    if (node->sym_type==INT || node->sym_type==INT_8 || node->sym_type==INT_16)
    {
        printf (" : %d",node->value.ival);
    }
    printf("\n");
    int cnt=node->cldno;
    if (cnt>0)
    {
        _SI* p=node->lc;
        travel(p,depth+1);
        cnt--;
        while (cnt>0)
        {
            p=p->rc;
            travel(p,depth+1);
            cnt--;
        }
    }
}
