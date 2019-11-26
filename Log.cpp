#include "Poland.h"
#include "Log.h"
#include <iostream>
#include <string>
#include "LT.h"
#include "IT.h"
#include <ctime>
#include "Error.h"
#include "MFST.h"
#include "GRB.h"
#include <string>
#include <list>
#include "Generate.h"
namespace Log
{
	bool CheckSepSymb(char c);
	LOG getlog(wchar_t logfile[])
	{
		static ofstream file;
		file.open(logfile);

		if (!file.is_open())
		{
			throw ERROR_THROW(112);
		}

		LOG newLog;
		wcscpy_s(newLog.logfile, logfile);
		newLog.stream = &file;

		return newLog;
	}
	using namespace std;
	void WriteLine(LOG log, char* c, ...)
	{
		//wchar_t string[PARM_MAX_SIZE];
		char output[PARM_MAX_SIZE];
		output[0] = '\0';
		//size_t outSize;

		char** ptr = &c;

		while (*ptr != "")
		{
			//mbstowcs_s(&outSize, string, *ptr, strlen(*ptr));
			char tempString[PARM_MAX_SIZE];
			strcpy_s(tempString, *ptr);
			strcat_s(output, tempString);
			ptr++;
		}

		//wcstombs_s(&outSize, output, log.logfile, wcslen(log.logfile));
		cout << output;

		*log.stream << output;
		//*log.stream << "asd";
	}



	void WriteLog(LOG log)
	{
		time_t current_time;
		tm * timeinfo = new tm;
		char buffer[80];                                // строка, в которой будет хранитьс€ текущее врем€

		time(&current_time);                               // текуща€ дата в секундах
		localtime_s(timeinfo, &current_time);               // текущее локальное врем€, представленное в структуре

		strftime(buffer, 80, "-----Date: %d.%m.%Y %H:%M:%S-----", timeinfo); // форматируем строку времени
		*log.stream << buffer << endl;
	}


	void WriteParm(LOG log, Parm::PARM parm)
	{
		char output[PARM_MAX_SIZE];
		size_t outSize;

		*log.stream << "-----PARAMETRS-----" << endl;

		wcstombs_s(&outSize, output, parm.in, wcslen(parm.in));

		*log.stream << "Input file: " << output << endl;

		wcstombs_s(&outSize, output, parm.out, wcslen(parm.out));

		*log.stream << "Output file: " << output << endl;

		wcstombs_s(&outSize, output, parm.log, wcslen(parm.log));

		*log.stream << "Log file: " << output << endl;
	}

	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "-----»сходные данные-----" << endl;
		*log.stream << "Amount of symbols: " << in.size << endl;
		*log.stream << "Amount of ignored symbols: " << in.ignor << endl;
		*log.stream << "Amount of lines: " << in.lines << endl;
		int pos = 0;
		int startPos = 0;
		int NumberOfLine = 0;
		bool newLine = true;
		int positionInLine = 0;	//нумар калонк≥ у ц€кучым радку
		IT::IdTable idTable = IT::CreateTable(TI_MAXSIZE);
		LT::LexTable lexTable = LT::CreateTable(LT_MAXSIZE);//табл≥ца лексем
		bool kavichka = false;	//ц≥ Єсць адкрываюча€ кавычка
		list<Error::ERROR> errorList;
		//стварэнне табл≥цы лексем
		while (startPos + pos < in.size) {
			if (in.text[pos + startPos] == '\"') {
				kavichka = !kavichka;
			}
			if (CheckSepSymb(in.text[pos + startPos]) && kavichka == false) {
				char lastChar = '\0';	//апошн≥ значымы с≥мвал у радку
				char *tempString = new char[pos + 1];
				int posTemp = 0;
				while (posTemp < pos) {		//слова пам€шчаецца у posTemp
					tempString[posTemp] = in.text[posTemp + startPos];
					if (tempString[posTemp]!=' ' && tempString[posTemp] != '\n' && tempString[posTemp] != '\t')
						lastChar=tempString[posTemp];
					posTemp++;
				}
				tempString[pos] = '\0';	
				if (newLine) {		//новы радок
					string numOfLine = "";
					numOfLine = to_string((int)NumberOfLine);
					WriteLine(log, &(numOfLine[0]), ": ", "");
					NumberOfLine++;
					positionInLine=0;
				}
				if (pos != 0) {		//кал≥ ц€кучае слова не с≥мвал пропуска
					char *result;
					result = CheckWord(tempString);	//праверка на лексемы
					if (result[0] == LEX_ERROR){
						result = IdKA(tempString);//праверка на ≥дынтыф≥катар
						if (result[0] == LEX_ERROR) {
							result = StrLiKA(tempString);//праверка на л≥тэрал радковы
							if (result[0] == LEX_ERROR) {
								result = IntLiKA(tempString);//праверка на л≥тэрал л≥чбавы
								if (result[0] == LEX_ERROR) {
									//throw(Error::geterrorin(101, NumberOfLine, positionInLine));
									errorList.push_back(Error::geterrorin(204, NumberOfLine, positionInLine));
									//WriteError(log,Error::geterrorin(101,NumberOfLine,positionInLine));
								}
							}
						}
					}
					LT::Add(lexTable, result, startPos, NumberOfLine);
					//positionInLine++;
					WriteLine(log, result, "");
				}
				char* sepSymbol = GetSepSymb(in.text[pos + startPos]);
				if (sepSymbol[0] != '\n' || newLine == false || IsNewLinePossible(lastChar) == true){
					WriteLine(log, sepSymbol, "");
					//positionInLine++;
				}
				LT::Add(lexTable, sepSymbol, pos + startPos, NumberOfLine);
				//раздз€л€льны с≥мвал == аперацы€
				if (sepSymbol[0] == 'v') {
					lexTable.table[lexTable.size-1].idxTI = int (in.text[pos + startPos]);
				}
				//есць пераход на новы радок
				if ((sepSymbol[0] == '\n' || sepSymbol[1] == '\n') && newLine == false){
					newLine = true;
				}
				else {
					newLine = false;
				}
				positionInLine+=pos+1;
				startPos = startPos + pos + 1;
				pos = 0;
			}
			else {
				pos++;
			}
		}
		if (errorList.empty() == false) {
			throw(errorList);
		}
		CheckStrForId((char*)in.text, idTable,lexTable);
		std::string str = IT::PrintTable(idTable);
		*log.stream << "\n" << str << endl;
		std::cout << "\n" << str << endl;
		std::cout << "\n" << LT::PrintTable(lexTable) << endl;
		
		lexTable.table[lexTable.size++]='$';
		MFST::Mfst *automatos = new MFST::Mfst(lexTable, GRB::getGreibach());
		automatos->start();

		GEN::Generator generator;
		string gen = generator.Generate(idTable,lexTable, (char*)in.text);

		*log.stream << "\n" << gen << endl;
		std::cout << "\n" << gen << endl;

		cout << LT::PrintTable(lexTable);

		/*int pos2 = 36;
		bool v = Poland::PolishNotation(&pos2, &lexTable, &idTable, (char*)in.text);
		cout << "\n" << LT::PrintTable(lexTable) << endl;
		*log.stream << "\n" << LT::PrintTable(lexTable) << endl;*/
	}


	void WriteError(LOG log, Error::ERROR error)
	{
		cout << "\n-----Error-----" << endl;
		cout << "Error " << error.id << ": " << error.message;
		if (error.inext.col != -1)
			cout << " (line " << error.inext.line << ", column " << error.inext.col << ")" << endl;
		else
			cout << endl;

		if (log.stream == NULL) {
			return;
		}
		*log.stream << "\n-----Error-----" << endl;
		*log.stream << "Error " << error.id << ": " << error.message;
		if (error.inext.col != -1)
			*log.stream << " (line " << error.inext.line << ", column " << error.inext.col << ")" << endl;
		else
			*log.stream << endl;
	}

	void Close(LOG log)
	{
		log.stream->close();
	}


	char *CheckWord(char* word) {
		char *str;
		str = new char[LEXEMA_FIXSIZE];
		if (strcmp(word, "integer") == 0) {
			str[0] = LEX_INTEGER;
		}
		else if (strcmp(word, "string") == 0) {
			str[0] = LEX_STRING;
		}
		else if (strcmp(word, "function") == 0) {
			str[0] = LEX_FUNCTION;
		}
		else if (strcmp(word, "declare") == 0) {
			str[0] = LEX_DECLARE;
		}
		else if (strcmp(word, "return") == 0) {
			str[0] = LEX_RETURN;
		}
		else if (strcmp(word, "main") == 0) {
			str[0] = LEX_MAIN;
		}
		else if (strcmp(word, "print") == 0) {
			str[0] = LEX_PRINT;
		} else {
			str[0] = LEX_ERROR;
		}

		str[LEXEMA_FIXSIZE] = '\0';
		return str;
	}	
	//праверыць слова на лексемы

	bool CheckSepSymb(char c) {
		if (c == ' ' || c == ',' || c == '.' || c == ';' || c == '\n' || c == '+' || 
		c == '-' || c == '=' || c == '/' || c == '*' || c == '\t' || c == '{' || c == '}' || c == '(' || c == ')') {
			return true;
		}
		return false;
	}	
	//праверыць, ц≥ з'€Ґл€ецца с≥мвал раздз€л€льным

	char* GetSepSymb(char c) {
		char *str;
		if (c == '+' || c == '-' || c == '/' || c == '*' || c == '%') {
			str = new char[2];
			str[0] = 'v';
			str[1] = '\0';
			return str;
		}
		else if (c == '=') {
			str = new char[2];
			str[0] = '=';
			str[1] = '\0';
			return str;
		}
		else if (c != ' ' /*&& c != ';'*/ && c != '\n' && c != '\t'/* && c != '{' && c != '}'*/) {
			str = new char[2];
			str[0] = c;
			str[1] = '\0';
			return str;
		}
		/*else if (c == ';' || c == '{' || c == '}') {
			str = new char[3];
			str[0] = c;
			str[1] = '\n';
			str[2] = '\0';
			return str;
		}*/
		else if (c == '\n') {
			str = new char[2];
			str[0] = '\n';
			str[1] = '\0';
			return str;
		}
		else {
			str = new char[1];
			str[0] = '\0';
		}
		return str;
	}
	//уп≥саць у лог‘јйл чытаемы с≥мвал раздз€ленн€
	bool IsNewLinePossible(char str) {
		if (str == '{' || str == ')' || str == ';' || str == '}') {
			return true;
		}
		else {
			return false;
		}
	}
	//ц≥ будзе новы радок, пасл€ дадзенага с≥мвала
	char* IdKA(char* word){
		FSTN::FST fst1(
			word,
			1, 
			FSTN::NODE(26,
				FSTN::RELATION('a', 0),
				FSTN::RELATION('b', 0),
				FSTN::RELATION('c', 0),
				FSTN::RELATION('d', 0),
				FSTN::RELATION('e', 0),
				FSTN::RELATION('f', 0),
				FSTN::RELATION('g', 0),
				FSTN::RELATION('h', 0),
				FSTN::RELATION('i', 0),
				FSTN::RELATION('j', 0),
				FSTN::RELATION('k', 0),
				FSTN::RELATION('l', 0),
				FSTN::RELATION('m', 0),
				FSTN::RELATION('n', 0),
				FSTN::RELATION('o', 0),
				FSTN::RELATION('p', 0),
				FSTN::RELATION('q', 0),
				FSTN::RELATION('r', 0),
				FSTN::RELATION('s', 0),
				FSTN::RELATION('t', 0),
				FSTN::RELATION('u', 0),
				FSTN::RELATION('v', 0),
				FSTN::RELATION('w', 0),
				FSTN::RELATION('x', 0),
				FSTN::RELATION('y', 0),
				FSTN::RELATION('z', 0)
			)
		);


		bool result = FSTN::execute(fst1);
		char *str;
		str = new char[LEXEMA_FIXSIZE];
		if (result == true) {
			str[0] = LEX_ID;
		} else {
			str[0] = LEX_ERROR;
		}
		str[LEXEMA_FIXSIZE] = '\0';
		return str;
		//return NULL;
	}	
	//праверыць слова, на аҐтамаце дл€ ≥д-ра

	char *IntLiKA(char* word) {
		FSTN::FST fst1(
			word,
			1,
			FSTN::NODE(10,
				FSTN::RELATION('1', 0),
				FSTN::RELATION('2', 0),
				FSTN::RELATION('3', 0),
				FSTN::RELATION('4', 0),
				FSTN::RELATION('5', 0),
				FSTN::RELATION('6', 0),
				FSTN::RELATION('7', 0),
				FSTN::RELATION('8', 0),
				FSTN::RELATION('9', 0),
				FSTN::RELATION('0', 0)
			)
		);


		bool result = FSTN::execute(fst1);
		char *str;
		str = new char[LEXEMA_FIXSIZE];
		if (result == true) {
			str[0] = LEX_LITERAL;
		}
		else {
			str[0] = LEX_ERROR;
		}
		str[LEXEMA_FIXSIZE] = '\0';
		return str;
	}	
	//праверыць слова, на аҐтамаце дл€ л≥тэрала л≥ка

	char* StrLiKA(char* word) {
		FSTN::FST fst1(
			word,
			3,
			FSTN::NODE(1, FSTN::RELATION('"', 1)),
			FSTN::NODE(38,
				FSTN::RELATION('1', 1),
				FSTN::RELATION('2', 1),
				FSTN::RELATION('3', 1),
				FSTN::RELATION('4', 1),
				FSTN::RELATION('5', 1),
				FSTN::RELATION('6', 1),
				FSTN::RELATION('7', 1),
				FSTN::RELATION('8', 1),
				FSTN::RELATION('9', 1),
				FSTN::RELATION('0', 1),
				FSTN::RELATION('a', 1),
				FSTN::RELATION('b', 1),
				FSTN::RELATION('c', 1),
				FSTN::RELATION('d', 1),
				FSTN::RELATION('e', 1),
				FSTN::RELATION('f', 1),
				FSTN::RELATION('g', 1),
				FSTN::RELATION('h', 1),
				FSTN::RELATION('i', 1),
				FSTN::RELATION('j', 1),
				FSTN::RELATION('k', 1),
				FSTN::RELATION('l', 1),
				FSTN::RELATION('m', 1),
				FSTN::RELATION('n', 1),
				FSTN::RELATION('o', 1),
				FSTN::RELATION('p', 1),
				FSTN::RELATION('q', 1),
				FSTN::RELATION('r', 1),
				FSTN::RELATION('s', 1),
				FSTN::RELATION('t', 1),
				FSTN::RELATION('u', 1),
				FSTN::RELATION('v', 1),
				FSTN::RELATION('w', 1),
				FSTN::RELATION('x', 1),
				FSTN::RELATION('y', 1),
				FSTN::RELATION('z', 1),
				FSTN::RELATION(' ', 1),
				FSTN::RELATION('"', 2)
			)
		);


		bool result = FSTN::execute(fst1);
		char *str;
		str = new char[LEXEMA_FIXSIZE];
		if (result == true) {
			str[0] = LEX_LITERAL;
		}
		else {
			str[0] = LEX_ERROR;
		}
		str[LEXEMA_FIXSIZE] = '\0';
		return str;
	}	
	//праверыць слова, на аҐтамаце дл€ л≥тэрала радка


}