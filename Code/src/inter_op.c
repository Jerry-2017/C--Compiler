#define __INTER_OP_C__

#include "../include/common.h"

void init_inter_op()
{
    inter_op_list_pointer=0;
    inter_op_table_pointer=0;
    inter_var_pointer=0;
}

int join_inter_op_b(int blk_id,int num,...)
{
    va_list valist;
    va_start(valist,num);
    int k[16];
    int i;
    int sid=-1,eid=-1;
    int eid=inter_op_table[k[num-1]].op_end;
    int s1id,e1id;
    for (i=0;i<num;i++)
    {
        k[i]=va_arg(valist, int);
        if (sid==-1 && inter_op_table[k[i]].op_start)
        {
            sid=s1id;
        }
        if (i!=0 && e1id!=-1)
            eid=inter_op_table[k[i-1]].op_end;
        if (i!=0 && inter_op_table[k[i]].op_start!=-1)
        {
            inter_op_list[e1id][0]=inter_op_table[k[i]].op_start;
        }
    }
    inter_op_table[blk_id].op_start=sid;
    inter_op_table[blk_id].op_end=eid;
    return blk_id;
}

int join_inter_op_bl(int blk_id,int num,int *l)
{
    int i;
    int sid=-1,eid=-1;
    int eid=inter_op_table[k[num-1]].op_end;
    int s1id,e1id;
    for (i=0;i<num;i++)
    {
        if (sid==-1 && inter_op_table[l[i]].op_start)
        {
            sid=s1id;
        }
        if (i!=0 && e1id!=-1)
            eid=inter_op_table[l[i-1]].op_end;
        if (i!=0 && inter_op_table[l[i]].op_start!=-1)
        {
            inter_op_list[e1id][0]=inter_op_table[l[i]].op_start;
        }
    }
    inter_op_table[blk_id].op_start=sid;
    inter_op_table[blk_id].op_end=eid;
    return blk_id;
}

int join_inter_op_l(int num,int *l)
{
    int i;
    int sid=-1,eid=-1;
    int eid=inter_op_table[k[num-1]].op_end;
    int s1id,e1id;
    for (i=0;i<num;i++)
    {
        if (sid==-1 && inter_op_table[l[i]].op_start)
        {
            sid=s1id;
        }
        if (i!=0 && e1id!=-1)
            eid=inter_op_table[l[i-1]].op_end;
        if (i!=0 && inter_op_table[l[i]].op_start!=-1)
        {
            inter_op_list[e1id][0]=inter_op_table[l[i]].op_start;
        }
    }
    return inter_new_op_block(sid,eid);
}

int join_inter_op(int num,...)
{
    va_list valist;
    va_start(valist,num);
    int k[16];
    int i;
    int sid=-1,eid=-1;
    int eid=inter_op_table[k[num-1]].op_end;
    int s1id,e1id;
    for (i=0;i<num;i++)
    {
        k[i]=va_arg(valist, int);
        if (sid==-1 && inter_op_table[k[i]].op_start)
        {
            sid=s1id;
        }
        if (i!=0 && e1id!=-1)
            eid=inter_op_table[k[i-1]].op_end;
        if (i!=0 && inter_op_table[k[i]].op_start!=-1)
        {
            inter_op_list[e1id][0]=inter_op_table[k[i]].op_start;
        }
    }
    return inter_new_op_block(sid,eid);
}

int inter_new_op_block(int start_id,int end_id)
{
    inter_op_table[inter_op_table_pointer].type=3;
    inter_op_table[inter_op_table_pointer].op_start=start_id;
    inter_op_table[inter_op_table_pointer].op_end=end_id;
    return inter_op_table_pointer++;
}

int inter_new_const_int(int ival)
{
    inter_op_table[inter_op_table_pointer].type=4;
    inter_op_table[inter_op_table_pointer].ival=ival;
    return inter_op_table_pointer++;    
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
    return inter_op_table_pointer++;
}

int inter_new_label()
{
    inter_op_table[inter_op_table_pointer].type=1;
    inter_op_table[inter_op_table_pointer].label_id=inter_var_pointer++;
    return inter_op_table_pointer++;
}

void inter_var_name(int var_id, char * name)
{
    if (inter_op_table[var_id].type==1)
    {
        int rvar_id=inter_op_table[var_id].var_id;
        sprintf(name,"_t%d",rvar_id);
    }
    else if (inter_op_table[var_id].type==4)
    {
        int rval=inter_op_table[var_id].ival;
        sprintf(name,"#%d",rvar);
    }
    return ;
}

void inter_label_name(int label_id, char * name)
{
    int rlabel_id=inter_op_table[label_id].label_id;
    sprintf(name,"_l%d",rlabel_id);
    return ;
}

void inter_func_name(int func_id, char *name)
{
    int rfunc_id=inter_op_table[func_id].func_table_id;
    name=func_table[rfunc_id].name;
    return ;
}

int add_op(char* op)
{
    strcpy((char*)&inter_op_char[inter_op_list_pointer],op);
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
    char *op_name,*relop;
    char op_str[MAX_VAR_LEN];
    int result_op=-1,op_num;

    char vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN],labels[MAX_VAR_LEN],vars3[MAX_VAR_LEN],funcs[MAX_VAR_LEN];
    int var_id1,var_id2,var_id3,label,func_id;
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
            
            var_id1=k[0];
            var_id2=k[1];
            label=k[3];

            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            inter_label_name(label,labels);
            sprintf(op_str,"IF %s %s %s GOTO %s",vars1,relop,vars2,labels);
            op_num=add_op(op_str);
            result_op=op_num;
            break;
        
        // LABEL
        case IOP_LABEL:
            label=k[0];
            inter_label_name(label,labels);
            sprintf(op_str,"LABEL %s :",labels);
            op_num=add_op(op_str);
            result_op=op_num;
            break;

        // LABEL
        case IOP_GOTO:
            label=k[0];
            inter_label_name(label,labels);
            sprintf(op_str,"GOTO %s :",labels);
            op_num=add_op(op_str);
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

            var_id1=k[0];
            inter_var_name(var_id1,vars1);
            sprintf(op_str,"%s %s",op_name,vars1);
            op_num=add_op(op_str);
            result_op=op_num;
            break;

        // FUNC
        case IOP_FUNC:
            func_id=k[0];
            inter_func_name(func_id,funcs);
            sprintf(op_str,"FUNCTION %s :",funcs);
            int op_num=add_op(op_str);
            result_op=op_num;
            break;

        //VAR1 VAR2 VAR3
        case IOP_ADD:
            op_name="+";
        case IOP_DIV:
            op_name="/";
        case IOP_MUL:
            op_name="*";
        case IOP_MINUS:
            op_name="-";

            var_id1=k[0];
            var_id2=k[1];
            var_id3=k[2];

            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            inter_var_name(var_id3,vars3);
            sprintf(op_str,"%s := %s %s %s",vars1,vars2,op_name,vars3);
            op_num=add_op(op_str);
            result_op=op_num;
            break;

        //VAR FUNC
        case IOP_CALL:
            var_id1=k[0];
            func_id=k[1];
            inter_var_name(var_id1,vars1);
            inter_func_name(func_id,funcs);
            sprintf(op_str,"%s := CALL %s",vars1,funcs);
            op_num=add_op(op_str);
            result_op=op_num;
            break;

        case IOP_ASSIGN:
            var_id1=k[0];
            var_id2=k[1];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"%s := %s",vars1,vars2);
            op_num=add_op(op_str);
            result_op=op_num;
            break;

        case IOP_AT:
            var_id1=k[0];
            var_id2=k[1];
            vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"%s := & %s",vars1,vars2);
            op_num=add_op(op_str);
            result_op=op_num;
            break;

        case IOP_LSTAR:
            var_id1=k[0];
            var_id2=k[1];
            vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"*%s := %s",vars1,vars2);
            op_num=add_op(op_str);
            result_op=op_num;
            break;

        case IOP_RSTAR:
            var_id1=k[0];
            var_id2=k[1];
            vars1[MAX_VAR_LEN];
            vars2[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"%s := *%s",vars1,vars2);
            op_num=add_op(op_str);
            result_op=op_num;
            break;
    }
    int result_block=inter_new_op_block(result_op,result_op);
    return result_block;
}   
