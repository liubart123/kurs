#include "Error.h"
#include <string.h>
//using namespace Error;
namespace Error
{
	//0-99 system errors
	//100-109-parms errors
	//110-119-files errors
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0,"nedopustimuy code oshibki"),
		ERROR_ENTRY(1,"system sboi"),
		ERROR_ENTRY_NODEF(2),
		ERROR_ENTRY_NODEF(3),
		ERROR_ENTRY_NODEF(4),
		ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6),
		ERROR_ENTRY_NODEF(7),
		ERROR_ENTRY_NODEF(8),
		ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10),
		ERROR_ENTRY_NODEF10(20),
		ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60),
		ERROR_ENTRY_NODEF10(70),
		ERROR_ENTRY_NODEF10(80),
		ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100,"parametr -in must be zadan"),
		ERROR_ENTRY_NODEF(101),
		//ERROR_ENTRY(101,"lexic error"),	//lex
		ERROR_ENTRY_NODEF(102),
		//ERROR_ENTRY(102,"id must be declarated"),	//id
		ERROR_ENTRY_NODEF(103),
		//ERROR_ENTRY(103,"не правільны тып літэрала"),	//id
		ERROR_ENTRY(104,"previshena length vhodnogo parametra"),
		ERROR_ENTRY_NODEF(105),
		//ERROR_ENTRY(105,"maximum number of lexem"),	//lex
		ERROR_ENTRY_NODEF(106),
		//ERROR_ENTRY(106,"maximum number of id"),	//lex
		ERROR_ENTRY_NODEF(107),
		//ERROR_ENTRY(107,"id syntax error"),	//lex
		ERROR_ENTRY_NODEF(108),
		//ERROR_ENTRY(108,"operation wasn't defined"),	
		ERROR_ENTRY_NODEF(109),
		//ERROR_ENTRY(109,"павінна быць закрываючая дужка"),	//poland
		ERROR_ENTRY(110,"oshibka pri otkritii faile -in"),	//log
		ERROR_ENTRY(111,"nedopustimiy simvol v -in faile"),	//lex
		ERROR_ENTRY(112,"oshibka pri sozdanii -log faila"),	//log
		ERROR_ENTRY_NODEF(113),
		//ERROR_ENTRY(113,"incorrect lexem in expression"),	//lexk
		ERROR_ENTRY_NODEF(114),
		ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116),
		ERROR_ENTRY_NODEF(117),
		ERROR_ENTRY_NODEF(118),
		ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120),
		ERROR_ENTRY_NODEF10(130),
		ERROR_ENTRY_NODEF10(140),
		ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160),
		ERROR_ENTRY_NODEF10(170),
		ERROR_ENTRY_NODEF10(180),
		ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY(200,"maximum number of lexem"),	//lex
		ERROR_ENTRY(201,"maximum number of id"),	//lex
		ERROR_ENTRY(202,"id syntax error"),	//lex
		ERROR_ENTRY(203,"id must be declarated"),	//id
		ERROR_ENTRY(204,"lexic error"),	//lex
		ERROR_ENTRY(205,"must be closing bracket"),	//poland
		ERROR_ENTRY(206,"incorrect lexem in expression"),	//lex
		ERROR_ENTRY(207,"operation wasn't defined"),
		ERROR_ENTRY(208,"wrong type of literal"),	//id
		ERROR_ENTRY(209,"double declaration"),	//id
		ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY_NODEF100(400),
		ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600,"incorrect structure of programm"),
		ERROR_ENTRY(601,"incorrect operator"),
		ERROR_ENTRY(602,"error in expression"),
		ERROR_ENTRY(603,"error in expression"),
		ERROR_ENTRY(604,"error in function's parametors"),
		ERROR_ENTRY(605,"error in executed function's parametors"),
		ERROR_ENTRY(606,"error in condition structure"),
		ERROR_ENTRY_NODEF(607),
		ERROR_ENTRY_NODEF(608),
		ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(610),
		ERROR_ENTRY_NODEF10(620),
		ERROR_ENTRY_NODEF10(630),
		ERROR_ENTRY_NODEF10(640),
		ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660),
		ERROR_ENTRY_NODEF10(670),
		ERROR_ENTRY_NODEF10(680),
		ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700),
		ERROR_ENTRY_NODEF100(800),
		ERROR_ENTRY_NODEF100(900),
	};


	ERROR geterror(int id)
	{
		if (0 < id && id < ERROR_MAX_ENTRY) {
			for (int i = 0; i < ERROR_MAX_ENTRY; i++) {
				if (id == errors[i].id) {
					return errors[i];
				}
			}
			return errors[0];
		}
		else {
			return errors[0];
		}
	}

	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		ERROR e;
		if (0 < id && id < ERROR_MAX_ENTRY) {
			for (int i = 0; i < ERROR_MAX_ENTRY; i++) {
				if (id == errors[i].id) {
					e.id = errors[i].id;
					strcpy_s(e.message, errors[i].message);
					e.inext.line = line;
					e.inext.col = col;
					return e;
				}
			}
			return errors[0];
		}
		else {
			return errors[0];
		}
	}

	//атрымаць з пазіцыі у тэксце нумар радка і калонку
	ERROR geterrortext(int id, char* text, int index) {
		int i = 0;
		int col = 0, line = 1;
		while (i <= index && text[i] != '\0') {
			col++;
			if (text[i] == '\n') {
				line++;
				col = 0;
			}

			i++;
		}
		if (i >= index) {
			return geterrorin(id, line, col);
		}
		else {
			return geterror(0);
		}
	}


}