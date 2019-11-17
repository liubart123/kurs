#include "Generate.h"


using namespace std;
namespace GEN {
	string Generator::Generate(IT::IdTable it, LT::LexTable lt) {
		string resultStr = "";
		resultStr += CONST_SECTION;
		int i;
		//секцы€ канстант
		for (i = 0; i < it.size; i++) {
			if (it.table[i].idtype == IT::IDTYPE::L) {
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
			}
		}	 

		//секцы€ дадзеных
		for (; i < it.size; i++) {

		}

		return resultStr;
	}	//атрмаць радок асм файла
	Generator::Generator() {

	}
}