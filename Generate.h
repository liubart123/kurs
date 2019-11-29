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
#define MAIN_ASM_FUNC "\
start :\
	call svv\
	push 0\
	call  ExitProcess\
	end start\
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
#define INT_TYPE "dd"
#define STR_TYPE "db"
#define END_OF_STR ", 0"
#define LOCALS "local"

//для секці кода
#define EXPR_INT "push	"		//i
#define EXPR_EQU "pop	"		//=
#define EXPR_SUM "pop	eax\npop	ebx\nadd	eax, ebx\npush	eax\n"	//+
#define EXPR_IMUL "pop	eax\npop	ebx\nimul	eax, ebx\npush	eax\n"	//*
#define EXPR_SUB "pop	eax\npop	ebx\nsub	eax, ebx\npush	eax\n"	// -
#define EXPR_DIV "pop	eax\npop	ebx\nidiv	eax, ebx\npush	eax\n"	// /
#define EXPR_CLEAR_STACK "pop	ebx"

//выклікі функцыі
#define EXPR_END_OF_FUNCTION "ret"
#define EXPR_CALL "call	"		//@
#define EXPR_PUSH_EAX "push	eax"	//запушыць еах у стэк(пасля выкліка функцыі)


namespace GEN {
	struct Generator {
		std::string Generate(IT::IdTable &it, LT::LexTable &lt, char *text);	//атрымаць асм файл
		Generator();
	};

	std::string NameToAssemblerName(IT::Entry &id);		//імя пераменнай у асэмблернае імя
	std::string DeclarationToAssembler(IT::Entry &lex, std::string name);	//запісаць дыклярацыю пераменнай у асэмблер
	std::string DeclarationToAssembler(IT::Entry &lex);	//запісаць дыклярацыю пераменнай у асэмблер
	
	std::string FunctionToAssembler(LT::LexTable &lexTable, IT::IdTable &idTable, int index, IT::FuncDefenition &func);	//генерацыя функцыі ў 






}