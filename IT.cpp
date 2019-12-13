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
		table->size=table->countOfStandartFuncs;
		table->maxsize=size;
		table->table = new Entry[size];
		AddStandartFuncs(*table);
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
		entry->countSystem = LT::DEC;
		idtable.table[idtable.size++] = *entry;
		delete entry;
	}

	//атрымаць радок з табл≥цы
	Entry *GetEntry(IdTable& idtable, int n) {
		return &idtable.table[n];
	}
	void DeleteTable(IdTable& idtable);
	void DEleteEntry(IdTable& idTable, int id){
		if (id < 0 || id >= idTable.size) {
			return;
		}
		for (int i = id; i < idTable.size-1; i++) {
			idTable.table[i]= idTable.table[i-1];
		}
		idTable.size--;
	}	//выдалць зап≥
	//праверыць тэкст на ≥д-ы
	void CheckStrForId(char* text, IdTable& idTable, LT::LexTable& lexTable,int MaxErrors) {

		std::list<Error::ERROR> errors;
		//л≥тэралы
		for (int i = 0; i < lexTable.size; i++) {
			LT::Entry *entry = LT::GetEntry(lexTable, i);	//радок лексемы
			char lex = entry->lexema[0];	//лексема
			if (lex == LEX_LITERAL) {
				char* id = WORDS::GetWord(text, entry->sn);
				entry->lexema[LEXEMA_FIXSIZE]='\0';
				Add(idTable, i, entry->lexema);
				GetEntry(idTable, idTable.size - 1)->countSystem = entry->countSys;
				GetEntry(idTable, idTable.size - 1)->idtype = IT::IDTYPE::L;
				if (id[0] >= '0' && id[0] <= '9' || id[0] == '-' || (id[0] >= 'a' && id[0] <= 'f')) {
					GetEntry(idTable, idTable.size - 1)->iddatatype = INT;
					if (GetEntry(idTable, idTable.size - 1)->countSystem != LT::COUNTSYSTEM::DEC) {
						int ind= 0;
						while (id[ind] != '\0') {
							ind++;
						}
						id[ind-1]='\0';
					}
					int res = strtoll(id, NULL, GetEntry(idTable, idTable.size - 1)->countSystem);
					GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
					if (abs(res) >= LITERAL_INT_MAX_SIZE) {
						if (errors.size() >= MaxErrors) {
							errors.push_back(Error::geterror(215));
							throw errors;
						}
						errors.push_back(Error::geterrorin(219, entry->line, entry->col));
						continue;
					}
					else if (res >= LITERAL_INT_MAX_SIZE / 2) {
						res = LITERAL_INT_MAX_SIZE - res;
						res=-res;
					}
					GetEntry(idTable, idTable.size - 1)->value.vint = res;
					GetEntry(idTable, idTable.size - 1)->countSystem = LT::COUNTSYSTEM::DEC;
					//праверка на с≥стэму зл≥чэнн€ л≥тэрала
					int ind = 0;
					while (id[ind + 1] != '\0') {
						ind++;
					}
					if (id[ind] == 'b') {
						GetEntry(idTable, idTable.size - 1)->countSystem= LT::BIN;
					}else if (id[ind] == 'h') {
						GetEntry(idTable, idTable.size - 1)->countSystem = LT::HEX;
					}else if (id[ind] == 'o') {
						GetEntry(idTable, idTable.size - 1)->countSystem = LT::OCT;
					}
				}
				else if(id[0] == '\'') {
					GetEntry(idTable, idTable.size - 1)->iddatatype = CHAR;
					GetEntry(idTable, idTable.size - 1)->value.cint = (id[1]);
				}
				else {
					GetEntry(idTable, idTable.size - 1)->iddatatype = STR;
					int length = WORDS::LengthString(id);
					GetEntry(idTable, idTable.size - 1)->value.vstr.len = 0;
					if (length <= 2) {
						if (errors.size() >= MaxErrors) {
							errors.push_back(Error::geterror(215));
							throw errors;
						}
						errors.push_back(Error::geterrorin(214, entry->line, entry->col));
					} else if (length >= LITERAL_STR_MAX_SIZE){

						if (errors.size() >= MaxErrors) {
							errors.push_back(Error::geterror(215));
							throw errors;
						}
						errors.push_back(Error::geterrorin(219, entry->line, entry->col));
						continue;
					}
					WORDS::StringIDCopy(GetEntry(idTable, idTable.size - 1)->value.vstr.str,id);
					GetEntry(idTable, idTable.size - 1)->value.vstr.len = length;
				}
				int pos = IsId(idTable, idTable.size - 1);
				if (pos != TI_NULLIDX) {
					entry->idxTI = pos;
					DEleteEntry(idTable,idTable.size-1);
				}else {
					entry->idxTI = idTable.size - 1;
				}
			}
		}

		if (!errors.empty())
			throw errors;
		FuncDefenition *curFunc=NULL;	//ц€куча€ функцы€

		//ствараюцца прататыпы функцый
		for (int i = 0; i < lexTable.size; i++) {
			LT::Entry *entry = LT::GetEntry(lexTable, i);	//радок лексемы
			char lex = entry->lexema[0];	//лексема
			char* id = WORDS::GetWord(text, entry->sn);
			WORDS::CutWord(id, ID_MAXSIZE-1);
			if (lex == LEX_ID) {
				if (i > 1) {
					//дыкл€рацы€ функцы≥
					if (LT::GetEntry(lexTable, i - 1)->lexema[0] == 'f') {
						if (i > 2 && LT::GetEntry(lexTable, i - 2)->lexema[0] == 'd') {
							if (errors.size() >= MaxErrors) {
								errors.push_back(Error::geterror(215));
								throw errors;
							}
							errors.push_back(Error::geterrorin(204, entry->line, entry->col));
						}
						Add(idTable, i, id);
						if (i >= 2 && LT::GetEntry(lexTable, i - 2)->lexema[0] == 't') {
							GetEntry(idTable, idTable.size - 1)->idtype = IT::IDTYPE::F;
						}
						else {
							if (errors.size() >= MaxErrors) {
								errors.push_back(Error::geterror(215));
								throw errors;
							}
							errors.push_back(Error::geterrorin(202, LT::GetEntry(lexTable, i)->line, LT::GetEntry(lexTable, i)->col));
						}
						entry->idxTI = idTable.size - 1;


						//задаецца тып ≥д-а
						if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 2)->sn)[0] == 'i') {
							GetEntry(idTable, idTable.size - 1)->iddatatype = INT;
							//GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
						}
						else if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 2)->sn)[0] == 'a') {
							GetEntry(idTable, idTable.size - 1)->iddatatype = ARRAY;
							//GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
						}
						else if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 2)->sn)[0] == 's' &&
							WORDS::GetWord(text, LT::GetEntry(lexTable, i - 2)->sn)[1] == 'a') {
							GetEntry(idTable, idTable.size - 1)->iddatatype = ARRAY_STR;
							//GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
						}
						else if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 2)->sn)[0] == 'c') {
							GetEntry(idTable, idTable.size - 1)->iddatatype = CHAR;
							//GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
						}
						else if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 2)->sn)[0] == 'v') {
							GetEntry(idTable, idTable.size - 1)->iddatatype = VOID;
							//GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
						}
						else {
							GetEntry(idTable, idTable.size - 1)->iddatatype = STR;
							//*(GetEntry(idTable, idTable.size - 1)->value.vstr.str) = TI_STR_DEFAULT;
						}
						curFunc = AddFuncDef(idTable, *(GetEntry(idTable, idTable.size - 1)));
						if (IsId(idTable, id, curFunc) == TI_NULLIDX) {
							if (errors.size() >= MaxErrors) {
								errors.push_back(Error::geterror(215));
								throw errors;
							}
							errors.push_back(Error::geterrorin(209, LT::GetEntry(lexTable, i)->line, LT::GetEntry(lexTable, i)->col));
						}
						GetEntry(idTable, idTable.size - 1)->funcId = curFunc;
					}
				}
			}
			else if (lex == LEX_MAIN) {
				Add(idTable, i, id);
				entry->idxTI = idTable.size - 1;
				curFunc = AddFuncDefMain(idTable, i);
				GetEntry(idTable, idTable.size - 1)->funcId = curFunc;
				GetEntry(idTable, idTable.size - 1)->idtype = IT::IDTYPE::F;
			}
		}
		curFunc = NULL;
		//абыход ≥д-аҐ
		for (int i = 0; i < lexTable.size; i++) {
			LT::Entry *entry = LT::GetEntry(lexTable, i);	//радок лексемы
			char lex = entry->lexema[0];	//лексема
			if (lex == LEX_ID) {
				char* id = WORDS::GetWord(text, entry->sn);
				WORDS::CutWord(id, ID_MAXSIZE-1);
				if (IsId(idTable, id, curFunc) == TI_NULLIDX) {
					if (curFunc == NULL) {
						//≥дэнтыф≥катар пазамежам≥ функцы≥
						if (errors.size() >= MaxErrors) {
							errors.push_back(Error::geterror(215));
							throw errors;
						}
						errors.push_back(Error::geterrorin(216, LT::GetEntry(lexTable, i)->line, LT::GetEntry(lexTable, i)->col));
						continue;
					}
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
							else if ((LT::GetEntry(lexTable, i - 2)->lexema[0] == ',' && LT::GetEntry(lexTable, i - 3)->lexema[0] == 'i') || (LT::GetEntry(lexTable, i - 2)->lexema[0] == '(' && LT::GetEntry(lexTable, i - 3)->lexema[0] == 'i')) {
								if (curFunc->curParams >= PARMS_MAX_COUNT - 1) {
									if (errors.size() >= MaxErrors) {
										errors.push_back(Error::geterror(215));
										throw errors;
									}
									errors.push_back(Error::geterrorin(217, LT::GetEntry(lexTable, i)->line, LT::GetEntry(lexTable, i)->col));
								}
								GetEntry(idTable, idTable.size - 1)->idtype = IT::IDTYPE::P;
								AddParm(idTable, *curFunc, idTable.size-1);
							}
							else {
								if (errors.size() >= MaxErrors) {
									errors.push_back(Error::geterror(215));
									throw errors;
								}
								errors.push_back(Error::geterrorin(202, LT::GetEntry(lexTable, i)->line, LT::GetEntry(lexTable, i)->col));
							}
							entry->idxTI = idTable.size - 1;

							//задаецца тып ≥д-а
							if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 1)->sn)[0] == 'i') {
								GetEntry(idTable, idTable.size - 1)->iddatatype = INT;
								//GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
							}
							else if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 1)->sn)[0] == 'a') {
								GetEntry(idTable, idTable.size - 1)->iddatatype = ARRAY;
								//GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
							}
							else if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 1)->sn)[0] == 's' &&
									WORDS::GetWord(text, LT::GetEntry(lexTable, i - 1)->sn)[1] == 'a') {
								GetEntry(idTable, idTable.size - 1)->iddatatype = ARRAY_STR;
								//GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
							}
							else if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 1)->sn)[0] == 'c') {
								GetEntry(idTable, idTable.size - 1)->iddatatype = CHAR;
								//GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
							}
							else if (WORDS::GetWord(text, LT::GetEntry(lexTable, i - 1)->sn)[0] == 's') {
								GetEntry(idTable, idTable.size - 1)->iddatatype = STR;
								//GetEntry(idTable, idTable.size - 1)->value.vint = TI_INT_DEFAULT;
							}
							else {
								if (errors.size() >= MaxErrors) {
									errors.push_back(Error::geterror(215));
									throw errors;
								}
								GetEntry(idTable, idTable.size - 1)->iddatatype = VOID;
								errors.push_back(Error::geterrorin(220, LT::GetEntry(lexTable, i)->line, LT::GetEntry(lexTable, i)->col));
							}

							//наданне значэнн€ ≥д-ру
							if (i < lexTable.size - 3) {
								if (LT::GetEntry(lexTable, i + 1)->lexema[0] == '='
									&& LT::GetEntry(lexTable, i + 2)->lexema[0] == 'l'
									&& LT::GetEntry(lexTable, i + 3)->lexema[0] == ';') {
									IDDATATYPE dt = GetEntry(idTable, LT::GetEntry(lexTable, i + 2)->idxTI)->iddatatype;
									if (dt == IT::IDDATATYPE::INT
									&& GetEntry(idTable, idTable.size - 1)->iddatatype == IT::IDDATATYPE::ARRAY
									|| GetEntry(idTable, idTable.size - 1)->iddatatype == IT::IDDATATYPE::ARRAY_STR) {
										//наданне мас≥ву памера
										GetEntry(idTable, idTable.size - 1)->value =
											GetEntry(idTable, LT::GetEntry(lexTable, i + 2)->idxTI)->value;
									}else if (dt != IT::IDDATATYPE::INT && dt != IT::IDDATATYPE::CHAR){
										if (errors.size() >= MaxErrors) {
											errors.push_back(Error::geterror(215));
											throw errors;
										}
										errors.push_back(Error::geterrorin(213, entry->line, entry->col));
									}
									if (GetEntry(idTable, idTable.size - 1)->iddatatype == IT::IDDATATYPE::STR) {
										if (errors.size() >= MaxErrors) {
											errors.push_back(Error::geterror(215));
											throw errors;
										}
										errors.push_back(Error::geterrorin(213, entry->line, entry->col));
									}
								}
								else if ((GetEntry(idTable, idTable.size - 1)->iddatatype == IT::IDDATATYPE::ARRAY
									|| GetEntry(idTable, idTable.size - 1)->iddatatype == IT::IDDATATYPE::ARRAY_STR)
									&& GetEntry(idTable, idTable.size - 1)->idtype != IT::IDTYPE::P) {
									if (errors.size() >= MaxErrors) {
										errors.push_back(Error::geterror(215));
										throw errors;
									}
									errors.push_back(Error::geterrorin(213, entry->line, entry->col));
								}
							}
						}
						else {
							if (errors.size() >= MaxErrors) {
								errors.push_back(Error::geterror(215));
								throw errors;
							}
							errors.push_back(Error::geterrorin(203, LT::GetEntry(lexTable, i)->line, LT::GetEntry(lexTable, i)->col));
						}
					}
					else {
						if (errors.size() >= MaxErrors) {
							errors.push_back(Error::geterror(215));
							throw errors;
						}
						errors.push_back(Error::geterrorin(203, LT::GetEntry(lexTable, i)->line, LT::GetEntry(lexTable, i)->col));
					}
				}
				else {
					if (LT::GetEntry(lexTable, i - 2)->lexema[0] == 'd' 
					|| (LT::GetEntry(lexTable, i - 1)->lexema[0] == 'f' && (GetEntry(idTable, IsId(idTable, id, curFunc))->idxfirstLE!=i || IsId(idTable, id, curFunc) < idTable.countOfStandartFuncs))) {
						//IsId(idTable, id, function);
						if (errors.size() >= MaxErrors) {
							errors.push_back(Error::geterror(215));
							throw errors;
						}
						errors.push_back(Error::geterrorin(209, LT::GetEntry(lexTable, i)->line, LT::GetEntry(lexTable, i)->col));//паҐторна€ дыкл€рацы€
					}
					entry->idxTI = IsId(idTable, id, curFunc);
					if (IT::GetEntry(idTable, entry->idxTI)->idtype == IT::F && LT::GetEntry(lexTable, i - 1)->lexema[0] == 'f') {
						curFunc = IT::GetEntry(idTable, entry->idxTI)->funcId;
					}
				}
			}
			else if (lex == LEX_MAIN) {
				curFunc = GetEntry(idTable,entry->idxTI)->funcId;
			}
			else if ( lex == LEX_LITERAL){
				if (curFunc == NULL) {
					//≥дэнтыф≥катар пазамежам≥ функцы≥
					if (errors.size() >= MaxErrors) {
						errors.push_back(Error::geterror(215));
						throw errors;
					}
					errors.push_back(Error::geterrorin(216, LT::GetEntry(lexTable, i)->line, LT::GetEntry(lexTable, i)->col));
				}
			}
		}

		if (!errors.empty())
			throw errors;
	}

	//ц≥ Єсць так≥ ≥дэнтыф≥катар у табл≥цы?
	int IsId(IdTable& idtable, char *id, FuncDefenition *func) {
		int pos = TI_NULLIDX;
		if (WORDS::StringCompare(id, SF1)) {
			return 0;
		}else if (WORDS::StringCompare(id, SF2)) {
			return 1;
		}
		else if (WORDS::StringCompare(id, SF3)) {
			return 2;
		}
		else if (WORDS::StringCompare(id, SF4)) {
			return 3;
		}
		else if (WORDS::StringCompare(id, SF5)) {
			return 4;
		}
		else if (WORDS::StringCompare(id, SF6)) {
			return 5;
		}
		else if (WORDS::StringCompare(id, SF7)) {
			return 6;
		}
		else if (WORDS::StringCompare(id, SF8)) {
			return 7;
		}
		else if (WORDS::StringCompare(id, SF9)) {
			return 8;
		}

		for (int i = idtable.countOfStandartFuncs; i < idtable.size; i++) {
			if (WORDS::StringCompare(id, GetEntry(idtable,i)->id) == true) {
				if (func == GetEntry(idtable, i)->funcId || GetEntry(idtable, i)->idtype==F) {
					pos = i;
					break;
				}
			}
		}
		return pos;
	}

	int IsId(IdTable& idtable,int id){
		int pos = TI_NULLIDX;
		for (int i = 0; i < idtable.size; i++) {
			if (i == id) {
				continue;
			}
			if (GetEntry(idtable, id)->iddatatype == GetEntry(idtable,i)->iddatatype) {
				if (GetEntry(idtable, id)->iddatatype == IT::IDDATATYPE::INT) {
					if (GetEntry(idtable, id)->value.vint == GetEntry(idtable, i)->value.vint
					&& GetEntry(idtable, id)->countSystem == GetEntry(idtable, i)->countSystem){
						pos = i;
						break;
					}
				}
				else if (GetEntry(idtable, id)->iddatatype == IT::IDDATATYPE::STR) {
					if (WORDS::StringCompare(GetEntry(idtable, id)->value.vstr.str,GetEntry(idtable, i)->value.vstr.str)) {
						pos = i;
						break;
					}
				}
			}
		}
		return pos;
	}	//≥ндэкс ≥дэнтыф≥катара з дадзеным значэннем

	//вывесц≥ табл≥цу
	std::string PrintTable(IdTable& idTable) {
		std::string str;
		str.append("\nID Table\n");
		for (int i = idTable.countOfStandartFuncs; i < idTable.size; i++) {
			Entry* entry = GetEntry(idTable,i);
			str.append(entry->id);
			str.append(":\t");
			if (entry->idtype == V) {
				str.append("variable");
			}
			else if (entry->idtype == L) {
				str.append("literal");
			}
			else if (entry->idtype == F) {
				str.append("function");
			}
			else {
				str.append("parametr");
			}
			str.append(",\t");
			if (entry->funcId!=NULL){
				str.append("in function: ");
				str.append(entry->funcId->name);
				str.append(",\t");
			}

			str.append("type: ");
			if (entry->iddatatype == INT) {
				str.append("integer");
				if (entry->idtype == L){
					str.append(",\t");
					str.append(std::to_string(entry->value.vint));
					if (entry->countSystem != LT::DEC) {
						switch (entry->countSystem)
						{
						case LT::HEX:str.append("hex"); break;
						case LT::OCT:str.append("oct"); break;
						case LT::BIN:str.append("bin"); break;
						}
					}
				}
			}
			else if (entry->iddatatype == ARRAY) {
				str.append("array");
				str.append(",\t");
				if (entry->idtype == L)
					str.append(std::to_string(entry->value.vint));
			}
			else if (entry->iddatatype == CHAR) {
				str.append("char");
				str.append(",\t");
				if (entry->idtype == L)
					str.append(std::to_string(entry->value.vint));
			}
			else if (entry->iddatatype == VOID) {
				str.append("void");
				str.append(",\t");
			}
			else if (entry->iddatatype == ARRAY_STR) {
				str.append("string array");
				str.append(",\t");
				if (entry->idtype == L)
					str.append(std::to_string(entry->value.vint));
			}
			else {
				str.append("string");
				str.append(",\t");
				if (entry->idtype == L)
					str.append(entry->value.vstr.str);
			}
			//str.append(",\t");
			//str.append(entry->funcId);
			str.append("\n");
		}
		//вывад функцый
		for (int i = idTable.countOfStandartFuncs; i < idTable.funcCount; i++) {
			str.append("\n");
			str.append("function: ");
			str.append(idTable.funcs[i]->name);
			str.append(", returning type is: ");
			if (idTable.funcs[i]->returnType == INT) {
				str.append("integer");
				str.append(",\t");
			}
			if (idTable.funcs[i]->returnType == ARRAY_STR) {
				str.append("string array");
				str.append(",\t");
			}
			if (idTable.funcs[i]->returnType == ARRAY) {
				str.append("array");
				str.append(",\t");
			}
			if (idTable.funcs[i]->returnType == CHAR) {
				str.append("char");
				str.append(",\t");
			}
			if (idTable.funcs[i]->returnType == VOID) {
				str.append("void");
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
		if (idTable.funcCount >= FUNCS_COUNT - 1) {
			throw Error::geterror(218);
		}
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
		if (idTable.funcCount >= FUNCS_COUNT - 1) {
			throw Error::geterror(218);
		}
		funcDef->name = "main";
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->startOfFunc = index;
		funcDef->returnType = VOID;
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

	void AddStandartFuncs(IdTable& idTable)	//дабав≥ць у табл≥цу стандартны€ функцы≥
	{
		IT::Entry *entry = new IT::Entry;
		entry->idtype=IT::IDTYPE::F;
		//printStr
		FuncDefenition *funcDef = new FuncDefenition();
		funcDef->name = SF1;
		WORDS::StringCopy(entry->id,SF1);
		funcDef->returnType = IT::IDDATATYPE::VOID;
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->curParams=1;
		entry->funcId=funcDef;
		idTable.table[0] = *entry;

		//strCopy
		funcDef = new FuncDefenition();
		funcDef->name = SF2;
		WORDS::StringCopy(entry->id, SF2);
		funcDef->returnType = IT::IDDATATYPE::VOID;
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->curParams = 2;
		entry->funcId = funcDef;
		idTable.table[1] = *entry;

		//intToChar
		funcDef = new FuncDefenition();
		funcDef->name = SF3;
		WORDS::StringCopy(entry->id, SF3);
		funcDef->returnType = IT::IDDATATYPE::STR;
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->curParams = 1;
		entry->funcId = funcDef;
		idTable.table[2] = *entry;

		//strConcat
		funcDef = new FuncDefenition();
		funcDef->name = SF4;
		WORDS::StringCopy(entry->id, SF4);
		funcDef->returnType = IT::IDDATATYPE::VOID;
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->curParams = 2;
		entry->funcId = funcDef;
		idTable.table[3] = *entry;

		//readNum
		funcDef = new FuncDefenition();
		funcDef->name = SF5;
		WORDS::StringCopy(entry->id, SF5);
		funcDef->returnType = IT::IDDATATYPE::INT;
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->curParams = 0;
		entry->funcId = funcDef;
		idTable.table[4] = *entry;

		//readLine
		funcDef = new FuncDefenition();
		funcDef->name = SF6;
		WORDS::StringCopy(entry->id, SF6);
		funcDef->returnType = IT::IDDATATYPE::STR;
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->curParams = 0;
		entry->funcId = funcDef;
		idTable.table[5] = *entry;

		//printLine
		funcDef = new FuncDefenition();
		funcDef->name = SF7;
		WORDS::StringCopy(entry->id, SF7);
		funcDef->returnType = IT::IDDATATYPE::VOID;
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->curParams = 0;
		entry->funcId = funcDef;
		idTable.table[6] = *entry;

		//_pow
		funcDef = new FuncDefenition();
		funcDef->name = SF8;
		WORDS::StringCopy(entry->id, SF8);
		funcDef->returnType = IT::IDDATATYPE::INT;
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->curParams = 2;
		entry->funcId = funcDef;
		idTable.table[7] = *entry;

		//rnd
		funcDef = new FuncDefenition();
		funcDef->name = SF9;
		WORDS::StringCopy(entry->id, SF9);
		funcDef->returnType = IT::IDDATATYPE::INT;
		idTable.funcs[idTable.funcCount++] = funcDef;
		funcDef->curParams = 0;
		entry->funcId = funcDef;
		idTable.table[8] = *entry;
	}
}