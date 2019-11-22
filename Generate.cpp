#include "Generate.h"
#define FUNC_SEPARATOR "------------------------------------------"
#define COMMENT ";"

using namespace std;
namespace GEN {
	string Generator::Generate(IT::IdTable it, LT::LexTable lt) {
		string resultStr = "";
		resultStr += CONST_SECTION;
		int i;
		//секцы€ канстант
		for (i = 0; i < it.size; i++) {
			if (it.table[i].idtype!=IT::IDTYPE::L)
				break;
			resultStr+=DeclarationToAssembler(it.table[i], CONST_NAME + to_string(i));
			/*if (it.table[i].idtype == IT::IDTYPE::L) {
				resultStr += "\n";
				resultStr += CONST_NAME;
				resultStr += to_string(i);
				resultStr += " ";
				switch (it.table[i].iddatatype)
				{
				case IT::IDDATATYPE::INT:
					resultStr += INT_TYPE;
					resultStr += " ";
					resultStr += to_string(it.table[i].value.vint);
					break;
				case IT::IDDATATYPE::STR:
					resultStr += STR_TYPE;
					resultStr += " ";
					resultStr += it.table[i].value.vstr.str;
					resultStr += END_OF_STR;
					break;
				default:
					break;
				}
				resultStr += "\n";

			}
			else {
				break;
			}*/
		}	 

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
			//resultStr += FUNC_SEPARATOR;
			//параметры функцы≥
			for (int j = 0; j < it.funcs[i]->curParams; j++) {
				resultStr += DeclarationToAssembler(it.table[it.funcs[i]->params[j]]);
			}

			resultStr += "\n";
			//секцы€ лакальных зьменных
			resultStr += COMMENT;
			resultStr += FUNC_SEPARATOR;
			resultStr += "locals";
			//resultStr += FUNC_SEPARATOR;
			//лакальны€ пераменны€
			for (int j = 0; j < it.funcs[i]->curLocals; j++) {
				resultStr += DeclarationToAssembler(it.table[it.funcs[i]->locals[j]]);
			}
			resultStr += "\n";
		}

		return resultStr;
	}	//атрмаць радок асм файла
	Generator::Generator() {

	}

	string DeclarationToAssembler(IT::Entry lex, string name) {
		string resultStr="";
		if (lex.idtype == IT::IDTYPE::L) {
			resultStr += "\n";
			resultStr += name;
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
				resultStr += lex.value.vstr.str;
				resultStr += END_OF_STR;
				break;
			default:
				break;
			}
			resultStr += "\n";
		}
		else {
			resultStr += "\n";
			resultStr += name;
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
				resultStr += lex.value.vstr.str;
				resultStr += END_OF_STR;
				break;
			default:
				break;
			}
			resultStr += "\n";
		}
		return resultStr;
	}//зап≥саць дыкл€рацыю пераменнай у асэмблер
	string DeclarationToAssembler(IT::Entry lex) {
		string resultStr = "";
		if (lex.idtype == IT::IDTYPE::L) {
			resultStr += "\n";
			resultStr += lex.id;
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
				resultStr += lex.value.vstr.str;
				resultStr += END_OF_STR;
				break;
			default:
				break;
			}
			resultStr += "\n";
		}
		else {
			resultStr += "\n";
			resultStr += lex.id;
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
				resultStr += lex.value.vstr.str;
				resultStr += END_OF_STR;
				break;
			default:
				break;
			}
			resultStr += "\n";
		}
		return resultStr;
	}//зап≥саць дыкл€рацыю пераменнай у асэмблер
}