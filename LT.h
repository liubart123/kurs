#pragma once
#define LEXEMA_FIXSIZE	1		//памер лексемы	
#define LT_MAXSIZE		200	//макс≥м колькасць радкоҐ у табл≥цы
#define LT_TI_NULLIDX	0xffffffff	//н€ма элемента ≥д-а
#define	LEX_INTEGER		't'
#define	LEX_STRING		't'
#define	LEX_ID			'i'
#define	LEX_LITERAL		'l'
#define	LEX_MAIN		'm'
#define	LEX_FUNCTION	'f'
#define	LEX_DECLARE		'd'
#define	LEX_RETURN		'r'
#define	LEX_PRINT		'p'
#define	LEX_SEMICOLON	':'
#define	LEX_COMMA		','
#define	LEX_LEFTBRACE	'{'
#define	LEX_BRACELET	'}'
#define	LEX_LEFTHESIS	')'
#define	LEX_RIGHTHESIS	'('
//арыфметычны€
#define	LEX_PLUS		'v'	//+
#define	LEX_MINUS		'v'	//-
#define	LEX_STAR		'v'	//*
#define	LEX_DIRSLASH	'v'	// /
#define	LEX_PROCENT		'v'	// %
//лаг≥чны€
#define	LEX_LESS		'v'	//<
#define	LEX_GREATER		'v'	//>
#define	LEX_OR			'v' //|
#define	LEX_AND			'v'	//&
#define	LEX_LOG_EQUALS	'v'	//equ ?
#define	LEX_NOT_EQUAL	'v'	//!

#define	LEX_EQUALS		'='
#define	LEX_ERROR		'e'
#define	LEX_SEMICOLON2	';'
#define	LEX_CYCLE		'w'	//while
#define	LEX_BREAK		'b'	//break;
#define	LEX_CONDITION	'k'	//if
#define	LEX_ELSE		'a'	//else
//#include "IT.h"
namespace LT
{
	struct Entry	//зап≥с у ьабл≥цы
	{
		char lexema[LEXEMA_FIXSIZE];	//лексема
		int sn;							//нумар радка у зыходным кодз≥
		int idxTI;						//≥ндэкс у табл≥цы ≥д
		int line;						//радок у зыходным тэксце
		Entry(char lex) {
			lexema[0]=lex;
			sn=-1;
			idxTI=-1;
		}
		Entry() {
		}
	};
	struct LexTable	//табл≥ца
	{
		int maxsize;
		int size;
		Entry* table;
		//IT::IdTable *tableID;	//табл≥ца ≥д-аҐ
	};
	LexTable CreateTable(int size);		//стварыць табл≥цу
	void Add(LexTable& lextable, char *lexema, int sn, int line);		//дадацт радок у табл≥цу
	Entry *GetEntry(LexTable& lextable,int n);		//атрымаць радок з табл≥цы
	void DeleteTable(LexTable& lextable);
	char* PrintTable(LexTable& lexTable);
}