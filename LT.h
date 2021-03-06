#pragma once
#define LEXEMA_FIXSIZE	1		//����� �������	
#define LT_MAXSIZE		800	//����� ��������� ����� � ������
#define LT_TI_NULLIDX	0xffffffff	//���� �������� ��-�
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
//������������
#define	LEX_PLUS		'v'	//+
#define	LEX_MINUS		'v'	//-
#define	LEX_STAR		'v'	//*
#define	LEX_DIRSLASH	'v'	// /
#define	LEX_PROCENT		'v'	// %
//�������
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

//�����
#define LEX_ARRAY		't'
#define LEX_ARRAY_STR	't'
#define LEX_PTR			'v'

//����������� �������
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
	struct Entry	//���� � ������
	{
		COUNTSYSTEM countSys = DEC;		//������ �������
		char lexema[LEXEMA_FIXSIZE];	//�������
		int sn;							//����� ����� � �������� ����
		int idxTI;						//������ � ������ ��
		int line;						//����� � �������� ������
		int col;						//����� ����� � �������� ������
		Entry(char lex) {
			countSys = DEC;
			lexema[0]=lex;
			sn=-1;
			idxTI=-1;
		}
		Entry() {
		}
		bool isAssignedId = false;		//�� ����� ������� ����� �� "="
	};
	struct LexTable	//������
	{
		int maxsize;	//���������� ����� ������
		int size;		//������ ����� ������
		Entry* table;	//��� � ������
	};
	LexTable CreateTable(int size);		//�������� ������
	void Add(LexTable& lextable, char *lexema, int sn, int line, int col);		//������ ����� � ������
	Entry *GetEntry(LexTable& lextable,int n);		//�������� ����� � ������
	void DeleteTable(LexTable& lextable);
	char* PrintTable(LexTable& lexTable);
}