#pragma once
#include <fstream>
#include "In.h"
#include "Parm.h"
#include "Error.h"
#include <stdlib.h>
#include "FST.h"
using namespace std;

namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE] = L"\0";
		std::ofstream* stream = NULL;
	};
	static const LOG INITLOG;
	LOG getlog(wchar_t logfile[]);
	void WriteLine(LOG log, char* c, ...);
	//void WriteLine(LOG log, wchar_t* c, ...);
	char* IdKA(char* word);
	void WriteLog(LOG log);
	void WriteParm(LOG log, Parm::PARM parm);
	void WriteIn(LOG log, In::IN in);
	void WriteError(LOG log, Error::ERROR error);
	void Close(LOG log);
	char* GetSepSymb(char c);
	char* StrLiKA(char* word);
	char *IntLiKA(char* word);
	char *CheckWord(char* word);
	bool IsNewLinePossible(char str);
	char* ChrLiKA(char* word);
}