#include "LT.h"
#include "IT.h"
#include <iostream>
#include "WORDS.h"
#include "Error.h"
namespace LT {
	//стварыць табліцу
	LexTable CreateTable(int size){
		LexTable *table = new LexTable();
		table->size=0;
		table->maxsize = size;
		table->table = new Entry[size];
		//table->tableID = &IT::CreateTable(TI_MAXSIZE);
		return *table;
	}

	//дадацт радок у табліцу
	void Add(LexTable& lextable, char *lexema,int sn, int line){
		if (lextable.size >= lextable.maxsize) {
			throw(Error::geterror(200));
		}
		Entry *entry = new Entry;
		if (lexema[0] == 'l' || lexema[0] == 'i') {
			WORDS::StringCopy(entry->lexema, lexema);
		}
		else if (lexema[0] == '\0' || lexema[0] == '\n') {
			return;
		} else {
			WORDS::StringCopy(entry->lexema, lexema);
		}
		entry->sn = sn;
		entry->line = line;
		lextable.table[lextable.size++]=*entry;
		delete entry;
	}

	//атрымаць радок з табліцы
	Entry *GetEntry(LexTable& lextable, int n){
		return &lextable.table[n];
	}

	void DeleteTable(LexTable& lextable) {
		delete[] lextable.table;
		delete &lextable;
	}


	char* PrintTable(LexTable& lexTable) {
		char* str=new char[lexTable.size+1];
		for (int i = 0; i < lexTable.size; i++) {
			str[i]=GetEntry(lexTable,i)->lexema[0];
		}
		str[lexTable.size]='\0';
		return str;
	}
}