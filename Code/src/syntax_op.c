#define __SYNTAX_OP_C__

#include "../include/common.h"

void init_syntax_action()
{
    sat_cnt=0;
    int i,j;
    for (i=0;i<MAX_SYNTAX_ACTION;i++)
        for (j=0;j<MAX_CHILDREN_EXPANSION;j++)
            symbol_action_table[i][j].isreg=false;

    REG_OP_FUNC(pass_declist,ROOT_FIRST_ACTION);
    REG_OP_FUNC(pass_compst,ROOT_FIRST_ACTION);
    REG_OP_FUNC(pass_var_dec,ROOT_FIRST_ACTION);
    REG_OP_FUNC(stmt,ROOT_FIRST_ACTION);
    REG_OP_FUNC(struct_def,ROOT_FIRST_ACTION);

    REG_OP_FUNC(pass_def,1);
    REG_OP_FUNC(func_arg_def,1);
    REG_OP_FUNC(array_def,1);
    REG_OP_FUNC(func_def,1);
    REG_OP_FUNC(func_dec,1);

    REG_OP_FUNC(func_def,2);

    REG_OP_FUNC(exp_struct,ROOT_LAST_ACTION);
    REG_OP_FUNC(stmt,ROOT_LAST_ACTION);
    REG_OP_FUNC(pass_var_dec,ROOT_LAST_ACTION);
    REG_OP_FUNC(var_def,ROOT_LAST_ACTION);
    REG_OP_FUNC(var_ref,ROOT_LAST_ACTION);
    REG_OP_FUNC(basic_type_val,ROOT_LAST_ACTION);
    REG_OP_FUNC(pass_compst,ROOT_LAST_ACTION);
    REG_OP_FUNC(exp_arr,ROOT_LAST_ACTION);
    REG_OP_FUNC(func_arg_def,ROOT_LAST_ACTION);
    REG_OP_FUNC(array_def,ROOT_LAST_ACTION);
    REG_OP_FUNC(exp_func_call,ROOT_LAST_ACTION);
    REG_OP_FUNC(exp_2_op,ROOT_LAST_ACTION);
    REG_OP_FUNC(exp_1_op,ROOT_LAST_ACTION);
    REG_OP_FUNC(basic_type,ROOT_LAST_ACTION);
    REG_OP_FUNC(struct_type,ROOT_LAST_ACTION);
    REG_OP_FUNC(struct_def,ROOT_LAST_ACTION);
    REG_OP_FUNC(pass_type,ROOT_LAST_ACTION);
    REG_OP_FUNC(func_def,ROOT_LAST_ACTION);
    
    REG_OP_FUNC(inter_op_join,ROOT_LAST_ACTION);
}

void bind_sym_action(_SI* node,int action_id)
{
    int i;
    for (i=0;i<MAX_CONCURRENT_SYNTAX_OP;i++)
        if (node->action_id[i]==-1)
        {
            node->action_id[i]=action_id;
            break;
        }
}

void register_action(int action_id,int action_type,void (*callee) (_SI*) )
{
    if (action_type==0x10) action_type=1;
    else if (action_type>0) action_type++;
    if (symbol_action_table[action_id][action_type].isreg==true)
        printf("Duplicate action register id %d\n",action_id);
    else {
        symbol_action_table[action_id][action_type].isreg=true;
        symbol_action_table[action_id][action_type].callee=callee;
    }
}

void do_syntax_action(int action_id, int action_type,_SI* node)
{
    if (action_type==0x10) action_type=1;
    else if (action_type>0) action_type++;
    if (action_id>=0 && symbol_action_table[action_id][action_type].isreg==true)
    {
        symbol_action_table[action_id][action_type].callee(node);
    }
}

void syntax_error(int error_id,int lineno,char *error_des)
{
    errorrec=true;
    printf("Error type %d at Line %d: %s\n",error_id,lineno,error_des);
}

MAKE_OP_FUNC(stmt,ROOT_FIRST_ACTION)
{
    if (node->sym_affix_type==1)  //Compst
    {
        cnodelist[0]->func_id=node->func_id;
    } 
    else if (node->sym_affix_type==3) //IF LP Exp RP Stmt ELSE Stmt 
    {
        cnodelist[4]->func_id=node->func_id;
        cnodelist[6]->func_id=node->func_id;
    }
    else if (node->sym_affix_type==4) //While ( Exp ) Stmt
    {
        cnodelist[4]->func_id=node->func_id;
    }
    else if (node->sym_affix_type==3) //IF LP Exp RP Stmt 
    {
        cnodelist[4]->func_id=node->func_id;
    }
}

MAKE_OP_FUNC(pass_compst,ROOT_FIRST_ACTION)
{
    new_env_block();
    //printf("new compst %d\n",node->compst_func_id);
    if (node->compst_func_id>=0)
    {
        int i;
        int fid=node->compst_func_id;
        int base=func_table[fid].arg_pos;
        int cnt=func_table[fid].arg_size;
        //printf("base %d size %d\n",base,cnt);
        for (i=0;i<cnt;i++)
        {
            //printf("new var %s\n",var_table[base+i].name);
            int p=add_variable(var_table[base+i].name,var_table[base+i].var_type);
            var_table[p].inter_var_id=var_table[base+i].inter_var_id;
        }
    }
    cnodelist[0]->func_id=node->func_id;
}

MAKE_OP_FUNC(pass_def,1)
{
    cnodelist[1]->val_type_id=cnodelist[0]->val_type_id;
}

MAKE_OP_FUNC(array_def,1)
{
    int index_type=cnodelist[2]->val_type_id;
    if (index_type!=TYPE_INT)
         syntax_error(12,cnodelist[0]->lineno,"array index is not an integer");
    int size=cnodelist[2]->value.ival;
    cnodelist[0]->val_type_id=add_type_array(node->val_type_id,size);
}

MAKE_OP_FUNC(pass_var_dec,ROOT_FIRST_ACTION)
{
    cnodelist[0]->val_type_id=node->val_type_id;
}

MAKE_OP_FUNC(pass_declist,ROOT_FIRST_ACTION)
{
    if (node->sym_affix_type==0)
    {
        cnodelist[0]->val_type_id=node->val_type_id;
    }
    else
    {
        cnodelist[0]->val_type_id=node->val_type_id;
        cnodelist[2]->val_type_id=node->val_type_id;
    }
}

MAKE_OP_FUNC(struct_def,ROOT_FIRST_ACTION)
{
    int tid=add_struct();
    char *stname="";
    if (cnodelist[1]->sym_affix_type==0)
    {
        _SI* ID=get_nth_child(cnodelist[1],1);
        stname=ID->value.pstr;
        //printf("sname %s\n",stname);
        if (find_type(stname,2)!=-1)
        {
            char tp[0x100];
            sprintf(tp,"Duplicated name \"%s\"",stname);
            syntax_error(16,cnodelist[0]->lineno,tp);            
            stname="";
        }
    }
    type_table[tid].name=stname;
    node->val_type_id=tid;
    new_env_struct_def(tid);
}

MAKE_OP_FUNC(func_arg_def,1)
{
    char *funcname=cnodelist[0]->value.pstr;
    int fid=find_func(funcname);
    if (fid!=-1)
    {
        if (func_table[fid].is_def)
        {
            errorrec=true;
            char tp[0x100];
            sprintf(tp,"Redefined function  \"%s\"",funcname);
            syntax_error(4,cnodelist[0]->lineno,tp);
            new_env_block();
        }
        else
        {
            func_dec_cons=true;
            if (func_table[fid].ret_type!=node->val_type_id)
                func_dec_cons=false;
            node->func_id=fid;
            new_env_func_dec_check(fid);
        }
    }
    else
    {
        func_dec_cons=true;
        int func_id=add_func(funcname,node->val_type_id);
        if (node->context_relate_id==0)
            func_table[func_id].is_def=true;
        else
        {
            func_table[func_id].is_def=false;
            func_table[func_id].dec_line=cnodelist[0]->lineno;
        }
        node->func_id=func_id;
        new_env_func_param_dec(func_id);
    }
}

MAKE_OP_FUNC(func_def,1)
{
    cnodelist[1]->val_type_id=cnodelist[0]->val_type_id;
    inter_func_param_mode=true;
    cnodelist[1]->context_relate_id=0;
}

MAKE_OP_FUNC(func_dec,1)
{
    cnodelist[1]->val_type_id=cnodelist[0]->val_type_id;
    cnodelist[1]->sym_affix_type=node->sym_affix_type;
    cnodelist[1]->context_relate_id=1;
}

MAKE_OP_FUNC(func_def,2)
{
    cnodelist[2]->func_id=cnodelist[1]->func_id;
    cnodelist[2]->compst_func_id=cnodelist[1]->func_id;
    inter_func_param_mode=false;
    //printf("**%d**",cnodelist[2]->func_id);
}


MAKE_OP_FUNC(array_def,ROOT_LAST_ACTION)
{
    node->val_type_id=cnodelist[0]->val_type_id;
    node->var_id=cnodelist[0]->var_id;
    node->inter_op_blk_id=cnodelist[0]->inter_op_blk_id;
}


MAKE_OP_FUNC(pass_compst,ROOT_LAST_ACTION)
{
    exit_env();
}

MAKE_OP_FUNC(func_arg_def,ROOT_LAST_ACTION)
{
    if (!func_dec_cons)
    {
        char tp[0x100];
        char* func_name=cnodelist[0]->value.pstr;
        sprintf(tp,"Inconsistent declaration of function \"%s\"",func_name);
        syntax_error(19,cnodelist[0]->lineno,tp);       
    }
    exit_env();

    if (node->sym_affix_type==0)
        node->inter_op_blk_id=cnodelist[2]->inter_op_blk_id;
}

MAKE_OP_FUNC(pass_var_dec,ROOT_LAST_ACTION)
{
    int tpv1,tpv2,tpv3,tpv4;
    if (node->sym_affix_type==1)
    {
        if (cnodelist[0]->val_type_id!=cnodelist[2]->val_type_id)
            syntax_error(5,cnodelist[0]->lineno,"Type mismatched for assignment.");       
    }

    if (node->sym_affix_type==1)
    {
        tpv1=cnodelist[0]->inter_op_blk_id;

        tpv2=cnodelist[2]->inter_op_blk_id;
        tpv3=inter_make_op(IOP_ASSIGN,2,tpv1,tpv2);
        tpv4=join_inter_op(2,tpv2,tpv3);
        node->inter_op_blk_id=tpv4;
    }
    
}

MAKE_OP_FUNC(func_def,ROOT_LAST_ACTION)
{
    int tpv1,tpv2,tpv3,tpv4,tpv5;
    tpv1=inter_get_func(cnodelist[1]->func_id);
    tpv2=inter_make_op(IOP_FUNC,1,tpv1);
    tpv3=cnodelist[1]->inter_op_blk_id;
    tpv4=cnodelist[2]->inter_op_blk_id;
    tpv5=join_inter_op(3,tpv2,tpv3,tpv4);
    node->inter_op_blk_id=tpv5;
}

MAKE_OP_FUNC(var_def,ROOT_LAST_ACTION)
{
    char *vname=cnodelist[0]->value.pstr;
    int tpv1,tpv2,tpv3,tpv4,tpv5;
    //printf("variable %s size %d var_id %d\n",vname,type_table[node->val_type_id].size,get_variable(vname));
    if (get_variable(vname)!=-1)
    {
        if (stack_env[se_cnt][0]==ENV_STRUCT_DEF)
        {
            char tp[0x100];
            sprintf(tp,"Redefined field \"%s\"",vname);
            syntax_error(16,cnodelist[0]->lineno,tp);
        }
        else
        {
            char tp[0x100];
            sprintf(tp,"Redefined Variable \"%s\"",vname);
            syntax_error(3,cnodelist[0]->lineno,tp);
        }
    }
    else
    {
        node->var_id=add_variable(vname,node->val_type_id);
        
        if (inter_func_param_mode==true)
        {
            tpv1=inter_get_variable(node->var_id);
            node->inter_op_blk_id=inter_make_op(IOP_PARAM,1,tpv1);
        }
        else
        {
            tpv1=node->var_id;
            tpv2=var_table[tpv1].var_type;
            tpv3=inter_get_variable(tpv1);
            if (tpv2!=TYPE_INT)
            {
                //printf("**********");
                tpv4=inter_new_const_int(type_table[tpv2].size);
                tpv5=inter_make_op(IOP_DEC,2,tpv3,tpv4);
                //inter_blk_add_var_ref(tpv5,tpv3);
                node->inter_op_blk_id=tpv5;
            }
            else
                node->inter_op_blk_id=tpv3;
        }
    }
}

MAKE_OP_FUNC(var_ref,ROOT_LAST_ACTION)
{
    char *vname=cnodelist[0]->value.pstr;
    int vid=get_variable_allenv(vname);
    if (vid==-1)
    {
        char tp[0x100];
        sprintf(tp,"Undefined Variable \"%s\"",vname);
        syntax_error(1,cnodelist[0]->lineno,tp);
    }
    else
    {
        node->var_id=vid;
        node->val_type_id=var_table[vid].var_type;
        node->is_left_val=true;

        node->inter_op_blk_id=inter_get_variable(vid);
    }
}

MAKE_OP_FUNC(exp_1_op,ROOT_LAST_ACTION)
{
    int tpv1,tpv2,tpv3,tpv4,tpv5,tpv6,tpv7,tpv8,tpv9;
    if (node->sym_affix_type==0 ) // (Exp)
    {
        node->val_type_id=cnodelist[1]->val_type_id;
        node->is_left_val=false;

        node->inter_op_blk_id=cnodelist[1]->inter_op_blk_id;
    }
    else if (node->sym_affix_type==1) //-Exp
    {
        int vt_id=cnodelist[1]->val_type_id;
        if (vt_id!=TYPE_INT && vt_id!=TYPE_FLOAT)
        {
            syntax_error(7,cnodelist[1]->lineno,"Type mismatched for operands");
            node->val_type_id=TYPE_INT;
            node->is_left_val=false;
        }
        else
        {
            node->val_type_id=vt_id;
            node->is_left_val=false;

            tpv1=inter_new_var();
            tpv2=inter_new_const_int(0);
            
            tpv3=cnodelist[1]->inter_op_blk_id;
            tpv4=inter_make_op(IOP_MINUS,3,tpv1,tpv2,tpv3);
            join_inter_op_b(tpv1,2,tpv3,tpv4);
            node->inter_op_blk_id=tpv1;
        }
    }
    else if (node->sym_affix_type==2) //not Exp
    {
        int vt_id=cnodelist[1]->val_type_id;
        if (vt_id!=TYPE_INT)
            syntax_error(7,cnodelist[1]->lineno,"Type mismatched for operands");
        node->val_type_id=TYPE_INT;
        node->is_left_val=false;

        tpv1=inter_new_var();
        tpv2=inter_new_const_int(1);
        tpv3=inter_new_const_int(0);

        tpv4=inter_new_label();

        tpv5=cnodelist[1]->inter_op_blk_id;
        tpv6=inter_make_op(IOP_ASSIGN,2,tpv1,tpv3);
        tpv7=inter_make_op(IOP_IF_EQ,3,tpv5,tpv2,tpv4);
        tpv8=inter_make_op(IOP_ASSIGN,2,tpv1,tpv2);
        tpv9=inter_make_op(IOP_LABEL,1,tpv4);

        join_inter_op_b(tpv1,5,tpv5,tpv6,tpv7,tpv8,tpv9);
        node->inter_op_blk_id=tpv1;
    }
}

MAKE_OP_FUNC(basic_type_val,ROOT_LAST_ACTION)
{
    int tpv1;
    memcpy(&node->value,&cnodelist[0]->value,sizeof(node->value));
    if (cnodelist[0]->sym_type==S_INT || cnodelist[0]->sym_type==S_INT_10 || cnodelist[0]->sym_type==S_INT_8 || cnodelist[0]->sym_type==S_INT_16)
        node->val_type_id=TYPE_INT;
    else if (cnodelist[0]->sym_type==S_FLOAT)
        node->val_type_id=TYPE_FLOAT;
    node->is_left_val=false;

    //printf("ival %d\n",node->value.ival);
    if (node->sym_type==S_INT)
    {
        tpv1=inter_new_const_int(node->value.ival);
        node->inter_op_blk_id=tpv1;
    }
    else
    {
       node->inter_op_blk_id=cnodelist[0]->inter_op_blk_id;
    }
}

MAKE_OP_FUNC(exp_2_op,ROOT_LAST_ACTION)
{
    int tpv1,tpv2,tpv3,tpv4,tpv5,tpv6,tpv7,tpv8,tpv9,tpv10,tpv11,tpv12,tpv13,tpv14;
    _SI * pnode;

    if (node->sym_affix_type==0 ) // Exp = Exp
    {
        if (!cnodelist[0]->is_left_val)
            syntax_error(6,cnodelist[0]->lineno,"The left-hand side of an assignment must be a variable.");
        else if (cnodelist[0]->val_type_id!=cnodelist[2]->val_type_id)
            syntax_error(5,cnodelist[0]->lineno,"Type mismatched for assignment.");
        node->val_type_id=cnodelist[0]->val_type_id;
        node->is_left_val=false;

        if (cnodelist[0]->inter_op_blk_id_lv==-1)
        {
            tpv1=cnodelist[0]->inter_op_blk_id;
            tpv2=cnodelist[2]->inter_op_blk_id;
            tpv3=inter_make_op(IOP_ASSIGN,2,tpv1,tpv2);
            tpv4=join_inter_op(3,tpv1,tpv2,tpv3);
            node->inter_op_blk_id=tpv4;
        }
        else
        {
            tpv1=cnodelist[0]->inter_op_blk_id_lv;
            tpv2=cnodelist[2]->inter_op_blk_id;
            tpv3=inter_make_op(IOP_LSTAR,2,tpv1,tpv2);
            tpv4=join_inter_op(3,tpv1,tpv2,tpv3);
            node->inter_op_blk_id=tpv4;
        }
    }
    else if (node->sym_affix_type==1 || node->sym_affix_type==2 ) // Exp and Exp Exp or Exp
    {
        int vt1_id=cnodelist[0]->val_type_id,vt2_id=cnodelist[2]->val_type_id;
        if (vt1_id!=TYPE_INT || vt2_id!=TYPE_INT)
            syntax_error(7,cnodelist[1]->lineno,"Type mismatched for operands");
        node->val_type_id=TYPE_INT;
        node->is_left_val=false;

        if (node->sym_affix_type==1)
        {
            tpv1=inter_new_var();
            tpv2=inter_new_label();
            tpv3=inter_new_const_int(1);
            tpv4=inter_new_const_int(0);
            
            tpv5=cnodelist[0]->inter_op_blk_id;
            tpv6=cnodelist[2]->inter_op_blk_id;
            tpv7=inter_make_op(IOP_ASSIGN,2,tpv1,tpv4);
            tpv8=inter_make_op(IOP_IF_NE,3,tpv5,tpv3,tpv2);
            tpv9=inter_make_op(IOP_IF_NE,3,tpv6,tpv3,tpv2);
            tpv10=inter_make_op(IOP_ASSIGN,2,tpv1,tpv3);
            tpv11=inter_make_op(IOP_LABEL,1,tpv2);
            join_inter_op_b(tpv1,7,tpv5,tpv6,tpv7,tpv8,tpv9,tpv10,tpv11);
            node->inter_op_blk_id=tpv1;
        }
        else
        {
            tpv1=inter_new_var();
            tpv2=inter_new_label();
            tpv3=inter_new_label();
            tpv4=inter_new_const_int(1);
            tpv5=inter_new_const_int(0);

            tpv6=cnodelist[0]->inter_op_blk_id;
            tpv7=cnodelist[2]->inter_op_blk_id;
            tpv8=inter_make_op(IOP_ASSIGN,2,tpv1,tpv5);
            tpv9=inter_make_op(IOP_IF_EQ,3,tpv6,tpv4,tpv2);
            tpv10=inter_make_op(IOP_IF_EQ,3,tpv7,tpv4,tpv2);
            tpv11=inter_make_op(IOP_GOTO,1,tpv3);
            tpv12=inter_new_label(tpv2);            
            tpv13=inter_make_op(IOP_ASSIGN,2,tpv1,tpv4);
            tpv14=inter_new_label(tpv3);            

            join_inter_op_b(tpv1,9,tpv6,tpv7,tpv8,tpv9,tpv10,tpv11,tpv12,tpv13,tpv14);
            node->inter_op_blk_id=tpv1;            
        }

    }
    else if (node->sym_affix_type==3 || node->sym_affix_type==4 || node->sym_affix_type==5 || node->sym_affix_type==6 || node->sym_affix_type==7)
    {
        int vt1_id=cnodelist[0]->val_type_id,vt2_id=cnodelist[2]->val_type_id;
        //printf("op1type %d op2type %d TYPE_INT %d TYPE_FLOAT %d\n",vt1_id,vt2_id,TYPE_INT,TYPE_FLOAT);
        if  (vt1_id!=vt2_id)
        {
            syntax_error(7,cnodelist[1]->lineno,"Type mismatched for operands");
        }

        node->val_type_id=cnodelist[0]->val_type_id;
        node->is_left_val=false;

        if (node->sym_affix_type==3 || node->sym_affix_type==4 || node->sym_affix_type==5 || node->sym_affix_type==6)
        {
            switch (node->sym_affix_type)
            {
                case 3:
                    tpv1=IOP_ADD;
                    break;
                case 4:
                    tpv1=IOP_MINUS;
                    break;
                case 5:
                    tpv1=IOP_MUL;
                    break;
                case 6:
                    tpv1=IOP_DIV;
                    break;
            }
            tpv2=inter_new_var();    
            tpv3=cnodelist[0]->inter_op_blk_id;
            tpv4=cnodelist[2]->inter_op_blk_id;
            tpv5=inter_make_op(tpv1,3,tpv2,tpv3,tpv4);
            join_inter_op_b(tpv2,3,tpv3,tpv4,tpv5);
            node->inter_op_blk_id=tpv2;
        }
        else
        {
            pnode=get_nth_child(cnodelist[1],1);
            switch (pnode->sym_type)
            {
                case S_BIGGER:
                    tpv1=IOP_IF_LEQ;
                    break;
                case S_BIGGEREQUAL:
                    tpv1=IOP_IF_L;
                    break;
                case S_EQUAL:
                    tpv1=IOP_IF_NE;
                    break;
                case S_LESS:
                    tpv1=IOP_IF_GEQ;
                    break;
                case S_LESSEQUAL:
                    tpv1=IOP_IF_G;
                    break;
                case S_NOTEQUAL:
                    tpv1=IOP_IF_EQ;
            }
            
            tpv2=inter_new_var();
            tpv3=inter_new_label();

            tpv4=inter_new_const_int(1);
            tpv5=inter_new_const_int(0);

            tpv6=cnodelist[0]->inter_op_blk_id;
            tpv7=cnodelist[2]->inter_op_blk_id;
            tpv8=inter_make_op(IOP_ASSIGN,2,tpv2,tpv5);
            tpv9=inter_make_op(tpv1,3,tpv6,tpv7,tpv3);
            tpv10=inter_make_op(IOP_ASSIGN,2,tpv2,tpv4);
            tpv11=inter_make_op(IOP_LABEL,1,tpv3);

            join_inter_op_b(tpv2,6,tpv6,tpv7,tpv8,tpv9,tpv10,tpv11);
            node->inter_op_blk_id=tpv2;
        }
    }
    //inter_output(node->inter_op_blk_id);
    //printf("\n\n");
}

MAKE_OP_FUNC(exp_func_call,ROOT_LAST_ACTION)
{
    char* funcname=cnodelist[0]->value.pstr;
    int funcid=find_func(funcname);
    int inter_arg[32],tpv1,tpv2,tpv3;
    int built_in_func=false;
    if (funcid==-1)
    {
        char tp[0x100];
        if (get_variable(funcname)!=-1)
        {
            sprintf(tp,"\"%s\" is not a function",funcname);
            syntax_error(11,cnodelist[0]->lineno,tp);
        }
        else
        {            
            sprintf(tp,"Undefined function \"%s\"",funcname);
            syntax_error(2,cnodelist[0]->lineno,tp);
        }
    }
    else if (node->sym_affix_type==0 || node->sym_affix_type==1)
    {
        if (funcid==inter_read_func_id || funcid==inter_write_func_id)
            built_in_func=1;
        int i=0;
        int base=func_table[funcid].arg_pos;
        int cnt=func_table[funcid].arg_size;
        bool f=true;
        _SI* arg_node=cnodelist[2];


        while (i<cnt)
        {
            _SI* expnode=get_nth_child(arg_node,1);
            if (expnode->val_type_id!=var_table[base+i].var_type)
            {
                tpv1=expnode->val_type_id;
                tpv2=var_table[base+i].var_type;
                //printf("diff val type %d %d %d ,%d %d %d\n",tpv1,type_table[tpv1]._array.elemtype,type_table[tpv1]._array.elemsize,tpv2,type_table[tpv2]._array.elemtype,type_table[tpv2]._array.elemsize);
                f=false;
                break;
            }
            tpv1=expnode->inter_op_blk_id;
            if (!built_in_func)
            {
                tpv2=inter_make_op(IOP_ARG,1,tpv1);
                inter_arg[2*cnt-i-1]=tpv2;
            }
            inter_arg[i]=tpv1;
            i++;
            if (arg_node->sym_affix_type==1 || i==cnt)
                break;
            else
            {
                arg_node=get_nth_child(arg_node,3);
            }
        }
        if (cnt!=0 || node->sym_affix_type!=1) if (i!=cnt || arg_node->sym_affix_type==0) f=false;
        if (!f) {
            char tp[0x100];
            sprintf(tp,"Function \"%s\" is not applicable for arguments",funcname);
            syntax_error(9,cnodelist[0]->lineno,tp);
        }
        else
        {
            if (built_in_func)
            {
                if (funcid==inter_read_func_id)
                {
                    tpv1=inter_new_var();
                    tpv2=inter_make_op(IOP_READ,1,tpv1);
                    join_inter_op_b(tpv1,1,tpv2);
                    node->inter_op_blk_id=tpv1;
                }
                else
                {
                    inter_arg[1]=inter_make_op(IOP_WRITE,1,inter_arg[0]);
                    node->inter_op_blk_id=join_inter_op_l(2,inter_arg);

                }
            }
            else
            {
                tpv1=inter_get_func(funcid);
                tpv2=inter_new_var();
                inter_arg[2*cnt]=inter_make_op(IOP_CALL,2,tpv2,tpv1);
                node->inter_op_blk_id=join_inter_op_bl(tpv2,2*cnt+1,inter_arg);
            }
        }
    }    
}

MAKE_OP_FUNC(exp_arr,ROOT_LAST_ACTION)
{
    int vid=cnodelist[0]->val_type_id;
    int tpv1,tpv2,tpv3,tpv4,tpv5,tpv6,tpv7,tpv8,tpv9;
    if (type_table[vid].type!=1)
    {
        syntax_error(10,cnodelist[0]->lineno,"it is not an array");
        node->val_type_id=vid;
    }
    else
    {
        int index_type=cnodelist[2]->val_type_id;
        node->val_type_id=type_table[vid]._array.elemtype;
        node->is_left_val=true;
        if (index_type!=TYPE_INT)
            syntax_error(12,cnodelist[0]->lineno,"array index is not an integer");
        else //Exp LB Exp RB 
        {
            tpv1=inter_new_var();
            tpv2=type_table[vid]._array.elemtype;
            tpv2=inter_new_const_int(type_table[tpv2].size);

            tpv3=cnodelist[0]->inter_op_blk_id;
            tpv4=cnodelist[2]->inter_op_blk_id;
            tpv5=inter_make_op(IOP_MUL,3,tpv1,tpv2,tpv4);
            tpv6=inter_make_op(IOP_ADD,3,tpv1,tpv3,tpv1);
            if (type_table[vid]._array.elemtype!=TYPE_INT)
                join_inter_op_b(tpv1,4,tpv3,tpv4,tpv5,tpv6);
            else
            {
                tpv7=inter_make_op(IOP_RSTAR,2,tpv1,tpv1);
                tpv8=join_inter_op(4,tpv3,tpv4,tpv5,tpv6);
                tpv9=join_inter_op(2,tpv8,tpv7);
                inter_blk_add_var_ref(tpv8,tpv1);
                inter_blk_add_var_ref(tpv9,tpv1);
                tpv1=tpv9;
                node->inter_op_blk_id_lv=tpv8;                        
            }
            node->inter_op_blk_id=tpv1;
        }
    }
}

MAKE_OP_FUNC(basic_type,ROOT_LAST_ACTION)
{
    node->val_type_id=find_type(cnodelist[0]->value.pstr,0);
    //printf("type %d\n",node->val_type_id);
}

MAKE_OP_FUNC(stmt,ROOT_LAST_ACTION)
{
    int tpv1,tpv2,tpv3,tpv4,tpv5,tpv6,tpv7,tpv8,tpv9,tpv10,tpv11;
    if (node->sym_affix_type==2) //return Exp ;
    {
        int funcid=node->func_id;
        int ret_type=func_table[funcid].ret_type;
        if (cnodelist[1]->val_type_id!=ret_type)
            syntax_error(8,cnodelist[0]->lineno,"Type mismatched for return");
    }

    switch (node->sym_affix_type)
    {
        case 0: // exp;
        case 1: // compst
            node->inter_op_blk_id=cnodelist[0]->inter_op_blk_id;
            break;
        case 2:
            tpv1=cnodelist[1]->inter_op_blk_id;
            tpv2=inter_make_op(IOP_RETURN,1,tpv1);
            node->inter_op_blk_id=join_inter_op(2,tpv1,tpv2);
            break;
        case 3: //IF LP Exp RP Stmt ELSE Stmt
            tpv1=inter_new_label();
            tpv2=inter_new_label();
            tpv3=inter_new_const_int(1);

            tpv4=cnodelist[2]->inter_op_blk_id;
            tpv5=inter_make_op(IOP_IF_NE,3,tpv4,tpv3,tpv1);
            tpv6=cnodelist[4]->inter_op_blk_id;
            tpv7=inter_make_op(IOP_GOTO,1,tpv2);
            tpv8=inter_make_op(IOP_LABEL,1,tpv1);
            tpv9=cnodelist[6]->inter_op_blk_id;
            tpv10=inter_make_op(IOP_LABEL,1,tpv2);

            tpv11=join_inter_op(7,tpv4,tpv5,tpv6,tpv7,tpv8,tpv9,tpv10);
            node->inter_op_blk_id=tpv11;
            break;
        case 6: //IF LP Exp RP Stmt
            tpv1=inter_new_label();
            tpv2=inter_new_const_int(1);

            tpv3=cnodelist[2]->inter_op_blk_id;
            tpv4=inter_make_op(IOP_IF_NE,3,tpv3,tpv2,tpv1);
            tpv5=cnodelist[4]->inter_op_blk_id;
            tpv6=inter_make_op(IOP_LABEL,1,tpv1);
            tpv7=join_inter_op(4,tpv3,tpv4,tpv5,tpv6);
            node->inter_op_blk_id=tpv7;
            break;            
        case 4: //WHILE LP Exp RP Stmt
            tpv1=inter_new_label();
            tpv2=inter_new_label();

            tpv3=inter_make_op(IOP_LABEL,1,tpv1);
            tpv4=cnodelist[2]->inter_op_blk_id; 
            tpv5=inter_new_const_int(1);
            tpv6=inter_make_op(IOP_IF_NE,3,tpv4,tpv5,tpv2);
            tpv7=cnodelist[4]->inter_op_blk_id;           
            tpv8=inter_make_op(IOP_GOTO,1,tpv1);
            tpv9=inter_make_op(IOP_LABEL,1,tpv2);

            tpv10=join_inter_op(7,tpv3,tpv4,tpv5,tpv6,tpv7,tpv8,tpv9);
            node->inter_op_blk_id=tpv10;
            break;                        
    }
    //printf("node affix type %d\n",node->sym_affix_type);
    //inter_output(node->inter_op_blk_id);
    //printf("\n\n");
}

MAKE_OP_FUNC(pass_type,ROOT_LAST_ACTION)
{
    node->val_type_id=cnodelist[0]->val_type_id;
}

MAKE_OP_FUNC(struct_type,ROOT_LAST_ACTION)
{
    _SI* pad=get_nth_child(cnodelist[1],1);
    char *stname=pad->value.pstr;
    int type_id=find_type(stname,2);
    if (type_id==-1)
    {
        char tp[0x100];
        sprintf(tp,"Undefined structure \"%s\"",stname);
        syntax_error(17,cnodelist[0]->lineno,tp);
    }
    node->val_type_id=type_id;

}

MAKE_OP_FUNC(struct_def,ROOT_LAST_ACTION)
{
    exit_env();
}

MAKE_OP_FUNC(exp_struct,ROOT_LAST_ACTION){
    //printf("********");
    int tid=cnodelist[0]->val_type_id;
    char *fieldid=cnodelist[2]->value.pstr;
    //printf("ssfield %s \n",fieldid);
    if (type_table[tid].type!=2)
    {
        syntax_error(13,cnodelist[0]->lineno,"Illegal use of \".\"");
    }
    else
    {
        int var_id=get_struct_variable(tid,fieldid);
        if (var_id==-1)
        {
            char tp[0x100];
            sprintf(tp,"Non-existent field \"%s\"",fieldid);
            syntax_error(14,cnodelist[0]->lineno,tp);
        }
        else
        {
            node->val_type_id=var_table[var_id].var_type;
        }
    }
    node->is_left_val=true;
}

MAKE_OP_FUNC(inter_op_join,ROOT_LAST_ACTION){

    int n=node->cldno;
    int a[16],al=0,i;
    for (i=0;i<n;i++)
    {
        if (cnodelist[i]->inter_op_blk_id!=-1)
            a[al++]=cnodelist[i]->inter_op_blk_id;
    }
    node->inter_op_blk_id=join_inter_op_l(al,a);   
}
