#include <iostream>
//#include <Windows.h>
#include <locale>
#include <cwchar>
#include "Log.h"
#include "Error.h"
#include "In.h"
#include "Parm.h"
#include <tchar.h>
#include <stack>
#include <conio.h>
#include <list>

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "RUS");


	Log::LOG log = Log::INITLOG;
	try
	{
		/*setlocale(LC_ALL, "rus");	
		SetConsoleOutputCP(1251);	
		unsigned char a;
		cin >> a;
		cout << a;
		char b;
		cin >> b;
		cout << b;*/
		//for (int i = 0; i < 256; i++) {
		//	cout << "/*" << char(i) << " (" << i << ")*/IN::T, ";	////* 0*/IN::T,
		//	if (i % 15 == 0) {
		//		cout << "\n";
		//	}
		//}

		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log, parm.source);
		//Log::WriteLine(log, (char*)"³ô³ûâo¢", "asd","");
		Log::WriteLog(log);
		Log::WriteParm(log,parm);
		wcout << L"in: " << parm.in << L'\n';
		wcout << L"out: " << parm.out << L'\n';
		wcout << L"log: " << parm.log << L'\n';
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log,in, parm.ErrorsMaxCount);
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log,e);
	}
	catch (list<Error::ERROR> e)
	{	
		while (!e.empty()) {
			Log::WriteError(log, e.front());
			e.pop_front();
		}
	}
	catch(exception e) {
		cout << e.what();
	}
	//system("pause");
	_getch();
	return 0;
}