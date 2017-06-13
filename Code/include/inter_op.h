#include "../include/common.h"

#ifndef __INTER_OP_H__
#define __INTER_OP_H__

typedef enum InterOpType{

    IOP_READ,
    IOP_WRITE,

    IOP_LABEL,

    IOP_IF_LEQ,
    IOP_IF_L,
    IOP_IF_G,
    IOP_IF_GEQ,
    IOP_IF_EQ,
    IOP_IF_NE,

    IOP_RET,
    IOP_CALL,
    IOP_FUNC,
    IOP_ARG,
    IOP_PARAM,

    IOP_ASSIGN,
    IOP_ADD,
    IOP_MINUS,
    IOP_DIV,    
    IOP_MUL,    
    IOP_AT,
    IOP_LSTAR,
    IOP_RSTAR
};

typedef struct inter_op_node{
    int type; //0 VAR 1 LABEL 2 FUNC 3 OP_BLOCK
    union {
        struct{
            int var_id;
            bool is_global;
            int var_table_id;
        };
        int label_id;
        struct{
            int func_table_id;
        };
        struct{
            int op_start;
            int op_end;
        };
    };

} _ION;

#define MAX_INTER_OP 0xfffff
#define MAX_VAR_LEN 0xff

#ifdef __INTER_OP_C__

int inter_op_list[MAX_INTER_OP][2];
char inter_op_char[MAX_INTER_OP][MAX_VAR_LEN];
_ION inter_op_table[MAX_INTER_OP];
int inter_op_list_pointer;
int inter_op_table_pointer;
int inter_var_pointer;
#else

extern int inter_op_list[MAX_INTER_OP][2];
extern char inter_op_char[MAX_INTER_OP][32];

extern _ION inter_op_table[MAX_INTER_OP];
extern int inter_op_list_pointer;
extern int inter_op_table_pointer;
extern int inter_var_pointer;

#endif

int inter_new_label();

int inter_label_name(int label_id,char *name);

int inter_get_variable(int var_id);

void inter_var_name(int var_id,char *name);

int add_op(char* op);

int make_inter_op(int inter_op_type,int num, ...);

#endif
