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
#include "SMN.h"
namespace Log
{
	bool CheckSepSymb(char c);
	LOG getlog(wchar_t logfile[], wchar_t outfile[])
	{
		static ofstream file;
		file.open(logfile);
		static ofstream file_out;
		file_out.open(outfile);

		if (!file.is_open() || !file_out.is_open())
		{
			throw ERROR_THROW(112);
		}

		LOG newLog;
		wcscpy_s(newLog.logfile, logfile);
		newLog.stream = &file;
		wcscpy_s(newLog.outfile, outfile);
		newLog.source_stream = &file_out;

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

		while (*ptr[0] != '\0')
		{
			//mbstowcs_s(&outSize, string, *ptr, strlen(*ptr));
			char tempString[PARM_MAX_SIZE];
			strcpy_s(tempString, *ptr);
			strcat_s(output, tempString);
			ptr++;
		}

		//wcstombs_s(&outSize, output, log.logfile, wcslen(log.logfile));
		//cout << output;

		*log.stream << output;
		//*log.stream << "asd";
	}
	void WriteLine(LOG log, char c)
	{
		*log.stream << c;
	}



	void WriteLog(LOG log)
	{
		time_t current_time;
		tm * timeinfo = new tm;
		char buffer[80];                                // строка, в которой будет храниться текущее время

		time(&current_time);                               // текущая дата в секундах
		localtime_s(timeinfo, &current_time);               // текущее локальное время, представленное в структуре

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

	void WriteIn(LOG log, In::IN in, int maxErrors, bool ShowDetails, bool showePolandDetails)
	{
		*log.stream << "-----Input data-----" << endl;
		*log.stream << "Amount of symbols: " << in.size << endl;
		*log.stream << "Amount of ignored symbols: " << in.ignor << endl;
		*log.stream << "Amount of lines: " << in.lines << endl;
		int pos = 0;
		int startPos = 0;
		int NumberOfLine = 0;
		bool newLine = true;
		int positionInLine = 0;	//нумар калонкі у цякучым радку
		IT::IdTable idTable = IT::CreateTable(TI_MAXSIZE);
		LT::LexTable lexTable = LT::CreateTable(LT_MAXSIZE);//табліца лексем
		bool kavichka = false;	//ці ёсць адкрываючая кавычка
		bool kavichka2 = false;	//ці ёсць адкрываючая кавычка адзінарная
		list<Error::ERROR> errorList;
		*log.stream << "\nLexic analys is performing...\n" << endl;
		std::cout << "\nLexic analys is performing...\n" << endl;
		//стварэнне табліцы лексем
		while (startPos + pos < in.size) {
			if (in.text[pos + startPos] == '\"') {
				kavichka = !kavichka;
			}
			if (in.text[pos + startPos] == '\'') {
				kavichka2 = !kavichka2;
			}
			if (CheckSepSymb(in.text[pos + startPos]) && (kavichka == false && kavichka2==false)) {
				char lastChar = '\0';	//апошні значымы сімвал у радку
				char *tempString = new char[pos + 1];
				int posTemp = 0;
				while (posTemp < pos) {		//слова памяшчаецца у posTemp
					tempString[posTemp] = in.text[posTemp + startPos];
					if (tempString[posTemp]!=' ' && tempString[posTemp] != '\n' && tempString[posTemp] != '\t')
						lastChar=tempString[posTemp];
					posTemp++;
				}
				tempString[pos] = '\0';	
				if (newLine) {		//новы радок
					NumberOfLine++;
					string numOfLine = "";
					numOfLine = to_string((int)NumberOfLine);
					WriteLine(log, &(numOfLine[0]), ": ", "");
					cout << numOfLine << ": ";
					positionInLine=0;
				}
				if (pos != 0) {		//калі цякучае слова не сімвал пропуска
					char *result;
					LT::COUNTSYSTEM countSystem = LT::DEC;
					result = CheckWord(tempString);	//праверка на ключавые словы
					if (result[0] == LEX_ERROR){
						result = ChrLiKA(tempString);//праверка на літэрал сімвала
						if (result[0] == LEX_ERROR) {
							result = StrLiKA(tempString);//праверка на літэрал радковы
							if (result[0] == LEX_ERROR) {
								result = IntLiKA(tempString);//праверка на літэрал лічбавы dec
								if (result[0] == LEX_ERROR) {
									result = IntLiKAHex(tempString);//праверка на літэрал лічбавы hex
									countSystem = LT::HEX;
									if (result[0] == LEX_ERROR) {
										result = IntLiKABin(tempString);//праверка на літэрал лічбавы bin
										countSystem = LT::BIN;
										if (result[0] == LEX_ERROR) {
											result = IntLiKAOct(tempString);//праверка на літэрал лічбавы oct
											countSystem = LT::OCT;
											if (result[0] == LEX_ERROR) {
												result = IdKA(tempString);//праверка на ідынтыфікатар
												if (result[0] == LEX_ERROR) {
													//throw(Error::geterrorin(101, NumberOfLine, positionInLine));
													if (errorList.size() >= maxErrors) {
														errorList.push_back(Error::geterror(215));
														throw errorList;
													}
													errorList.push_back(Error::geterrorin(204, NumberOfLine, positionInLine));
													//WriteError(log,Error::geterrorin(101,NumberOfLine,positionInLine));
												}
											}
										}
									}
								}
							}
						}
					}
					LT::Add(lexTable, result, startPos, NumberOfLine, positionInLine);
					LT::GetEntry(lexTable, lexTable.size - 1)->countSys = countSystem;
					WriteLine(log, result, "");
					cout << result;
				}
				char* sepSymbol = GetSepSymb(in.text[pos + startPos]);
				LT::Add(lexTable, sepSymbol, pos + startPos, NumberOfLine, positionInLine+pos);
				WriteLine(log, sepSymbol, "");
				cout << sepSymbol;
				//раздзяляльны сімвал == аперацыя
				if (sepSymbol[0] == 'v') {
					lexTable.table[lexTable.size-1].idxTI = int (in.text[pos + startPos]);
				}
				//есць пераход на новы радок
				if ((sepSymbol[0] == '\n')){
					newLine = true;
				}else {
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

		*log.stream << "\nLexic analys was successfull\n" << endl;
		std::cout << "\nLexic analys was successfull\n" << endl;

		*log.stream << "\nIDs analys is performing...\n" << endl;
		std::cout << "\nIDs analys is performing...\n" << endl;
		CheckStrForId((char*)in.text, idTable,lexTable, maxErrors);
		*log.stream << "\nIDs analys was successfull\n" << endl;
		std::cout << "\nIDs analys was successfull\n" << endl;
		//*log.stream << "\nIDs table:\n" << endl;
		std::string str = IT::PrintTable(idTable);
		*log.stream << "\n" << str << endl;
		std::cout << "\n" << str << endl;
		std::cout << "\n" << LT::PrintTable(lexTable) << endl;

		lexTable.table[lexTable.size++]='$';
		MFST::Mfst *automatos = new MFST::Mfst(lexTable, GRB::getGreibach(), maxErrors);
		*log.stream << "\nSyntax analys is performing...\n" << endl;
		cout << "\nSyntax analys is performing...\n" << endl;
		WriteLog(log);
		if (!automatos->start(log, ShowDetails)) {
			cout << "\nSyntax error...\n" << endl;
			return;
		}
		WriteLog(log);

		*log.stream << "\nnSyntax analys was completed successfull\n" << endl;
		cout << "\nnSyntax analys was completed successfull\n" << endl;

		*log.stream << "\nSemantic's analys is performing...\n" << endl;
		cout << "\nSemantic's analys is performing...\n" << endl;
		SMN::SmnTest(idTable, lexTable);

		*log.stream << "\nSemantic's analys was completed suucessfull\n" << endl;
		cout << "\nSemantic's analys was completed suucessfull\n" << endl;
		GEN::Generator generator;
		*log.stream << "\nCode generating is performing...\n" << endl;
		cout << "\nCode generating is performing...\n" << endl;
		string gen = generator.Generate(idTable,lexTable, (char*)in.text, showePolandDetails, log);
		*log.stream << "\nCode generating was completed suucessfull\n" << endl;
		cout << "\nCode generating was completed suucessfull\n" << endl;

		//*log.stream << "\n" << gen << endl;
		//std::cout << "\n" << gen << endl;
		*log.source_stream << gen;

		//cout << LT::PrintTable(lexTable);
		/*int pos2 = 36;
		bool v = Poland::PolishNotation(&pos2, &lexTable, &idTable, (char*)in.text);
		cout << "\n" << LT::PrintTable(lexTable) << endl;
		*log.stream << "\n" << LT::PrintTable(lexTable) << endl;*/
	}


	void WriteError(LOG log, Error::ERROR error)
	{
		cout << "\n-----Error-----" << endl;
		cout << "Error " << error.id << ": " << error.message;
		if (error.inext.col != -1 && error.inext.line != -1)
			cout << " (line " << error.inext.line << ", column " << error.inext.col << ")" << endl;
		else if (error.inext.line != -1)
				cout << " (line " << error.inext.line << ")" << endl;
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
		else if (strcmp(word, "void") == 0) {
			str[0] = LEX_VOID;
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
		else if (strcmp(word, "if") == 0) {
			str[0] = LEX_CONDITION;
		}
		else if (strcmp(word, "else") == 0) {
			str[0] = LEX_ELSE;
		}
		else if (strcmp(word, "while") == 0) {
			str[0] = LEX_CYCLE;
		}
		else if (strcmp(word, "break") == 0) {
			str[0] = LEX_BREAK;
		}
		else if (strcmp(word, "array") == 0) {
			str[0] = LEX_ARRAY;
		}
		else if (strcmp(word, "sarray") == 0) {
			str[0] = LEX_ARRAY_STR;
		}
		else if (strcmp(word, "char") == 0) {
			str[0] = LEX_CHAR;
		}
		else if (strcmp(word, SF1) == 0) {
			str[0] = LEX_ID;
		}
		else if (strcmp(word, SF2) == 0) {
			str[0] = LEX_ID;
		}
		else if (strcmp(word, SF3) == 0) {
			str[0] = LEX_ID;
		}
		else if (strcmp(word, SF4) == 0) {
			str[0] = LEX_ID;
		}
		else if (strcmp(word, SF5) == 0) {
			str[0] = LEX_ID;
		}
		else if (strcmp(word, SF6) == 0) {
			str[0] = LEX_ID;
		}
		else if (strcmp(word, SF7) == 0) {
			str[0] = LEX_ID;
		}
		else if (strcmp(word, SF8) == 0) {
			str[0] = LEX_ID;
		}
		else if (strcmp(word, SF9) == 0) {
			str[0] = LEX_ID;
		}
		else {
			str[0] = LEX_ERROR;
		}

		str[LEXEMA_FIXSIZE] = '\0';
		return str;
	}	
	//праверыць слова на лексемы

	bool CheckSepSymb(char c) {
		if (c == ' ' || c == ',' || c == '.' || c == ';' || c == '\n' || c == '+' || 
		c == '-' || c == '=' || c == '/' || c == '*' || c == '\t' || c == '{' || c == '}' || c == '(' || c == ')'
			|| c == '&' || c == '|' || c == '>' || c == '<' || c == '!' || c == '?'
			|| c == '[' || c == ']' || c == '^' || c == '%') {
			return true;
		}
		return false;
	}	
	//праверыць, ці з'яўляецца сімвал раздзяляльным

	char* GetSepSymb(char c) {
		char *str;
		if (c == '+' || c == '-' || c == '/' || c == '*' || c == '%'
			|| c == '>' || c == '<' || c == '|' || c == '&' || c == '^' || c == '!' || c == '=' || c == '?'
			|| c == '[' || c == ']') {
			str = new char[2];
			switch (c) {
				case '+':str[0] = LEX_PLUS; break;
				case '-':str[0] = LEX_MINUS; break;
				case '/':str[0] = LEX_DIRSLASH; break;
				case '*':str[0] = LEX_STAR; break;
				case '>':str[0] = LEX_GREATER; break;
				case '<':str[0] = LEX_LESS; break;
				case '|':str[0] = LEX_OR; break;
				case '&':str[0] = LEX_AND; break;
				case '!':str[0] = LEX_NOT_EQUAL; break;
				case '?':str[0] = LEX_LOG_EQUALS; break;
				case '%':str[0] = LEX_PROCENT; break;
				case '=':str[0] = LEX_EQUALS; break;
				case '^':str[0] = LEX_PTR; break;
				case ']':str[0] = LEX_LEFTARR; break;
				case '[':str[0] = LEX_RIGHTARR; break;
			}
			str[1] = '\0';
			return str;
		}
		else if (c != ' '&& c != '\n' && c != '\t') {
			str = new char[2];
			str[0] = c;
			str[1] = '\0';
			return str;
		}
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
	//упісаць у логФАйл чытаемы сімвал раздзялення
	bool IsNewLinePossible(char str) {
		if (str == '{' || str == ')' || str == ';' || str == '}') {
			return true;
		}
		else {
			return false;
		}
	}
	//ці будзе новы радок, пасля дадзенага сімвала
	char* IdKA(char* word){
		FSTN::FST fst1(
			word,
			2, 
			FSTN::NODE(26,
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
				FSTN::RELATION('z', 1)
			),
			FSTN::NODE(36,
				FSTN::RELATION('0', 1),
				FSTN::RELATION('1', 1),
				FSTN::RELATION('2', 1),
				FSTN::RELATION('3', 1),
				FSTN::RELATION('4', 1),
				FSTN::RELATION('5', 1),
				FSTN::RELATION('6', 1),
				FSTN::RELATION('7', 1),
				FSTN::RELATION('8', 1),
				FSTN::RELATION('9', 1),
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
				FSTN::RELATION('z', 1)
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
	//праверыць слова, на аўтамаце для ід-ра

	char *IntLiKA(char* word) {
		FSTN::FST fst1(
			word,
			2,
			FSTN::NODE(10,
				FSTN::RELATION('1', 1),
				FSTN::RELATION('2', 1),
				FSTN::RELATION('3', 1),
				FSTN::RELATION('4', 1),
				FSTN::RELATION('5', 1),
				FSTN::RELATION('6', 1),
				FSTN::RELATION('7', 1),
				FSTN::RELATION('8', 1),
				FSTN::RELATION('9', 1),
				FSTN::RELATION('0', 1)
			),
			FSTN::NODE(23,
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
				FSTN::RELATION('1', 2),
				FSTN::RELATION('2', 2),
				FSTN::RELATION('3', 2),
				FSTN::RELATION('4', 2),
				FSTN::RELATION('5', 2),
				FSTN::RELATION('6', 2),
				FSTN::RELATION('7', 2),
				FSTN::RELATION('8', 2),
				FSTN::RELATION('9', 2),
				FSTN::RELATION('0', 2),
				FSTN::RELATION('b', 2),
				FSTN::RELATION('o', 2),
				FSTN::RELATION('h', 2)
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

	char *IntLiKABin(char* word) {
		FSTN::FST fst1(
			word,
			3,
			FSTN::NODE(2,
				FSTN::RELATION('1', 1),
				FSTN::RELATION('0', 1)
			),
			FSTN::NODE(3,
				FSTN::RELATION('1', 1),
				FSTN::RELATION('0', 1),
				FSTN::RELATION('b', 2)
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

	char *IntLiKAOct(char* word) {
		FSTN::FST fst1(
			word,
			3,
			FSTN::NODE(8,
				FSTN::RELATION('1', 1),
				FSTN::RELATION('2', 1),
				FSTN::RELATION('3', 1),
				FSTN::RELATION('4', 1),
				FSTN::RELATION('5', 1),
				FSTN::RELATION('6', 1),
				FSTN::RELATION('7', 1),
				FSTN::RELATION('0', 1)
			),
			FSTN::NODE(9,
				FSTN::RELATION('1', 1),
				FSTN::RELATION('2', 1),
				FSTN::RELATION('3', 1),
				FSTN::RELATION('4', 1),
				FSTN::RELATION('5', 1),
				FSTN::RELATION('6', 1),
				FSTN::RELATION('7', 1),
				FSTN::RELATION('0', 1),
				FSTN::RELATION('o', 1)
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

	char *IntLiKAHex(char* word) {
		FSTN::FST fst1(
			word,
			3,
			FSTN::NODE(16,
				FSTN::RELATION('0', 1),
				FSTN::RELATION('1', 1),
				FSTN::RELATION('2', 1),
				FSTN::RELATION('3', 1),
				FSTN::RELATION('4', 1),
				FSTN::RELATION('5', 1),
				FSTN::RELATION('6', 1),
				FSTN::RELATION('7', 1),
				FSTN::RELATION('8', 1),
				FSTN::RELATION('9', 1),
				FSTN::RELATION('a', 1),
				FSTN::RELATION('b', 1),
				FSTN::RELATION('c', 1),
				FSTN::RELATION('d', 1),
				FSTN::RELATION('e', 1),
				FSTN::RELATION('f', 1)
			),
			FSTN::NODE(17,
				FSTN::RELATION('0', 1),
				FSTN::RELATION('1', 1),
				FSTN::RELATION('2', 1),
				FSTN::RELATION('3', 1),
				FSTN::RELATION('4', 1),
				FSTN::RELATION('5', 1),
				FSTN::RELATION('6', 1),
				FSTN::RELATION('7', 1),
				FSTN::RELATION('8', 1),
				FSTN::RELATION('9', 1),
				FSTN::RELATION('a', 1),
				FSTN::RELATION('b', 1),
				FSTN::RELATION('c', 1),
				FSTN::RELATION('d', 1),
				FSTN::RELATION('e', 1),
				FSTN::RELATION('f', 1),
				FSTN::RELATION('h', 2)
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

	//праверыць слова, на аўтамаце для літэрала ліка

	char* StrLiKA(char* word) {
		FSTN::FST fst1(
			word,
			3,
			FSTN::NODE(1, FSTN::RELATION('"', 1)),
			FSTN::NODE(182,
			FSTN::RELATION(' ', 1),
			FSTN::RELATION('!', 1),
			FSTN::RELATION('#', 1),
			FSTN::RELATION('$', 1),
			FSTN::RELATION('%', 1),
			FSTN::RELATION('&', 1),
			FSTN::RELATION('(', 1),
			FSTN::RELATION(')', 1),
			FSTN::RELATION('*', 1),
			FSTN::RELATION('+', 1),
			FSTN::RELATION(',', 1),
			FSTN::RELATION('-', 1),
			FSTN::RELATION('.', 1),
			FSTN::RELATION('/', 1),
			FSTN::RELATION('0', 1),
			FSTN::RELATION('1', 1),
			FSTN::RELATION('2', 1),
			FSTN::RELATION('3', 1),
			FSTN::RELATION('4', 1),
			FSTN::RELATION('5', 1),
			FSTN::RELATION('6', 1),
			FSTN::RELATION('7', 1),
			FSTN::RELATION('8', 1),
			FSTN::RELATION('9', 1),
			FSTN::RELATION(':', 1),
			FSTN::RELATION(';', 1),
			FSTN::RELATION('<', 1),
			FSTN::RELATION('=', 1),
			FSTN::RELATION('>', 1),
			FSTN::RELATION('?', 1),
			FSTN::RELATION('@', 1),
			FSTN::RELATION('A', 1),
			FSTN::RELATION('B', 1),
			FSTN::RELATION('C', 1),
			FSTN::RELATION('D', 1),
			FSTN::RELATION('E', 1),
			FSTN::RELATION('F', 1),
			FSTN::RELATION('G', 1),
			FSTN::RELATION('H', 1),
			FSTN::RELATION('I', 1),
			FSTN::RELATION('J', 1),
			FSTN::RELATION('K', 1),
			FSTN::RELATION('L', 1),
			FSTN::RELATION('M', 1),
			FSTN::RELATION('N', 1),
			FSTN::RELATION('O', 1),
			FSTN::RELATION('P', 1),
			FSTN::RELATION('Q', 1),
			FSTN::RELATION('R', 1),
			FSTN::RELATION('S', 1),
			FSTN::RELATION('T', 1),
			FSTN::RELATION('U', 1),
			FSTN::RELATION('V', 1),
			FSTN::RELATION('W', 1),
			FSTN::RELATION('X', 1),
			FSTN::RELATION('Y', 1),
			FSTN::RELATION('Z', 1),
			FSTN::RELATION('[', 1),
			FSTN::RELATION('\\', 1),
			FSTN::RELATION(']', 1),
			FSTN::RELATION('^', 1),
			FSTN::RELATION('_', 1),
			FSTN::RELATION('`', 1),
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
			FSTN::RELATION('{', 1),
			FSTN::RELATION('|', 1),
			FSTN::RELATION('}', 1),
			FSTN::RELATION('~', 1),
			FSTN::RELATION('?', 1),
			FSTN::RELATION('\'', 1),
			FSTN::RELATION('?', 1),
			FSTN::RELATION(':', 1),
			FSTN::RELATION('%', 1),
			FSTN::RELATION('<', 1),
			FSTN::RELATION('>', 1),
			FSTN::RELATION('Ў', 1),
			FSTN::RELATION('ў', 1),
			FSTN::RELATION('?', 1),
			FSTN::RELATION(' ', 1),
			FSTN::RELATION('Ё', 1),
			FSTN::RELATION('c', 1),
			FSTN::RELATION('Є', 1),
			FSTN::RELATION('<', 1),
			FSTN::RELATION('?', 1),
			FSTN::RELATION('-', 1),
			FSTN::RELATION('R', 1),
			FSTN::RELATION('+', 1),
			FSTN::RELATION('ч', 1),
			FSTN::RELATION('ё', 1),
			FSTN::RELATION('№', 1),
			FSTN::RELATION('є', 1),
			FSTN::RELATION('>', 1),
			FSTN::RELATION('А', 1),
			FSTN::RELATION('Б', 1),
			FSTN::RELATION('В', 1),
			FSTN::RELATION('Г', 1),
			FSTN::RELATION('Д', 1),
			FSTN::RELATION('Е', 1),
			FSTN::RELATION('Ж', 1),
			FSTN::RELATION('З', 1),
			FSTN::RELATION('И', 1),
			FSTN::RELATION('Й', 1),
			FSTN::RELATION('К', 1),
			FSTN::RELATION('Л', 1),
			FSTN::RELATION('М', 1),
			FSTN::RELATION('Н', 1),
			FSTN::RELATION('О', 1),
			FSTN::RELATION('П', 1),
			FSTN::RELATION('Р', 1),
			FSTN::RELATION('С', 1),
			FSTN::RELATION('Т', 1),
			FSTN::RELATION('У', 1),
			FSTN::RELATION('Ф', 1),
			FSTN::RELATION('Х', 1),
			FSTN::RELATION('Ц', 1),
			FSTN::RELATION('Ч', 1),
			FSTN::RELATION('Ш', 1),
			FSTN::RELATION('Щ', 1),
			FSTN::RELATION('Ъ', 1),
			FSTN::RELATION('Ы', 1),
			FSTN::RELATION('Ь', 1),
			FSTN::RELATION('Э', 1),
			FSTN::RELATION('Ю', 1),
			FSTN::RELATION('Я', 1),
			FSTN::RELATION('а', 1),
			FSTN::RELATION('б', 1),
			FSTN::RELATION('в', 1),
			FSTN::RELATION('г', 1),
			FSTN::RELATION('д', 1),
			FSTN::RELATION('е', 1),
			FSTN::RELATION('ж', 1),
			FSTN::RELATION('з', 1),
			FSTN::RELATION('и', 1),
			FSTN::RELATION('й', 1),
			FSTN::RELATION('к', 1),
			FSTN::RELATION('л', 1),
			FSTN::RELATION('м', 1),
			FSTN::RELATION('н', 1),
			FSTN::RELATION('о', 1),
			FSTN::RELATION('п', 1),
			FSTN::RELATION('р', 1),
			FSTN::RELATION('с', 1),
			FSTN::RELATION('т', 1),
			FSTN::RELATION('у', 1),
			FSTN::RELATION('ф', 1),
			FSTN::RELATION('х', 1),
			FSTN::RELATION('ц', 1),
			FSTN::RELATION('ч', 1),
			FSTN::RELATION('ш', 1),
			FSTN::RELATION('щ', 1),
			FSTN::RELATION('ъ', 1),
			FSTN::RELATION('ы', 1),
			FSTN::RELATION('ь', 1),
			FSTN::RELATION('э', 1),
			FSTN::RELATION('ю', 1),
			FSTN::RELATION('я', 1),
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
	//праверыць слова, на аўтамаце для літэрала радка

	char* ChrLiKA(char* word) {
		FSTN::FST fst1(
			word,
			4,
			FSTN::NODE(1, FSTN::RELATION('\'', 1)),
			FSTN::NODE(52,
				FSTN::RELATION('1', 2),
				FSTN::RELATION('2', 2),
				FSTN::RELATION('3', 2),
				FSTN::RELATION('4', 2),
				FSTN::RELATION('5', 2),
				FSTN::RELATION('6', 2),
				FSTN::RELATION('7', 2),
				FSTN::RELATION('8', 2),
				FSTN::RELATION('9', 2),
				FSTN::RELATION('0', 2),
				FSTN::RELATION('a', 2),
				FSTN::RELATION('b', 2),
				FSTN::RELATION('c', 2),
				FSTN::RELATION('d', 2),
				FSTN::RELATION('e', 2),
				FSTN::RELATION('f', 2),
				FSTN::RELATION('g', 2),
				FSTN::RELATION('h', 2),
				FSTN::RELATION('i', 2),
				FSTN::RELATION('j', 2),
				FSTN::RELATION('k', 2),
				FSTN::RELATION('l', 2),
				FSTN::RELATION('m', 2),
				FSTN::RELATION('n', 2),
				FSTN::RELATION('o', 2),
				FSTN::RELATION('p', 2),
				FSTN::RELATION('q', 2),
				FSTN::RELATION('r', 2),
				FSTN::RELATION('s', 2),
				FSTN::RELATION('t', 2),
				FSTN::RELATION('u', 2),
				FSTN::RELATION('v', 2),
				FSTN::RELATION('w', 2),
				FSTN::RELATION('x', 2),
				FSTN::RELATION('y', 2),
				FSTN::RELATION('z', 2),
				FSTN::RELATION(' ', 2),
				FSTN::RELATION(',', 2),
				FSTN::RELATION('\\', 2),
				FSTN::RELATION('\n', 2),
				FSTN::RELATION('\t', 2),
				FSTN::RELATION(';', 2),
				FSTN::RELATION(':', 2),
				FSTN::RELATION('.', 2),
				FSTN::RELATION('*', 2),
				FSTN::RELATION('/', 2),
				FSTN::RELATION('+', 2),
				FSTN::RELATION('-', 2),
				FSTN::RELATION('!', 2),
				FSTN::RELATION('@', 2),
				FSTN::RELATION('$', 2),
				FSTN::RELATION('%', 2)
			),
			FSTN::NODE(1, FSTN::RELATION('\'', 3))
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
	//праверыць слова, на аўтамаце для літэрала радка


}