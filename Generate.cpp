#include "Generate.h"
#define FUNC_SEPARATOR "------------------------------------------"
#define COMMENT ";"

using namespace std;
namespace GEN {
	string Generator::Generate(IT::IdTable &it, LT::LexTable &lt, char *text) {
		string resultStr = "";//куды зап≥сваецца вын≥к працы
		resultStr += CONST_SECTION;
		resultStr += "\n";
		int i;
		//секцы€ канстант
		for (i = 0; i < it.size; i++) {
			if (IT::GetEntry(it, i)->idtype!=IT::IDTYPE::L)
				break;
			resultStr+=DeclarationToAssembler(*IT::GetEntry(it, i), CONST_NAME + to_string(i));
			resultStr += "\n";
		}	 

		resultStr += DATA_SECTION;

		resultStr += "\n\n";
		//секцы€ дадзеных
		for (i = 0; i < it.funcCount; i++) {
			resultStr += COMMENT;
			resultStr += FUNC_SEPARATOR;
			resultStr += "Function: ";
			resultStr += it.funcs[i]->name;
			//resultStr += FUNC_SEPARATOR;
			resultStr += "\n";

			//секцы€ параметраҐ
			resultStr += COMMENT;
			resultStr += FUNC_SEPARATOR;
			resultStr += "parametars";
			resultStr += "\n";
			//resultStr += FUNC_SEPARATOR;
			//параметры функцы≥
			for (int j = 0; j < it.funcs[i]->curParams; j++) {
				resultStr += it.funcs[i]->name + "_";
				resultStr += DeclarationToAssembler(*IT::GetEntry(it, it.funcs[i]->params[j]));
				//resultStr += "\n";
			}

			resultStr += "\n";
			//секцы€ лакальных зьменных
			resultStr += COMMENT;
			resultStr += FUNC_SEPARATOR;
			resultStr += "locals";
			resultStr += "\n";
			//resultStr += FUNC_SEPARATOR;
			//лакальны€ пераменны€
			for (int j = 0; j < it.funcs[i]->curLocals; j++) {
				resultStr += it.funcs[i]->name + "_";
				resultStr += DeclarationToAssembler(*IT::GetEntry(it,it.funcs[i]->locals[j]));
				//resultStr += "\n";
			}
			resultStr += "\n";
		}


		//польска€ натацы€
		for (i = 1; i < lt.size; i++) {
			if ((LT::GetEntry(lt, i+1)->lexema[0] == LEX_EQUALS) ||
				(LT::GetEntry(lt, i)->lexema[0] == LEX_ID && LT::GetEntry(lt, i-1)->lexema[0] != LEX_FUNCTION && IT::GetEntry(it, LT::GetEntry(lt, i)->idxTI)->idtype==IT::IDTYPE::F)
			) {
				Poland::PolishNotation(&i, &lt, &it, text);
			}
		}
		resultStr+=CODE_SECTION;

		//генерацы€ секцы≥ кода
		for (int i = 0; i < it.funcCount; i++) {
			resultStr+=FunctionToAssembler(lt,it,i, *(it.funcs[i]));
		}
		std::cout << resultStr;

		return resultStr;
	}	//атрмаць радок асм файла
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
	}//зап≥саць дыкл€рацыю пераменнай у асэмблер
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
	}//зап≥саць дыкл€рацыю пераменнай у асэмблер

	std::string FunctionToAssembler(LT::LexTable &lexTable, IT::IdTable &idTable, int index, IT::FuncDefenition &func){
		std::string str = "\n";
		str += COMMENT;
		str += "--------function--------";
		str += idTable.funcs[index]->name;
		str += "\n";
		str += idTable.funcs[index]->name;
		str += PROC_START;
		int i= idTable.funcs[index]->startOfFunc;
		std::string curLine = "";	//радок кода, €к≥ зараз перакладаецца на асэмблер
		std::string translatedText="";		//пераклдадзены кот
		string NameOfVarTOEqual="";//	≥м€ пераменнай, €кой прысвойваецца значэнне
		do {
			curLine="\n";
			curLine+=COMMENT;
			translatedText="\n";
			//пачатак разбору радка
			if ((LT::GetEntry(lexTable, i)->lexema[0] == 'i' 
				|| LT::GetEntry(lexTable, i)->lexema[0] == 'l')	//пачатак выразу €к≥ трэба разабраць
				&& (LT::GetEntry(lexTable, i + 1)->lexema[0] == 'l'
					|| LT::GetEntry(lexTable, i + 1)->lexema[0] == 'i'
					|| LT::GetEntry(lexTable, i + 1)->lexema[0] == SPEC_SUMBOL)
				|| LT::GetEntry(lexTable, i)->lexema[0] == SPEC_SUMBOL
				|| LT::GetEntry(lexTable, i - 1)->lexema[0] == LEX_RETURN) {
				NameOfVarTOEqual = IT::GetEntry(idTable,(LT::GetEntry(lexTable, i)->idxTI))->AssemblerName;
				do {
					//зап≥с лексем выразу, дл€ каментары€
					if (LT::GetEntry(lexTable, i)->lexema[0] == 'v') {
						curLine += char(LT::GetEntry(lexTable, i)->idxTI);
					}
					else {
						curLine += LT::GetEntry(lexTable, i)->lexema[0];
					}
					//curLine+="\n";

					if (LT::GetEntry(lexTable, i)->lexema[0] == 'v') {
						switch (LT::GetEntry(lexTable, i)->idxTI) {
							case (int)('+') : translatedText += EXPR_SUM; break;
								case (int)('-') : translatedText += EXPR_SUB; break;
									case (int)('*') : translatedText += EXPR_IMUL; break;
										case (int)('/') : translatedText += EXPR_DIV; break;
						}
					}
					else if (LT::GetEntry(lexTable, i)->lexema[0] == '=') {
						translatedText += EXPR_EQU;
						translatedText += NameOfVarTOEqual;
						translatedText += "\n";
						translatedText += EXPR_CLEAR_STACK;
					}
					else if (LT::GetEntry(lexTable, i)->lexema[0] == 'i') {
						translatedText += EXPR_INT;
						translatedText += (IT::GetEntry(idTable, LT::GetEntry(lexTable, i)->idxTI))->AssemblerName;
					}
					else if (LT::GetEntry(lexTable, i)->lexema[0] == 'l') {
						translatedText += EXPR_INT;
						translatedText += (IT::GetEntry(idTable, LT::GetEntry(lexTable, i)->idxTI))->AssemblerName;
					}
					else if (LT::GetEntry(lexTable, i)->lexema[0] == SPEC_SUMBOL) {
						translatedText += EXPR_CALL;
						translatedText += IT::GetEntry(idTable, LT::GetEntry(lexTable, i)->idxTI)->funcId->name;
					}
					else {
					}
					translatedText+="\n";
					i++;
				} while (LT::GetEntry(lexTable, i)->lexema[0] != ';' && LT::GetEntry(lexTable, i)->lexema[0] != 'f' && LT::GetEntry(lexTable, i)->lexema[0] != 'm');
				str += COMMENT;
				str += curLine;
				str += translatedText;
			}
			i++;
		} while (LT::GetEntry(lexTable, i)->lexema[0] != 'f' && LT::GetEntry(lexTable, i)->lexema[0] != 'm');
		str+="\n";
		str += idTable.funcs[index]->name;
		str += PROC_END;
		return str;
	}	//генерацы€ функцы≥ Ґ асэмблер

}