Source-C = ./src/syntax.tab.c ./src/kernel.c ./src/symbol.c ./src/type.c ./src/syntax_op.c ./src/variable.c

parser: lex syntax 
	gcc $(Source-C) -lfl -ly -o parser
lex: 
	flex ./src/lex.l
	mv lex.yy.c ./src/
syntax: 
	bison -d -v ./src/syntax.y
	mv ./syntax.tab.h ./include/
	mv ./syntax.tab.c ./src/
clean:
	rm ./src/lex.yy.c ./src/syntax.tab.c ./include/syntax.tab.h syntax.output parser ./lexical_result.txt
testl: parser
	rm -f lexical_result.txt
	find ./lexical_gramma_testcase/ -name "*.in" | sort | xargs -i -t ./parser {} >> lexical_result.txt
testr: parser
	rm -f lexical_result.txt
	find ./syntax_testcase/ -name "*.in" | sort | xargs -i -t ./parser {} >> lexical_result.txt

