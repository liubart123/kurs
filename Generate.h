#pragma once
#include "IT.h"
#include "LT.h"
#include <string>
#include "Poland.h"
#include <stack>

#define START_OF_ASM "\
.586\n\
.MODEL FLAT, STDCALL;	\n\
includelib kernel32.lib;\n\
ExitProcess PROTO : DWORD;\n\
includelib kernel32.lib;\n\
includelib user32.lib;\n\
includelib vcruntimed.lib;\n\
includelib ucrtd.lib;\n\
includelib ..\\Debug\\CLib.lib\n\
printStr PROTO : ptr byte\n\
strCopy PROTO : ptr byte, : ptr byte\n\
intToChar PROTO : word\n\
strConcat PROTO : ptr byte, : ptr byte\n\
readNum PROTO\n\
readLine PROTO\n\
printLine PROTO\n\
rnd PROTO\n\
_pow PROTO : word, : word\n\
"

#define CODE_SECTION "\
.code\
"
#define MAIN_ASM_FUNC "\n\
start :\n\
	call main\n\
	push 0\n\
	call  ExitProcess\n\
	end start\n\
"

#define DATA_SECTION "\
.data\
"

#define CONST_SECTION "\
.const\
"

#define PROC_START "	proc"
#define PROC_END "	endp"
//імёны для секцыі дадзеных
#define CONST_NAME "lit"
#define INT_TYPE "word"
#define STR_TYPE "byte"
#define END_OF_STR ", 0"
#define LOCALS "local"

//для секці кода
#define EXPR_INT "push	"		//i
#define EXPR_EQU "pop	eax\n\
pop	ebx\n\
mov	[ebx], ax\n"		//=
#define EXPR_EQU_STR "pop	eax\n\
pop	ebx\n\
mov	[ebx], al\n"		//=
#define EXPR_SUM "pop	eax\npop	ebx\nadd	ax, bx\npush	eax\n"	//+
#define EXPR_IMUL "pop	eax\npop	ebx\nimul	ax, bx\npush	eax\n"	//*
#define EXPR_SUB "pop	eax\npop	ebx\nsub	bx, ax\npush	ebx\n"	// -
#define EXPR_DIV "pop	ebx\npop	eax\ncwd			\nidiv	bx\npush	eax\n"	// /
#define EXPR_MOD "pop	ebx\npop	eax\ncwd			\nidiv	bx\nmovsx	edx, dx\npush	edx\n"	// %
#define EXPR_CLEAR_STACK "pop	ebx"
#define EXPR_RETURN "pop	eax\n\
ret	"	//return


//выклікі функцыі
#define EXPR_END_OF_FUNCTION ""
#define EXPR_CALL "call	"		//@
#define EXPR_PUSH_AX "push	ax"	//запушыць еах у стэк(пасля выкліка функцыі)
#define EXPR_PUSH_EAX "push	eax"	//запушыць еах у стэк(пасля выкліка функцыі)

//while
#define WHILE_METKA "while"
#define WHILE_FINALLY "finally_while"	//метка для кода пасля блёка

//if
#define IF_METKA "if"		//метка для if
#define ELSE_METKA "else"	//метка для else/else if
#define FINALLY "finally"	//метка для кода пасля блёка
#define MOVE_TO_IF "pop	eax\n\
cmp	eax, 0\n\
je	"//пераход, калі ўмова у стэку не выконваецца
#define MOVE_TO "jmp	"	//перайсці да меткі
#define METKA ":;------------------------------"
//a<b
#define IF_LESS "pop	eax\n\
pop	ebx\n\
cmp	ebx,eax\n\
LAHF\n\
shr	ah, 7\n\
xor	ecx, ecx\n\
mov	cl, ah\n\
and	cl,1\n\
push	ecx\n\
"
//a>b
#define IF_LARGER "pop	eax\n\
pop	ebx\n\
cmp	eax,ebx\n\
LAHF\n\
shr	ah, 7\n\
xor	ecx, ecx\n\
mov	cl, ah\n\
and	cl,1\n\
push	ecx\n\
"
//a==b
#define IF_EQUALS "pop	ebx\n\
pop	eax\n\
cmp	eax,ebx\n\
LAHF\n\
shr ah,	6\n\
xor	ecx, ecx\n\
mov	cl, ah\n\
and	cl,1\n\
push	ecx\n\
"
//a!b
#define IF_NOT_EQUALS "pop	ebx\n\
pop	eax\n\
cmp	eax,ebx\n\
LAHF\n\
shr	ah, 6\n\
xor	ecx, ecx\n\
mov	cl, ah\n\
not cx\n\
and	cl,1\n\
push	ecx\n\
"
//&
#define IF_AND "pop	ebx\n\
pop	eax\n\
and	eax,ebx\n\
and	eax,1\n\
push	eax\n\
"
//|
#define IF_OR "pop	ebx\n\
pop	eax\n\
or	eax,ebx\n\
and	eax,1\n\
push	eax\n\
"
//масівы
#define ARRAY1 "["		//пачатак дыкларацыі масіва
#define ARRAY2 "]"		//канец дэкларацыі масіва
#define ARRAY_NAME  "arr"	//радок, які пішацца пасля назвы масіва
#define PTR	"ptr "	//указальнік у асэмблеры
#define LEA "lea	eax,"	//ў еах адрас зменнай
#define ADDR_TO_PTR "mov	"	//прысвоіць адрас масіва ўказальніку
#define PUSH_ADDR "push	"
#define PUSH_VALUE	"pop	eax\n\
mov	ebx, [eax]\n\
push	ebx\n"	//атрымаць значэнне па адрасе ў стэку і запушыць яго
#define PUSH_VALUE_STR	"pop	eax\n\
mov	bl, [eax]\n\
push	ebx\n"	//атрымаць значэнне па адрасе ў стэку і запушыць яго

namespace GEN {
	struct Generator {
		std::string Generate(IT::IdTable &it, LT::LexTable &lt, char *text);	//атрымаць асм файл
		Generator();
	};

	//struct Block {
	//	TYPE_OF_BLOCK  type;
	//	bool isOpened = false;	//ці была адкрываючая дужка
	//	bool isClosed = false;	//ці была закрываючая дужка
	//};	//структура для блёка
	//struct Blocks {
	//	int count = 0;
	//	stack<Block> stack;
	//	string funcName;
	//	void OpenBlock(Block bl);
	//	void CloseBlock();
	//	bool isClosedBlock();
	//};	//стэк з блёкамі
	//enum TYPE_OF_BLOCK {
	//	_if=0,
	//	_else_if,
	//	_else,
	//	cycle
	//};	 //тып пблёка

	std::string NameToAssemblerName(IT::Entry &id);		//імя пераменнай у асэмблернае імя
	std::string DeclarationToAssembler(IT::Entry &lex, std::string name);	//запісаць дыклярацыю пераменнай у асэмблер
	std::string DeclarationToAssembler(IT::Entry &lex);	//запісаць дыклярацыю пераменнай у асэмблер
	
	std::string FunctionToAssembler(LT::LexTable &lexTable, IT::IdTable &idTable, int index, IT::FuncDefenition &func);	//генерацыя функцыі ў 

	bool DoesExprHaveEquals(LT::LexTable &lexTable, IT::IdTable &idTable, int index);	//ці мае выраз аперацыю прысвойвання





}