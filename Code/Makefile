Source-C = ./src/syntax.tab.c ./src/kernel.c ./src/symbol.c ./src/type.c ./src/syntax_op.c ./src/variable.c ./src/inter_op.c

parser: lex syntax 
	gcc $(Source-C) -lfl -ly -o parser
lex: 
	flex ./src/lex.l
	mv lex.yy.c ./src/
syntax: 
	bison -d -v --report=all ./src/syntax.y
	mv ./syntax.tab.h ./include/
	mv ./syntax.tab.c ./src/
clean:
	rm -rf ./src/lex.yy.c ./src/syntax.tab.c ./include/syntax.tab.h syntax.output parser ./*_result.txt syntax.output
commit: clean
	git add -A
	git commit
	git push
testl: parser
	rm -f lexical_result.txt
	find ./lexical_gramma_testcase/ -name "*.in" | sort | xargs -i -t ./parser {} >> lexical_result.txt
tests: parser
	rm -f syntax_result.txt
	find ./syntax_testcase/ -name "*.in" | sort | xargs -i -t ./parser {} >> sytax_result.txt
testi: parser
	rm -f intercode_result.txt
	find ./intercode_testcase/ -name "*.in" | sort | xargs -i -t ./parser {} >> intercode_result.txt

calc:
	find  ./ -name "*.c" | xargs wc -l
	find  ./ -name "*.h" | xargs wc -l
