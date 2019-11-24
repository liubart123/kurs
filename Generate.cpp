#include "Generate.h"
#define FUNC_SEPARATOR "------------------------------------------"
#define COMMENT ";"

using namespace std;
namespace GEN {
	string Generator::Generate(IT::IdTable &it, LT::LexTable &lt, char *text) {
		string resultStr = "";
		resultStr += CONST_SECTION;
		resultStr += "\n";
		int i;
		//������ ��������
		for (i = 0; i < it.size; i++) {
			if (it.table[i].idtype!=IT::IDTYPE::L)
				break;
			resultStr+=DeclarationToAssembler(it.table[i], CONST_NAME + to_string(i));
			resultStr += "\n";
		}	 

		resultStr += DATA_SECTION;

		resultStr += "\n\n";
		//������ ��������
		for (i = 0; i < it.funcCount; i++) {
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
				resultStr += DeclarationToAssembler(it.table[it.funcs[i]->params[j]]);
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
				resultStr += DeclarationToAssembler(it.table[it.funcs[i]->locals[j]]);
				//resultStr += "\n";
			}
			resultStr += "\n";
		}


		//�������� �������
		for (i = 0; i < lt.size; i++) {
			if (LT::GetEntry(lt, i)->lexema[0] == LEX_SEMICOLON || LT::GetEntry(lt, i)->lexema[0] == LEX_LEFTBRACE) {
				i++;
				if (i >= lt.size) {
					break;
				}
				int pos = LT::GetEntry(lt, i)->sn;
				Poland::PolishNotation(&i, &lt, &it, text);
			}
		}


		return resultStr;
	}	//������� ����� ��� �����
	Generator::Generator() {

	}

	string DeclarationToAssembler(IT::Entry lex, string name) {
		string resultStr="";
		//resultStr += "\n";
		resultStr = name;
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
	}//������� ���������� ���������� � ��������
	string DeclarationToAssembler(IT::Entry lex) {
		string resultStr = "";
		//resultStr += "\n";
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
}