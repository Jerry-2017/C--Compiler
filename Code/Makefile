parser: syntax.tab.c kernel.c
	gcc syntax.tab.c kernel.c  -lfl -ly -o parser
lex.yy.c: lex.l
	flex lex.l
syntax.tab.c: syntax.y lex.yy.c
	bison -d -v syntax.y
clean:
	rm lex.yy.c syntax.tab.c syntax.tab.h syntax.output parser
test: parser
	rm -f lexical_result.txt
	find ./lexical_gramma_testcase/ -name "*.in" | sort | xargs -i -t ./parser {} >> lexical_result.txt
#
