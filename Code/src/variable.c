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
    fpdec_cnt=0;
    stack_env[se_cnt][0]=ENV_FUNC_PARM_DEC;
    stack_env[se_cnt][1]=funcid;
    func_table[funcid].arg_pos=vt_cnt;
}

void new_env_func_dec_check(int funcid)
{
    se_cnt++;
    fpdec_cnt=0;
    stack_env[se_cnt][0]=ENV_FUNC_PARM_DEC_CHECK;
    stack_env[se_cnt][1]=funcid;
}

void new_env_struct_def(int structid)
{
    se_cnt++;
    stack_env[se_cnt][0]=ENV_STRUCT_DEF;
    stack_env[se_cnt][1]=structid;
    type_table[structid]._struct.elempos=-1;
    type_table[structid]._struct.elemsize=0;
    type_table[structid]._struct.lastpos=-1;
}

void exit_env()
{
    switch (stack_env[se_cnt][0])
    {
        case ENV_BLOCK:
            break;
        case ENV_FUNC_PARM_DEC:
        {
            int fid=stack_env[se_cnt][1];
            func_table[fid].arg_size=fpdec_cnt;
            break;
        }
        case ENV_FUNC_PARM_DEC_CHECK:
        {
            int fid=stack_env[se_cnt][1];
            if ( func_table[fid].arg_size!=fpdec_cnt)
                func_dec_cons=false;
        }
        case ENV_STRUCT_DEF:
            break;
    }
    se_cnt--;
}

int add_variable(char *var_name,int type_id)
{
    //printf("varname %s type %d\n",var_name,type_id);
    switch (stack_env[se_cnt][0])
    {
        case ENV_BLOCK:
        {
            var_table[vt_cnt].name=var_name;
            var_table[vt_cnt].var_type=type_id;
            var_table[vt_cnt].env_type=ENV_BLOCK;            
            var_table[vt_cnt].env_id=stack_env[se_cnt][1];
            var_table[vt_cnt].inter_var_id=-1;
            return vt_cnt++;
        }
        case ENV_FUNC_PARM_DEC:
        {
            int fid=stack_env[se_cnt][1];
            int base=func_table[fid].arg_pos;
            var_table[vt_cnt].env_type=ENV_FUNC_PARM_DEC;
            var_table[vt_cnt].env_id=fid;
            var_table[vt_cnt].name=var_name;
            var_table[vt_cnt].var_type=type_id;
            var_table[vt_cnt].inter_var_id=-1;
            fpdec_cnt++;
            return vt_cnt++;
        }
        case ENV_FUNC_PARM_DEC_CHECK:
        {
            int fid=stack_env[se_cnt][1];
            int base=func_table[fid].arg_pos;
            int vid=base+fpdec_cnt;
            if (var_table[vid].var_type != type_id || strcmp(var_table[vid].name,var_name)!=0)
            {
                func_dec_cons=false;
            }
            return base+fpdec_cnt++;
        }
        case ENV_STRUCT_DEF:
        {
            int sid=stack_env[se_cnt][1];
            if (type_table[sid]._struct.elempos==-1)
            {
                type_table[sid]._struct.elempos=vt_cnt;
                type_table[sid]._struct.lastpos=vt_cnt;
            }
            else
            {
                int varid=type_table[sid]._struct.lastpos;
                var_table[varid].next_var=vt_cnt;
                type_table[sid]._struct.lastpos=vt_cnt;
            }
            var_table[vt_cnt].env_type=ENV_STRUCT_DEF;
            var_table[vt_cnt].env_id=sid;
            var_table[vt_cnt].name=var_name;
            var_table[vt_cnt].var_type=type_id;
            var_table[vt_cnt].next_var=-1;
            type_table[sid]._struct.elemsize+=1;
            return vt_cnt++;
        }
    }
}

int get_variable_allenv(char *var_name)
{
    int _scnt=se_cnt;
    int i;
    while (_scnt>=0)
    {
        for (i=0;i<vt_cnt;i++)
            if (var_table[i].env_type==stack_env[_scnt][0] && var_table[i].env_id==stack_env[_scnt][1])
            {
                if (strcmp(var_name,var_table[i].name)==0)
                    return i;
            }
        _scnt--;
    }
    return -1;

}

int get_variable(char *var_name)
{
    int i;
    int _scnt=se_cnt;
    for (i=0;i<vt_cnt;i++)
        if (var_table[i].env_type==stack_env[_scnt][0] && var_table[i].env_id==stack_env[_scnt][1])
        {
            if (strcmp(var_name,var_table[i].name)==0)
                return i;
        }

    return -1;
}

int get_struct_variable(int stid,char *var_name)
{
    int cnt=type_table[stid]._struct.elemsize;
    int start=type_table[stid]._struct.elempos;
    int i=0;
    for (;i<cnt;i++)
    {
        if (strcmp(var_table[start].name,var_name)==0)
            return start;
        start=var_table[start].next_var;
    }
    return -1;
}

int compare_type(int var1_id,int var2_id)
{
    return var_table[var1_id].var_type==var_table[var2_id].var_type;
}



