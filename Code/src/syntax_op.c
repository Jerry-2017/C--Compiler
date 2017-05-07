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
    REG_OP_FUNC(basic_type_val,ROOT_FIRST_ACTION);
    REG_OP_FUNC(var_ref,ROOT_FIRST_ACTION);
}

void bind_sym_action(_SI* node,int action_id)
{
    node->action_id=action_id;
}

void register_action(int action_id,int action_type,void (*callee) (_SI*) )
{
    if (action_type>0) action_type++;
    if (action_type==-1) action_type=1;
    if (symbol_action_table[action_id][action_type].isreg==true)
        printf("Duplicate action register id %d\n",action_id);
    else {
        symbol_action_table[action_id][action_type].isreg=true;
        symbol_action_table[action_id][action_type].callee=callee;
    }
}

void do_syntax_action(int action_id, int action_type,_SI* node)
{
    if (action_type>0) action_type++;
    if (action_type==-1) action_type=1;
    if (action_id>=0 && symbol_action_table[action_id][action_type].isreg==true)
    {
        symbol_action_table[action_id][action_type].callee(node);
    }
}

MAKE_OP_FUNC(array_def,ROOT_FIRST_ACTION)
{
    int size=cnodelist[2]->value.ival;
    cnodelist[0]->val_type_id=add_type_array(node->val_type_id,size);
}

MAKE_OP_FUNC(var_def,ROOT_FIRST_ACTION)
{
    char *vname=cnodelist[0]->value.pstr;
    //printf("variable %s size %d var_id %d\n",vname,type_table[node->val_type_id].size,get_variable(vname));
    if (get_variable(vname)!=-1)
    {
        errorrec=true;
        printf("Error Type 3 on line %d : Duplicate Variable Definition for %s\n",cnodelist[0]->lineno,vname);
    }
    else
        node->var_id=add_variable(vname,node->val_type_id);
}

MAKE_OP_FUNC(var_ref,ROOT_FIRST_ACTION)
{
    char *vname=cnodelist[0]->value.pstr;
    int vid=get_variable(vname);
    if (vid==-1)
    {
        errorrec=true;
        printf("Error Type 1 on line %d : Undefined Variable Occurence for %s\n",cnodelist[0]->lineno,vname);
    }
    else
        node->var_id=vid;
}

MAKE_OP_FUNC(basic_type_val,ROOT_FIRST_ACTION)
{
    memcpy(&cnodelist[0]->value,&node->value,sizeof(node->value));
    if (node->sym_type==S_INT)
        node->val_type_id=find_type("int",BASIC_TYPE);
    else if (node->sym_type==S_FLOAT)
        node->val_type_id=find_type("float",BASIC_TYPE);
}
