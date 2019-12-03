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
//���� ��� ������ ��������
#define CONST_NAME "lit"
#define INT_TYPE "word"
#define STR_TYPE "byte"
#define END_OF_STR ", 0"
#define LOCALS "local"

//��� ����� ����
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


//����� �������
#define EXPR_END_OF_FUNCTION ""
#define EXPR_CALL "call	"		//@
#define EXPR_PUSH_AX "push	ax"	//�������� ��� � ����(����� ������ �������)
#define EXPR_PUSH_EAX "push	eax"	//�������� ��� � ����(����� ������ �������)

//while
#define WHILE_METKA "while"
#define WHILE_FINALLY "finally_while"	//����� ��� ���� ����� ����

//if
#define IF_METKA "if"		//����� ��� if
#define ELSE_METKA "else"	//����� ��� else/else if
#define FINALLY "finally"	//����� ��� ���� ����� ����
#define MOVE_TO_IF "pop	ax\n\
cmp	ax, 0\n\
je	"//�������, ��� ����� � ����� �� �����������
#define MOVE_TO "jmp	"	//�������� �� ����
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
//�����
#define ARRAY1 "["		//������� ���������� �����
#define ARRAY2 "]"		//����� ���������� �����
#define ARRAY_NAME  "arr"	//�����, �� ������ ����� ����� �����
#define PTR	"ptr"	//��������� � ���������
#define LEA "lea	eax,"	//� ��� ����� �������
#define ADDR_TO_PTR "mov	"	//�������� ����� ����� ����������
#define PUSH_ADDR "push	"
#define PUSH_VALUE	"pop	eax\n\
mov	bx, [eax]\n\
push	bx\n"	//�������� �������� �� ������ � ����� � �������� ���

namespace GEN {
	struct Generator {
		std::string Generate(IT::IdTable &it, LT::LexTable &lt, char *text);	//�������� ��� ����
		Generator();
	};

	//struct Block {
	//	TYPE_OF_BLOCK  type;
	//	bool isOpened = false;	//�� ���� ����������� �����
	//	bool isClosed = false;	//�� ���� ����������� �����
	//};	//��������� ��� ����
	//struct Blocks {
	//	int count = 0;
	//	stack<Block> stack;
	//	string funcName;
	//	void OpenBlock(Block bl);
	//	void CloseBlock();
	//	bool isClosedBlock();
	//};	//���� � �����
	//enum TYPE_OF_BLOCK {
	//	_if=0,
	//	_else_if,
	//	_else,
	//	cycle
	//};	 //��� �����

	std::string NameToAssemblerName(IT::Entry &id);		//��� ���������� � ����������� ���
	std::string DeclarationToAssembler(IT::Entry &lex, std::string name);	//������� ���������� ���������� � ��������
	std::string DeclarationToAssembler(IT::Entry &lex);	//������� ���������� ���������� � ��������
	
	std::string FunctionToAssembler(LT::LexTable &lexTable, IT::IdTable &idTable, int index, IT::FuncDefenition &func);	//��������� ������� � 

	bool DoesExprHaveEquals(LT::LexTable &lexTable, IT::IdTable &idTable, int index);	//�� ��� ����� �������� ������������





}