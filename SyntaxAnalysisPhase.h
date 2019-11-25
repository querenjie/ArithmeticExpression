#pragma once
#include <iostream>
#include <list>
#include "LexicalAnalysisWordAndType.h"

using namespace std;

/**
*	�﷨�����׶�Ҫ��������顣���﷨�������̲��õ����еݹ��Ԥ�����
*	ԭʼ����������˼ά���ķ�����ʽ��
*		E->E+F|E-F|E*F|E/F|F
*		F->(E)|i
*	�����׵ó�������ݹ����ķ�����ʽ�������£�
*		E->FE'
*		E'->+FE'|-FE'|*FE'|/FE'|��
*		F->(E)|i
*	���з��ս��E�Ǳ��ʽ�����ս��F�Ǳ��ʽ�е����ӡ�
*	�ս����{'+', '-', '*', '/', '(', ')'��'i'}������i�ǲ�������
*	��ʼ����E
*/
class SyntaxAnalysisPhase {
private:
	void E();
	void E_();			//E_����E'����Ϊ������������'''�������á�_'��ʾ'''��
	void F();

	string* wordPtr;				//����ʷ���������еĴʵ����string�����С�����ȥ����"="����Ϊ�����δ���﷨�ж��塣
	string* typePtr;				//��ӦwordPtr�е�ÿ���ʵ����͡�
	int lengthOfWordAndTypePtr;		//wordPtr��typePtr�е�Ԫ�ظ���

	int currentIndex = 0;			//��ǰ��ȡwordPtr������λ��
	bool passedSyntaxCheck = true;	//�Ƿ�ͨ���﷨���

	/**
	*	�ж����ʽ�еĵ�ǰ�ַ����Ƿ�ƥ���ķ�����ʽ�Ĺ淶��
	*	�����ƥ������־passedSyntaxCheckΪfalse��
	*	����token���ķ�����ʽ�е�ĳ���ս��
	*/
	void match(string token);
public:
	/**
	*	���캯�������ʷ������Ľ����Ϊ��δ�������ȥ��'='���ʺ����Էֱ𱣴浽wordPtr��typePtr�����������С�
	*	wordPtr��typePtr�������������ͬλ�õ�Ԫ���ж�Ӧ��ϵ���ֱ���ĳ�����ʺͶ�Ӧ�Ĵ��ԡ�
	*/
	SyntaxAnalysisPhase(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);
	~SyntaxAnalysisPhase();

	bool checkSyntax();		//����﷨
};

SyntaxAnalysisPhase::SyntaxAnalysisPhase(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList) {
	int length = lexicalAnalysisResultList.size();
	string *wordPtr = new string[length];
	string* typePtr = new string[length];
	int i = 0;
	for (list<LexicalAnalysisWordAndType>::iterator iter = lexicalAnalysisResultList.begin(); iter != lexicalAnalysisResultList.end(); iter++) {
		if (iter->getWord() != "=") {
			wordPtr[i] = iter->getWord();
			typePtr[i] = iter->getType();
			i++;
		}
	}
	this->wordPtr = wordPtr;
	this->typePtr = typePtr;
	this->lengthOfWordAndTypePtr = i;
	
}

SyntaxAnalysisPhase::~SyntaxAnalysisPhase() {

}

/**
*	E->FE'
*/
void SyntaxAnalysisPhase::E() {
	F();
	E_();
}

/**
*	E'->+FE'|-FE'|*FE'|/FE'|��
*/
void SyntaxAnalysisPhase::E_() {
	if (this->currentIndex >= this->lengthOfWordAndTypePtr) {
		return;
	}
	string currentWordInWordPtr = this->wordPtr[this->currentIndex];

	if (currentWordInWordPtr == "+") {
		this->match("+");
		F();
		E_();
	} else if (currentWordInWordPtr == "-") {
		this->match("-");
		F();
		E_();
	}
	else if (currentWordInWordPtr == "*") {
		this->match("*");
		F();
		E_();
	}
	else if (currentWordInWordPtr == "/") {
		this->match("/");
		F();
		E_();
	}
}

/**
*	F->(E)|i
*/
void SyntaxAnalysisPhase::F() {
	if (this->currentIndex >= this->lengthOfWordAndTypePtr) {
		return;
	}
	
	if (this->typePtr[this->currentIndex] == "OPERAND") {
		this->match("i");
	}
	else if (this->wordPtr[this->currentIndex] == "(") {
		this->match("(");
		E();
		if (this->wordPtr[this->currentIndex] == ")") {
			this->match(")");
		}
		else {
			cout << "Syntax Error, at word offset of " << (this->currentIndex + 1) << ", lack of word \")\" " << endl;
			this->passedSyntaxCheck = false;
		}
	}
	else {
		cout << "Syntax Error, at word offset of " << (this->currentIndex + 1) << " with content " << this->wordPtr[this->currentIndex] << endl;
		this->passedSyntaxCheck = false;
	}

}

void SyntaxAnalysisPhase::match(string token) {
	string currentWord = this->wordPtr[currentIndex];
	string currentType = this->typePtr[currentIndex];
	if (token == "i") {
		if (currentType == "OPERAND") {
			cout << "matched��" << currentWord << endl;
			this->currentIndex++;
		}
		else {
			this->passedSyntaxCheck = false;
			cout << "mismatched word at offset " << (this->currentIndex + 1) << " with content " << currentWord << endl;
		}
	}
	else {
		if (currentWord == token) {
			cout << "matched��" << currentWord << endl;
			this->currentIndex++;
		}
		else {
			this->passedSyntaxCheck = false;
			cout << "mismatched word at offset " << (this->currentIndex + 1) << " with content " << currentWord << endl;
		}
	}
}

bool SyntaxAnalysisPhase::checkSyntax() {
	E();
	if (this->currentIndex < this->lengthOfWordAndTypePtr) {
		this->passedSyntaxCheck = false;
		cout << "Syntax error occured at word offset of " << (this->currentIndex + 1) << " with word content " << this->wordPtr[this->currentIndex] << endl;
	}
	return this->passedSyntaxCheck;
}

