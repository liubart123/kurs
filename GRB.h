#pragma once
#include "Error.h"
#define EMPTY '#'
typedef short GRBHALPHABET;	//сімвалы альфабэта
namespace GRB {
	struct Rule {	//правіла у граматыцы
		GRBHALPHABET nn;	//нэтэрмінал(левы сімвал)
		int idError;	
		short size;	//колькасць правых частваў правіла
		struct Chain {	//правая частва правіла
			short size;		//даўжыня
			GRBHALPHABET* nt;	//ланцуг тэрміналаў і нетэрміналаў
			Chain() {size=0;nt=0;};
			Chain(short psize, GRBHALPHABET s, ...);
			char* getCChain();	//атрымаць правую чатску правіла
			static GRBHALPHABET T(char t){return GRBHALPHABET(t);};	//трэмінал
			static GRBHALPHABET N(char n) { return -GRBHALPHABET(n);};	//не тэрмінал
			static bool isT(GRBHALPHABET s) { return s > 0; };
			static bool isN(GRBHALPHABET s) { return !isT(s); };
			static char alphabet_to_char(GRBHALPHABET s)
			{
				return isT(s)?char(s):char(-s);
			}
		}* chains;
		Rule() {
			nn = 0x00;
			size = 0;
		}
		Rule(GRBHALPHABET pnn, int idError, short psize, Chain c, ...);
		char *getCRule(short nchain);	//атрымаць правіла ў выглядзе радка
		short getNextChain(
			GRBHALPHABET t, Rule::Chain& pchain, short j
		);	//атрымаць наступны за j ланцуг або -1 
	};
	struct Greibach {
		short size;		//колькасць правіл
		GRBHALPHABET startN;	//стартавы сімвал
		GRBHALPHABET stbottomT;	//дно стэка
		Rule* rules;//правілы
		Greibach() {
			short size = 0;
			startN = 0;
			stbottomT = 0;
			rules = 0;
		};
		Greibach(
			GRBHALPHABET pstartN,
			GRBHALPHABET pstbottomT,
			short psize,
			Rule r, ...
		);
		short getRule(	
			GRBHALPHABET pnn,
			Rule& prule
		);	//атрмаць нумар правіла, альбо -1
		Rule getRule(short n);	//атрмаць правіла з нумара
	};
	Greibach getGreibach();
}
