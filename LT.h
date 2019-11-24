#pragma once
#define LEXEMA_FIXSIZE	1		//����� �������	
#define LT_MAXSIZE		200	//����� ��������� ����� � ������
#define LT_TI_NULLIDX	0xffffffff	//���� �������� ��-�
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
#define	LEX_PLUS		'v'
#define	LEX_MINUS		'v'
#define	LEX_STAR		'v'
#define	LEX_DIRSLASH	'v'
#define	LEX_ERROR		'e'
#define	LEX_SEMICOLON	';'
//#include "IT.h"
namespace LT
{
	struct Entry	//���� � ������
	{
		char lexema[LEXEMA_FIXSIZE];	//�������
		int sn;							//����� ����� � �������� ����
		int idxTI;						//������ � ������ ��
		int line;						//����� � �������� ������
		Entry(char lex) {
			lexema[0]=lex;
			sn=-1;
			idxTI=-1;
		}
		Entry() {
		}
	};
	struct LexTable	//������
	{
		int maxsize;
		int size;
		Entry* table;
		//IT::IdTable *tableID;	//������ ��-�
	};
	LexTable CreateTable(int size);		//�������� ������
	void Add(LexTable& lextable, char *lexema, int sn, int line);		//������ ����� � ������
	Entry *GetEntry(LexTable& lextable,int n);		//�������� ����� � ������
	void DeleteTable(LexTable& lextable);
	char* PrintTable(LexTable& lexTable);
}