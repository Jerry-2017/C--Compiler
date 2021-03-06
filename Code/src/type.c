#define __TYPE_C__

#include "../include/common.h"


int check_dup_type(_TI* pti)
{
    int ret=-1,i;
    if (pti->type==BASIC_TYPE)
    {
        for (i=0;i<ttcnt;i++)
            if (type_table[i].type==0 && strcmp(pti->name,type_table[i].name)==0)
            {
                ret=i;
                break;
            }
    }
    else if (pti->type==ARRAY_TYPE)
    {
        for (i=0;i<ttcnt;i++)
            if (type_table[i].type==1 && pti->_array.elemtype==type_table[i]._array.elemtype &&
                     pti->_array.elemsize==type_table[i]._array.elemsize)
            {
                ret=i;
                break;
            }
    }
    else if (pti->type==2)
    {
        ret=-1;
    }
    return ret;
}

int add_type(_TI* pti)
{
    int ret=check_dup_type(pti);
    if (ret==-1)
    {
        memcpy(&type_table[ttcnt],pti,sizeof(_TI));
        ret=ttcnt++;
    }
    return ret;
}

int add_struct()
{
    _TI temp;
    temp.type=2;
    temp.name="";
    return add_type(&temp);
}

int find_type(char *tname,int type)
{
    int i;
    for (i=0;i<ttcnt;i++)
        if (type_table[i].type==type && strcmp(tname,type_table[i].name)==0)
            return i;
    return -1;
}


int add_type_array(int typeid,int arrsize)
{
    _TI temp;
    temp.type=ARRAY_TYPE;
    temp._array.elemsize=arrsize;
    temp._array.elemtype=typeid;
    temp.size=arrsize*type_table[typeid].size;
    return add_type(&temp);
}

void init_type_table()
{
    ttcnt=0;
    _TI temp;
    temp.type=0;
    temp.name="int";
    temp.size=4;
    TYPE_INT=add_type(&temp);
    temp.type=0;
    temp.name="float";
    temp.size=4;
    TYPE_FLOAT=add_type(&temp);
    
}

int find_func(char* name)
{
    int i;
    for (i=0;i<ft_cnt;i++)
        if (strcmp(func_table[i].name,name)==0)
            return i;
    return -1;
}

int add_func(char *name,int ret_type)
{
    func_table[ft_cnt].name=name;
    func_table[ft_cnt].ret_type=ret_type;
    func_table[ft_cnt].inter_func_id=-1;
    return ft_cnt++;
}

void check_func_def()
{
    int i;
    for (i=0;i<ft_cnt;i++)
        if (!func_table[i].is_def)
        {
            char tp[0x100];
            sprintf(tp,"Undefined function \"%s\"",func_table[i].name);
            syntax_error(18,func_table[i].dec_line,tp);
        }
}

void pass_type(_SI *src,_SI *dst)
{
    src->val_type_id=dst->val_type_id;
}

void init_type()
{
    init_type_table();
}
