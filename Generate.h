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
#define EXPR_EQU "pop	ax\n\
pop	ebx\n\
mov	[ebx], ax\n"		//=
#define EXPR_SUM "pop	ax\npop	bx\nadd	ax, bx\npush	ax\n"	//+
#define EXPR_IMUL "pop	ax\npop	bx\nimul	ax, bx\npush	ax\n"	//*
#define EXPR_SUB "pop	ax\npop	bx\nsub	bx, ax\npush	bx\n"	// -
#define EXPR_DIV "pop	ax\npop	bx\nidiv	bx, ax\npush	bx\n"	// /
#define EXPR_CLEAR_STACK "pop	bx"
#define EXPR_RETURN "pop	ax\n\
ret\n"	//return


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
#define MOVE_TO_IF "pop	ax\n\
cmp	ax, 0\n\
je	"//пераход, калі ўмова у стэку не выконваецца
#define MOVE_TO "jmp	"	//перайсці да меткі
#define METKA ":;------------------------------"
//a<b
#define IF_LESS "pop	ax\n\
pop	bx\n\
cmp	bx,ax\n\
LAHF\n\
shr	ah, 7\n\
xor	cx, cx\n\
mov	cl, ah\n\
and	cl,1\n\
push	cx\n\
"
//a>b
#define IF_LARGER "pop	ax\n\
pop	bx\n\
cmp	ax,bx\n\
LAHF\n\
shr	ah, 7\n\
xor	cx, cx\n\
mov	cl, ah\n\
and	cl,1\n\
push	cx\n\
"
//a==b
#define IF_EQUALS "pop	bx\n\
pop	ax\n\
cmp	ax,bx\n\
LAHF\n\
shr ah,	6\n\
xor	cx, cx\n\
mov	cl, ah\n\
and	cl,1\n\
push	cx\n\
"
//a!b
#define IF_NOT_EQUALS "pop	bx\n\
pop	ax\n\
cmp	ax,bx\n\
LAHF\n\
shr	ah, 6\n\
xor	cx, cx\n\
mov	cl, ah\n\
not cx\n\
and	cl,1\n\
push	cx\n\
"
//&
#define IF_AND "pop	bx\n\
pop	ax\n\
and	ax,bx\n\
and	ax,1\n\
push	ax\n\
"
//|
#define IF_OR "pop	bx\n\
pop	ax\n\
or	ax,bx\n\
and	ax,1\n\
push	ax\n\
"
//масівы
#define ARRAY1 "["		//пачатак дыкларацыі масіва
#define ARRAY2 "]"		//канец дэкларацыі масіва
#define ARRAY_NAME  "arr"	//радок, які пішацца пасля назвы масіва
#define PTR	"ptr"	//указальнік у асэмблеры
#define LEA "lea	eax,"	//ў еах адрас зменнай
#define ADDR_TO_PTR "mov	"	//прысвоіць адрас масіва ўказальніку
#define PUSH_ADDR "push	"
#define PUSH_VALUE	"pop	eax\n\
mov	bx, [eax]\n\
push	bx\n"	//атрымаць значэнне па адрасе ў стэку і запушыць яго

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