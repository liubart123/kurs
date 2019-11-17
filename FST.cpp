#include <iostream>
#include "FST.h"
namespace FSTN{

	void addToArr(short num, short *arr) {
		int i = 0;
		while (arr[i] != -1) {
			if (num == arr[i]) {
				return;
			}
			i++;
		}
		arr[i] = num;
		return;
	}
	void step(FST& fst) {
		short *rstates = new short[100];
		memset(rstates, -1, sizeof(short) * 100);
		int finalPosS = 0;
		int i = 0;
		while (fst.rstates[i] != -1) {
			int j = 0;
			while (j < fst.nodes[fst.rstates[i]].n_relation) {
				if (fst.nodes[fst.rstates[i]].relations[j].symbol == fst.string[fst.position]) {
					//rstates[finalPosS] = fst.nodes[fst.rstates[i]].relations[j].nnode;
					addToArr(fst.nodes[fst.rstates[i]].relations[j].nnode, rstates);
					finalPosS++;
				}
				j++;
			}
			i++;
		}
		fst.rstates = rstates;
	};
	bool execute(FST &fst) {
		fst.rstates = new short[MAXSIZEFORCOMBINATIONS];
		memset(fst.rstates, -1, sizeof(short)*MAXSIZEFORCOMBINATIONS);
		fst.rstates[0] = 0;
		int i = 0;
		fst.position = 0;
		short lstring = strlen(fst.string);
		bool rc = true;
		for (i = 0; i < lstring && rc; i++) {
			step(fst);
			if (fst.rstates[0] == -1) {
				break;
			}
			fst.position++;
		};
		i = 0;
		while (fst.rstates[i] != fst.nstates - 1 && fst.rstates[i] != -1) {
			i++;
		}
		if (fst.rstates[i] != -1) {
			delete[] fst.rstates;
			return true;
		}
		else {
			delete[] fst.rstates;
			return false;
		}
		return false;
	}
}


