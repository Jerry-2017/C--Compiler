%{
#include "stdio.h"
#include "../include/common.h"
//#define YYSTYPE _SI*

%}



%union {
    _SI* a;
};

%token <a>  END ID KEY INT INT_8 INT_16 STRING CHAR FLOAT TYPE  DOT COMMA QUESTION SEMI ASSIGNOP PLUS MINUS STAR DIV AND OR NOT
%token <a> EQUAL LESS BIGGER LESSEQUAL BIGGEREQUAL NOTEQUAL LP RP LB RB LC RC DEFINE STRUCT CASE IF ELSE WHILE RETURN ERROR 

%type <a> Program ExtDefList ExtDef ExtDecList Specifer StructSpecifer
%type <a> OptTag Tag VarDec  FunDec VarList ParamDec Compst StmtList Stmt DefList Def DecList Dec Exp Args
%type <a> Int
//%type Exp Factor Term


%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT
%%

/*
Program :

FuncDef : TYPE ID LP mDef RP 
mDef :  | mDef1
mDef1 : VarDef COMMA mDef1 | VarDef

// Definition of variable
VarDef : TYPE ID */

Program : ExtDefList { $$=add_sym_node(S_PROGRAM,1,$1); root=$$; }
        ;

ExtDefList : ExtDef ExtDefList { $$=add_sym_node(S_EXTDEFLIST,2,$1,$2); }
        | { $$=add_sym_node(S_EXTDEFLIST,0); }
        ;

ExtDef : Specifer ExtDecList SEMI { $$=add_sym_node(S_EXTDEF,3,$1,$2,$3); }
        |Specifer SEMI { $$=add_sym_node(S_EXTDEF,2,$1,$2); }
        |Specifer FunDec Compst { $$=add_sym_node(S_EXTDEF,3,$1,$2,$3); }
        ;

ExtDecList : VarDec { $$=add_sym_node(S_EXTDECLIST,1,$1); }
        |VarDec COMMA ExtDecList { $$=add_sym_node(S_EXTDECLIST,3,$1,$2,$3); }
        ;

Specifer : TYPE { $$=add_sym_node(S_SPECIFER,1,$1); }
        |StructSpecifer { $$=add_sym_node(S_SPECIFER,1,$1); }
        ;

StructSpecifer : STRUCT OptTag LC DefList RC  
        { $$=add_sym_node(S_STRUCTSPECIFER,5,$1,$2,$3,$4,$5); }
        | STRUCT Tag { $$=add_sym_node(S_STRUCTSPECIFER,2,$1,$2); };

OptTag:ID { $$=add_sym_node(S_OPTTAG,1,$1); }
        |{ $$=add_sym_node(S_OPTTAG,0); }
        ;
Tag:ID { $$=add_sym_node(S_TAG,1,$1); }
        ;

VarDec:ID { $$=add_sym_node(S_VARDEC,1,$1); bind_sym_action($$,SYN_OP_TYPE(var_def)); }
        | VarDec LB INT RB { $$=add_sym_node(S_VARDEC,4,$1,$2,$3,$4);  bind_sym_action($$,SYN_OP_TYPE(array_def)); }
        ;
FunDec:ID LP VarList RP 
        { $$=add_sym_node(S_FUNDEC,4,$1,$2,$3,$4); }
        |ID LP RP { $$=add_sym_node(S_FUNDEC,3,$1,$2,$3); }
        ;
VarList:ParamDec COMMA VarList { $$=add_sym_node(S_VARLIST,3,$1,$2,$3); }
        |ParamDec { $$=add_sym_node(S_VARLIST,1,$1); }
        ;
ParamDec:Specifer VarDec { $$=add_sym_node(S_PARAMDEC,2,$1,$2); }
        ;


Compst:LC StmtList RC { $$=add_sym_node(S_COMPST,3,$1,$2,$3); }
        ;

StmtList:Stmt StmtList{ $$=add_sym_node(S_STMTLIST,2,$1,$2); }
        | { $$=add_sym_node(S_STMTLIST,0); }
        ;
Stmt:Exp SEMI { $$=add_sym_node(S_STMT,2,$1,$2); }
        |Compst { $$=add_sym_node(S_STMT,1,$1); }
        |RETURN Exp SEMI { $$=add_sym_node(S_STMT,3,$1,$2,$3); }
        |IF LP Exp RP Stmt ELSE Stmt { $$=add_sym_node(S_STMT,7,$1,$2,$3,$4,$5,$6,$7); }
        |WHILE LP Exp RP Stmt { $$=add_sym_node(S_STMT,5,$1,$2,$3,$4,$5); }
        |Def { $$=add_sym_node(S_STMT,1,$1); };

DefList:Def DefList{ $$=add_sym_node(S_DEFLIST,2,$1,$2); }
        | { $$=add_sym_node(S_DEFLIST,0); }
        ;
Def:Specifer DecList SEMI
		{ $$=add_sym_node(S_DEF,3,$1,$2,$3); }
        ;
DecList:Dec { $$=add_sym_node(S_DECLIST,1,$1); }
        |Dec COMMA DecList { $$=add_sym_node(S_DECLIST,3,$1,$2,$3); }
        ;
Dec:VarDec { $$=add_sym_node(S_DEC,1,$1); }
        |VarDec ASSIGNOP Exp { $$=add_sym_node(S_DEC,3,$1,$2,$3); }
        ;

Exp:Exp ASSIGNOP Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); }
        |Exp AND Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); }
        |Exp PLUS Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); }
        |Exp STAR Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); }
        |Exp DIV Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); }
        |LP Exp RP{ $$=add_sym_node(S_EXP,3,$1,$2,$3); }
        |MINUS Exp { $$=add_sym_node(S_EXP,2,$1,$2); }
        |NOT Exp { $$=add_sym_node(S_EXP,2,$1,$2); }
        |ID LP Args RP { $$=add_sym_node(S_EXP,4,$1,$2,$3,$4); }
        |ID LP RP { $$=add_sym_node(S_EXP,3,$1,$2,$3); }
        |Exp LB Exp RB { $$=add_sym_node(S_EXP,4,$1,$2,$3,$4); }
        |Exp DOT ID  { $$=add_sym_node(S_EXP,3,$1,$2,$3); }
        |ID { $$=add_sym_node(S_EXP,1,$1); }
        |Int { $$=add_sym_node(S_EXP,1,$1); }
        |FLOAT{ $$=add_sym_node(S_EXP,1,$1); }
        ;

Int :INT{ $$=add_sym_node(S_EXP,1,$1); }  
        | INT_8{ $$=add_sym_node(S_EXP,1,$1); } 
        | INT_16{ $$=add_sym_node(S_EXP,1,$1); }

Args:Exp COMMA Args { $$=add_sym_node(S_ARGS,3,$1,$2,$3); }
        |Exp { $$=add_sym_node(S_ARGS,1,$1); }
        ;

%%
#include "../src/lex.yy.c"
#include "../include/common.h"
/*
int main() {
    yyparse();
}*/

void yyerror(char* msg) {
printf( "Error TYPE B in lineno: %d\n", yylineno);
errorrec=1;
}


int main(int argc, char** argv)
{
    init();
    if (argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f)
    {
        perror(argv[1]);
        return 1;
    }
    printf("File: %s\n",argv[1]);
    yyrestart(f);
    root=NULL;
    yyparse();
    if (!errorrec) {
        travel_syntax_tree(root,0);
        //travel (root,0);
    }
    return 0;
}
