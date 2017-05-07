#define __SYNTAX_OP_C__

#include "../include/common.h"

void init_syntax_action()
{
    sat_cnt=0;
    int i,j;
    for (i=0;i<MAX_SYNTAX_ACTION;i++)
        for (j=0;j<MAX_CHILDREN_EXPANSION;j++)
            symbol_action_table[i][j].isreg=false;

    REG_OP_FUNC(array_def,ROOT_FIRST_ACTION);
    REG_OP_FUNC(var_def,ROOT_FIRST_ACTION);
}

void bind_sym_action(_SI* node,int action_id)
{
    node->action_id=action_id;
}

void register_action(int action_id,int action_type,void (*callee) (_SI*) )
{
    if (action_id>0) action_id++;
    if (action_id==-1) action_id=1;
    if (symbol_action_table[action_id][action_type].isreg==true)
        printf("Duplicate action register id %d\n",action_id);
    else {
        symbol_action_table[action_id][action_type].isreg=true;
        symbol_action_table[action_id][action_type].callee=callee;
    }
}

void do_syntax_action(int action_id, int action_type,_SI* node)
{
    if (action_id>0 && symbol_action_table[action_id][action_type].isreg==true)
        (*symbol_action_table[action_id][action_type].callee)(node);
}

MAKE_OP_FUNC(array_def,ROOT_FIRST_ACTION)
{
    int size=cnodelist[2]->value.ival;
    cnodelist[0]->val_type_id=add_type_array(node->val_type_id,size);
}

MAKE_OP_FUNC(var_def,ROOT_FIRST_ACTION)
{
    printf("variable %s size %d\n",cnodelist[0]->sym_str,type_table[node->val_type_id].size);
    add_variable(cnodelist[0]->sym_str,node->val_type_id);
}
