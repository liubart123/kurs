#include "LT.h"
#include "IT.h"
#include <iostream>
#include <stack>
#include "Error.h"
#include <list>
#define NUM_OF_PRIR_LEVEL 8	//��������� ������ ����������
#define NUM_OF_OPER_IN_LEVEL 4	//��������� ������ �� ������ ����� ����������
#define SPEC_SUMBOL '@'	//����� �������
#define SPEC_ARR_SUMBOL ':'	//����� �������
#define FILL_SYMBOL '#'	//����� ���������
namespace Poland {

	//������� ��������� ��������, ����� -1, ��� �������� �� ���������
	int GetPriorityOfOperation(LT::Entry c);
	//�������� �������
	bool PolishNotation(int *lexTable_pos, LT::LexTable *lexTable, IT::IdTable *idTable, char* text);
}