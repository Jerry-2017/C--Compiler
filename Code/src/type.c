#define __TYPE_C__

#include "../include/common.h"


int check_dup_type(_TI* pti)
{
    int ret;
    if (pti->type==0)
    {
        for (int i=0;i<ttcnt;i++)
            if (type_table[i].type==0 && strcmp(pti->name,type_table[i].name)==0)
            {
                ret=i;
                break;
            }
        ret=-1;
    }
    else if (pti->type==1)
    {
        for (int i=0;i<ttcnt;i++)
            if (type_table[i].type==1 && pti->_array.elemtype==type_table[i]._array.elemtype &&
                     pti->_array.elemsize==type_table[i]._array.elemsize)
            {
                ret=i;
                break;
            }
        ret=-1;
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
    add_type(&temp);
    temp.type=0;
    temp.name="float";
    temp.size=4;
    add_type(&temp);
    
}

int find_func(char* name)
{
    int i;
    for (i=0;i<ft_cnt;i++)
        if (strcmp(func_table[i].name,name)==0)
            return i;
    return -1;
}

int add_func(char *name)
{
    func_table[ft_cnt].name=name;
    return ft_cnt++;
}

void pass_type(_SI *src,_SI *dst)
{
    src->val_type_id=dst->val_type_id;
}

void init_type()
{
    init_type_table();
}
