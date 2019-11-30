#pragma once
#include "IT.h"
#include "LT.h"
#include <string>
#include "Poland.h"
#include <stack>

#define START_OF_ASM "\
.586\
.MODEL FLAT, STDCALL;	\
includelib kernel32.lib;\
ExitProcess PROTO : DWORD;\
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
#define INT_TYPE "dd"
#define STR_TYPE "db"
#define END_OF_STR ", 0"
#define LOCALS "local"

//��� ����� ����
#define EXPR_INT "push	"		//i
#define EXPR_EQU "pop	"		//=
#define EXPR_SUM "pop	eax\npop	ebx\nadd	eax, ebx\npush	eax\n"	//+
#define EXPR_IMUL "pop	eax\npop	ebx\nimul	eax, ebx\npush	eax\n"	//*
#define EXPR_SUB "pop	eax\npop	ebx\nsub	eax, ebx\npush	eax\n"	// -
#define EXPR_DIV "pop	eax\npop	ebx\nidiv	eax, ebx\npush	eax\n"	// /
#define EXPR_CLEAR_STACK "pop	ebx"

//����� �������
#define EXPR_END_OF_FUNCTION "ret"
#define EXPR_CALL "call	"		//@
#define EXPR_PUSH_EAX "push	eax"	//�������� ��� � ����(����� ������ �������)

//if
#define IF_METKA "if"		//����� ��� if
#define ELSE_METKA "else"	//����� ��� else/else if
#define FINALLY "finally"	//����� ��� ���� ����� ����
#define MOVE_TO_IF "push	eax\n\
imul	eax,	1\n\
je	"//�������, ��� ����� � ����� �� �����������
#define MOVE_TO "jmp	"	//�������� �� ����
#define METKA ":"
//a>b
#define IF_LESS "pop	eax\n\
pop	ebx\n\
cmp	eax,ebx\n\
LAHF\n\
shr	ah, 7\n\
mov	cl, ah\n\
and	ecx,1\n\
push	ecx\n\
"
//a<b
#define IF_LARGER "pop	ebx\n\
pop	eax\n\
cmp	eax,ebx\n\
LAHF\n\
shr	ah, 7\n\
mov	cl, ah\n\
and	ecx,1\n\
push	ecx\n\
"
//a==b
#define IF_EQUALS "pop	ebx\n\
pop	eax\n\
cmp	eax,ebx\n\
LAHF\n\
shr ah, 6\n\
mov	cl, ah\n\
and	ecx,1\n\
push	ecx\n\
"
#define IF_NOT_EQUALS "pop	ebx\n\
pop	eax\n\
cmp	eax,ebx\n\
LAHF\n\
shr	ah, 6\n\
mov	cl, ah\n\
not cl\n\
and	ecx,1\n\
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






}