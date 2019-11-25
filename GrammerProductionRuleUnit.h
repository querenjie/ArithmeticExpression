#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
	本类封装文法中的一个产生式，例如：E->FE'
	调用的方式：
		GrammerProductionRuleUnit gpru("E", "F E'");
*/
class GrammerProductionRuleUnit {
private:
	string leftsideVn;		//产生式左边的非终结符。必须注意的是左边只有一个非终结符，并且没有其他终结符或非终结符。
	vector<string> rightsideVnVtVector;		//产生式右边的终结符或非终结符的集合，存放是按照顺序存放的。

public:
	/*
		构造函数
		第一个参数：	产生式的左边的唯一的非终结符。必须只有这么一个非终结符，不能有其他的元素存在。
		第二个参数：	产生式右边的表达式，其中的各符号之间用一个空格隔开，以便识别终结符。整个表达式是一个字符串。
		作用：			将第一个参数直接赋值给私有属性leftsideVn，第二个参数要根据空格切分字符串，取出其中的各个符号，依次放入私有属性rightsideVnVts数组中，同时计数元素个数保存到私有属性lengthOfRightsideVnVtsArray中。
	*/
	GrammerProductionRuleUnit(string leftsideVn, string rightsideVnVtStatement);
	
	~GrammerProductionRuleUnit();

	string getLeftsideVn();		//获取产生式左边的非终结符。

	vector<string> getRightsideVnVtVector();	//获取产生式右边的终结符或非终结符的集合
};

/*
	string leftsideVn：	产生式的左边的唯一的非终结符。必须只有这么一个非终结符，不能有其他的元素存在。
	string rightsideVnVtStatement： 产生式右边的表达式，其中的各符号之间用一个空格隔开，以便识别终结符。整个表达式是一个字符串。例如：F E’
*/
GrammerProductionRuleUnit::GrammerProductionRuleUnit(string leftsideVn, string rightsideVnVtStatement) {
	this->leftsideVn = leftsideVn;

	//以下代码是对右边的表达式进行切分符号，依次把符号写入rightsideVnVtVector
	int lengthOfRightsideVnVtStatement = rightsideVnVtStatement.size();
	string currentSymbol = "";
	for (int i = 0; i < lengthOfRightsideVnVtStatement; i++) {
		char c = rightsideVnVtStatement.at(i);
		if (c != ' ') {
			currentSymbol = currentSymbol.append(1, c);
		}
		else {
			if (currentSymbol != "") {
				this->rightsideVnVtVector.push_back(currentSymbol);
				currentSymbol = "";
			}
		}
	}
	if (currentSymbol != "") {
		this->rightsideVnVtVector.push_back(currentSymbol);
		currentSymbol = "";
	}

}

GrammerProductionRuleUnit::~GrammerProductionRuleUnit() {

}

string GrammerProductionRuleUnit::getLeftsideVn() {
	return this->leftsideVn;
}

vector<string> GrammerProductionRuleUnit::getRightsideVnVtVector() {
	return this->rightsideVnVtVector;
}