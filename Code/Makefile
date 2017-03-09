.PHONY: clean update commit make test 

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99
# 编译目标:src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)

parser: syntax $(filter-out $(LFO),$(OBJS))
	$(CC) -o parser $(filter-out $(LFO),$(OBJS)) -lfl –ly
syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)
lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)
syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

make: 
	echo "nothing"
update:
	git pull origin master
commit: clean
	cd ../
	git add -A
	git commit
	git push origin master
clean:
	find . -name "*.o" | xargs rm -f
	find . -name "*~" | xargs rm -f
	rm -f $(OBJS)
	#find . -name "*.o" | xargs rm -fr lex.yy.c syntax.tab.c syntax.tab.h syntax.output 
	rm -f scanner
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
testl: lexical
	$(CC) lex.yy.c -lfl -o scanner
	rm -f lexical_result.txt
	find ./lexical_gramma_testcase/ -name "*.in" | sort | xargs -i -t ./scanner {} >> lexical_result.txt
	
test: 
	./parser test.cnm
