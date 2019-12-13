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
		ERROR_ENTRY(104,"too long parametr"),
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
		ERROR_ENTRY(110,"error opening -in fail"),	//log
		ERROR_ENTRY(111,"forbidden symbol -in fail"),	//lex
		ERROR_ENTRY(112,"error opening -log fail"),	//log
		ERROR_ENTRY_NODEF(113),
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
		ERROR_ENTRY(202,"incorrect id's declaration syntax"),	//id
		ERROR_ENTRY(203,"id must be declarated, or wrong declaration was appeard"),	//id
		ERROR_ENTRY(204,"lexic error"),	//lex
		ERROR_ENTRY(205,"must be closing bracket"),	//poland useless
		ERROR_ENTRY(206,"incorrect lexem in expression"),	//poland	 useless
		ERROR_ENTRY(207,"operation wasn't defined"),	//poland useless
		ERROR_ENTRY(208,"wrong type of literal"),	//id useless
		ERROR_ENTRY(209,"double declaration"),	//id
		ERROR_ENTRY(210,"unexpected bracket"),	//generation useless
		ERROR_ENTRY(211,"using of addres, instead of a value"),	//semnt
		ERROR_ENTRY(212,"there must be opening bracket"),	//semnt
		ERROR_ENTRY(213,"invalid array using"),	//semnt, id
		ERROR_ENTRY(214,"empty string"),	// id
		ERROR_ENTRY(215,"errors count reached the limit"),	// lex, id, smn
		ERROR_ENTRY(216,"global identificators and literals are forbidden"),	// id
		ERROR_ENTRY(217,"parametrs count reached the limit"),	// id
		ERROR_ENTRY(218,"functions count reached the limit"),	// id
		ERROR_ENTRY(219,"literal size reached the limit"),	// lex
		ERROR_ENTRY(220,"variable or parametr can't have type void"),	// id
		ERROR_ENTRY(221,"unexpected break"),	// generation
		ERROR_ENTRY(222,"main can be only one"),	// smn
		ERROR_ENTRY(223,"main must be declaired"),	// smn
		ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY_NODEF100(400),
		ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600,"incorrect structure of programm"),
		ERROR_ENTRY(601,"error in body structure"),
		ERROR_ENTRY(602,"error in expression"),
		ERROR_ENTRY(603,"error in expression"),
		ERROR_ENTRY(604,"error in declared function's parametors"),
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