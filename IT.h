#pragma once
#define ID_MAXSIZE		10	//назва
#define TI_MAXSIZE		200	//колькасць ід
#define TI_INT_DEFAULT	0x00000000
#define TI_STR_DEFAULT	0x00
#define TI_NULLIDX		0xffffffff
#define TI_STR_MAXSIZE	255
#define PARMS_MAX_COUNT 10	//колькасць параметраў
#define FUNCS_COUNT		10	//колькасць функцый
#include "LT.h"
#include <string>

namespace IT
{
	enum IDDATATYPE{INT=1,STR=2,VOID=3};
	enum IDTYPE	{V=1,F=2,P=3,L=4};		//пераменная, функцыя, параметр, літэрал
	struct FuncDefenition {
		IDDATATYPE returnType;
		int params[PARMS_MAX_COUNT];
		int locals[TI_MAXSIZE];
		std::string name;
		short curLocals=0;	//індэкс апошняй пераменнай
		short curParams=0;	//індэкс апошняга параметра
		short startOfFunc = -1;	//лексема, з якой пачынаецца апісанне функцыі
	};		//структура для захоўвання функцый
	


	struct Entry {
		int idxfirstLE;		//індэкс у табліцы лексем
		char id[ID_MAXSIZE];	//значэнне
		IDDATATYPE iddatatype;	//тып дадзеных	
		IDTYPE idtype;		//тып ідэнтыфікатара
		FuncDefenition *funcId;//функцыя, у якой вызначаны ід
		std::string AssemblerName;
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
	struct IdTable	//табліца
	{
		int maxsize;
		int size;
		Entry* table;
		FuncDefenition **funcs;
		int funcCount=0;
	};
	IdTable CreateTable(int size);		//стварыць табліцу
	void Add(IdTable& idtable, int idxfirstLE, char *id);		//дадацт радок у табліцу
	Entry *GetEntry(IdTable& idtable, int n);		//атрымаць радок з табліцы
	void DeleteTable(IdTable& idtable);
	int IsId(IdTable& idtable, char *id, FuncDefenition *func);	//індэкс ідэнтыфікатара з дадзеным значэннем

	std::string PrintTable(IdTable& idTable);
	void CheckStrForId(char* text, IdTable& idTable, LT::LexTable& lexTable);	//праверыць тэкст на ід-ы

	FuncDefenition *AddFuncDef(IdTable& idTable,Entry& lex);//дабавціь вызначэнне функцыі
	FuncDefenition *AddFuncDefMain(IdTable& idTable,int index);//дабавціь вызначэнне функцыі


	void AddParm(IdTable& idTable, FuncDefenition& f, int index);
	void AddVal(IdTable& idTable, FuncDefenition& f, int index);
}