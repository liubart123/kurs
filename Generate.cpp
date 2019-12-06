#include "Generate.h"
#define FUNC_SEPARATOR "------------------------------------------"
#define COMMENT ";"

using namespace std;
namespace GEN {
	string Generator::Generate(IT::IdTable &it, LT::LexTable &lt, char *text) {
		string resultStr = "";//���� ���������� ���� �����
		resultStr += START_OF_ASM;
		resultStr += CONST_SECTION;
		resultStr += "\n";
		int i;
		//������ ��������
		for (i = it.countOfStandartFuncs ; i < it.size; i++) {
			if (IT::GetEntry(it, i)->idtype!=IT::IDTYPE::L)
				break;
			resultStr+=DeclarationToAssembler(*IT::GetEntry(it, i), CONST_NAME + to_string(i-it.countOfStandartFuncs));
			resultStr += "\n";
		}	 

		//��������� ���� ��-� � ����������� ����
		for (; i < it.size; i++) {
			NameToAssemblerName(*IT::GetEntry(it, i));
		}

		//������ ��������
		for (i = 0; false && i < it.funcCount; i++) {
			resultStr += COMMENT;
			resultStr += FUNC_SEPARATOR;
			resultStr += "Function: ";
			resultStr += it.funcs[i]->name;
			//resultStr += FUNC_SEPARATOR;
			resultStr += "\n";

			//������ ���������
			resultStr += COMMENT;
			resultStr += FUNC_SEPARATOR;
			resultStr += "parametars";
			resultStr += "\n";
			//resultStr += FUNC_SEPARATOR;
			//��������� �������
			for (int j = 0; j < it.funcs[i]->curParams; j++) {
				resultStr += it.funcs[i]->name + "_";
				resultStr += DeclarationToAssembler(*IT::GetEntry(it, it.funcs[i]->params[j]));
				//resultStr += "\n";
			}

			resultStr += "\n";
			//������ ��������� ��������
			resultStr += COMMENT;
			resultStr += FUNC_SEPARATOR;
			resultStr += "locals";
			resultStr += "\n";
			//resultStr += FUNC_SEPARATOR;
			//��������� ����������
			for (int j = 0; j < it.funcs[i]->curLocals; j++) {
				resultStr += it.funcs[i]->name + "_";
				resultStr += DeclarationToAssembler(*IT::GetEntry(it,it.funcs[i]->locals[j]));
				//resultStr += "\n";
			}
			resultStr += "\n";
		}


		//�������� �������
		for (i = 1; i < lt.size - 2; i++) {
			//i=...
			if (LT::GetEntry(lt, i+1)->lexema[0] == LEX_EQUALS
				&& (LT::GetEntry(lt, i)->lexema[0] == LEX_ID)
			) {
				Poland::PolishNotation(&i, &lt, &it, text);
			}
			//@(...)
			else if (LT::GetEntry(lt, i)->lexema[0] == LEX_ID
				&& LT::GetEntry(lt, i - 1)->lexema[0] != LEX_FUNCTION
				&& IT::GetEntry(it, LT::GetEntry(lt, i)->idxTI)->idtype==IT::IDTYPE::F) 
			{
				Poland::PolishNotation(&i, &lt, &it, text);
			}
			//if(...) or while(...)
			else if (i>2 && LT::GetEntry(lt, i - 1)->lexema[0] == LEX_RIGHTHESIS
			&& (LT::GetEntry(lt, i - 2)->lexema[0] == LEX_CONDITION || LT::GetEntry(lt, i - 2)->lexema[0] == LEX_CYCLE)) {
				Poland::PolishNotation(&i, &lt, &it, text);
			}
			//return ...
			else if (LT::GetEntry(lt, i)->lexema[0] == LEX_RETURN/* && LT::GetEntry(lt, i+1)->lexema[0] != LEX_SEMICOLON2*/) {
				Poland::PolishNotation(&i, &lt, &it, text);
			}
			else if (LT::GetEntry(lt, i+1)->lexema[0] == LEX_RIGHTARR/* && LT::GetEntry(lt, i+1)->lexema[0] != LEX_SEMICOLON2*/) {
				Poland::PolishNotation(&i, &lt, &it, text);
			}
		}

		cout << LT::PrintTable(lt);
		resultStr+=CODE_SECTION;
		//��������� ������ ����
		for (int i = it.countOfStandartFuncs; i < it.funcCount; i++) {
			resultStr+=FunctionToAssembler(lt,it,i, *(it.funcs[i]));
			//std::cout << FunctionToAssembler(lt, it, i, *(it.funcs[i]));
		}
		//std::cout << resultStr;
		resultStr+="\n";
		resultStr+= MAIN_ASM_FUNC;
		return resultStr;
	}	//������� ����� ��� �����
	Generator::Generator() {

	}

	string DeclarationToAssembler(IT::Entry &lex, string name) {
		string resultStr="";
		//resultStr += "\n";
		resultStr = name;
		lex.AssemblerName = resultStr;
		resultStr += " ";
		switch (lex.iddatatype)
		{
		case IT::IDDATATYPE::INT:
			resultStr += INT_TYPE;
			resultStr += " ";
			resultStr += to_string(lex.value.vint);
			break;
		case IT::IDDATATYPE::CHAR:
			resultStr += STR_TYPE;
			resultStr += " \'";
			resultStr += lex.value.cint;
			resultStr +="\'";
			break;
		case IT::IDDATATYPE::STR:
			resultStr += STR_TYPE;
			resultStr += " ";
			resultStr += "\"";
			resultStr += lex.value.vstr.str;
			resultStr += "\"";
			resultStr += END_OF_STR;
			break;
		default:
			break;
		}
		//resultStr += "\n";
		return resultStr;
	}//������� ���������� ���������� � ��������
	string DeclarationToAssembler(IT::Entry &lex) {
		string resultStr = "";
		//resultStr += "\n";
		resultStr += lex.id;
		lex.AssemblerName = resultStr;
		resultStr += " ";
		switch (lex.iddatatype)
		{
		case IT::IDDATATYPE::INT:
			resultStr += INT_TYPE;
			resultStr += " ";
			resultStr += to_string(lex.value.vint);
			break;
		case IT::IDDATATYPE::STR:
			resultStr += STR_TYPE;
			resultStr += " ";
			resultStr += "\"";
			resultStr += lex.value.vstr.str;
			resultStr += "\"";
			resultStr += END_OF_STR;
			break;
		default:
			break;
		}
		resultStr += "\n";
		return resultStr;
	}//������� ���������� ���������� � ��������

	struct Block {
		string name = "";
		int countOfElses= 0;
	};

	std::string FunctionToAssembler(LT::LexTable &lexTable, IT::IdTable &idTable, int index, IT::FuncDefenition &func){
		std::string str = "\n";
		str += COMMENT;
		str += "--------function--------";
		str += idTable.funcs[index]->name;
		str += "\n";
		str += idTable.funcs[index]->name;
		str += PROC_START;
		str += "	";
		//��������� �������
		for (int i = 0; i < func.curParams; i++) {
			if (i != 0) {
				str+=", ";
			}
			str += IT::GetEntry(idTable, func.params[i])->AssemblerName;
			str += ": ";
			switch (IT::GetEntry(idTable, func.params[i])->iddatatype) {
				case IT::IDDATATYPE::INT:str += INT_TYPE; break;
				case IT::IDDATATYPE::CHAR:str += STR_TYPE; break;
				case IT::IDDATATYPE::STR:str += PTR; str += STR_TYPE; break;
				case IT::IDDATATYPE::ARRAY:str+=PTR; str += STR_TYPE; break;
				case IT::IDDATATYPE::ARRAY_STR:str += PTR; str += STR_TYPE; break;
			}
		}
		str+="\n";
		//��������� ���������� �������
		for (int i = 0; i < func.curLocals; i++) {
			IT::Entry *it = IT::GetEntry(idTable, func.locals[i]);
			str+=LOCALS;
			str +="	";
			str += it->AssemblerName;
			switch (it->iddatatype) {
			case IT::IDDATATYPE::INT:
				str += " : "; 
				str += INT_TYPE; break;
			case IT::IDDATATYPE::CHAR:
				str += " : ";
				str += STR_TYPE; break;
			case IT::IDDATATYPE::STR:
				//���������� �����
				str += ARRAY_NAME;
				str += ARRAY1;
				str += to_string(TI_STR_MAXSIZE +1);
				str += ARRAY2;
				str += " : ";
				str += STR_TYPE; 
				
				//���������� ���������� �� ���
				str+="\n";
				str += LOCALS;
				str += "	";
				str += it->AssemblerName;
				str += " : ";
				str += PTR;
				str += " ";
				str += STR_TYPE; break;
			case IT::IDDATATYPE::ARRAY:
				//���������� �����
				str += ARRAY_NAME;
				str += ARRAY1;
				str += to_string(it->value.vint);
				str += ARRAY2;
				str += " : ";
				str += INT_TYPE; 
				
				//���������� ���������� �� ���
				str += "\n";
				str += LOCALS;
				str += "	";
				str += it->AssemblerName;
				str += " : ";
				str += PTR;
				str += " ";
				str += INT_TYPE; break;
			case IT::IDDATATYPE::ARRAY_STR:
				//���������� �����
				str += ARRAY_NAME;
				str += ARRAY1;
				str += to_string(it->value.vint);
				str += ARRAY2;
				str += " : ";
				str += STR_TYPE;

				//���������� ���������� �� ���
				str += "\n";
				str += LOCALS;
				str += "	";
				str += it->AssemblerName;
				str += " : ";
				str += PTR;
				str += " ";
				str += STR_TYPE; break;
			}
			str+="\n";
		}

		//���������� ����������� ����� �����
		for (int i = 0; i < func.curLocals; i++) {
			IT::Entry *it = IT::GetEntry(idTable, func.locals[i]);
			if (it->iddatatype == IT::IDDATATYPE::ARRAY
				|| it->iddatatype == IT::IDDATATYPE::ARRAY_STR
				|| it->iddatatype == IT::IDDATATYPE::STR) {
				//����������� ����� ����� � ���
				str += LEA;
				str += it->AssemblerName;
				str += ARRAY_NAME;
				str += "\n";
				//�������� ����� ����� �� ���������
				str+=ADDR_TO_PTR;
				str+= it->AssemblerName;
				str+=", eax";
				str += "\n";
			}
		}

		//���������� �������� ����������
		int s = 8;	//������ �� ebp
		for (int i = 0; i < func.curParams; i++) {
			switch (IT::GetEntry(idTable, func.params[i])->iddatatype) {
				case IT::IDDATATYPE::INT:
					str += "mov	ax,	[ebp+";
					str += to_string(s);
					str += "]\n";
					str += "mov	";
					str += IT::GetEntry(idTable, func.params[i])->AssemblerName;
					str += ", ax\n";
					s += 4;
					break;
				case IT::IDDATATYPE::CHAR:
					str += "mov	al,	[ebp+";
					str += to_string(s);
					str += "]\n";
					str += "mov	";
					str += IT::GetEntry(idTable, func.params[i])->AssemblerName;
					str += ", al\n";
					s += 4;
					break;
				case IT::IDDATATYPE::STR:
					str += "mov	eax,	[ebp+";
					str += to_string(s);
					str += "]\n";
					str += "mov	";
					str += IT::GetEntry(idTable, func.params[i])->AssemblerName;
					str += ", eax\n";
					s += 4;
					break;
				case IT::IDDATATYPE::ARRAY:
					str += "mov	eax,	[ebp+";
					str += to_string(s);
					str += "]\n";
					str += "mov	";
					str += IT::GetEntry(idTable, func.params[i])->AssemblerName;
					str += ", eax\n";
					s += 4;
					break;
				case IT::IDDATATYPE::ARRAY_STR:
					str += "mov	eax,	[ebp+";
					str += to_string(s);
					str += "]\n";
					str += "mov	";
					str += IT::GetEntry(idTable, func.params[i])->AssemblerName;
					str += ", eax\n";
					s += 4;
					break;
			}
		}
		

		int i = idTable.funcs[index]->startOfFunc;
		std::string curLine = "";	//����� ����, �� ����� ������������� �� ��������
		std::string translatedText = "";	//������������� ���

		stack<Block> Blocks;		//���� � if-��
		int countOfConditions = 0;//��������� if-a�
		int countOfCycles = 0;//��������� while-�

		//��������� ����
		i+=2;
		while (LT::GetEntry(lexTable, i)->lexema[0] != 'f' 
				&& LT::GetEntry(lexTable, i)->lexema[0] != 'm' 
				&& i < lexTable.size) 
		{
			curLine = "\n";
			curLine += COMMENT;
			translatedText = "\n";
			//������� ������� �����
			if ((LT::GetEntry(lexTable, i)->lexema[0] == 'i'	//(i||l)&&(i||l||@||r)||@||(
			|| LT::GetEntry(lexTable, i)->lexema[0] == 'l')	//������� ������ �� ����� ���������
			&& (LT::GetEntry(lexTable, i + 1)->lexema[0] == 'l'
				|| LT::GetEntry(lexTable, i + 1)->lexema[0] == 'i'
				|| LT::GetEntry(lexTable, i + 1)->lexema[0] == SPEC_SUMBOL
				|| LT::GetEntry(lexTable, i + 1)->lexema[0] == LEX_RETURN
				|| LT::GetEntry(lexTable, i + 1)->lexema[0] == SPEC_ARR_SUMBOL)
			|| LT::GetEntry(lexTable, i)->lexema[0] == SPEC_SUMBOL
			|| LT::GetEntry(lexTable, i)->lexema[0] == LEX_RIGHTHESIS)
			{
				LT::Entry *lex = LT::GetEntry(lexTable, i);
				IT::Entry *lastAssigned = NULL;	//��� ������ ���������� �����, ��� ������, � �� ��� ����������� �����
				stack<IT::Entry*> lastArrays;
				bool firstSymb = true;
				while (lex->lexema[0] != ';'
					&& lex->lexema[0] != 'f'
					&& lex->lexema[0] != 'm'
					&& lex->lexema[0] != FILL_SYMBOL
					&& lex->lexema[0] != LEX_LEFTHESIS
					&& i < lexTable.size)
				{
					//���� ������ ������, ��� ����������
					if (lex->lexema[0] == 'v') {
						curLine += char(lex->idxTI);
					}
					else if (lex->lexema[0] == LEX_RETURN) {
						curLine += LEX_RETURN;
					}
					else {
						curLine += lex->lexema[0];
					}
					//�������� �������
					//�������� ��������
					if (lex->lexema[0] == 'v') {
						switch (lex->idxTI) {
							case (int)('+') : translatedText += EXPR_SUM; break;
							case (int)('-') : translatedText += EXPR_SUB; break;
							case (int)('*') : translatedText += EXPR_IMUL; break;
							case (int)('/') : translatedText += EXPR_DIV; break;
							case (int)('>') : translatedText += IF_LARGER; break;
							case (int)('<') : translatedText += IF_LESS; break;
							case (int)('?') : translatedText += IF_EQUALS; break;
							case (int)('!') : translatedText += IF_NOT_EQUALS; break;
							case (int)('&') : translatedText += IF_AND; break;
							case (int)('|') : translatedText += IF_OR; break;
							case (int)('^') :{
								if (lastArrays.top()->iddatatype == IT::ARRAY) {
									translatedText += PUSH_VALUE; break;
								} else if (lastArrays.top()->iddatatype == IT::STR) {
									translatedText += PUSH_VALUE; break;
								}
								lastArrays.pop();
							}
						}
					}
					else if (lex->lexema[0] == 'i') {
						//��� ��-��� ������������� ��������, � ���� ����������� ��� �����
						if (lex->isAssignedId) {
							lastAssigned = IT::GetEntry(idTable, lex->idxTI);
							translatedText += LEA;
							translatedText += (IT::GetEntry(idTable, lex->idxTI))->AssemblerName;
							translatedText += "\n";
							translatedText += EXPR_PUSH_EAX;
						}//����� � ���� ����������� ��� ��������
						else {
							if (IT::GetEntry(idTable, lex->idxTI)->iddatatype == IT::INT) {
								translatedText += "movzx	eax, ";
								translatedText += (IT::GetEntry(idTable, lex->idxTI))->AssemblerName;
								translatedText += "\npush	eax";
							} else if (IT::GetEntry(idTable, lex->idxTI)->iddatatype == IT::CHAR) {
								translatedText += "xor	eax,eax\nmovzx	eax, ";
								translatedText += (IT::GetEntry(idTable, lex->idxTI))->AssemblerName;
								translatedText += "\npush	eax";
							}
						}
					}
					else if (lex->lexema[0] == 'l') {
						if ((IT::GetEntry(idTable, lex->idxTI))->iddatatype == IT::IDDATATYPE::CHAR) {
							translatedText += "movzx	eax, ";
							translatedText += (IT::GetEntry(idTable, lex->idxTI))->AssemblerName;
							translatedText += "\npush	eax";
						}
						else if ((IT::GetEntry(idTable, lex->idxTI))->iddatatype == IT::IDDATATYPE::STR) {
							translatedText += "lea	eax, ";
							translatedText += (IT::GetEntry(idTable, lex->idxTI))->AssemblerName;
							translatedText += "\npush	eax";
						}
						else {
							translatedText += "movzx	eax, ";
							translatedText += (IT::GetEntry(idTable, lex->idxTI))->AssemblerName;
							translatedText += "\npush	eax";
						}
					}
					else if (lex->lexema[0] == LEX_EQUALS) {
						if (lastAssigned!=NULL){
							if (lastAssigned->iddatatype == IT::IDDATATYPE::INT) {
								translatedText += EXPR_EQU;
							}
							else if (lastAssigned->iddatatype == IT::IDDATATYPE::ARRAY) {
								translatedText += EXPR_EQU;
							}
							else if (lastAssigned->iddatatype == IT::IDDATATYPE::STR) {
								translatedText += EXPR_EQU_STR;
							}
							else if (lastAssigned->iddatatype == IT::IDDATATYPE::CHAR) {
								translatedText += EXPR_EQU_STR;
							}
						}
						else if (lastArrays.empty() == false) {
							if (lastArrays.top()->iddatatype == IT::IDDATATYPE::INT) {
								translatedText += EXPR_EQU;
							}
							else if (lastArrays.top()->iddatatype == IT::IDDATATYPE::ARRAY) {
								translatedText += EXPR_EQU;
							}
							else if (lastArrays.top()->iddatatype == IT::IDDATATYPE::STR) {
								translatedText += EXPR_EQU_STR;
							}
							else if (lastArrays.top()->iddatatype == IT::IDDATATYPE::CHAR) {
								translatedText += EXPR_EQU_STR;
							}
						}
						//translatedText += (IT::GetEntry(idTable, lex->idxTI))->AssemblerName;
					}
					else if (lex->lexema[0] == LEX_RETURN) {
						if (func.returnType == IT::IDDATATYPE::VOID) {
							translatedText += "ret	";
							translatedText += to_string(func.curParams * 4);
						}else{
							translatedText += EXPR_RETURN;
							translatedText += to_string(func.curParams * 4);
						}
						break;
					}
					else if (lex->lexema[0] == SPEC_SUMBOL) {
						translatedText += EXPR_CALL;
						translatedText += IT::GetEntry(idTable, lex->idxTI)->funcId->name;
						translatedText += "\n";
						translatedText += EXPR_PUSH_EAX;
					}
					else if (lex->lexema[0] == SPEC_ARR_SUMBOL) {	//��� ����� ����� � ����� ������� 
						if (lex->isAssignedId) {
							lastAssigned = IT::GetEntry(idTable, lex->idxTI);
						}
						lastArrays.push(IT::GetEntry(idTable, lex->idxTI));
						translatedText += "xor	eax, eax\n";
						translatedText += "pop	eax\n";
						translatedText += "imul	eax,";
						switch ((IT::GetEntry(idTable, lex->idxTI))->iddatatype) {
							case IT::IDDATATYPE::ARRAY: {
								translatedText += "2";
								break;
							}
							case IT::IDDATATYPE::ARRAY_STR: {
								translatedText += "256";
								break;
							}
							case IT::IDDATATYPE::STR: {
								translatedText += "1";
								break;
							}
						}
						translatedText += "\nmov ebx,";
						translatedText += (IT::GetEntry(idTable, lex->idxTI))->AssemblerName;
						translatedText += "\nadd	eax, ebx\n";
						translatedText += "push	eax";
					}
					else {
					}
					translatedText += "\n";
					i++;
					lex = LT::GetEntry(lexTable, i);
					firstSymb=false;
				}
				str += COMMENT;
				if (lex->lexema[0] == LEX_LEFTHESIS) {
					curLine+=')';
				}
				str += curLine;
				str += translatedText;
			}
			else if (LT::GetEntry(lexTable, i)->lexema[0] == LEX_CONDITION	//...if
			&& LT::GetEntry(lexTable, i - 1)->lexema[0] != LEX_ELSE) {
				str+=COMMENT;
				str+="if\n";
				Block bl = Block();
				bl.name = IF_METKA;
				bl.name += func.name;
				bl.name += to_string(countOfConditions++);
				Blocks.push(bl);
			}
			else if (LT::GetEntry(lexTable, i)->lexema[0] == LEX_CYCLE) {//...while
				str += COMMENT;
				str += "while\n";
				Block bl = Block();
				bl.name = WHILE_METKA;
				bl.name += func.name;
				bl.name += to_string(countOfCycles++);
				bl.countOfElses=-1;
				str += bl.name;
				str += METKA;
				str += "\n";
				Blocks.push(bl);
			}  
			else if (LT::GetEntry(lexTable, i)->lexema[0] == LEX_ELSE
			&& LT::GetEntry(lexTable, i+1)->lexema[0] == LEX_CONDITION) {	//else if...
				str += Blocks.top().name;
				str += ELSE_METKA;
				str += to_string(Blocks.top().countOfElses++);
				str += METKA;
				str += "\n";
			}
			else if (LT::GetEntry(lexTable, i)->lexema[0] == LEX_ELSE) {	//else...
				str += Blocks.top().name;
				str += ELSE_METKA;
				str += to_string(Blocks.top().countOfElses++);
				str += METKA;
				str += "\n";
			}
			else if (LT::GetEntry(lexTable, i)->lexema[0] == LEX_LEFTBRACE		//...{
			&& LT::GetEntry(lexTable, i - 1)->lexema[0] != LEX_ELSE
			&&	!Blocks.empty()) {
				if (Blocks.top().countOfElses == -1)	//��� ������ ��� ��� while
				{
					str += MOVE_TO_IF;
					str += Blocks.top().name;
					str += FINALLY;
					str += "\n";
				}else {
					str += MOVE_TO_IF;
					str += Blocks.top().name;
					str += ELSE_METKA;
					str += to_string(Blocks.top().countOfElses);
					str += "\n";
				}
			}
			else if (LT::GetEntry(lexTable, i)->lexema[0] == LEX_BRACELET	//}else
				&& LT::GetEntry(lexTable, i + 1)->lexema[0] == LEX_ELSE) {
				str += MOVE_TO;
				str += Blocks.top().name;
				str += FINALLY;
				//str += to_string(Blocks.top().countOfElses);
				str += "\n";
			}
			else if (LT::GetEntry(lexTable, i)->lexema[0] == LEX_BRACELET	//}...
				&& LT::GetEntry(lexTable, i + 1)->lexema[0] != LEX_ELSE) {
				if (Blocks.empty()) {
					break;
				}
				if (Blocks.top().countOfElses != -1)	//��� ������ ��� ��� if
				{
					str += Blocks.top().name;
					str += ELSE_METKA;
					str += to_string(Blocks.top().countOfElses);
					str += METKA;
					str += "\n";
				}
				else {
					str+=MOVE_TO;
					str+= Blocks.top().name;
					str += "\n";
				}
				str += Blocks.top().name;
				str += FINALLY;
				str += METKA;
				str += "\n";
				Blocks.pop();
			}
			i++;
		}
		str += "\n";
		str += EXPR_END_OF_FUNCTION;
		str += "\n";
		str += idTable.funcs[index]->name;

		str += PROC_END;
		return str;
	}	//��������� ������� � ��������

	bool DoesExprHaveEquals(LT::LexTable &lexTable, IT::IdTable &idTable, int index)	//�� ��� ����� �������� ������������
	{
		LT::Entry* lex = LT::GetEntry(lexTable,index);
		do {
			if (lex->lexema[0] == LEX_EQUALS) {
				return true;
			}
			lex = LT::GetEntry(lexTable, ++index);
		}while(lex->lexema[0]!=LEX_SEMICOLON2);
		return false;
	}
	std::string NameToAssemblerName(IT::Entry &id){
		string res = id.id;
		if (id.idtype != IT::IDTYPE::F) {
			res += id.funcId->name;
		}
		id.AssemblerName = res;
		return res;
	}	//��� ���������� � ����������� ���
}