#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "common.h"

typedef struct YYLTYPE  {  
    int first_line;  
    int first_column;  
    int last_line;  
    int last_column;  
} YYLTYPE;

//#define NULL (void*)0

void yyerror(char *msg);
void travel_syntax_tree(_SI* node,int depth);
void travel(_SI* node,int depth);
int yylex (void);

void destruct();
void init();

#ifdef __KERNEL_C__

_SI* root;
int _isend=false;
int lines=0;
int errorrec=0;

#else

extern int yylineno;
extern char* yytext;
extern _SI* root;
extern int _isend;
extern int lines;
extern int errorrec;

#endif

#endif

