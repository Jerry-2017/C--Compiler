#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#define MAX_VARIABLE_NUM 0xffff

#include "../include/common.h"

typedef struct VariableTableItem
{
    char *name;
    int var_type;
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

#else

extern _VI var_table[MAX_VARIABLE_NUM];
extern int vt_cnt;

#endif

void init_variable();
int add_variable(char *var_name,int type_id);
int compare_type(int var1_id,int var2_id);

#endif
