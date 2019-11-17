#pragma once
#define ID_MAXSIZE		10
#define TI_MAXSIZE		200
#define TI_INT_DEFAULT	0x00000000
#define TI_STR_DEFAULT	0x00
#define TI_NULLIDX		0xffffffff
#define TI_STR_MAXSIZE	255
#include "LT.h"
#include <string>

namespace IT
{
	enum IDDATATYPE{INT=1,STR=2};
	enum IDTYPE	{V=1,F=2,P=3,L=4};		//����������, �������, ��������, ������
	
	struct Entry {
		int idxfirstLE;		//������ � ������ ������
		char id[ID_MAXSIZE];	//��������
		IDDATATYPE iddatatype;	//��� ��������	
		IDTYPE idtype;		//��� ��������������
		char funcId[ID_MAXSIZE];//�������, � ���� ��������� ��
		union
		{
			int vint;
			struct
			{
				char len;
				char str[TI_STR_MAXSIZE-1];
			} vstr;
		} value;
	};
	struct IdTable	//������
	{
		int maxsize;
		int size;
		Entry* table;
	};
	IdTable CreateTable(int size);		//�������� ������
	void Add(IdTable& idtable, int idxfirstLE, char *id);		//������ ����� � ������
	Entry *GetEntry(IdTable& idtable, int n);		//�������� ����� � ������
	void DeleteTable(IdTable& idtable);
	int IsId(IdTable& idtable, char *id, std::string func);	//������ �������������� � �������� ���������

	std::string PrintTable(IdTable& idTable);
	void CheckStrForId(char* text, IdTable& idTable, LT::LexTable& lexTable);	//��������� ����� �� ��-�

}