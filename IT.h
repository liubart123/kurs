#pragma once
#define ID_MAXSIZE		10	//назва
#define TI_MAXSIZE		200	//колькасць ід
#define TI_INT_DEFAULT	0x00000000
#define TI_STR_DEFAULT	0x00
#define TI_NULLIDX		0xffffffff
#define TI_STR_MAXSIZE	255
#define PARMS_MAX_COUNT 10	//колькасць параметраў
#define FUNCS_COUNT		20	//колькасць функцый
#define LITERAL_STR_MAX_SIZE 255	//максімальны памер літэрала радка
#define LITERAL_INT_MAX_SIZE 65535	//максімальны лік літэрала лікавага
#include "LT.h"
#include <string>
#include <stack>
#include <list>

namespace IT
{
	enum IDDATATYPE{INT=1,
		STR=2,		//радок
		VOID=3,		//функцыя не вяртая значэння
		ARRAY,		//масіў лікаў
		ARRAY_STR,	//масіў радкоў
		CHAR		//сімвал
	};	//тып дадзеных
	enum IDTYPE	{V=1,F=2,P=3,L=4};		//пераменная, функцыя, параметр, літэрал
	struct FuncDefenition {
		IDDATATYPE returnType;
		int params[PARMS_MAX_COUNT];	//індэксы ў табліцы ідэнтыфікатараў параметраў
		int locals[TI_MAXSIZE];	//індэксы ў табліцы ідэнтыфікатараў лакальных зменных
		std::string name;	//імя функцыі
		short curLocals=0;	//індэкс апошняй пераменнай
		short curParams=0;	//індэкс апошняга параметра
		short startOfFunc = -1;	//лексема, з якой пачынаецца апісанне функцыі
	};		//структура для захоўвання функцый

	struct Entry {
		int idxfirstLE;		//індэкс у табліцы лексем
		char id[ID_MAXSIZE];	//значэнне
		LT::COUNTSYSTEM countSystem = LT::DEC;	//сістэма злічэння
		IDDATATYPE iddatatype;	//тып дадзеных	
		IDTYPE idtype;		//тып ідэнтыфікатара
		FuncDefenition *funcId;//функцыя, у якой вызначаны ід
		std::string AssemblerName;	//імя якое просвоена ідэнтыфікатару ў асэмблеры
		union
		{
			int vint;	//лікавае значэнне
			char cint;	//сімвальнае значэнне
			struct
			{
				char len;		//даўжыня радка
				char str[TI_STR_MAXSIZE-1];	//значэнне радка
			} vstr;
		} value;	//значэнне ідэнтыфікатара
	};
	struct IdTable	//табліца
	{
		int maxsize;	//максімальны памер табліцы
		int size;		//цякучы памер табліцы
		Entry* table;	//указальнік на масіў элеметнаў
		int countOfStandartFuncs = 9;	//колькасць стандартных функцый 
		FuncDefenition **funcs;			//масіў функцый
		int funcCount=0;	//колькасць карыстальніцкіх функцый
	};
	IdTable CreateTable(int size);		//стварыць табліцу
	void Add(IdTable& idtable, int idxfirstLE, char *id);		//дадацт радок у табліцу
	Entry *GetEntry(IdTable& idtable, int n);		//атрымаць радок з табліцы
	void DeleteTable(IdTable& idtable);
	int IsId(IdTable& idtable, char *id, FuncDefenition *func);	//індэкс ідэнтыфікатара з дадзеным значэннем
	int IsId(IdTable& idtable, int id);	//індэкс лексема з дадзеным значэннем
	void DEleteEntry(IdTable& idTable, int id);	//выдалць запіс
	std::string PrintTable(IdTable& idTable);
	void CheckStrForId(char* text, IdTable& idTable, LT::LexTable& lexTable, int MaxErrors);	//праверыць тэкст на ід-ы

	FuncDefenition *AddFuncDef(IdTable& idTable,Entry& lex);//дабавціь вызначэнне функцыі
	FuncDefenition *AddFuncDefMain(IdTable& idTable,int index);//дабавціь вызначэнне функцыі


	void AddParm(IdTable& idTable, FuncDefenition& f, int index);
	void AddVal(IdTable& idTable, FuncDefenition& f, int index);

	void AddStandartFuncs(IdTable& idTable);	//дабавіць у табліцу стандартныя функцыі
}