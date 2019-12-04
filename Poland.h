#include "LT.h"
#include "IT.h"
#include <iostream>
#include <stack>
#include "Error.h"
#include <list>
#define NUM_OF_PRIR_LEVEL 8	//колькасць розных пры€рытэтаҐ
#define NUM_OF_OPER_IN_LEVEL 4	//колькасць с≥мвалаҐ на кожным ҐзроҐн≥ прф€рытэта
#define SPEC_SUMBOL '@'	//с≥мвал функцы≥
#define SPEC_ARR_SUMBOL ':'	//с≥мвал функцы≥
#define FILL_SYMBOL '#'	//с≥мвал запаҐненн€
namespace Poland {

	//в€рнуць пры€рытэт аперацы≥, альбо -1, кал≥ аперацы€ не знойдзена
	int GetPriorityOfOperation(LT::Entry c);
	//польска€ натацы€
	bool PolishNotation(int *lexTable_pos, LT::LexTable *lexTable, IT::IdTable *idTable, char* text);
}