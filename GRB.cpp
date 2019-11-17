#include "GRB.h"
#define GRB_ERROR_SERIES 600
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

namespace GRB {
	Rule::Rule(GRBHALPHABET pnn, int idError, short psize, Chain c, ...) {
		nn=pnn;
		this->idError = idError;
		size=psize;
		int i=0;
		chains = new Chain[size];
		while (i < size) {
			chains[i]=*(&c+i);
			i++;
		}
	};
	Rule::Chain::Chain(short psize, GRBHALPHABET s, ...) {
		size=psize;
		int i=0;
		nt=new GRBHALPHABET[size];
		while (i < size) {
			nt[i]=*(&s+i*2);
			i++;
		}
	};
	char* Rule::Chain::getCChain()
	{
		int i=0;
		char *str = new char[size+1];
		while (i < size) {
			str[i]=alphabet_to_char(nt[i]);
			i++;
		}
		str[size] = '\0';
		return str;
	}//атрымаць правую чатску правіла

	
	short Greibach::getRule(GRBHALPHABET pnn,Rule& prule) {
		int i=0;
		while (i < size) {
			if (pnn == rules[i].nn) {
				prule = rules[i];
				return i;
			}
			i++;
		}
		return -1;
	}

	Rule Greibach::getRule(short n) {
		return (rules[n]);
	}

	Greibach::Greibach(GRBHALPHABET pstartN,GRBHALPHABET pstbottomT,short psize,Rule r, ...) {
		startN=pstartN;
		stbottomT=pstbottomT;
		size=psize;
		int i=0;
		rules = new Rule[size];
		while (i < size) {
			rules[i]= *(&r + i);
			i++;
		}
	}
	short Rule::getNextChain(GRBHALPHABET t, Rule::Chain& pchain, short j) {
		j++;
		while(j<size && chains[j].nt[0] != t){
			j++;
		}
		short rc = j<size?j:-1;
		if (rc >= 0) {
			pchain = chains[rc];
		}
		return rc;
	};
	Greibach greibach(NS('S'),TS('$'),6,
		Rule(NS('S'),GRB_ERROR_SERIES+0,
				4,
				Rule::Chain(8,TS('m'),TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
				Rule::Chain(14, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
				Rule::Chain(9, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
				Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'))
			),
		Rule(NS('N'), GRB_ERROR_SERIES + 1,
				10,
				Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
				Rule::Chain(6, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
				Rule::Chain(7, TS('d'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
				Rule::Chain(3, TS('r'), NS('E'), TS(';')),
				Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
				Rule::Chain(8, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
				Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
				Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
				Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
				Rule::Chain(9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N'))
			),
		Rule(NS('E'), GRB_ERROR_SERIES + 2,
				8,
				Rule::Chain(1, TS('i')),
				Rule::Chain(1, TS('l')),
				Rule::Chain(3, TS('('), NS('E'), TS(')')),
				Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
				Rule::Chain(2, TS('i'), NS('M')),
				Rule::Chain(2, TS('l'), NS('M')),
				Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
				Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
			),
		Rule(NS('M'), GRB_ERROR_SERIES + 3,
			2,
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 4,
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 5,
			4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		)
	);

	Greibach getGreibach() {
		return greibach;
	}
}
