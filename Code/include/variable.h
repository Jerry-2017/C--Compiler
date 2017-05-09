#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#define MAX_VARIABLE_NUM 0xffff
#define MAX_ENV_STACK 0xff

#define ENV_BLOCK 0
#define ENV_FUNC_PARM_DEC 1
#define ENV_STRUCT_DEF 2


#include "../include/common.h"

typedef struct VariableTableItem
{
    char *name;
    int var_type;
    int env_type;
    int env_id;
    union {
        struct {
            int base;
        }_array;
        struct {
            int base;
        }_struct;                
        float fval;
        int ival;
        char* pstr;
    };
}_VI;


#ifdef __VARIABLE_C__

int vt_cnt;
_VI var_table[MAX_VARIABLE_NUM];
int stack_env[MAX_ENV_STACK][2]; // Mode Attribute
int block_id;
int fpdec_cnt;
int se_cnt;

#else

extern _VI var_table[MAX_VARIABLE_NUM];
extern int vt_cnt;
extern int stack_env[MAX_ENV_STACK][2];
extern int se_cnt;
extern int block_id;

#endif

void init_variable();
int add_variable(char *var_name,int type_id);
int compare_type(int var1_id,int var2_id);
int get_variable(char *var_name);
int get_variable_allenv(char *var_name);

void new_env_block();
void new_env_func_param_dec(int funcid);
void new_env_struct_dec(int structid);
void exit_env();


#endif
