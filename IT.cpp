#include "IT.h"
#include <iostream>
#include "WORDS.h"
#include "LT.h"
#include "Error.h"
#include <string>
namespace IT {

	//стварыць табл≥цу
	IdTable CreateTable(int size){
		IdTable *table = new IdTable();
		table->funcs = new FuncDefenition*[FUNCS_COUNT];
		table->size=0;
		table->maxsize=size;
		table->table = new Entry[size];
		return *table;
	}

	//дадацт радок у табл≥цу
	void Add(IdTable& idtable, int idxfirstLE, char *id) {
		if (idtable.size >= idtable.maxsize) {
			throw(Error::geterror(201));
		}
		Entry *entry = new Entry();
		WORDS::StringCopy(entry->id, id);
		entry->idxfirstLE=idxfirstLE;
		idtable.table[idtable.size++] = *entry;
		delete entry;
	}

	//атрымаць радок з табл≥цы
	Entry *GetEntry(IdTable& idtable, int n) {
		return &idtable.table[n];
	}
	void DeleteTable(IdTable& idtable);

	//праверыць тэкст на ≥д-ы
	void CheckStrForId(char* text, IdTable& idTable, LT::LexTable& lexTable) {
		//л≥тэралы
		std::string function = "";//функцы€, у €кой знаходз≥цца ≥д-ар
		for (int i = 0; i < lexTable.size; i++) {
			LT::Entry *entry = LT::GetEntry(lexTable, i);	//радок лексемы
			char lex = entry->lexema[0];	//лексема
			if (lex == LEX_LITERAL) {
				char* id = WORDS::GetWord(text, entry->sn);
				entry->lexema[LEXEMA_FIXSIZE]='\0';
				Add(idTable, i, entry->lexema);
				GetEntry(idTable, idTable.size - 1)->idtype = IT::IDTYPE::L;
				if (id[0] >= '0' && id[0] <= '9') {
					GetEntry(idTable, idTable.size - 1)->iddatatype = INT;
					GetEntry(idTable, idTable.size - 1)->value.vint = atoi(id);
				} else {
					GetEntry(idTable, idTable.size - 1)->iddatatype = STR;
					GetEntry(idTable, idTable.size - 1)->value.vstr.len = WORDS::LengthWord(id);
					WORDS::StringIDCopy(GetEntry(idTable, idTable.size - 1)->value.vstr.str,id);
				}
				entry->idxTI = idTable.size - 1;
			}
			else {

			}
		}

		FuncDefenition *curFunc=NULL;	//ц€куча€ функцы€
		//≥дэнтыф≥катары
		std::string parent = "";
		for (int i = 0; i < lexTable.size; i++) {
			LT::Entry *entry = LT::GetEntry(lexTable, i);	//радок лексемы
			char lex = entry->lexema[0];	//лексема
			if (lex == LEX_ID) {
				char* id = WORDS::GetWord(text, entry->sn);
				WORDS::CutWord(id, ID_MAXSIZE);
				if (IsId(idTable, id, curFunc) == TI_NULLIDX) {
					if (i > 1) {
						//дыкл€рацы€ пераменнай/параметра
						if (LT::GetEntry(lexTable, i - 1)->lexema[0] == 't') {
							Add(idTable, i, id);
							GetEntry(idTable, idTable.size - 1)->funcId=curFunc;
							//WORDS::StringCopy(GetEntry(idTable, idTable.size - 1)->funcId, function);
							if (LT::GetEntry(lexTable, i - 2)->lexema[0] == 'd') {
								GetEntry(idTable, idTable.size - 1)->idtype = IT::IDTYPE::V;
								AddVal(idTable,*curFunc,idTable.size-1);
							} 
							else if ((LT::GetEntry(lexTable, i - 2)->lexema[0] == ',' && LT::GetEntry(lexTable, i - 3)->lexema[0] == 'i') || LT::GetEntry(lexTable, i - 2)->lexema[0] == '(') {
								GetEntry(idTable, idTable.size - 1)->idtype = IT::IDTYPE::P;
								AddParm(idTable, *curFunc, idTable.size-1);
							}
							else {
								throw(Error::geterrortext(202, text, LT::GetEntry(lexTable, i)->sn));
							}
							entry->idxTI = idTable.size - 1;

							//задаецца тып ≥д-а
							if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 1)->sn)[0] == 'i') {
								GetEntry(idTable, idTable.size - 1)->iddatatype = INT;
								GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
							}
							else {
								GetEntry(idTable, idTable.size - 1)->iddatatype = STR;
								*(GetEntry(idTable, idTable.size - 1)->value.vstr.str) = TI_STR_DEFAULT;
							}

							//наданне значэнн€ ≥д-ру
							if (i < lexTable.size - 3) {
								if (LT::GetEntry(lexTable, i + 1)->lexema[0] == '='
									&& LT::GetEntry(lexTable, i + 2)->lexema[0] == 'l'
									&& LT::GetEntry(lexTable, i + 3)->lexema[0] == ';') {
									//пасл€ '=' н€ма м≥нуса
									IDDATATYPE dt = GetEntry(idTable, LT::GetEntry(lexTable, i + 2)->idxTI)->iddatatype;
									if (dt != GetEntry(idTable, idTable.size - 1)->iddatatype) {
										throw(Error::geterrortext(208, text, LT::GetEntry(lexTable, i + 2)->sn));
									}
									GetEntry(idTable, idTable.size - 1)->iddatatype =
										dt;
									GetEntry(idTable, idTable.size - 1)->value =
										GetEntry(idTable, LT::GetEntry(lexTable, i + 2)->idxTI)->value;
								} else if (i < lexTable.size - 4
									&& LT::GetEntry(lexTable, i + 1)->lexema[0] == '='
									&& LT::GetEntry(lexTable, i + 2)->lexema[0] == '-'
									&& LT::GetEntry(lexTable, i + 3)->lexema[0] == 'l'
									&& LT::GetEntry(lexTable, i + 4)->lexema[0] == ';'
									&& GetEntry(idTable, idTable.size - 1)->iddatatype == INT) {
									//пасл€ '=' Єсць м≥нус
									IDDATATYPE dt = GetEntry(idTable, LT::GetEntry(lexTable, i + 3)->idxTI)->iddatatype;
									GetEntry(idTable, idTable.size - 1)->iddatatype = dt;
									if (dt == IT::IDDATATYPE::STR) {
										throw (Error::geterrortext(208,text, LT::GetEntry(lexTable, i + 2)->sn));
									}
									GetEntry(idTable, idTable.size - 1)->value.vint =
										-GetEntry(idTable, LT::GetEntry(lexTable, i + 3)->idxTI)->value.vint;
								}
							}
						}
						//дыкл€рацы€ функцы≥
						else if (LT::GetEntry(lexTable, i - 1)->lexema[0] == 'f') {
							if (i > 2 && LT::GetEntry(lexTable, i - 2)->lexema[0] == 'd') {
								throw(Error::geterrortext(204,text,entry->sn));
							}
							Add(idTable, i, id);
							//GetEntry(idTable, idTable.size - 1)->funcId = (char*)(function[0]);
							if (i >= 2 && LT::GetEntry(lexTable, i - 2)->lexema[0] == 't') {
								GetEntry(idTable, idTable.size - 1)->idtype = IT::IDTYPE::F;
							}
							else {
								throw(Error::geterrortext(202, text, LT::GetEntry(lexTable, i)->sn));
							}
							function="";
							function.append(id);
							entry->idxTI = idTable.size - 1;

							//задаецца тып ≥д-а
							if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 2)->sn)[0] == 'i') {
								GetEntry(idTable, idTable.size - 1)->iddatatype = INT;
								GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
							}
							else {
								GetEntry(idTable, idTable.size - 2)->iddatatype = STR;
								*(GetEntry(idTable, idTable.size - 1)->value.vstr.str) = TI_STR_DEFAULT;
							}
							curFunc = AddFuncDef(idTable, *(GetEntry(idTable, idTable.size - 1)));
							//наданне значэнн€ ≥д-ру
							if (i < lexTable.size - 2) {
								if (LT::GetEntry(lexTable, i + 1)->lexema[0] == '='
									&& LT::GetEntry(lexTable, i + 2)->lexema[0] == 'l') {
									IDDATATYPE dt = GetEntry(idTable, LT::GetEntry(lexTable, i + 2)->idxTI)->iddatatype;
									GetEntry(idTable, idTable.size - 1)->iddatatype =
										dt;
									GetEntry(idTable, idTable.size - 1)->idtype =
										GetEntry(idTable, LT::GetEntry(lexTable, i + 2)->idxTI)->idtype;
									GetEntry(idTable, idTable.size - 1)->value =
										GetEntry(idTable, LT::GetEntry(lexTable, i + 2)->idxTI)->value;
								}
							}
						}
						else {
							throw(Error::geterrortext(202, text, LT::GetEntry(lexTable, i)->sn));
						}
					}
					else {
						throw(Error::geterrortext(203, text, LT::GetEntry(lexTable, i)->sn));
					}
				}
				else {
					if (LT::GetEntry(lexTable, i - 2)->lexema[0] == 'd') {
						//IsId(idTable, id, function);
						throw(Error::geterrortext(209, text, LT::GetEntry(lexTable, i)->sn));//паҐторна€ дыкл€рацы€
					}
					entry->idxTI = IsId(idTable, id, curFunc);
				}
			}
			else if (lex==LEX_FUNCTION){
				parent.append(WORDS::GetWord(text, LT::GetEntry(lexTable, i)->sn));
			}
			else if (lex == LEX_CONDITION && LT::GetEntry(lexTable, i - 1)->lexema[0] != LEX_ELSE) {
				BlockDefenition *bl = new BlockDefenition();
				curFunc->Blocks.push(bl);
				curFunc->BlocksP.push(bl);
			}
			else if (lex == LEX_ELSE) {
				curFunc->Blocks.top()->countOfElseIf++;
			}
			else if (lex == LEX_BRACELET) {
				if (i < lexTable.size - 1 && LT::GetEntry(lexTable, i+1)->lexema[0] == LEX_ELSE) 
				{
					//curFunc->Blocks.top().countOfElseIf++;
				}
				else {
					if (!curFunc->Blocks.empty()){
						curFunc->Blocks.pop();
					}
				}
			}
			else if (lex == LEX_MAIN) {
				curFunc = AddFuncDefMain(idTable, i);
			}
		}
	}

	//ц≥ Єсць так≥ ≥дэнтыф≥катар у табл≥цы?
	int IsId(IdTable& idtable, char *id, FuncDefenition *func) {
		int pos = TI_NULLIDX;
		for (int i = 0; i < idtable.size; i++) {
			if (WORDS::StringCompare(id, idtable.table[i].id) == true) {
				if (func == idtable.table[i].funcId || idtable.table[i].idtype == IT::IDTYPE::F) {
					pos = i;
					break;
				}
			}
		}
		return pos;
	}

	//вывесц≥ табл≥цу
	std::string PrintTable(IdTable& idTable) {
		std::string str;
		str.append("\nID Table\n");
		for (int i = 0; i < idTable.size; i++) {
			str.append(GetEntry(idTable, i)->id);
			str.append(":\t");
			if (GetEntry(idTable, i)->idtype == V) {
				str.append("variable");
			}
			else if (GetEntry(idTable, i)->idtype == L) {
				str.append("literal");
			}
			else if (GetEntry(idTable, i)->idtype == F) {
				str.append("function");
			}
			else {
				str.append("parametr");
			}
			str.append(",\t");

			if (GetEntry(idTable, i)->iddatatype == INT) {
				str.append("integer");
				str.append(",\t");
				str.append(std::to_string(GetEntry(idTable, i)->value.vint));
			}
			else {
				str.append("string");
				str.append(",\t");
				str.append(GetEntry(idTable, i)->value.vstr.str);
			}
			str.append(",\t");
			//str.append(GetEntry(idTable, i)->funcId);
			str.append("\n");
		}
		//вывад функцый
		for (int i = 0; i < idTable.funcCount; i++) {
			str.append("\n");
			str.append("function: ");
			str.append(idTable.funcs[i]->name);
			str.append(", returning type is: ");
			if (idTable.funcs[i]->returnType == INT) {
				str.append("integer");
				str.append(",\t");
			}
			else {
				str.append("string");
				str.append(",\t");
			}
			str.append(", count of locals is: ");
			str.append(std::to_string(idTable.funcs[i]->curLocals));
			str.append(", count of parametors is: ");
			str.append(std::to_string(idTable.funcs[i]->curParams));
		}
		return str;
	}

	FuncDefenition *AddFuncDef(IdTable& idTable, Entry& lex) {
		FuncDefenition *funcDef = new FuncDefenition();
		funcDef->name=lex.id;
		funcDef->returnType=lex.iddatatype;
		idTable.funcs[idTable.funcCount++] = funcDef;
		lex.funcId=funcDef;
		funcDef->startOfFunc = lex.idxfirstLE;
		return funcDef;
	}	//дабавц≥ь вызначэнне функцы≥

	FuncDefenition *AddFuncDefMain(IdTable& idTable, int index){
		FuncDefenition *funcDef=new FuncDefenition();
		funcDef->name = "main";
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->startOfFunc = index;
		return funcDef;
	}//дабавц≥ь вызначэнне функцы≥
	//дабав≥ць параметр у функцы≥ю
	void AddParm(IdTable& idTable, FuncDefenition& f, int index) {
		f.params[f.curParams++]=index;
	}	
	//дабав≥ць пераменуюу у функцы≥ю
	void AddVal(IdTable& idTable, FuncDefenition& f, int index) {
		f.locals[f.curLocals++] = index;
	}
}