#include "./include/common.h"

#define __INTER_OP_C__

void init_inter_op()
{
    inter_op_list_pointer=0;
    inter_op_table_pointer=0;
    inter_var_pointer=0;
}

int join_inter_op(int num,...)
{
    va_list valist;
    va_start(valist,num);
    int k[16];
    int i;
    int sid=inter_op_table[k[0]].op_start;
    int eid=inter_op_table[k[num-1]].op_end;
    for (i=0;i<num;i++)
    {
        k[i]=va_arg(valist, int);
        if (i!=0)
        {
            int e1id=inter_op_table[k[i-1]].op_end;
            int s2id=inter_op_table[k[i]].op_start;
            inter_op_list[e1id][0]]=s2id;
        }
    }
    return inter_new_block(sid,eid);
}

int inter_new_op_block(int start_id,int end_id)
{
    inter_op_table[inter_op_table_pointer].type=3;
    inter_op_table[inter_op_table_pointer].op_start=start_id;
    inter_op_table[inter_op_table_pointer].op_end=end_id;
    return inter_op_table_pointer;
}

int inter_get_variable(int var_id) //for other modules
{
    int t=var_table[var_id].inter_var_id;
    if (t==-1)
    {
        t=inter_new_var();
        var_table[var_id].inter_var_id=t;
        inter_op_table[t].var_table_id=var_id;
    }
    return t;
}

int inter_get_func(int func_id)
{
    int t=func_table[func_id].inter_func_id;
    if (t==-1)
    {
        inter_op_table[inter_op_table_pointer].type=2;
        inter_op_table[inter_op_table_pointer].func_table_id=func_id;
        t=inter_op_table_pointer++;
        func_table[func_id].inter_func_id=t;
    }
    return t;
}

int inter_new_var()
{
    inter_op_table[inter_op_table_pointer].type=0;
    inter_op_table[inter_op_table_pointer].var_id=inter_var_pointer++;
    return inter_op_table++;
}

int inter_new_label()
{
    inter_op_table[inter_op_table_pointer].type=1;
    inter_op_table[inter_op_table_pointer].label_id=inter_var_pointer++;
    return inter_op_table_pointer++;
}

void inter_var_name(int var_id, char * name)
{
    int rvar_id=inter_table[var_id].var_id;
    sprintf(name,"_t%d",rvar_id);
    return ;
}

void inter_func_name(int func_id, char *name)
{
    int rfunc_id=inter_table[func_id].func_id;
    name=func_table[rfunc_id].name;
    return ;
}

int add_op(char* op)
{
    strcpy(inter_op_char+inter_op_list_pointer,op);
    inter_op_list[inter_op_list_pointer][0]=-1;
    inter_op_list[inter_op_list_pointer][1]=-1;
    inter_op_table[inter_op_table_pointer].op_start=inter_op_list_pointer;
    inter_op_table[inter_op_table_pointer].op_end=inter_op_list_pointer;
    inter_op_list_pointer++;
    return inter_op_table_pointer++;
}

int make_inter_op(int inter_op_type,int num, ...)
{
    
    va_list valist;
    va_start(valist,num);
    int k[10];
    int i;
    for (i=0;i<num;i++)
        k[i]=va_arg(valist, int);
    char* op_name,relop;
    char op_str[MAX_VAR_LEN];
    int result_op=-1;
    switch (inter_op_type)
    {
        // EXP1 EXP2 TRUE
        case IOP_IF_LEQ:
            relop="<=";
        case IOP_IF_L:
            relop="<";
        case IOP_IF_G:
            relop=">";
        case IOP_IF_GEQ:
            relop=">=";
        case IOP_IF_EQ:
            relop="==";    
        case IOP_IF_NE:
            relop="!=";      
            
            int var_id1=k[0],var_id2=k[1],do_list=k[2],label_true=k[3];
            char vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN],labels[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            inter_label_name(label_true,labels);
            sprintf(op_str,"IF %s %s %s GOTO %s",vars1,relop,vars2,labels)
            int opnum=add_op(op_str);
            result_op=opnum;
            break;
        
        // LABEL
        case IOP_LABEL:
            int label=k[0];
            char labels[MAX_VAR_LEN];
            inter_label_name(label,labels);
            sprintf(op_str,"LABEL %s :",labels);
            int op_num=add_op(op_str);
            result_op=op_num;
            break;

        // LABEL
        case IOP_GOTO:
            int label=k[0];
            char labels[MAX_VAR_LEN];
            inter_label_name(label,labels);
            sprintf(op_str,"GOTO %s :",labels);
            int op_num=add_op(op_str);
            result_op=op_num;            
            break;

        // VAR
        case IOP_ARG:
            op_name="ARG";
        case IOP_PARAM:
            op_name="PARAM";
        case IOP_RETURN:
            op_name="RETURN";
        case IOP_READ:
            op_name="READ";
        case IOP_WRITE:
            op_name="WRITE";

            int var_id=k[0];
            char vars[MAX_VAR_LEN];
            inter_var_name(var_id,vars);
            sprintf(op_str,"%s %s",op_name,vars);
            int op_num=add_op(op_str);
            result_op=op_num;
            break;

        // FUNC
        case IOP_FUNC:
            int func_id=k[0];
            char funcs[MAX_VAR_LEN];
            inter_func_name(func_id,funcs);
            sprintf(op_str,"FUNCTION %s :",op_name,funcs);
            int op_num=add_op(op_str);
            result_op=op_num;
            break;

        //VAR1 VAR2 VAR3
        case IOP_ADD:
            op_str="+";
        case IOP_DIV:
            op_str="/";
        case IOP_MUL:
            op_str="*";
        case IOP_MINUS:
            op_str="-";

            int var_id1=k[0],var_id2=k[1],var_id3=k[2];
            char vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN],vars3[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            inter_var_name(var_id3,vars3);
            sprintf(op_str,"%s := %s %s %s",vars1,vars2,op_str,vars3);
            int op_num=add_op(op_str);
            result_op=op_num;
            break;

        //VAR FUNC
        case IOP_CALL:
            int var_id=k[0],func_id=k[1];
            char vars[MAX_VAR_LEN],funcs[MAX_VAR_LEN];
            inter_var_name(var_id,vars);
            inter_func_name(func_id,funcs);
            sprintf(op_str,"%s := CALL %s",vars,funcs);
            int op_num=add_op(op_str);
            result_op=op_num;
            break;

        case IOP_ASSIGN:
            int var_id1=k[0],var_id2=k[1];
            char vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"%s := %s",vars1,vars2);
            int op_num=add_op(op_str);
            result_op=op_num;
            break;

        case IOP_AT:
            int var_id1=k[0],var_id2=k[1];
            char vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"%s := & %s",vars1,vars2);
            int op_num=add_op(op_str);
            result_op=op_num;
            break;

        case IOP_LSTAR:
            int var_id1=k[0],var_id2=k[1];
            char vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"*%s := %s",vars1,vars2);
            int op_num=add_op(op_str);
            result_op=op_num;
            break;

        case IOP_RSTAR:
            int var_id1=k[0],var_id2=k[1];
            char vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"%s := *%s",vars1,vars2);
            int op_num=add_op(op_str);
            result_op=op_num;
            break;
    }
    int result_block=inter_new_block(result_op,result_op);
    return result_block;
}   
