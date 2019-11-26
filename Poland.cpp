#include "Poland.h"
namespace Poland {
	char operations_chars[NUM_OF_PRIR_LEVEL][NUM_OF_OPER_IN_LEVEL] = { {'='}, {'(', ')'} , {'[',']'}, {'/' ,'*'},{'+' ,'-'}, {','} };

	//в€рнуць пры€рытэт аперацы≥, альбо -1, кал≥ аперацы€ не знойдзена
	int GetPriorityOfOperation(LT::Entry c) {
		for (int i = 0; i < NUM_OF_PRIR_LEVEL; i++) {
			for (int j = 0; j < NUM_OF_OPER_IN_LEVEL; j++) {
				if (c.lexema[0] == 'v') {
					if ((char)(c.idxTI) == operations_chars[i][j]) {
						return i;
					}
				}
				else {
					if (c.lexema[0] == operations_chars[i][j]) {
						return i;
					}
				}
			}
		}
		return -1;
	}
	//польска€ натацы€
	bool PolishNotation(int *lexTable_pos, LT::LexTable *lexTable, IT::IdTable *idTable, char* text) {
		std::stack<LT::Entry> steck;	//стэк
		std::list<LT::Entry> str;//зыходны радок
		std::list<LT::Entry> polandStr;//радок у ѕольскай Ќатацџ≥
		int i = 0;
		do {
			str.push_back(*LT::GetEntry(*lexTable, *(lexTable_pos)+i));
		} while (LT::GetEntry(*lexTable, *(lexTable_pos)+(++i))->lexema[0] != ';' );
		int positionOfLexem = 0;	//паз≥цы€ ц€кучай лексемы, дл€ памылак
		//зап≥с натацы≥ у радок
		while (str.empty() == false) {
			if (str.begin()->lexema[0] == 'i' || str.begin()->lexema[0] == 'l') {
				if (str.begin()->lexema[0] == 'i' && IT::GetEntry(*idTable, str.begin()->idxTI)->idtype == IT::IDTYPE::F) {
					steck.push(*str.begin());
					steck.top().lexema[0] = SPEC_SUMBOL;
				}
				else {
					polandStr.push_back(*str.begin());
				}
			}
			else if (str.begin()->lexema[0] == 'v' || str.begin()->lexema[0] == '=' || str.begin()->lexema[0] == '('
				|| str.begin()->lexema[0] == ')'
				|| str.begin()->lexema[0] == '['
				|| str.begin()->lexema[0] == ']'
				|| str.begin()->lexema[0] == ',')
			{
				int oper = GetPriorityOfOperation(*str.begin());
				if (oper == -1) {
					//throw (Error::geterror(108));
					throw(Error::geterrortext(207, text, lexTable->table[*lexTable_pos + positionOfLexem].sn));
					//throw(Error::geterrortext(108, text, *lexTable_pos + positionOfLexem));
				}
				else {
					if (str.begin()->lexema[0] == ',') {
						while (steck.empty() == false && steck.top().lexema[0] != '(') {
							polandStr.push_back(steck.top());
							steck.pop();
						}
						//steck.push(*str.begin());
					}
					else if (str.begin()->lexema[0] == '(' || str.begin()->lexema[0] == '[') {
						steck.push(*str.begin());
					}
					else if (str.begin()->lexema[0] == ')') {
						while (steck.empty() == false && steck.top().lexema[0] != '(') {
							polandStr.push_back(steck.top());
							steck.pop();
						}
						if (steck.top().lexema[0] == '(') {
							steck.pop();
							LT::Entry temp = steck.top();
							if (steck.top().lexema[0] == SPEC_SUMBOL && IT::GetEntry(*idTable, steck.top().idxTI)->idtype == IT::IDTYPE::F) {
								//steck.top().lexema[0]= SPEC_SUMBOL;
								polandStr.push_back(steck.top());
								steck.pop();
							}
						}
						else {
							throw Error::geterror(205);
						}
					}
					else if (str.begin()->lexema[0] == ']') {
						while (steck.empty() == false && steck.top().lexema[0] != '[') {
							polandStr.push_back(steck.top());
							steck.pop();
						}
						steck.push(*new LT::Entry(SPEC_SUMBOL));
					}
					else {
						while (steck.empty() == false && steck.top().lexema[0] != '(' && steck.top().lexema[0] != '['
							&& oper >= GetPriorityOfOperation(steck.top().lexema[0])) {
							polandStr.push_back(steck.top());
							steck.pop();
						}
						steck.push(str.begin()->lexema[0]);
					}
				}
			}
			else if (str.begin()->lexema[0] == '\n') {
				

			}
			else if (str.begin()->lexema[0] == LEX_SEMICOLON || str.begin()->lexema[0] == LEX_BRACELET || str.begin()->lexema[0] == LEX_DECLARE
				|| str.begin()->lexema[0] == LEX_RETURN || str.begin()->lexema[0] == LEX_STRING || str.begin()->lexema[0] == LEX_INTEGER) {
				return false;
			}
			else {
				throw(Error::geterrortext(206, text, lexTable->table[*lexTable_pos + positionOfLexem].sn));
			}
			str.pop_front();
			positionOfLexem++;
		}

		//вызваленне стэка
		while (steck.empty() == false) {
			polandStr.push_back(steck.top());
			steck.pop();
		}

		//зм€ненне табл≥цы лексем новым≥ дадзеным≥
		for (int a = 0; a < i; a++) {
			if (polandStr.empty() == false) {
				*LT::GetEntry(*lexTable, *(lexTable_pos)+(a)) = *polandStr.begin();
				polandStr.pop_front();
			}
			else {
				LT::GetEntry(*lexTable, *(lexTable_pos)+(a))->lexema[0] = FILL_SYMBOL;
			}
		}

		if (steck.empty() == true) {
			*(lexTable_pos)+=i;
			return true;
		}
		else { return false; }


	}
}