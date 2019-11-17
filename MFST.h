#pragma once
#include <iostream>
#include <stack>
#include "GRB.h"
#include "LT.h"
#define MFST_DAIGN_NUMBER 10	//��������� ����������
typedef std::stack<short> MFSTSTACK;
namespace MFST {
	struct MfstState {
		short nrule;		//����� �������� ������
		short lenta_position;	//������ �� ������
		short nrulechain;	//����� ��������
		MFSTSTACK st;	//����
		MfstState(short pposition, MFSTSTACK pst, short pnurelchain, short pnrule) {
			lenta_position=pposition;
			nrulechain=pnurelchain;
			st=pst;
			nrule = pnrule;
		}
	};

	struct Mfst {
		enum ENUM_STEP {
			NS_OK,		//��������� ������ � ������� ������� � ����
			NS_NORULE,	//�� ��������� ������, ������� ��������
			NS_NORULECHAIN,	//�� ��������� ����������� �������, ������� � �������� �����
			NS_ERROR,	//�������� ������������ ����� ��������
			TS_OK,		//������ ����� == ������ �����, ������ � ���� ������� ������
			TS_NOK,		//������ ����� != ������ �����, ���� ��������
			LENTA_END,	//������� ������ >= ������  �����
			SURPRISE	//�������� ��� ��������
		};

		GRBHALPHABET* lenta;	//������ � ������
		short lenta_position;	//������� ������ �� ������
		short nrule;		//����� �������� ������
		short nrulechain;		//����� �������� ��������
		short lenta_size;	//����� �����
		GRB::Greibach greibach;		//���������
		LT::LexTable lexTable;	//������ ������	
		MFSTSTACK st;	//����
		std::stack<MfstState> statesStack;	//����, �� ������� ������ ��������� �������

		ENUM_STEP step();	//������ ����
		bool saveState();	//������� ���� �������
		bool resState();	//������� ��������� 
		bool pushChain(GRB::Rule::Chain chain);	//�������� � ���� ��������
		bool start();	//������ ������ �������
		void StateInfo();	//���������� �� ����� �������
		bool isTopStateEqual();	//�� ����������� ������� ����� � �������� ���� �������

		Mfst(LT::LexTable table, GRB::Greibach gr) {
			lexTable=table;
			greibach=gr;
			st.push(gr.stbottomT);
			st.push(gr.startN);
			char* clenta = LT::PrintTable(table);
			int psize = table.size;
			GRBHALPHABET* plenta=new GRBHALPHABET[psize];
			for (int i = 0; i < psize; i++) {
				plenta[i]=GRB::Rule::Chain::T(clenta[i]);
			}
			lenta_size = table.size;
			lenta=plenta;
			lenta_position=0;
		}

		struct MfstDiagnosis {
			short lenta_position = -1;	//������ �� ������
			ENUM_STEP step = ENUM_STEP::SURPRISE;	//��� ���������� �����
			short nrule = -1;
			short nrulechain = -1;
			MfstDiagnosis(){
			};
			MfstDiagnosis(short lenta_position, ENUM_STEP step, short nrule, short nrulechain);
		} diagnosis[MFST_DAIGN_NUMBER];


		void pushNewDiagnosis(Mfst::ENUM_STEP step, int chain, int rule);
		void printLastDiagnosis();
	};
	//������� ������ � �������
	void printRule(GRB::Rule rule, GRB::Rule::Chain chain);
	void printRule(GRBHALPHABET nt, GRB::Rule::Chain chain);
	std::string getStrRule(GRB::Rule rule, GRB::Rule::Chain nchain);

	void PrintMessage(std::string str);
	void PrintMessage(char* str);

	void printError(Mfst::ENUM_STEP e);	//������� � ������� �������
}