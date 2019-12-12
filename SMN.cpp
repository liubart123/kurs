#include "SMN.h"

using namespace SMN;

bool SMN::SmnTest(IT::IdTable &idTable, LT::LexTable &lexTable)	//�������� ��������
{
	std::list<Error::ERROR> errors;
	for (int i = 0; i < lexTable.size; i++) {
		LT::Entry *entry = LT::GetEntry(lexTable,i);
		//��� ������� == ��
		if (entry->lexema[0] == 'i') {
			IT::Entry *idEntry = IT::GetEntry(idTable, entry->idxTI);

			//�� �����
			if (idEntry->iddatatype == IT::ARRAY
				|| idEntry->iddatatype == IT::STR
				|| idEntry->iddatatype == IT::ARRAY_STR) {
				//����� ������ ��������  ^
				if (LT::GetEntry(lexTable, i - 1)->lexema[0] != 'v'
					|| LT::GetEntry(lexTable, i - 1)->idxTI != char('^')) {
					if (idEntry->idtype == IT::IDTYPE::V || idEntry->idtype == IT::IDTYPE::P){
						//�� ���� ����� ����� '['
						if (LT::GetEntry(lexTable, i + 1)->lexema[0] != '[' ) {
							if (LT::GetEntry(lexTable, i - 1)->lexema[0] == 'v' || LT::GetEntry(lexTable, i - 1)->lexema[0] == '=') {
								errors.push_back(Error::geterrorin(212, entry->line, entry->col));
							}
							if (((LT::GetEntry(lexTable, i + 1)->lexema[0] == LEX_EQUALS || idEntry->iddatatype == IT::STR) && LT::GetEntry(lexTable, i - 1)->lexema[0] == 't')
								|| (idEntry->idtype == IT::IDTYPE::P && LT::GetEntry(lexTable, i - 1)->lexema[0] == 't')) {
								continue;
							}
							errors.push_back(Error::geterrorin(213, entry->line, entry->col));
						}
						else{
							int tempPos = i;
							while (LT::GetEntry(lexTable, tempPos)->lexema[0] != ']' && tempPos<lexTable.size) {
								tempPos++;
							}
							tempPos++;
							//��� ��� �� �������� � �� ����� ����� '=' � �� ����� ����� return
							if ((LT::GetEntry(lexTable, tempPos)->lexema[0] != ')'
								&&LT::GetEntry(lexTable, tempPos + 1)->lexema[0] != '{')
								&& LT::GetEntry(lexTable, tempPos)->lexema[0] != ','
								&& LT::GetEntry(lexTable, tempPos)->lexema[0] != '='
								&& LT::GetEntry(lexTable, i-1)->lexema[0] != 'r')
							{
								errors.push_back(Error::geterrorin(211, entry->line, entry->col));
								continue;
							}
							if (LT::GetEntry(lexTable, i - 1)->lexema[0] == 'v'
								&& LT::GetEntry(lexTable, i - 1)->idxTI != char('^')) {
								errors.push_back(Error::geterrorin(211, entry->line, entry->col));
							}
						}
					}
				}
			}

			//�� - �������
			if (idEntry->idtype == IT::F) {
				if (LT::GetEntry(lexTable, i + 1)->lexema[0] != '(') {
					errors.push_back(Error::geterrorin(212, entry->line, entry->col));
				}
			}
			
		}
	}
	if (errors.empty() == false) {
		throw errors;
	}
	return true;
}
