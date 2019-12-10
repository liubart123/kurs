#include "Error.h"
#include "In.h"
#include <fstream>
#include <iostream>
using namespace std;
namespace In
{
	IN getin(wchar_t infile[]) {
		ifstream f;
		char arr[] = { "inFile.txt" };
		f.open(infile);
		int curSize = 0;
		char tempChar;
		int ingor = 0;
		int line = 0;
		int pos = 0;
		char *p = new char[IN_MAX_LEN_TEXT];
		while (f.eof() != true) {
			f.get(tempChar);
			/*std::cout << (char)tempChar;
			std::cout << (unsigned char)tempChar;
			std::cout << '\0';*/
			if (tempChar == EOF)
				break;

			if (TABLE[(char)tempChar] == IN::T) {
				p[curSize]=tempChar;
				curSize++;
				if (tempChar == IN_CODE_ENDL) {
					line++;
					//p[curSize] = tempChar;
					//curSize++;
					pos = 0;
				} else {
					pos++;
				}
			}
			else if (TABLE[(int)tempChar] == IN::F) {
				throw ERROR_THROW_IN(111,line+1,pos);
			}
			else if (TABLE[(int)(unsigned char)tempChar] == IN::I) {
				pos++;
				ingor++;
			}
		}
		f.close();
		IN str;
		p[--curSize] = '\0';
		str.ignor = ingor;
		str.size = curSize;
		str.lines = line - 1;
		str.text = (unsigned char*)p;
		return str;
	}
}