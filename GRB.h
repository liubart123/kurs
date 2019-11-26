#pragma once
#include "Error.h"
#define EMPTY '#'
typedef short GRBHALPHABET;	//������ ���������
namespace GRB {
	struct Rule {	//������ � ���������
		GRBHALPHABET nn;	//���������(���� �����)
		int idError;	
		short size;	//��������� ������ ������ ������
		struct Chain {	//������ ������ ������
			short size;		//������
			GRBHALPHABET* nt;	//������ �������� � ����������
			Chain() {size=0;nt=0;};
			Chain(short psize, GRBHALPHABET s, ...);
			char* getCChain();	//�������� ������ ������ ������
			static GRBHALPHABET T(char t){return GRBHALPHABET(t);};	//�������
			static GRBHALPHABET N(char n) { return -GRBHALPHABET(n);};	//�� �������
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
		char *getCRule(short nchain);	//�������� ������ � �������� �����
		short getNextChain(
			GRBHALPHABET t, Rule::Chain& pchain, short j
		);	//�������� �������� �� j ������ ��� -1 
	};
	struct Greibach {
		short size;		//��������� �����
		GRBHALPHABET startN;	//�������� �����
		GRBHALPHABET stbottomT;	//��� �����
		Rule* rules;//������
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
		);	//������� ����� ������, ����� -1
		Rule getRule(short n);	//������� ������ � ������
	};
	Greibach getGreibach();
}
