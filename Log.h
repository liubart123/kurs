#pragma once
#include <fstream>
#include <string>
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
		wchar_t outfile[PARM_MAX_SIZE] = L"\0";
		std::ofstream* source_stream = NULL;
	};
	static const LOG INITLOG;
	LOG getlog(wchar_t logfile[], wchar_t outfile[]);
	void WriteLine(LOG log, char* c, ...);
	void WriteLine(LOG log, char c);
	//void WriteLine(LOG log, wchar_t* c, ...);
	char* IdKA(char* word);
	void WriteLog(LOG log);
	void WriteParm(LOG log, Parm::PARM parm);
	void WriteIn(LOG log, In::IN in, int maxErrors, bool showeDetails);
	void WriteError(LOG log, Error::ERROR error);
	void Close(LOG log);
	char* GetSepSymb(char c);
	char* StrLiKA(char* word);
	char *IntLiKA(char* word);
	char *IntLiKAHex(char* word);
	char *IntLiKAOct(char* word);
	char *IntLiKABin(char* word);
	char *CheckWord(char* word);
	bool IsNewLinePossible(char str);
	char* ChrLiKA(char* word);
}