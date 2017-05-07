#ifndef __TYPE_H__
#define __TYPE_H__

#define MAX_TYPENUM 0xffff
#define MAX_STRUCT_VAR_DEF 0xffff
#define MAX_FUNC_ARG_DEC 0xffff

#define BASIC_TYPE 0
#define ARRAY_TYPE 1
#define STRUCT_TYPE 2

#include "../include/common.h"

typedef struct TypeTableItem{
    int type; // 0 basic type 1 array 2 struct
    char *name;
    int size;
    union {
        struct {
            int elemsize;
            int elemtype;
        }_array;
        struct {
            int stkpos;
            int stksize;
        }_struct;
    };
}_TI;

typedef struct FuncTableItem
{
    char *name;
    int ret_type;
    int arg_pos;
    int arg_size;
}_FI;

#ifdef __TYPE_C__

int ttcnt;

_SI func_arg_table[MAX_FUNC_ARG_DEC];
int farg_tabel_cnt;

_TI type_table[MAX_TYPENUM];
int type_table_cnt;

_VI struct_arg_table[MAX_STRUCT_VAR_DEF];
int sarg_table_cnt;

#else 

extern int _ttcnt;

extern _SI func_arg_table[MAX_FUNC_ARG_DEC];
extern int farg_tabel_cnt;

extern _TI type_table[MAX_TYPENUM];
extern int type_table_cnt;

extern _VI struct_arg_table[MAX_STRUCT_VAR_DEF];
extern int sarg_table_cnt;

#endif

void pass_type(_SI *src,_SI *dst);
void init_type();
int add_type();
int add_type_array(int typeid,int arrsize);

#endif
