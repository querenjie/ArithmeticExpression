#pragma once
#include <iostream>
#include <list>
#include "LexicalAnalysisWordAndType.h"

using namespace std;

/**
*	语法分析阶段要处理的事情。本语法分析过程采用的是有递归的预测分析
*	原始的贴近人类思维的文法产生式：
*		E->E+F|E-F|E*F|E/F|F
*		F->(E)|i
*	很容易得出消除左递归后的文法产生式定义如下：
*		E->FE'
*		E'->+FE'|-FE'|*FE'|/FE'|空
*		F->(E)|i
*	其中非终结符E是表达式，非终结符F是表达式中的因子。
*	终结符：{'+', '-', '*', '/', '(', ')'，'i'}，其中i是操作数。
*	开始符是E
*/
class SyntaxAnalysisPhase {
private:
	void E();
	void E_();			//E_就是E'，因为方法名不能有'''，所以用’_'表示'''。
	void F();

	string* wordPtr;				//保存词法分析结果中的词到这个string数组中。其中去掉了"="，因为这个并未在语法中定义。
	string* typePtr;				//对应wordPtr中的每个词的类型。
	int lengthOfWordAndTypePtr;		//wordPtr或typePtr中的元素个数

	int currentIndex = 0;			//当前读取wordPtr的索引位置
	bool passedSyntaxCheck = true;	//是否通过语法检测

	/**
	*	判定表达式中的当前字符串是否匹配文法产生式的规范。
	*	如果不匹配则会标志passedSyntaxCheck为false。
	*	参数token是文法产生式中的某个终结符
	*/
	void match(string token);
public:
	/**
	*	构造函数。将词法分析的结果作为入参传进来。去除'='，词和属性分别保存到wordPtr和typePtr这两个数组中。
	*	wordPtr和typePtr这两个数组的相同位置的元素有对应关系，分别是某个单词和对应的词性。
	*/
	SyntaxAnalysisPhase(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);
	~SyntaxAnalysisPhase();

	bool checkSyntax();		//检查语法
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
*	E'->+FE'|-FE'|*FE'|/FE'|空
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
			cout << "matched：" << currentWord << endl;
			this->currentIndex++;
		}
		else {
			this->passedSyntaxCheck = false;
			cout << "mismatched word at offset " << (this->currentIndex + 1) << " with content " << currentWord << endl;
		}
	}
	else {
		if (currentWord == token) {
			cout << "matched：" << currentWord << endl;
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

