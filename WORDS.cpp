#include <iostream>
#include "WORDS.h"

namespace WORDS {
	//скапіраваць радок без кавычак
	void StringIDCopy(char *dest, char *res) {
		int i = 0;//res
		int j = 0;//dest
		while (*(res + i) != '\0') {
			if (*(res + i) != '\"') {
				*(dest + j) = *(res + i);
				j++;
			}
			i++;
		}
		*(dest + j) = '\0';
	}
	//скапіраваць радок
	void StringCopy(char *dest, char *res) {
		int i=0;
		while (*(res + i) != '\0') {
			*(dest+i) = *(res + i);
			i++;
		}
		*(dest + i) = '\0';
	}//скапіраваць радок
	void StringCopy(char *dest, std::string res) {
		int i = 0;
		while (res[i] != '\0') {
			*(dest + i) = res[i];
			i++;
		}
		*(dest + i) = '\0';
	}

	//атрымаць слова з тэкста
	char *GetWord(char *res, int position) {
		char *dest;
		int i=0;
		bool kavichka = false;
		bool kavichka2 = false;
		while ((res[position + i] != ' ' &&
			res[position + i] != ',' &&
			res[position + i] != '.' &&
			res[position + i] != ';' &&
			res[position + i] != ':' &&
			res[position + i] != '(' &&
			res[position + i] != ')' &&
			res[position + i] != '{' &&
			res[position + i] != '}' &&
			res[position + i] != '[' &&
			res[position + i] != ']' &&
			res[position + i] != '=' &&
			res[position + i] != '+' &&
			res[position + i] != '-' &&
			res[position + i] != '/' &&
			res[position + i] != '*' &&
			res[position + i] != '^' &&
			res[position + i] != '>' &&
			res[position + i] != '!' &&
			res[position + i] != '<' &&
			res[position + i] != '&' &&
			res[position + i] != '|' &&
			res[position + i] != '?' &&
			res[position + i] != '%' &&
			res[position + i] != '\n' &&
			res[position + i] != '\0') || (kavichka==true && res[position + i] != '\"') 
			|| (kavichka2 == true && res[position + i] != '\'')) {
			if (res[position + i] == '\"') {
				if (kavichka == true) {
					i++;
					break;
				}
				kavichka = true;
			}else if (res[position + i] == '\'') {
				if (kavichka2 == true) {
					i++;
					break;
				}
				kavichka2 = true;
			}
			i++;
		}
		if (i == 0) {
			dest = new char[i+1];
			dest[i] = res[position + i];
			dest[i+1] = '\0';
		}else {
			dest = new char[i];
			dest[i] = '\0';
		}
		i--;
		while (i >= 0) {
			dest[i]=res[position+i--];
		}
		return dest;
	}


	int LengthWord(char *res) {
		int i = 0;
		while (res[i] != ' ' &&
			res[i] != ',' &&
			res[i] != '.' &&
			res[i] != ';' &&
			res[i] != ':' &&
			res[i] != '(' &&
			res[i] != ')' &&
			res[i] != '{' &&
			res[i] != '}' &&
			res[i] != '[' &&
			res[i] != ']' &&
			res[i] != '=' &&
			res[i] != '+' &&
			res[i] != '-' &&
			res[i] != '/' &&
			res[i] != '*' &&
			res[i] != '^' &&
			res[i] != '\0') {
			i++;
		}
		return i;
	}

	int LengthString(char *res) {
		int i = 0;
		while (res[i] != '\0') {
			i++;
		}
		return i;
	}

	//параўнаць радкі
	bool StringCompare(char* str1, char *str2) {
		int i=0;
		while (str1[i] != '\0' || str2[i] != '\0') {
			if (str1[i] != str2[i]) {
				return false;
			}
			i++;
		}
		return true;
	}

	bool StringCompare(std::string str1, std::string str2){
		int i = 0;
		while (str1[i] != '\0' || str2[i] != '\0') {
			if (str1[i] != str2[i]) {
				return false;
			}
			i++;
		}
		return true;
	}
	//Абрэзаць слова
	void CutWord(char* str1, int size) {
		int i=0;
		while (str1[i] != '\0' && i < size) {
			i++;
		}
		str1[i]='\0';
	}
}