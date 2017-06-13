
# 实验报告1

- 编译原理实验一
- 徐秋灵 141130112


## 完成内容
- 语法错误的识别
- 文法错误的识别
- 16进制8进制的识别
- 指数浮点形式
- 两种注释的识别
- 字符串的识别
- 即所有要求内容和附加内容

## 使用方式

- make testl
- 样例在code/lexical_testcase中,编译器输出文件为lexical_result.txt

## 报告正文

###技术难点

- 主要用了flex的宏模式来识别字符串和注释,即在识别注释时切换到另一种模式进行特别的识别.
- 使用了变长的C宏命令方便简化编写程序
- 使用了静态结构上动态建立语法树来加快程序的速度
- 使用了优先级符号来消除BISON程序二义性


# 实验报告2

- 编译原理实验二
- 徐秋灵 141130112

## 完成内容
- 函数表
- 变量表
- SDT框架
- 类型表
- 结构表
- 除Struct结构等价外的所有错误检查

## 使用方式

- make test
- 样例在code/syntax_testcase中,编译器输出文件为lexical_result.txt

## 报告正文

### 实验步骤

首先，通过建立语法书遍历-动作响应框架，使得动作能很方便的加入语法节点的遍历，通过插入动作节点和定义遍历方式属性使得可以方便的传递继承属性与综合属性。

然后，通过建立变量环境栈，把所有变量的声明和定义，（包括函数和结构中的）综合到同一张表中，通过对变量环境栈的维护的搜索，查找最近邻变量和发现变量冲突。

通过建立类型表，对所有类型（数组，基础类型，结构）实现统一的查重与引用。

在建立的结构上，通过对属性的定义实现错误处理的发现与错误恢复。

### 技术难点

如何把SDT框架写得简洁好用，通过大量的宏定义，宏展开，考虑到大部分属性是S属性和L属性，所以通过合理定义前序遍历动作，中序遍历动作和后序遍历动作实现精简的SDT。

数组的定义需要对节点进行逆序扫描，来与继承属性一致，数组的访问需要顺序扫描节点，来与继承属性一致。

合理的文件架构使得写代码和修改变得更为方便，通过 .h 的宏声明使得所有文件只需包含公共头文件即可。

编写环境栈时，给环境设定4中种类，一种是代码块类型，一种是函数申明类型，一种是函数定义类型，一种是结构体定义类型，即新增变量都可以直接在该框架下进行，不用额外增加代码机制来实现。


### 实验结果

    File: ./syntax_testcase/testcase01.in
    Error type 1 at Line 4: Undefined Variable "j"
    Error type 6 at Line 4: The left-hand side of an assignment must be a variable.

    File: ./syntax_testcase/testcase02.in
    Error type 2 at Line 4: Undefined function "inc"

    File: ./syntax_testcase/testcase03.in
    Error type 3 at Line 4: Redefined Variable "i"

    File: ./syntax_testcase/testcase04.in
    Error type 4 at Line 5: Redefined function  "func"

    File: ./syntax_testcase/testcase05.in
    Error type 5 at Line 4: Type mismatched for assignment.

    File: ./syntax_testcase/testcase06.in
    Error type 6 at Line 4: The left-hand side of an assignment must be a variable.

    File: ./syntax_testcase/testcase07.in
    Error type 7 at Line 4: Type mismatched for operands

    File: ./syntax_testcase/testcase08.in
    Error type 8 at Line 4: Type mismatched for return

    File: ./syntax_testcase/testcase09.in
    Error type 9 at Line 8: Function "func" is not applicable for arguments

    File: ./syntax_testcase/testcase101opt.in
    Error type 18 at Line 1: Undefined function "func"

    File: ./syntax_testcase/testcase102opt.in
    Error type 19 at Line 6: Inconsistent declaration of function "func"
    Error type 18 at Line 5: Undefined function "func"

    File: ./syntax_testcase/testcase103opt.in

    File: ./syntax_testcase/testcase104opt.in
    Error type 3 at Line 9: Redefined Variable "i"

    File: ./syntax_testcase/testcase105opt.in
    Error type 5 at Line 15: Type mismatched for assignment.

    File: ./syntax_testcase/testcase106opt.in
    Error type 5 at Line 14: Type mismatched for assignment.

    File: ./syntax_testcase/testcase10.in
    Error type 10 at Line 4: it is not an array

    File: ./syntax_testcase/testcase11.in
    Error type 11 at Line 4: "i" is not a function

    File: ./syntax_testcase/testcase12.in
    Error type 12 at Line 4: array index is not an integer

    File: ./syntax_testcase/testcase13.in
    Error type 13 at Line 8: Illegal use of "."

    File: ./syntax_testcase/testcase14.in
    Error type 14 at Line 8: Non-existent field "n"
    Error type 7 at Line 8: Type mismatched for operands

    File: ./syntax_testcase/testcase15.in
    Error type 16 at Line 4: Redefined field "x"

    File: ./syntax_testcase/testcase16.in
    Error type 16 at Line 5: Duplicated name "Position"

    File: ./syntax_testcase/testcase17.in
    Error type 17 at Line 3: Undefined structure "Position"

