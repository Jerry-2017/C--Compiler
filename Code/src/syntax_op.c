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
    REG_OP_FUNC(pass_declist,ROOT_FIRST_ACTION);

    REG_OP_FUNC(pass_def,1);
    REG_OP_FUNC(func_arg_def,1);

    REG_OP_FUNC(func_arg_def,ROOT_LAST_ACTION);
}

void bind_sym_action(_SI* node,int action_id)
{
    node->action_id=action_id;
}

void register_action(int action_id,int action_type,void (*callee) (_SI*) )
{
    if (action_type==0x10) action_type=1;
    else if (action_type>0) action_type++;
    if (symbol_action_table[action_id][action_type].isreg==true)
        printf("Duplicate action register id %d\n",action_id);
    else {
        symbol_action_table[action_id][action_type].isreg=true;
        symbol_action_table[action_id][action_type].callee=callee;
    }
}

void do_syntax_action(int action_id, int action_type,_SI* node)
{
    if (action_type==0x10) action_type=1;
    else if (action_type>0) action_type++;
    if (action_id>=0 && symbol_action_table[action_id][action_type].isreg==true)
    {
        symbol_action_table[action_id][action_type].callee(node);
    }
}

void syntax_error(int error_id,int lineno,char *error_des)
{
    printf("Error type %d at Line %d: %s\n",error_id,lineno,error_des);
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
        char tp[0x100];
        sprintf(tp,"Redefined Variable \"%s\".\n",vname);
        syntax_error(3,cnodelist[0]->lineno,tp);
    }
    else
        node->var_id=add_variable(vname,node->val_type_id);
}

MAKE_OP_FUNC(var_ref,ROOT_FIRST_ACTION)
{
    char *vname=cnodelist[0]->value.pstr;
    int vid=get_variable_allenv(vname);
    if (vid==-1)
    {
        errorrec=true;
        char tp[0x100];
        sprintf(tp,"Undefined Variable \"%s\".\n",vname);
        syntax_error(1,cnodelist[0]->lineno,tp);
    }
    else
        node->var_id=vid;
}

MAKE_OP_FUNC(pass_def,1)
{
    cnodelist[1]->val_type_id=cnodelist[0]->val_type_id;
}

MAKE_OP_FUNC(pass_declist,ROOT_FIRST_ACTION)
{
    if (node->sym_affix_type==0)
    {
        cnodelist[0]->val_type_id=node->val_type_id;
    }
    else
    {
        cnodelist[0]->val_type_id=node->val_type_id;
        cnodelist[2]->val_type_id=node->val_type_id;
    }
}

MAKE_OP_FUNC(basic_type_val,ROOT_FIRST_ACTION)
{
    memcpy(&cnodelist[0]->value,&node->value,sizeof(node->value));
    if (node->sym_type==S_INT)
        node->val_type_id=find_type("int",BASIC_TYPE);
    else if (node->sym_type==S_FLOAT)
        node->val_type_id=find_type("float",BASIC_TYPE);
}

MAKE_OP_FUNC(func_arg_def,1)
{
    char *funcname=cnodelist[0]->value.pstr;
    if (find_func(funcname)!=-1)
    {
        errorrec=true;
        char tp[0x100];
        sprintf(tp,"Redefined function  \"%s\".\n",funcname);
        syntax_error(4,cnodelist[0]->lineno,tp);
    }
    else
    {
        int func_id=add_func(funcname);
        new_env_func_param_dec(func_id);
    }
}

MAKE_OP_FUNC(func_arg_def,ROOT_LAST_ACTION)
{
    exit_env();
}
