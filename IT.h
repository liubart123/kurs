#pragma once
#define ID_MAXSIZE		10	//�����
#define TI_MAXSIZE		200	//��������� ��
#define TI_INT_DEFAULT	0x00000000
#define TI_STR_DEFAULT	0x00
#define TI_NULLIDX		0xffffffff
#define TI_STR_MAXSIZE	255
#define PARMS_MAX_COUNT 10	//��������� ���������
#define FUNCS_COUNT		20	//��������� �������
#define LITERAL_STR_MAX_SIZE 255	//���������� ����� ������� �����
#define LITERAL_INT_MAX_SIZE 65535	//���������� �� ������� �������
#include "LT.h"
#include <string>
#include <stack>
#include <list>

namespace IT
{
	enum IDDATATYPE{INT=1,
		STR=2,		//�����
		VOID=3,		//������� �� ������ ��������
		ARRAY,		//��� ���
		ARRAY_STR,	//��� �����
		CHAR		//�����
	};	//��� ��������
	enum IDTYPE	{V=1,F=2,P=3,L=4};		//����������, �������, ��������, ������
	struct FuncDefenition {
		IDDATATYPE returnType;
		int params[PARMS_MAX_COUNT];	//������� � ������ �������������� ���������
		int locals[TI_MAXSIZE];	//������� � ������ �������������� ��������� �������
		std::string name;	//��� �������
		short curLocals=0;	//������ ������� ����������
		short curParams=0;	//������ �������� ���������
		short startOfFunc = -1;	//�������, � ���� ���������� ������� �������
	};		//��������� ��� ��������� �������

	struct Entry {
		int idxfirstLE;		//������ � ������ ������
		char id[ID_MAXSIZE];	//��������
		LT::COUNTSYSTEM countSystem = LT::DEC;	//������ �������
		IDDATATYPE iddatatype;	//��� ��������	
		IDTYPE idtype;		//��� ��������������
		FuncDefenition *funcId;//�������, � ���� ��������� ��
		std::string AssemblerName;	//��� ���� ��������� �������������� � ���������
		union
		{
			int vint;	//������ ��������
			char cint;	//��������� ��������
			struct
			{
				char len;		//������ �����
				char str[TI_STR_MAXSIZE-1];	//�������� �����
			} vstr;
		} value;	//�������� ��������������
	};
	struct IdTable	//������
	{
		int maxsize;	//���������� ����� ������
		int size;		//������ ����� ������
		Entry* table;	//���������� �� ��� ��������
		int countOfStandartFuncs = 9;	//��������� ����������� ������� 
		FuncDefenition **funcs;			//��� �������
		int funcCount=0;	//��������� �������������� �������
	};
	IdTable CreateTable(int size);		//�������� ������
	void Add(IdTable& idtable, int idxfirstLE, char *id);		//������ ����� � ������
	Entry *GetEntry(IdTable& idtable, int n);		//�������� ����� � ������
	void DeleteTable(IdTable& idtable);
	int IsId(IdTable& idtable, char *id, FuncDefenition *func);	//������ �������������� � �������� ���������
	int IsId(IdTable& idtable, int id);	//������ ������� � �������� ���������
	void DEleteEntry(IdTable& idTable, int id);	//������� ����
	std::string PrintTable(IdTable& idTable);
	void CheckStrForId(char* text, IdTable& idTable, LT::LexTable& lexTable, int MaxErrors);	//��������� ����� �� ��-�

	FuncDefenition *AddFuncDef(IdTable& idTable,Entry& lex);//�������� ���������� �������
	FuncDefenition *AddFuncDefMain(IdTable& idTable,int index);//�������� ���������� �������


	void AddParm(IdTable& idTable, FuncDefenition& f, int index);
	void AddVal(IdTable& idTable, FuncDefenition& f, int index);

	void AddStandartFuncs(IdTable& idTable);	//������� � ������ ����������� �������
}