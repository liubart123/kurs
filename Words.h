namespace WORDS{
	void StringCopy(char *dest, char *res);
	void StringIDCopy(char *dest, char *res);
	
	char *GetWord(char *res, int position);
	bool StringCompare(char* str1, char *str2);
	bool StringCompare(std::string str1, std::string str2);
	void CutWord(char* str1, int size);
	int LengthWord(char *res);
	void StringCopy(char *dest, std::string res);
	//bool StringCompare(char* str1, char str2);
}