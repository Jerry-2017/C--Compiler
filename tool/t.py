import os
s="""{%s} { printf(" %s "); add_sym_seq(NULL,%s) };\n"""
a=open("t.txt","ab")
tb="""INT, FLOAT, STRING, CHAR,
        TYPE,
        DOT, COMMA, QUESTION, SEMI,
        ASSIGNOP,
        PLUS, MINUS, STAR, DIV, AND, OR, NOT,
        EQUAL, LESS, BIGGER,  LESSEQUAL, BIGGEREQUAL, NOTEQUAL,
        LP, RP, LB, RB, LC, RC,
        DEFINE, STRUCT, CASE, IF, ELSE, WHILE """
tb=tb.replace(" ","")
tb=tb.replace("\n","")
tb=tb.split(",")
for i in tb:
	a.write(s%(i,i,i))

