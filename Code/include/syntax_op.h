#ifndef __SYNTAX_OP_H__
#define __SYNTAX_OP_H__

#include "../include/common.h"

#define ROOT_FIRST_ACTION 0
#define ROOT_LAST_ACTION -1

#define MAX_CHILDREN_EXPANSION 10
#define MAX_SYNTAX_ACTION 0xffff


#define SYN_OP_TYPE(name) SYN_OP_##name

#define GET_OP_FUNC(name,flag) _syn_op_##name##_##flag

#define REG_OP_FUNC(name,flag) register_action(SYN_OP_TYPE(name),flag,GET_OP_FUNC(array_def,flag));

#define MAKE_OP_FUNC(name,flag) \
    void GET_OP_FUNC(name,flag) (_SI* node)

enum SyntaxActionType{
    SYN_OP_TYPE(array_def),
    SYN_OP_TYPE(var_def)
};

typedef struct SyntaxActionNode{
    void (*callee) (_SI*);
    bool isreg;
}_SAN;

void bind_sym_action(_SI* node,int action_id);
void register_action(int action_id,int action_type,void (*callee) (_SI*) );
void init_syntax_action();
void do_syntax_action(int action_id, int action_type,_SI* node);

#ifdef __SYNTAX_OP_C__

_SAN symbol_action_table[MAX_SYNTAX_ACTION][MAX_CHILDREN_EXPANSION];
int sat_cnt;
_SI** cnodelist;

#else

extern _SAN symbol_action_table[MAX_SYNTAX_ACTION][MAX_CHILDREN_EXPANSION];
extern int sat_cnt;
extern _SI** cnodelist;

#endif

MAKE_OP_FUNC(array_def,ROOT_FIRST_ACTION);
MAKE_OP_FUNC(var_def,ROOT_FIRST_ACTION);

#endif
