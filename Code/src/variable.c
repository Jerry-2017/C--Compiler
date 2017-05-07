#define __VARIABLE_C__

#include "../include/common.h"

void init_variable()
{
    vt_cnt=0;
    se_cnt=-1;
    block_id=0;
    new_env_block();
}

void new_env_block()
{
    se_cnt++;
    stack_env[se_cnt][0]=ENV_BLOCK;
    stack_env[se_cnt][1]=block_id++;
}

void new_env_func_param_dec(int funcid)
{
    se_cnt++;
    stack_env[se_cnt][0]=ENV_FUNC_PARM_DEC;
    stack_env[se_cnt][1]=funcid;
}

void new_env_struct_dec(int structid)
{
    se_cnt++;
    stack_env[se_cnt][0]=ENV_STRUCT_DEF;
    stack_env[se_cnt][1]=structid;
}

void exit_env()
{
    se_cnt--;
}

int add_variable(char *var_name,int type_id)
{
    var_table[vt_cnt].name=var_name;
    var_table[vt_cnt].var_type=type_id;
    return vt_cnt++;
}

int get_variable(char *var_name)
{
    int i=0;
    for (i=0;i<vt_cnt;i++)
    {
        if (strcmp(var_name,var_table[i].name)==0)
            return i;
    }
    return -1;
}


int compare_type(int var1_id,int var2_id)
{
    return var_table[var1_id].var_type==var_table[var2_id].var_type;
}


