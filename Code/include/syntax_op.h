#ifndef __SYNTAX_OP_H__
#define __SYNTAX_OP_H__

#include "../include/common.h"

#define ROOT_FIRST_ACTION 0
#define ROOT_LAST_ACTION 16

#define MAX_CHILDREN_EXPANSION 10
#define MAX_SYNTAX_ACTION 0xffff


#define SYN_OP_TYPE(name) SYN_OP_##name

#define GET_OP_FUNC(name,flag) _syn_op_##name##_##flag

#define REG_OP_FUNC(name,flag) register_action(SYN_OP_TYPE(name),flag,GET_OP_FUNC(name,flag));

#define MAKE_OP_FUNC(name,flag) \
    void GET_OP_FUNC(name,flag) (_SI* node)

enum SyntaxActionType{
    SYN_OP_TYPE(array_def),
    SYN_OP_TYPE(var_def),
    SYN_OP_TYPE(basic_type_val),
    SYN_OP_TYPE(var_ref),
    SYN_OP_TYPE(pass_def),
    SYN_OP_TYPE(pass_declist),
    SYN_OP_TYPE(func_arg_def),
    SYN_OP_TYPE(exp_1_op),
    SYN_OP_TYPE(exp_2_op),
    SYN_OP_TYPE(exp_func_call),
    SYN_OP_TYPE(exp_arr),
    SYN_OP_TYPE(func_def),
    SYN_OP_TYPE(pass_compst),
    SYN_OP_TYPE(basic_type),
    SYN_OP_TYPE(pass_var_dec),
    SYN_OP_TYPE(stmt),
    SYN_OP_TYPE(struct_type),
    SYN_OP_TYPE(struct_def),
    SYN_OP_TYPE(pass_type),
    SYN_OP_TYPE(exp_struct)
};

typedef struct SyntaxActionNode{
    void (*callee) (_SI* node);
    bool isreg;
}_SAN;

void bind_sym_action(_SI* node,int action_id);
void register_action(int action_id,int action_type,void (*callee) (_SI*) );
void init_syntax_action();
void do_syntax_action(int action_id, int action_type,_SI* node);
void syntax_error(int error_id,int lineno,char *error_des);

#ifdef __SYNTAX_OP_C__

_SAN symbol_action_table[MAX_SYNTAX_ACTION][MAX_CHILDREN_EXPANSION];
int sat_cnt;
_SI** cnodelist;

#else

extern _SAN symbol_action_table[MAX_SYNTAX_ACTION][MAX_CHILDREN_EXPANSION];
extern int sat_cnt;
extern _SI** cnodelist;

#endif

MAKE_OP_FUNC(pass_declist,ROOT_FIRST_ACTION);
MAKE_OP_FUNC(pass_compst,ROOT_FIRST_ACTION);
MAKE_OP_FUNC(pass_var_dec,ROOT_FIRST_ACTION);
MAKE_OP_FUNC(stmt,ROOT_FIRST_ACTION);
MAKE_OP_FUNC(struct_def,ROOT_FIRST_ACTION);

MAKE_OP_FUNC(array_def,1);
MAKE_OP_FUNC(func_arg_def,1);
MAKE_OP_FUNC(pass_def,1);
MAKE_OP_FUNC(func_def,1);

MAKE_OP_FUNC(func_def,2);


MAKE_OP_FUNC(pass_type,ROOT_LAST_ACTION);
MAKE_OP_FUNC(struct_type,ROOT_LAST_ACTION);
MAKE_OP_FUNC(struct_def,ROOT_LAST_ACTION);
MAKE_OP_FUNC(stmt,ROOT_LAST_ACTION);
MAKE_OP_FUNC(pass_var_dec,ROOT_LAST_ACTION);
MAKE_OP_FUNC(basic_type,ROOT_LAST_ACTION);
MAKE_OP_FUNC(var_def,ROOT_LAST_ACTION);
MAKE_OP_FUNC(var_ref,ROOT_LAST_ACTION);
MAKE_OP_FUNC(basic_type_val,ROOT_LAST_ACTION);
MAKE_OP_FUNC(pass_compst,ROOT_LAST_ACTION);
MAKE_OP_FUNC(exp_arr,ROOT_LAST_ACTION);
MAKE_OP_FUNC(array_def,ROOT_LAST_ACTION);
MAKE_OP_FUNC(func_arg_def,ROOT_LAST_ACTION);
MAKE_OP_FUNC(exp_1_op,ROOT_LAST_ACTION);
MAKE_OP_FUNC(exp_2_op,ROOT_LAST_ACTION);
MAKE_OP_FUNC(exp_func_call,ROOT_LAST_ACTION);
MAKE_OP_FUNC(exp_struct,ROOT_LAST_ACTION);

#endif
