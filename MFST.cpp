#include "MFST.h"
#include "GRB.h"
#include "Log.h"
#include <iostream>
#include <stack>
#include <string>
#include <iomanip>

namespace MFST {
	//крок
	Mfst::ENUM_STEP Mfst::step() {
		StateInfo();
		//праверка, ц≥ знаходз≥цца у в€ршын≥  стэка тэрм≥нальны с≥мвал
		if (GRB::Rule::Chain::isT(st.top()) == true) {
			if (st.top() == lenta[lenta_position]) {
				lenta_position++;
				st.pop();
				return Mfst::ENUM_STEP::TS_OK;
			}
			else {
				//pushNewDiagnosis(Mfst::ENUM_STEP::NS_NORULECHAIN);
				return Mfst::ENUM_STEP::TS_NOK;
			}
		}
		GRB::Rule rule;
		int oldRule = nrule;
		nrule = greibach.getRule(st.top(), rule);	//знаходз≥м прав≥ла
		GRB::Rule::Chain chain;

		//прав≥ла знойдзена
		if (nrule != greibach.size && nrule!=-1) {
			//выбар ланцужка
			int oldChain = nrulechain;
			if (statesStack.empty()==true || isTopStateEqual()==false) {
				nrulechain = rule.getNextChain(lenta[lenta_position], chain, -1);
			}
			else {
				nrulechain = rule.getNextChain(lenta[lenta_position], chain, statesStack.top().nrulechain);	
				//знаходз≥м правую чатску прав≥ла
				statesStack.pop();
			}
			
			if (nrulechain != -1) {
				printRule(rule,chain);
				saveState();
				st.pop();
				pushChain(chain);
				return Mfst::ENUM_STEP::NS_OK;
			}
			else {
				pushNewDiagnosis(Mfst::ENUM_STEP::NS_NORULECHAIN, oldChain, oldRule);
				return Mfst::ENUM_STEP::NS_NORULECHAIN;
			}
		}
		else {
			pushNewDiagnosis(Mfst::ENUM_STEP::NS_NORULECHAIN, -1, oldRule);
			return Mfst::ENUM_STEP::NS_NORULE;
		}


		return Mfst::ENUM_STEP::LENTA_END;
	}

	//захаваць стан
	bool Mfst::saveState(){
		statesStack.push(*new MFST::MfstState(lenta_position, st, nrulechain, nrule));
		cout << "\n-------------------------------------State was saved: " << statesStack.size() << "\n";
		return false;	
	}

	//аднав≥ць апошн≥ стан
	bool Mfst::resState() {
		if (statesStack.empty()) {
			return false;
		}
		MfstState state = statesStack.top();
		lenta_position = state.lenta_position;
		nrulechain = state.nrulechain;
		nrule = state.nrule;
		st = state.st;
		cout << "\n-------------------------------------State was reseted: " << statesStack.size() << "\n";
		return true;
	}

	//зак≥нуць у стэк правую частку прав≥ла
	bool Mfst::pushChain(GRB::Rule::Chain chain) {
		for (int a = chain.size - 1; a >= 0; a--) {
			st.push(chain.nt[a]);
		}
		return false;
	}

	//пачаць работу аҐтамата
	bool Mfst::start() {
		bool cycle = true;
		//statesStack.push(*new MFST::MfstState(0,st,0));
		Mfst::ENUM_STEP result;
		bool Syntax = true;
		int countOfCycles = 0;
		while (cycle) {
			result = step();
			switch (result)
			{
				case MFST::Mfst::NS_OK:
					break;
				case MFST::Mfst::NS_NORULE:
					printError(result);
					cycle = false;
					break;
				case MFST::Mfst::NS_NORULECHAIN:
					printError(result);
					if (!resState()) {
						cycle = false;
						Syntax = false;
					}
					//cycle = false;
					break;
				case MFST::Mfst::NS_ERROR:
					cycle = false;
					break;
				case MFST::Mfst::TS_OK:
					if (lenta_position == lenta_size) {
						cycle = false;
						Syntax = true;
					}
					//cycle = false;
					break;
				case MFST::Mfst::TS_NOK:
					printError(result);
					if (!resState()) {
						cycle = false;
						Syntax = false;
					}
					//cycle = false;
					break;
				case MFST::Mfst::LENTA_END:
					cycle = false;
					break;
				case MFST::Mfst::SURPRISE:
					cycle = false;
					break;
				default:
					break;
			}
			countOfCycles++;
			PrintMessage(":");
			PrintMessage(to_string(countOfCycles));
		}
		if (Syntax == true) {
			PrintMessage((char*)"\nYes!!! Count of cycles:");
			PrintMessage(to_string(countOfCycles));
			while (statesStack.empty() == false) {
				printRule(greibach.getRule(statesStack.top().nrule), greibach.getRule(statesStack.top().nrule).chains[statesStack.top().nrulechain]);
				statesStack.pop();
			}
			return true;
		}
		else {
			printLastDiagnosis();
		}
		return cycle;
	}
	
	//вывесц≥ радок аб прав≥ле
	void printRule(GRB::Rule rule , GRB::Rule::Chain nchain) {
		std::cout << "\n-------------------------------------" << GRB::Rule::Chain::alphabet_to_char(rule.nn);
		std::cout << " -> ";
		std::cout << nchain.getCChain();
	}
	void printRule(GRBHALPHABET nt, GRB::Rule::Chain nchain){
		std::cout << "\n-------------------------------------" << GRB::Rule::Chain::alphabet_to_char(nt);
		std::cout << " -> ";
		std::cout << nchain.getCChain();
	}
	std::string getStrRule(GRB::Rule rule, GRB::Rule::Chain nchain) {
		std::string str;
		str+= GRB::Rule::Chain::alphabet_to_char(rule.nn);
		str+= " -> ";
		str+= nchain.getCChain();
		return str;
	}
	//≥нфармацы€ аб станов≥шч
	void Mfst::StateInfo(){
		int length = 15;
		char *str = new char[length + 1];
		MFSTSTACK st2 = st;
		int i;
		cout << '\n';
		for (i = 0; i < length && i < lenta_size - lenta_position; i++) {
			str[i] = GRB::Rule::Chain::alphabet_to_char(lenta[lenta_position+i]);
		}
		str[i] = '\0';
		cout << setw(length) << str;
		cout << '\t';
		for (i = 0; st2.empty() == false && i < length; i++) {
			str[i] = GRB::Rule::Chain::alphabet_to_char(st2.top());
			st2.pop();
		}
		str[i] = '\0';
		cout << setw(length) << str << '\n';
	}	//≥нфармацы€ аб стане аҐтамата

	//ц≥ аднолькавы€ в€ршын€ стэка ≥ ц€перашн≥ стан аҐтамата
	bool Mfst::isTopStateEqual() {
		if (statesStack.top().lenta_position == lenta_position
			&& statesStack.top().st == st
			&& statesStack.top().nrulechain == nrulechain
			&& statesStack.top().nrule == nrule) {
			return true;
		};
		return false;
	}

	void PrintMessage(char* str) {
		std::cout << str;
	}
	void PrintMessage(string str) {
		std::cout << str;
	}

		//вывесц≥ у кансоль памылку
	void printError(Mfst::ENUM_STEP e){
		if (e == Mfst::ENUM_STEP::NS_NORULE) {
			std::cout << "\n-------------------------------------" << "No rule";
		} else if (e == Mfst::ENUM_STEP::NS_NORULECHAIN) {
			std::cout << "\n-------------------------------------" << "No chain";
		}
		else if (e == Mfst::ENUM_STEP::TS_NOK) {
			std::cout << "\n-------------------------------------" << "incorrect symbol";
		}
		else if (e == Mfst::ENUM_STEP::NS_NORULE) {
			std::cout << "\n-------------------------------------" << "No rule";
		}
		else if (e == Mfst::ENUM_STEP::NS_NORULE) {
			std::cout << "\n-------------------------------------" << "No rule";
		}
	}

	Mfst::MfstDiagnosis::MfstDiagnosis(short lenta_position, ENUM_STEP step, short nrule, short nrulechain) {
		this->lenta_position=lenta_position;
		this->nrule=nrule;
		this->nrulechain=nrulechain;
		this->step=step;
	}   

	//захаваць новую ды€гностыку
	void Mfst::pushNewDiagnosis(Mfst::ENUM_STEP step, int chain, int rule) {
		PrintMessage("\n-------------------------------------diagnosesWasSaved");
		int k=0;
		while (k < MFST_DAIGN_NUMBER && lenta_position <= diagnosis[k].lenta_position) {
			k++;
		}
		if (k < MFST_DAIGN_NUMBER) {
			diagnosis[k] = MfstDiagnosis(lenta_position,step,rule, chain);
			for (int j = k + 1; j < MFST_DAIGN_NUMBER; j++) {
				diagnosis[j].lenta_position=-1;
			}
		}
	}

	//вывесц≥ апошн≥€ ды€гностык≥
	void Mfst::printLastDiagnosis() {
		int k=0;
		while (k < MFST_DAIGN_NUMBER && diagnosis[k].lenta_position != -1) {
			if (diagnosis[k].nrule < 0 || diagnosis[k].nrulechain < -1) {
				return;
			}
			if (diagnosis[k].nrulechain == -1) {
				return;
			}
			std::string str="\n";
			str+="line: ";
			str+=std::to_string(LT::GetEntry(lexTable, diagnosis[k].lenta_position)->line);
			str+=", ";
			str+=Error::geterror(greibach.getRule(diagnosis[k].nrule).idError).message;
			
			
			PrintMessage((char*)(&str[0]));
			k++;
		}
	}
}