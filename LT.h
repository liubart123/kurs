#pragma once
#define LEXEMA_FIXSIZE	1		//памер лексемы	
#define LT_MAXSIZE		800	//макс≥м колькасць радкоҐ у табл≥цы
#define LT_TI_NULLIDX	0xffffffff	//н€ма элемента ≥д-а
#define	LEX_INTEGER		't'
#define	LEX_STRING		't'
#define	LEX_VOID		't'
#define	LEX_CHAR		't'
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
#define	LEX_LEFTARR		']'
#define	LEX_RIGHTARR	'['
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

//мас≥вы
#define LEX_ARRAY		't'
#define LEX_ARRAY_STR	't'
#define LEX_PTR			'v'

//стандартны€ функцы≥
#define SF1 "printStr"
#define SF2 "strCopy"
#define SF3 "intToChar"
#define SF4 "strConcat"
#define SF5 "readNum"
#define SF6 "readLine"
#define SF7 "printLine"
#define SF8 "_pow" 
#define SF9 "rnd"
namespace LT
{
	enum COUNTSYSTEM { DEC=10, HEX=16, OCT=8, BIN=2 };
	struct Entry	//зап≥с у ьабл≥цы
	{
		COUNTSYSTEM countSys = DEC;
		char lexema[LEXEMA_FIXSIZE];	//лексема
		int sn;							//нумар радка у зыходным кодз≥
		int idxTI;						//≥ндэкс у табл≥цы ≥д
		int line;						//радок у зыходным тэксце
		int col;						//нумар колнк≥ Ґ зыходным тэксце
		Entry(char lex) {
			countSys = DEC;
			lexema[0]=lex;
			sn=-1;
			idxTI=-1;
		}
		Entry() {
		}
		bool isAssignedId = false;
	};
	struct LexTable	//табл≥ца
	{
		int maxsize;
		int size;
		Entry* table;
		//IT::IdTable *tableID;	//табл≥ца ≥д-аҐ
	};
	LexTable CreateTable(int size);		//стварыць табл≥цу
	void Add(LexTable& lextable, char *lexema, int sn, int line, int col);		//дадацт радок у табл≥цу
	Entry *GetEntry(LexTable& lextable,int n);		//атрымаць радок з табл≥цы
	void DeleteTable(LexTable& lextable);
	char* PrintTable(LexTable& lexTable);
}