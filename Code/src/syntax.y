%{
#include "stdio.h"
#include "../include/common.h"
//#define YYSTYPE _SI*

%}



%union {
    _SI* a;
};

%token <a>  END ID KEY INT_10 INT_8 INT_16 STRING CHAR FLOAT TYPE  DOT COMMA QUESTION SEMI ASSIGNOP PLUS MINUS STAR DIV AND OR NOT
%token <a> EQUAL LESS BIGGER LESSEQUAL BIGGEREQUAL NOTEQUAL LP RP LB RB LC RC DEFINE STRUCT CASE IF ELSE WHILE RETURN ERROR 

%type <a> Program ExtDefList ExtDef ExtDecList Specifer StructSpecifer Relop
%type <a> OptTag Tag VarDec  FunDec VarList ParamDec Compst StmtList Stmt DefList Def DecList Dec Exp Args
%type <a> Int
//%type Exp Factor Term


%right ASSIGNOP
%left OR
%left AND
%left EQUAL LESS BIGGER LESSEQUAL BIGGEREQUAL NOTEQUAL
%left PLUS MINUS
%left STAR DIV
%right NEG NOT 
%left LP RP LB RB DOT

%nonassoc THEN
%nonassoc ELSE

%%

/*
Program :

FuncDef : TYPE ID LP mDef RP 
mDef :  | mDef1
mDef1 : VarDef COMMA mDef1 | VarDef

// Definition of variable
VarDef : TYPE ID */

Program : ExtDefList { $$=add_sym_node(S_PROGRAM,1,$1); root=$$;  }
        ;

ExtDefList : ExtDef ExtDefList { $$=add_sym_node(S_EXTDEFLIST,2,$1,$2);  }
        | { $$=add_sym_node(S_EXTDEFLIST,0); }
        ;

ExtDef : Specifer ExtDecList SEMI { $$=add_sym_node(S_EXTDEF,3,$1,$2,$3); bind_sym_action($$,SYN_OP_TYPE(pass_def));  }
        |StructSpecifer SEMI { $$=add_sym_node(S_EXTDEF,2,$1,$2); }
        |Specifer FunDec SEMI { $$=add_sym_node(S_EXTDEF,3,$1,$2,$3); bind_sym_action($$,SYN_OP_TYPE(func_dec)); }
        |Specifer FunDec Compst { $$=add_sym_node(S_EXTDEF,3,$1,$2,$3); bind_sym_action($$,SYN_OP_TYPE(func_def)); }
        ;

ExtDecList : VarDec { $$=add_sym_node(S_EXTDECLIST,1,$1);  }
        |VarDec COMMA ExtDecList { $$=add_sym_node(S_EXTDECLIST,3,$1,$2,$3);  }
        ;

Specifer : TYPE { $$=add_sym_node(S_SPECIFER,1,$1); bind_sym_action($$,SYN_OP_TYPE(basic_type)); }
        |StructSpecifer { $$=add_sym_node(S_SPECIFER,1,$1); bind_sym_action($$,SYN_OP_TYPE(pass_type)); }
        ;

StructSpecifer : STRUCT OptTag LC DefList RC  
        { $$=add_sym_node(S_STRUCTSPECIFER,5,$1,$2,$3,$4,$5); bind_sym_action($$,SYN_OP_TYPE(struct_def)); }
        | STRUCT Tag { $$=add_sym_node(S_STRUCTSPECIFER,2,$1,$2); bind_sym_action($$,SYN_OP_TYPE(struct_type)); };

OptTag:ID { $$=add_sym_node(S_OPTTAG,1,$1); $$->sym_affix_type=0; }
        |{ $$=add_sym_node(S_OPTTAG,0); $$->sym_affix_type=1; }
        ;
Tag:ID { $$=add_sym_node(S_TAG,1,$1); }
        ;

VarDec:ID { $$=add_sym_node(S_VARDEC,1,$1); bind_sym_action($$,SYN_OP_TYPE(var_def)); }
        | VarDec LB Int RB { $$=add_sym_node(S_VARDEC,4,$1,$2,$3,$4); $$->reverse_scan=true; bind_sym_action($$,SYN_OP_TYPE(array_def)); }
        ;
FunDec:ID LP VarList RP { $$=add_sym_node(S_FUNDEC,4,$1,$2,$3,$4); $$->sym_affix_type=0; bind_sym_action($$,SYN_OP_TYPE(func_arg_def)); }
        |ID LP RP { $$=add_sym_node(S_FUNDEC,3,$1,$2,$3); $$->sym_affix_type=1; bind_sym_action($$,SYN_OP_TYPE(func_arg_def)); }
        ;
VarList:ParamDec COMMA VarList { $$=add_sym_node(S_VARLIST,3,$1,$2,$3); } // no need to add action
        |ParamDec { $$=add_sym_node(S_VARLIST,1,$1);  } // no need to add action
        ;
ParamDec:Specifer VarDec { $$=add_sym_node(S_PARAMDEC,2,$1,$2); bind_sym_action($$,SYN_OP_TYPE(pass_def));  }
        ;


Compst:LC StmtList RC { $$=add_sym_node(S_COMPST,3,$1,$2,$3); bind_sym_action($$,SYN_OP_TYPE(pass_compst)); }
        ;

StmtList:Stmt StmtList{ $$=add_sym_node(S_STMTLIST,2,$1,$2); }
        | { $$=add_sym_node(S_STMTLIST,0); }
        ;
Stmt:Exp SEMI { $$=add_sym_node(S_STMT,2,$1,$2); $$->sym_affix_type=0; bind_sym_action($$,SYN_OP_TYPE(stmt)); }
        |Compst { $$=add_sym_node(S_STMT,1,$1); $$->sym_affix_type=1; bind_sym_action($$,SYN_OP_TYPE(stmt)); }
        |RETURN Exp SEMI { $$=add_sym_node(S_STMT,3,$1,$2,$3); $$->sym_affix_type=2; bind_sym_action($$,SYN_OP_TYPE(stmt)); }
        |IF LP Exp RP Stmt ELSE Stmt { $$=add_sym_node(S_STMT,7,$1,$2,$3,$4,$5,$6,$7); $$->sym_affix_type=3; bind_sym_action($$,SYN_OP_TYPE(stmt)); }
        |WHILE LP Exp RP Stmt { $$=add_sym_node(S_STMT,5,$1,$2,$3,$4,$5); $$->sym_affix_type=4; bind_sym_action($$,SYN_OP_TYPE(stmt)); }
        |Def { $$=add_sym_node(S_STMT,1,$1); $$->sym_affix_type=5; bind_sym_action($$,SYN_OP_TYPE(stmt)); };
        |IF LP Exp RP Stmt %prec THEN { $$=add_sym_node(S_STMT,5,$1,$2,$3,$4,$5); $$->sym_affix_type=6; bind_sym_action($$,SYN_OP_TYPE(stmt)); };

DefList:Def DefList{ $$=add_sym_node(S_DEFLIST,2,$1,$2); $$->sym_affix_type=4; }
        | { $$=add_sym_node(S_DEFLIST,0); }
        ;
Def:Specifer DecList SEMI
		{ $$=add_sym_node(S_DEF,3,$1,$2,$3); bind_sym_action($$,SYN_OP_TYPE(pass_def)); }
        ;
DecList:Dec { $$=add_sym_node(S_DECLIST,1,$1); $$->sym_affix_type=0; bind_sym_action($$,SYN_OP_TYPE(pass_declist)); }
        |Dec COMMA DecList { $$=add_sym_node(S_DECLIST,3,$1,$2,$3); $$->sym_affix_type=1; bind_sym_action($$,SYN_OP_TYPE(pass_declist)); }
        ;
Dec:VarDec { $$=add_sym_node(S_DEC,1,$1); $$->sym_affix_type=0; bind_sym_action($$,SYN_OP_TYPE(pass_var_dec)); }
        |VarDec ASSIGNOP Exp { $$=add_sym_node(S_DEC,3,$1,$2,$3); $$->sym_affix_type=1; bind_sym_action($$,SYN_OP_TYPE(pass_var_dec)); }
        ;

Exp:Exp ASSIGNOP Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); $$->sym_affix_type=0; bind_sym_action($$,SYN_OP_TYPE(exp_2_op)); }
        |Exp AND Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); $$->sym_affix_type=1; bind_sym_action($$,SYN_OP_TYPE(exp_2_op)); }
        |Exp OR Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); $$->sym_affix_type=2; bind_sym_action($$,SYN_OP_TYPE(exp_2_op)); }
        |Exp PLUS Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); $$->sym_affix_type=3; bind_sym_action($$,SYN_OP_TYPE(exp_2_op)); }
        |Exp MINUS Exp { $$=add_sym_node(S_EXP,3,$1,$2,$3); $$->sym_affix_type=4; bind_sym_action($$,SYN_OP_TYPE(exp_2_op)); }
        |Exp STAR Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); $$->sym_affix_type=5; bind_sym_action($$,SYN_OP_TYPE(exp_2_op)); }
        |Exp DIV Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); $$->sym_affix_type=6; bind_sym_action($$,SYN_OP_TYPE(exp_2_op)); }
        |Exp Relop Exp{ $$=add_sym_node(S_EXP,3,$1,$2,$3); $$->sym_affix_type=7; bind_sym_action($$,SYN_OP_TYPE(exp_2_op)); }
        |LP Exp RP{ $$=add_sym_node(S_EXP,3,$1,$2,$3); $$->sym_affix_type=0; bind_sym_action($$,SYN_OP_TYPE(exp_1_op)); }
        |MINUS Exp %prec NEG { $$=add_sym_node(S_EXP,2,$1,$2); $$->sym_affix_type=1; bind_sym_action($$,SYN_OP_TYPE(exp_1_op)); }
        |NOT Exp { $$=add_sym_node(S_EXP,2,$1,$2); $$->sym_affix_type=2; bind_sym_action($$,SYN_OP_TYPE(exp_1_op)); }
        |ID LP Args RP { $$=add_sym_node(S_EXP,4,$1,$2,$3,$4); $$->sym_affix_type=0; bind_sym_action($$,SYN_OP_TYPE(exp_func_call));}
        |ID LP RP { $$=add_sym_node(S_EXP,3,$1,$2,$3); $$->sym_affix_type=1; bind_sym_action($$,SYN_OP_TYPE(exp_func_call)); }
        |Exp LB Exp RB { $$=add_sym_node(S_EXP,4,$1,$2,$3,$4); bind_sym_action($$,SYN_OP_TYPE(exp_arr)); }
        |Exp DOT ID  { $$=add_sym_node(S_EXP,3,$1,$2,$3); bind_sym_action($$,SYN_OP_TYPE(exp_struct)); }
        |ID { $$=add_sym_node(S_EXP,1,$1); bind_sym_action($$,SYN_OP_TYPE(var_ref));}
        |Int { $$=add_sym_node(S_EXP,1,$1); bind_sym_action($$,SYN_OP_TYPE(basic_type_val)); }
        |FLOAT{ $$=add_sym_node(S_EXP,1,$1); bind_sym_action($$,SYN_OP_TYPE(basic_type_val)); }
        ;

Relop: BIGGER { $$=add_sym_node(S_RELOP,1,$1);}
        |BIGGEREQUAL { $$=add_sym_node(S_RELOP,1,$1);}
        |EQUAL { $$=add_sym_node(S_RELOP,1,$1);}
        |LESS { $$=add_sym_node(S_RELOP,1,$1);}
        |LESSEQUAL { $$=add_sym_node(S_RELOP,1,$1);}
        |NOTEQUAL { $$=add_sym_node(S_RELOP,1,$1);}
        ;

Int :INT_10{ $$=add_sym_node(S_INT,1,$1); bind_sym_action($$,SYN_OP_TYPE(basic_type_val)); }  
        | INT_8{ $$=add_sym_node(S_INT,1,$1); bind_sym_action($$,SYN_OP_TYPE(basic_type_val)); } 
        | INT_16{ $$=add_sym_node(S_INT,1,$1); bind_sym_action($$,SYN_OP_TYPE(basic_type_val)); }

Args:Exp COMMA Args { $$=add_sym_node(S_ARGS,3,$1,$2,$3); $$->sym_affix_type=0; }
        |Exp { $$=add_sym_node(S_ARGS,1,$1); $$->sym_affix_type=1; }
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
        //travel_syntax_tree(root,0);
        travel (root,0);
        destruct();
    }
    printf("\n");
    inter_output(root->inter_op_blk_id);
    return 0;
}
