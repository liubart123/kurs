#include "Poland.h"
namespace Poland {
	char operations_chars[NUM_OF_PRIR_LEVEL][NUM_OF_OPER_IN_LEVEL] = {  
			{'(', ')', ' ', ' '}, 
			{'[', ']', ' ', ' '}, 
			{'/' ,'*', '^', ':'}, 
			{'+' ,'-', ' ', ' '}, 
			{'>' ,'<', '!', '?'},
			{'&' ,'|', ' ', ' '},
			{',', ' ', ' ', ' '}, 
			{'=', LEX_RETURN, ' ', ' '} };

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
		} while (LT::GetEntry(*lexTable, *(lexTable_pos)+(++i))->lexema[0] != ';' && LT::GetEntry(*lexTable, *(lexTable_pos)+(i))->lexema[0] != '{');
		if (LT::GetEntry(*lexTable, *(lexTable_pos)+(i))->lexema[0] == '{') {
			str.pop_back();
			i--;
		}
		int positionOfLexem = 0;	//паз≥цы€ ц€кучай лексемы, дл€ памылак
		//зап≥с натацы≥ у радок
		while (str.empty() == false) {
			char lex = str.begin()->lexema[0];
			if (lex == 'i' || lex == 'l') {
				IT::Entry *it = IT::GetEntry(*idTable, str.begin()->idxTI);
				if (lex == 'i' && IT::GetEntry(*idTable, str.begin()->idxTI)->idtype == IT::IDTYPE::F) {
					steck.push(*str.begin());
					steck.top().lexema[0] = SPEC_SUMBOL;
				} else if (lex == 'i' && (it->iddatatype == IT::IDDATATYPE::ARRAY
				|| it->iddatatype == IT::IDDATATYPE::STR
				|| it->iddatatype == IT::IDDATATYPE::ARRAY_STR)) {
					steck.push(*str.begin());
					if (positionOfLexem == 0) {
						steck.top().isAssignedId = true;
					}
					steck.top().lexema[0] = SPEC_ARR_SUMBOL;
				}
				else {
					if (positionOfLexem == 0 && LT::GetEntry(*lexTable, *lexTable_pos + 1)->lexema[0] == LEX_EQUALS) {
						str.begin()->isAssignedId=true;
					}
					polandStr.push_back(*str.begin());
				}
			}
			else if (lex == 'v' || lex == '=' || lex == '('
				|| lex == ')'
				|| lex == '['
				|| lex == ']'
				|| lex == ','
				|| lex == '!'
				|| lex == LEX_RETURN)
			{
				/*if (lex == '=' && polandStr.empty() == false) {
					polandStr.begin()->isAssignedId = true;
				}*/
				int oper = GetPriorityOfOperation(*str.begin());
				if (oper == -1) {
					//throw (Error::geterror(108));
					throw(Error::geterrortext(207, text, lexTable->table[*lexTable_pos + positionOfLexem].sn));
					//throw(Error::geterrortext(108, text, *lexTable_pos + positionOfLexem));
				}
				else {
					if (lex == ',') {
						while (steck.empty() == false && steck.top().lexema[0] != '(') {
							polandStr.push_back(steck.top());
							steck.pop();
						}
						//steck.push(*str.begin());
					}
					else if (lex == '(' || lex == '[') {
						steck.push(*str.begin());
					}
					else if (lex == ')') {
						while (steck.empty() == false && steck.top().lexema[0] != '(') {
							polandStr.push_back(steck.top());
							steck.pop();
						}
						if (steck.empty()) {
							throw Error::geterror(205);
						}
						if (steck.top().lexema[0] == '(') {
							steck.pop();
							if (steck.empty()) {

							}else{
								LT::Entry temp = steck.top();
								if (temp.lexema[0] == SPEC_SUMBOL && IT::GetEntry(*idTable, steck.top().idxTI)->idtype == IT::IDTYPE::F) {
									//steck.top().lexema[0]= SPEC_SUMBOL;
									polandStr.push_back(steck.top());
									steck.pop();
								}
							}
						}
					}
					else if (lex == ']') {
						while (steck.empty() == false && steck.top().lexema[0] != '[') {
							polandStr.push_back(steck.top());
							steck.pop();
						}
						if (steck.empty()==true) {
							throw Error::geterror(205);
						}
						steck.pop();
						steck.push(*new LT::Entry(SPEC_ARR_SUMBOL));
						steck.pop();
					}
					else {
						while (steck.empty() == false && steck.top().lexema[0] != '(' && steck.top().lexema[0] != '['
							&& oper >= GetPriorityOfOperation(steck.top())) {
							polandStr.push_back(steck.top());
							steck.pop();
						}
						steck.push(*str.begin());
					}
				}
			}
			else if (lex == '\n') {
				

			}
			else if (lex == LEX_SEMICOLON || lex == LEX_BRACELET || lex == LEX_DECLARE
				|| lex == LEX_RETURN || lex == LEX_STRING || lex == LEX_INTEGER) {
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