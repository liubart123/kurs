#define MAXSIZEFORRELATIONS 100
#define MAXSIZEFORCOMBINATIONS 30
namespace FSTN {
	struct RELATION {
		char symbol;
		short nnode;
		RELATION(
			char c = 0x00,
			short ns = 0) {
			this->symbol=c;
			this->nnode=ns;
		}
	};
	struct NODE {
		short n_relation;
		RELATION *relations;
		NODE(){};
		NODE(
			short n,
			RELATION rel, ...) {
			this->n_relation=n;
			relations = new RELATION[MAXSIZEFORRELATIONS];
			for (int i = 0; i < n; i++) {
				relations[i] = *(&rel + i);
			}
		}
	};
	struct FST {
		char* string;
		short position;
		short nstates;
		NODE* nodes;
		short* rstates;
		//short positionOfError = -1;
		FST(char* s, short ns, NODE n, ...) {
			nodes=new NODE[ns];
			this->nstates=ns;
			this->string=s;
			for (int i = 0; i < ns; i++) {
				this->nodes[i]=*(&n+i);
			}
		}
	};
	void addToArr(short num, short *arr);
	void step(FST& fst);
	bool execute(FST &fst);

}