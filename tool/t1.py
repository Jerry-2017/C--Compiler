s=""" S_END,
    S_ID, 
    S_KEY,    
    S_INT_10, S_INT_8, S_INT_16, S_FLOAT, S_STRING, S_CHAR,
    S_TYPE,
    S_DOT, S_COMMA, S_QUESTION, S_SEMI,
    S_ASSIGNOP,
    S_PLUS, S_MINUS, S_STAR, S_DIV, S_AND, S_OR, S_NOT,
    S_EQUAL, S_LESS, S_BIGGER,  S_LESSEQUAL, S_BIGGEREQUAL, S_NOTEQUAL,
    S_LP, S_RP, S_LB, S_RB, S_LC, S_RC,
    S_DEFINE, S_STRUCT, S_CASE, S_IF, S_ELSE, S_WHILE ,
    S_ERROR,

    S_PROGRAM,
    S_EXTDEFLIST,
    S_EXTDEF,
    S_EXTDECLIST,
    S_SPECIFER,
    S_STRUCTSPECIFER,
    S_OPTTAG,
    S_TAG,
    S_VARDEC,
    S_FUNDEC,
    S_VARLIST,
    S_PARAMDEC,
    S_COMPST,
    S_STMTLIST,
    S_STMT,
    S_DEFLIST,
    S_DEF,
    S_DECLIST,
    S_DEC,
    S_EXP,
    S_INT"""

s=s.replace(" ","")
s=s.replace("\t","")
s=s.replace("\r","")
s=s.replace("\n","")
f=open("code.c","w")
for i in s.split(","):
    j=i.replace("S_","").lower()
    j=i[2]+j[1:]
    f.write("sym_trans_table[%s]=\"%s\";\n"%(i,j))
f.close()
