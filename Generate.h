#pragma once
#include "IT.h"
#include "LT.h"
#include <string>

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

#define CONST_NAME "lit"
#define INT_TYPE "dd"
#define STR_TYPE "db"
#define END_OF_STR ", 0"

namespace GEN {
	struct Generator {
		std::string Generate(IT::IdTable it, LT::LexTable lt);	//атрымаць асм файл
		Generator();
	};

	std::string DeclarationToAssembler(IT::Entry lex, std::string name);	//запісаць дыклярацыю пераменнай у асэмблер
	std::string DeclarationToAssembler(IT::Entry lex);	//запісаць дыклярацыю пераменнай у асэмблер
}