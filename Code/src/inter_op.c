#define __INTER_OP_C__

#include "../include/common.h"

void init_inter_op()
{
    inter_op_list_pointer=0;
    inter_op_table_pointer=0;
    inter_var_pointer=0;
    inter_label_pointer=0;

    inter_read_func_id=add_func("read",TYPE_INT);
    func_table[inter_read_func_id].is_def=true;
    new_env_func_param_dec(inter_read_func_id);
    exit_env();

    inter_write_func_id=add_func("write",TYPE_INT);
    func_table[inter_write_func_id].is_def=true;
    new_env_func_param_dec(inter_write_func_id);
    add_variable("content",TYPE_INT);
    exit_env();

}

int join_inter_op_bl(int blk_id,int num,int *k)
{
    int i;
    int sid=-1,eid=-1;
    int s1id=-1,e1id=-1;
    int num1=0;
    if (blk_id==-1 )
        printf("receive -1 block_id\n");
    //if (inter_op_table[blk_id].op_start!=-1)
    //    printf("multi init block_id %d\n",blk_id);
    for (i=0;i<num;i++)
        if (k[i]!=-1 && inter_op_table[k[i]].op_start!=-1 && inter_op_table[k[i]].op_end!=-1) k[num1++]=k[i];
    for (i=0;i<num1;i++)
    {
        s1id=inter_op_table[k[i]].op_start;
        if (sid==-1)
           sid=s1id;
        if (i!=0)
        {
            eid=inter_op_table[k[i-1]].op_end;
            e1id=eid;
        }
        if (i!=0 && s1id!=-1 && e1id!=-1)
        {
            inter_op_list[e1id][0]=s1id;
        }
    }
    if (num1>0)
        eid=inter_op_table[k[num1-1]].op_end;
    if (sid==-1 || eid==-1)
        return -1;
    inter_op_table[blk_id].op_start=sid;
    inter_op_table[blk_id].op_end=eid;
    return blk_id;
}


int join_inter_op_b(int blk_id,int num,...)
{
    va_list valist;
    va_start(valist,num);
    int k[16],i;
    for (i=0;i<num;i++)
    {
        k[i]=va_arg(valist, int);
    }
    return join_inter_op_bl(blk_id,num,k);
}


int join_inter_op_l(int num,int *k)
{
    return join_inter_op_bl(inter_op_table_pointer++,num,k);
}

int join_inter_op(int num,...)
{
    va_list valist;
    va_start(valist,num);
    int k[16],i;
    for (i=0;i<num;i++)
    {
        k[i]=va_arg(valist, int);
    }
    return join_inter_op_bl(inter_op_table_pointer++,num,k);
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
    inter_op_table[inter_op_table_pointer].op_start=-1;
    inter_op_table[inter_op_table_pointer].op_end=-1;
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
    inter_op_table[inter_op_table_pointer].op_start=-1;
    inter_op_table[inter_op_table_pointer].op_end=-1;
    return inter_op_table_pointer++;
}

int inter_new_label()
{
    inter_op_table[inter_op_table_pointer].type=1;
    inter_op_table[inter_op_table_pointer].label_id=inter_label_pointer++;
    inter_op_table[inter_op_table_pointer].op_start=-1;
    inter_op_table[inter_op_table_pointer].op_end=-1;
    return inter_op_table_pointer++;
}

void inter_var_name(int var_id, char * name)
{
    sprintf(name,"None");
    if (inter_op_table[var_id].type==0)
    {
        int rvar_id=inter_op_table[var_id].var_id;
        sprintf(name,"t%d",rvar_id);
    }
    else if (inter_op_table[var_id].type==4)
    {
        int rval=inter_op_table[var_id].ival;
        sprintf(name,"#%d",rval);
    }
    return ;
}

void inter_label_name(int label_id, char * name)
{
    sprintf(name,"None");
    if (inter_op_table[label_id].type==1)
    {
        int rlabel_id=inter_op_table[label_id].label_id;
        sprintf(name,"l%d",rlabel_id);
    }
    return ;
}

void inter_func_name(int func_id, char *name)
{
    int rfunc_id=inter_op_table[func_id].func_table_id;
    strcpy(name,func_table[rfunc_id].name);
    return ;
}

int inter_add_op(char* op)
{
    strcpy(inter_op_char[inter_op_list_pointer],op);
    inter_op_list[inter_op_list_pointer][0]=-1;
    inter_op_list[inter_op_list_pointer][1]=-1;
    inter_op_table[inter_op_table_pointer].op_start=inter_op_list_pointer;
    inter_op_table[inter_op_table_pointer].op_end=inter_op_list_pointer;
    inter_op_table[inter_op_table_pointer].type=3;
    //printf("%s %d %d\n",inter_op_char[inter_op_list_pointer],inter_op_list_pointer,inter_op_table_pointer);
    inter_op_list_pointer++;
    return inter_op_table_pointer++;
}

int inter_make_op(int inter_op_type,int num, ...)
{
    //printf("inter make %d\n",inter_op_type);
    va_list valist;
    va_start(valist,num);
    int k[10];
    int i;
    for (i=0;i<num;i++)
        k[i]=va_arg(valist, int);
    char *op_name,*relop;
    char op_str[MAX_VAR_LEN];
    int result_op=-1,op_num;
    //printf("num %d\n",num);
    char vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN],labels[MAX_VAR_LEN],vars3[MAX_VAR_LEN],funcs[MAX_VAR_LEN];
    int var_id1,var_id2,var_id3,label,func_id;
    int pass_flag=false;
    switch (inter_op_type)
    {
        // EXP1 EXP2 TRUE
        case IOP_IF_LEQ:
            if (!pass_flag)
                relop="<=";
            pass_flag=true;
        case IOP_IF_L:
            if (!pass_flag)
                relop="<";
            pass_flag=true;
        case IOP_IF_G:
            if (!pass_flag)
                relop=">";
            pass_flag=true;
        case IOP_IF_GEQ:
            if (!pass_flag)
                relop=">=";
            pass_flag=true;
        case IOP_IF_EQ:
            if (!pass_flag)
                relop="==";    
            pass_flag=true;
        case IOP_IF_NE:
            if (!pass_flag)
                relop="!=";      
            pass_flag=true;
            
            var_id1=k[0];
            var_id2=k[1];
            label=k[2];

            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            inter_label_name(label,labels);
            sprintf(op_str,"IF %s %s %s GOTO %s",vars1,relop,vars2,labels);
            op_num=inter_add_op(op_str);
            result_op=op_num;
            break;
        
        // LABEL
        case IOP_LABEL:
            label=k[0];
            inter_label_name(label,labels);
            sprintf(op_str,"LABEL %s :",labels);
            op_num=inter_add_op(op_str);
            result_op=op_num;
            break;

        // LABEL
        case IOP_GOTO:
            label=k[0];
            inter_label_name(label,labels);
            sprintf(op_str,"GOTO %s",labels);
            op_num=inter_add_op(op_str);
            result_op=op_num;            
            break;

        // VAR
        case IOP_ARG:
            if (!pass_flag)
                op_name="ARG";
            pass_flag=true;
        case IOP_PARAM:
            if (!pass_flag)
                op_name="PARAM";
            pass_flag=true;
        case IOP_RETURN:
            if (!pass_flag)
                op_name="RETURN";
            pass_flag=true;
        case IOP_READ:
            if (!pass_flag)
                op_name="READ";
            pass_flag=true;
        case IOP_WRITE:
            if (!pass_flag)
                op_name="WRITE";
            pass_flag=true;

            var_id1=k[0];
            inter_var_name(var_id1,vars1);
            sprintf(op_str,"%s %s",op_name,vars1);
            op_num=inter_add_op(op_str);
            result_op=op_num;
            break;

        // FUNC
        case IOP_FUNC:
            func_id=k[0];
            inter_func_name(func_id,funcs);
            sprintf(op_str,"FUNCTION %s :",funcs);
            int op_num=inter_add_op(op_str);
            result_op=op_num;
            break;

        //VAR1 VAR2 VAR3
        case IOP_ADD:
            if (!pass_flag)
                op_name="+";
            pass_flag=true;
        case IOP_DIV:
            if (!pass_flag)
                op_name="/";
            pass_flag=true;
        case IOP_MUL:
            if (!pass_flag)
                op_name="*";
            pass_flag=true;
        case IOP_MINUS:
            if (!pass_flag)
                op_name="-";
            pass_flag=true;

            var_id1=k[0];
            var_id2=k[1];
            var_id3=k[2];

            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            inter_var_name(var_id3,vars3);
            sprintf(op_str,"%s := %s %s %s",vars1,vars2,op_name,vars3);
            op_num=inter_add_op(op_str);
            result_op=op_num;
            break;

        //VAR FUNC
        case IOP_CALL:
            var_id1=k[0];
            func_id=k[1];
            inter_var_name(var_id1,vars1);
            inter_func_name(func_id,funcs);
            sprintf(op_str,"%s := CALL %s",vars1,funcs);
            op_num=inter_add_op(op_str);
            result_op=op_num;
            break;

        case IOP_ASSIGN:
            var_id1=k[0];
            var_id2=k[1];
            //printf("%d %d\n",inter_op_table[var_id1].type,inter_op_table[var_id2].type);
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"%s := %s",vars1,vars2);
            op_num=inter_add_op(op_str);
            result_op=op_num;
            break;

        case IOP_AT:
            var_id1=k[0];
            var_id2=k[1];
            vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"%s := & %s",vars1,vars2);
            op_num=inter_add_op(op_str);
            result_op=op_num;
            break;

        case IOP_LSTAR:
            var_id1=k[0];
            var_id2=k[1];
            vars1[MAX_VAR_LEN],vars2[MAX_VAR_LEN];
            inter_var_name(var_id1,vars1);
            inter_var_name(var_id2,vars2);
            sprintf(op_str,"*%s := %s",vars1,vars2);
            op_num=inter_add_op(op_str);
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
            op_num=inter_add_op(op_str);
            result_op=op_num;
            break;
    }
    return result_op;
}   

void inter_output(int inter_root)
{
    if (inter_root==-1) return;
    int s=inter_op_table[inter_root].op_start,e=inter_op_table[inter_root].op_end;
    int j;
    /**printf("********");
    for (j=0;j<20;j++)
        printf("%d : %s %d\n",j,inter_op_char[j],inter_op_list[j][0]);
    printf("********");**/
    int i=s;
    while (i!=-1)
    {
        printf("%s\n",inter_op_char[i]);
        if (i==e) break;
        i=inter_op_list[i][0];
    }
    return;    
}

