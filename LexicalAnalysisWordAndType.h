#pragma once
#include <iostream>
#include <string>
using namespace std;

class LexicalAnalysisWordAndType {
private:
	string word;		//����
	string type;		//����

public:
	void setWord(string word) {
		this->word = word;
	}

	void setType(string type) {
		this->type = type;
	}

	string getWord() {
		return this->word;
	}

	string getType() {
		return this->type;
	}
};