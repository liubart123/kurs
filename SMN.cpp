#include "SMN.h"

using namespace SMN;

bool SMN::SmnTest(IT::IdTable &idTable, LT::LexTable &lexTable, int maxErrors)	//праверка семантык≥
{
	bool DoubleMain = false;
	std::list<Error::ERROR> errors;
	for (int i = 0; i < lexTable.size; i++) {
		LT::Entry *entry = LT::GetEntry(lexTable,i);
		//кал≥ лексема == ≥д
		if (entry->lexema[0] == 'i') {
			IT::Entry *idEntry = IT::GetEntry(idTable, entry->idxTI);

			//≥д мас≥ва
			if (idEntry->iddatatype == IT::ARRAY
				|| idEntry->iddatatype == IT::STR
				|| idEntry->iddatatype == IT::ARRAY_STR) {
				//перад мас≥вам аперацы€  ^
				if (LT::GetEntry(lexTable, i - 1)->lexema[0] != 'v'
					|| LT::GetEntry(lexTable, i - 1)->idxTI != char('^')) {
					if (idEntry->idtype == IT::IDTYPE::V || idEntry->idtype == IT::IDTYPE::P){
						//ц≥ Єсць пасл€ мас≥ва '['
						if (LT::GetEntry(lexTable, i + 1)->lexema[0] != '[' ) {
							if (LT::GetEntry(lexTable, i - 1)->lexema[0] == 'v' || LT::GetEntry(lexTable, i - 1)->lexema[0] == '=' ) {
								if (errors.size() >= maxErrors) {
									errors.push_back(Error::geterrorin(215, entry->line, entry->col));
									throw errors;
								}
								errors.push_back(Error::geterrorin(212, entry->line, entry->col));
							}
							if (((LT::GetEntry(lexTable, i + 1)->lexema[0] == LEX_EQUALS || idEntry->iddatatype == IT::STR) && LT::GetEntry(lexTable, i - 1)->lexema[0] == 't')
								|| (idEntry->idtype == IT::IDTYPE::P && LT::GetEntry(lexTable, i - 1)->lexema[0] == 't')) {
								continue;
							}
							if (errors.size() >= maxErrors) {
								errors.push_back(Error::geterrorin(215, entry->line, entry->col));
								throw errors;
							}
							errors.push_back(Error::geterrorin(213, entry->line, entry->col));
						}
						else{
							int tempPos = i;
							while (LT::GetEntry(lexTable, tempPos)->lexema[0] != ']' && tempPos<lexTable.size) {
								tempPos++;
							}
							tempPos++;
							//кал≥ мас≥Ґ не параметр ≥ не ста≥ць перад '=' ≥ не ста≥ць пасл€ return
							if ((LT::GetEntry(lexTable, tempPos)->lexema[0] != ')'
								&& LT::GetEntry(lexTable, tempPos + 1)->lexema[0] != '{'
								&& (LT::GetEntry(lexTable, tempPos - 1 )->lexema[0] != ',' || LT::GetEntry(lexTable, tempPos - 1)->lexema[0] != '('))
								&& LT::GetEntry(lexTable, tempPos)->lexema[0] != ','
								&& LT::GetEntry(lexTable, tempPos)->lexema[0] != '='
								&& LT::GetEntry(lexTable, i-1)->lexema[0] != 'r')
							{
								if (errors.size() >= maxErrors) {
									errors.push_back(Error::geterrorin(215, entry->line, entry->col));
									throw errors;
								}
								errors.push_back(Error::geterrorin(211, entry->line, entry->col));
								continue;
							}
							if (LT::GetEntry(lexTable, i - 1)->lexema[0] == 'v'
								&& LT::GetEntry(lexTable, i - 1)->idxTI != char('^')) {
								if (errors.size() >= maxErrors) {
									errors.push_back(Error::geterrorin(215, entry->line, entry->col));
									throw errors;
								}
								errors.push_back(Error::geterrorin(211, entry->line, entry->col));
							}
						}
					}
				}
			}

			//≥д - функцы€
			if (idEntry->idtype == IT::F) {
				if (LT::GetEntry(lexTable, i + 1)->lexema[0] != '(') {
					if (errors.size() >= maxErrors) {
						errors.push_back(Error::geterrorin(215, entry->line, entry->col));
						throw errors;
					}
					errors.push_back(Error::geterrorin(212, entry->line, entry->col));
				}
			}
			
		}
		else if (entry->lexema[0] == 'm') {
			if (DoubleMain == true) {
				if (errors.size() >= maxErrors) {
					errors.push_back(Error::geterrorin(215, entry->line, entry->col));
					throw errors;
				}
				errors.push_back(Error::geterrorin(222, entry->line, entry->col));
			} 
			DoubleMain = true;
		}
	}
	if (DoubleMain == false) {
		errors.push_back(Error::geterror(223));
	}
	if (errors.empty() == false) {
		throw errors;
	}
	return true;
}
