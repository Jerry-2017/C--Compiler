#define __KERNEL_C__

#include "../include/common.h"

void init()
{
    //printf("start to init\n");
    //printf("start to init symbol\n");
    init_sym();
    //printf("start to init syntax_acation\n");
    init_syntax_action();
    //printf("start to init type\n");
    init_type();
    //printf("start to init variable\n");
    init_variable();
}

void travel(_SI* node,int depth)
{
    if (node==root)
        printf("Start SDT Processing\n");
    printf("%s\n",sym_str(node->sym_type));
    int i=0;
    _SI* nl[10];
    int cnt=node->cldno;
    if (cnt>0)
    {
        _SI* p=node->lc;
        nl[i++]=p;
        cnt--;
        while (cnt>0)
        {
            p=p->rc;
            nl[i++]=p;
            cnt--;
        }
    }
    cnodelist=nl;
    do_syntax_action(node->action_id,ROOT_FIRST_ACTION,node);
    i=1;

    cnt=node->cldno;
    if (cnt>0)
    {
        _SI* p=node->lc;
        travel(p,depth+1);
        cnodelist=nl;
        do_syntax_action(node->action_id,i++,node);
        cnt--;
        while (cnt>0)
        {
            p=p->rc;
            travel(p,depth+1);
            cnt--;
        }
    }
    cnodelist=nl;
    do_syntax_action(node->action_id,ROOT_LAST_ACTION,node);
}

void travel_syntax_tree(_SI* node,int depth)
{
    int i;
    for (i=0;i<depth*2;i++)
        printf(" ");
    printf("%s",sym_str(node->sym_type));
    if (node->sym_type==S_ID)
    {
        printf (" : %s",node->value.pstr);
    }
    if (node->sym_type==S_FLOAT)
    {
        printf (" : %f",node->value.fval);
    }
    if (node->sym_type==S_INT_10 || node->sym_type==S_INT_8 || node->sym_type==S_INT_16)
    {
        printf (" : %d",node->value.ival);
    }
    printf(" [%d]",node->lineno);
    printf("\n");
    int cnt=node->cldno;
    if (cnt>0)
    {
        _SI* p=node->lc;
        travel_syntax_tree(p,depth+1);
        cnt--;
        while (cnt>0)
        {
            p=p->rc;
            travel_syntax_tree(p,depth+1);
            cnt--;
        }
    }
}
