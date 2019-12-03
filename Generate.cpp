#include "Generate.h"
#define FUNC_SEPARATOR "------------------------------------------"
#define COMMENT ";"

using namespace std;
namespace GEN {
	string Generator::Generate(IT::IdTable &it, LT::LexTable &lt, char *text) {
		string resultStr = "";//куды зап≥сваецца вын≥к працы
		resultStr += START_OF_ASM;
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

		//перавесц≥ ≥мЄны ≥д-аҐ у асэмблерны€ ≥мЄны
		for (; i < it.size; i++) {
			NameToAssemblerName(*IT::GetEntry(it, i));
		}

		//resultStr += DATA_SECTION;

		//resultStr += "\n\n";
		//секцы€ дадзеных
		for (i = 0; false && i < it.funcCount; i++) {
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
			else if (i>2 && LT::GetEntry(lt, i - 1)->lexema[0] == LEX_RIGHTHESIS
			&& (LT::GetEntry(lt, i - 2)->lexema[0] == LEX_CONDITION || LT::GetEntry(lt, i - 2)->lexema[0] == LEX_CYCLE)) {
				Poland::PolishNotation(&i, &lt, &it, text);
			}
			else if (LT::GetEntry(lt, i)->lexema[0] == LEX_RETURN && LT::GetEntry(lt, i+1)->lexema[0] != LEX_SEMICOLON2) {
				Poland::PolishNotation(&i, &lt, &it, text);
			}
		}

		cout << LT::PrintTable(lt);
		resultStr+=CODE_SECTION;
		//генерацы€ секцы≥ кода
		for (int i = 0; i < it.funcCount; i++) {
			resultStr+=FunctionToAssembler(lt,it,i, *(it.funcs[i]));
			//std::cout << FunctionToAssembler(lt, it, i, *(it.funcs[i]));
		}
		//std::cout << resultStr;
		resultStr+="\n";
		resultStr+= MAIN_ASM_FUNC;
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
		//параметры фукнцы≥
		for (int i = 0; i < func.curParams; i++) {
			if (i != 0) {
				str+=", ";
			}
			str += IT::GetEntry(idTable, func.params[i])->AssemblerName;
			str += ": ";
			switch (IT::GetEntry(idTable, func.params[i])->iddatatype) {
				case IT::IDDATATYPE::INT:str += INT_TYPE; break;
				case IT::IDDATATYPE::STR:str += STR_TYPE; break;
			}
		}
		str+="\n";
		//лакальны€ пераменный функцы≥
		for (int i = 0; i < func.curLocals; i++) {
			str+=LOCALS;
			str +="	";
			str += IT::GetEntry(idTable, func.locals[i])->AssemblerName;
			str +=" : ";
			switch (IT::GetEntry(idTable, func.locals[i])->iddatatype) {
			case IT::IDDATATYPE::INT:str += INT_TYPE; break;
			case IT::IDDATATYPE::STR:str += STR_TYPE; break;
			}
			str+="\n";
		}

		int i= idTable.funcs[index]->startOfFunc;
		std::string curLine = "";	//радок кода, €к≥ зараз перакладаецца на асэмблер
		std::string translatedText = "";	//пераклдадзены кот
		stack<string> NamesOfVarsToEqual;	//≥мЄны пераменных, €к≥м парысвайваецца значэнне

		stack<Block> Blocks;		//стэк з if-ам≥
		int countOfConditions = 0;//колькасць if-aҐ
		int countOfCycles = 0;//колькасць while-аҐ

		//генерацы€ кода
		i++;
		while (LT::GetEntry(lexTable, i)->lexema[0] != 'f' 
				&& LT::GetEntry(lexTable, i)->lexema[0] != 'm' 
				&& i < lexTable.size) 
		{
			curLine = "\n";
			curLine += COMMENT;
			translatedText = "\n";
			//пачатак разбору радка
			if ((LT::GetEntry(lexTable, i)->lexema[0] == 'i'	//(i||l)&&(i||l||@)||@
			|| LT::GetEntry(lexTable, i)->lexema[0] == 'l')	//пачатак выразу €к≥ трэба разабраць
			&& (LT::GetEntry(lexTable, i + 1)->lexema[0] == 'l'
				|| LT::GetEntry(lexTable, i + 1)->lexema[0] == 'i'
				|| LT::GetEntry(lexTable, i + 1)->lexema[0] == SPEC_SUMBOL
				|| LT::GetEntry(lexTable, i + 1)->lexema[0] == LEX_RETURN)
			|| LT::GetEntry(lexTable, i)->lexema[0] == SPEC_SUMBOL
			|| LT::GetEntry(lexTable, i)->lexema[0] == LEX_RIGHTHESIS)
			{
				if (LT::GetEntry(lexTable, i)->lexema[0] == 'i'){
					NamesOfVarsToEqual.push(IT::GetEntry(idTable, (LT::GetEntry(lexTable, i)->idxTI))->AssemblerName);
				}
				while (LT::GetEntry(lexTable, i)->lexema[0] != ';'
					&& LT::GetEntry(lexTable, i)->lexema[0] != 'f'
					&& LT::GetEntry(lexTable, i)->lexema[0] != 'm'
					&& LT::GetEntry(lexTable, i)->lexema[0] != FILL_SYMBOL
					&& LT::GetEntry(lexTable, i)->lexema[0] != LEX_LEFTHESIS
					&& i < lexTable.size)
				{
					//зап≥с лексем выразу, дл€ каментары€
					if (LT::GetEntry(lexTable, i)->lexema[0] == 'v') {
						curLine += char(LT::GetEntry(lexTable, i)->idxTI);
					}
					else if (LT::GetEntry(lexTable, i)->lexema[0] == LEX_RETURN) {
						curLine += LEX_RETURN;
					}
					else {
						curLine += LT::GetEntry(lexTable, i)->lexema[0];
					}
					//апрацоҐка лексемы
					//апрацоҐка аперацы≥
					if (LT::GetEntry(lexTable, i)->lexema[0] == 'v') {
						switch (LT::GetEntry(lexTable, i)->idxTI) {
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
						}
					}
					else if (LT::GetEntry(lexTable, i)->lexema[0] == '=') {
						translatedText += EXPR_EQU;
						translatedText += NamesOfVarsToEqual.top();
						NamesOfVarsToEqual.pop();
					}
					else if (LT::GetEntry(lexTable, i)->lexema[0] == 'i') {
						translatedText += EXPR_INT;
						translatedText += (IT::GetEntry(idTable, LT::GetEntry(lexTable, i)->idxTI))->AssemblerName;
					}
					else if (LT::GetEntry(lexTable, i)->lexema[0] == 'l') {
						translatedText += EXPR_INT;
						translatedText += (IT::GetEntry(idTable, LT::GetEntry(lexTable, i)->idxTI))->AssemblerName;
					}
					else if (LT::GetEntry(lexTable, i)->lexema[0] == LEX_RETURN) {
						translatedText += EXPR_RETURN;
					}
					else if (LT::GetEntry(lexTable, i)->lexema[0] == SPEC_SUMBOL) {
						translatedText += EXPR_CALL;
						translatedText += IT::GetEntry(idTable, LT::GetEntry(lexTable, i)->idxTI)->funcId->name;
						translatedText += "\n";
						translatedText += EXPR_PUSH_EAX;
					}
					else {
					}
					translatedText += "\n";
					i++;
				}
				str += COMMENT;
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
				if (Blocks.top().countOfElses == -1)	//кал≥ ц€кучы блЄк быҐ while
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
				if (Blocks.top().countOfElses != -1)	//кал≥ ц€кучы блЄк быҐ if
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
	}	//генерацы€ функцы≥ Ґ асэмблер


	std::string NameToAssemblerName(IT::Entry &id){
		string res = id.id;
		if (id.idtype != IT::IDTYPE::F) {
			res += id.funcId->name;
		}
		id.AssemblerName = res;
		return res;
	}	//≥м€ пераменнай у асэмблернае ≥м€
}