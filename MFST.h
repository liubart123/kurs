#pragma once
#include <iostream>
#include <stack>
#include "GRB.h"
#include "LT.h"
#define MFST_DAIGN_NUMBER 10	//колькасць ды€гностык
typedef std::stack<short> MFSTSTACK;
namespace MFST {
	struct MfstState {
		short nrule;		//нумар ц€кучага прав≥ла
		short lenta_position;	//паз≥цы€ на стужцы
		short nrulechain;	//нумар ланцужка
		MFSTSTACK st;	//стэк
		MfstState(short pposition, MFSTSTACK pst, short pnurelchain, short pnrule) {
			lenta_position=pposition;
			nrulechain=pnurelchain;
			st=pst;
			nrule = pnrule;
		}
	};

	struct Mfst {
		enum ENUM_STEP {
			NS_OK,		//знойдзена прав≥ла ≥ ланужок зап≥саны Ґ стэк
			NS_NORULE,	//не знойдзена прав≥ла, памылка граматык≥
			NS_NORULECHAIN,	//не знойдзены падыходз€чы ланужок, памылка у зыходным кодзе
			NS_ERROR,	//нев€домы нетэрм≥нальны с≥мвал граматык≥
			TS_OK,		//ц€кучы с≥мвал == в€ршын≥ стэка, стужка ≥ стэк адсунул≥ паз≥цыю
			TS_NOK,		//ц€кучы с≥мвал != в€ршын≥ стэка, стан адноҐлены
			LENTA_END,	//ц€куча€ паз≥цы€ >= памеру  стужк≥
			SURPRISE	//нечаканы код в€ртанн€
		};

		GRBHALPHABET* lenta;	//стужка з лексем
		short lenta_position;	//ц€куча€ паз≥цы€ на стужцы
		short nrule;		//нумар ц€кучага прав≥ла
		short nrulechain;		//нумар ц€кучага ланцужка
		short lenta_size;	//памер стужк≥
		GRB::Greibach greibach;		//граматыка
		LT::LexTable lexTable;	//табл≥ца лексем	
		MFSTSTACK st;	//стэк
		std::stack<MfstState> statesStack;	//стэк, €к≥ захоҐвае м≥нулы€ станов≥шчы аҐтамата

		ENUM_STEP step();	//зраб≥ць крок
		bool saveState();	//захаваь стан аҐтамата
		bool resState();	//аднав≥ць станов≥шча 
		bool pushChain(GRB::Rule::Chain chain);	//пакласц≥ у стэк ланцужок
		bool start();	//пачаць работу аҐтамата
		void StateInfo();	//≥нфармацы€ аб стане аҐтамата
		bool isTopStateEqual();	//ц≥ аднолькавы€ в€ршын€ стэка ≥ ц€перашн≥ стан аҐтамата

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
			short lenta_position = -1;	//паз≥цы€ на стужцы
			ENUM_STEP step = ENUM_STEP::SURPRISE;	//код зав€ршэнн€ кроку
			short nrule = -1;
			short nrulechain = -1;
			MfstDiagnosis(){
			};
			MfstDiagnosis(short lenta_position, ENUM_STEP step, short nrule, short nrulechain);
		} diagnosis[MFST_DAIGN_NUMBER];


		void pushNewDiagnosis(Mfst::ENUM_STEP step, int chain, int rule);
		void printLastDiagnosis();
	};
	//вывесц≥ прав≥ла у кансоль
	void printRule(GRB::Rule rule, GRB::Rule::Chain chain);
	void printRule(GRBHALPHABET nt, GRB::Rule::Chain chain);
	std::string getStrRule(GRB::Rule rule, GRB::Rule::Chain nchain);

	void PrintMessage(std::string str);
	void PrintMessage(char* str);

	void printError(Mfst::ENUM_STEP e);	//вывесц≥ у кансоль памылку
}