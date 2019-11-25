#pragma once
#include <iostream>
#include <map>
#include <list>
#include "LexicalAnalysisWordAndType.h"
#include <string>
#include "GrammerProductionRuleUnit.h"
#include <vector>
#include <stack>

using namespace std;

/**
	语法分析阶段要处理的事情。本语法分析过程采用的是非递归的预测分析，用的是预测分析表。
	原始的贴近人类思维的文法产生式：
		E->E+F|E-F|E*F|E/F|F
		F->(E)|i
	很容易得出消除左递归后的文法产生式定义如下：
		E->FE'
		E'->+FE'|-FE'|*FE'|/FE'|空
		F->(E)|i
	其中非终结符E是表达式，非终结符F是表达式中的因子。
	终结符：{'+', '-', '*', '/', '(', ')'，'i'}，其中i是操作数。
	开始符是E
	
	非递归的预测分析方法需要创建预测分析表，预测分析表的创建需要先得出文法中各非终结符的First集和Follow集。
	先分析First集：
		First(F)={'(', 'i'}
		First(E')={'+', '-', '*', '/', 空}
		First(E)=First(F)={'(', 'i'}
	然后分析Follow集：(所有非终结符的Follow集都包含'=')
		Follow(E)={')', '='}
		First(E')-{空}属于Follow(F)，故Follow(F)={'+', '-', '*', '/'}
		由于E'经过0步或若干步能推导出空并且有E->FE'，所有Follow(E)都属于Follow(E')，故Follow(E')={')', '='}
	最后整理一下结果如下：
		First(E)=First(F)={'(', 'i'}
		First(E')={'+', '-', '*', '/', 空}
		Follow(E)=Follow(E')={')', '='}
		Follow(F)={'+', '-', '*', '/'}
	在构造预测分析表之前先检查一下文法是否符合LL(1)文法
		由F->(E)|i，得到First((E))交First(i)=空集
		由E'->+FE'，得到First(+FE')交Follow(E')={'+'}交{')', '='}=空集
		由E'->-FE'，得到First(-FE')交Follow(E')={'-'}交{')', '='}=空集
		由E'->-FE'，得到First(*FE')交Follow(E')={'*'}交{')', '='}=空集
		由E'->-FE'，得到First(/FE')交Follow(E')={'/'}交{')', '='}=空集
		所以文法是LL(1)文法
	构造预测分析表
		对E->FE'，因First(FE')={'(', 'i'}，故E->FE'记入M[E,(]、M[E,i]
		对E'->+FE'，因First(+FE')={'+'}，故E'->+FE'记入M[E',+]
		对E'->-FE'，因First(-FE')={'-'}，故E'->-FE'记入M[E',-]
		对E'->*FE'，因First(*FE')={'*'}，故E'->*FE'记入M[E',*]
		对E'->/FE'，因First(/FE')={'/'}，故E'->/FE'记入M[E',/]
		对E'->空，因Follow(E')={')', '='}，故E'->空记录M[E',)]、M[E',=]
		对F->(E)，显然F->(E)应记入M[F,(]
		对F->i，显然F->i应记入M[F,i]
		凡是没记入M的都是要进行错误处理的。
		预测分析表：
			_________________________________________________________________________________________________________________________________________
			|		|		i		|		+		|		-		|		*		|		/		|		（		|		）		|		=		|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	E	|	E->FE'		|				|				|				|				|	E->FE'		|				|				|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	E'	|				|	E'->+FE'	|	E'->-FE'	|	E'->*FE'	|	E'->/FE'	|				|	E'->空		|	E'->空		|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	F	|	F->i		|				|				|				|				|	F->(E)		|				|				|
			-----------------------------------------------------------------------------------------------------------------------------------------
*/
class SyntaxAnalysisPhase2 {
private:
	string* wordPtr;				//保存词法分析结果中的词到这个string数组中。其中最后的"="不要去掉。
	string* typePtr;				//对应wordPtr中的每个词的类型。
	int lengthOfWordAndTypePtr;		//wordPtr或typePtr中的元素个数
	vector<GrammerProductionRuleUnit> grammerProductionRuleVector;	//存放一系列文法产生式。每个元素都是GrammerProductionRuleUnit类型的对象，其中封装了一个产生式。
	map<string, GrammerProductionRuleUnit> predictAnalyticalMatrix;		//预测分析表，key的形式：非终结符,输入符，例如：E',+  对于操作数全部用i代替。
	vector<string> vtVector;	//存放所有的终结符，在程序中可以方便判断某个符号是否为终结符。
	vector<string> vnVector;	//存放所有的非终结符，在程序中可以方便判断某个符号是否为非终结符。

	/**
	*	重新将词法分析结果取出放到string数组中，目的是数组操作比较方便。
	*	会更改属性wordPtr、typePtr、lengthOfWordAndTypePtr的值
	*/
	void reencapsulateWordAndType(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);	

	/**
		构造文法产生式的存储表，存放在grammerProductionRuleVector中
		构造预测分析表，存放在predictAnalyticalMatrix中
	*/
	void initGrammerProductionRuleVectorAndPredictAnalyticalMatrix();

	void initVtVector();	//初始化所有终结符，存放在vtVector中。

	void initVnVector();	//初始化所有非终结符，存放在vnVector中。

	bool isVt(string s);		//判断s是否为终结符

	bool isVn(string s);		//判断s是否为非终结符
public:

	/**
	*	构造函数。将词法分析的结果作为入参传进来。
		功能：
			1、词法分析结果中去除'='，词和属性分别保存到wordPtr和typePtr这两个数组中。wordPtr和typePtr这两个数组的相同位置的元素有对应关系，分别是某个单词和对应的词性。
			2、构造文法产生式的存储表，存放在grammerProductionRulesMap中
	*/
	SyntaxAnalysisPhase2(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);

	~SyntaxAnalysisPhase2();

	bool checkSyntax();		//检查语法
};

void SyntaxAnalysisPhase2::reencapsulateWordAndType(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList) {
	int length = lexicalAnalysisResultList.size();
	string* wordPtr = new string[length];
	string* typePtr = new string[length];
	int i = 0;
	for (list<LexicalAnalysisWordAndType>::iterator iter = lexicalAnalysisResultList.begin(); iter != lexicalAnalysisResultList.end(); iter++) {
		wordPtr[i] = iter->getWord();
		typePtr[i] = iter->getType();
		i++;
	}
	this->wordPtr = wordPtr;
	this->typePtr = typePtr;
	this->lengthOfWordAndTypePtr = i;
}

void SyntaxAnalysisPhase2::initGrammerProductionRuleVectorAndPredictAnalyticalMatrix() {
	/*
		E->FE'
		E'->+FE'|-FE'|*FE'|/FE'|空
		F->(E)|i
	*/
	GrammerProductionRuleUnit grammerProductionRuleUnit01("E", "F E'");
	GrammerProductionRuleUnit grammerProductionRuleUnit02("E'", "+ F E'");
	GrammerProductionRuleUnit grammerProductionRuleUnit03("E'", "- F E'");
	GrammerProductionRuleUnit grammerProductionRuleUnit04("E'", "* F E'");
	GrammerProductionRuleUnit grammerProductionRuleUnit05("E'", "/ F E'");
	GrammerProductionRuleUnit grammerProductionRuleUnit06("E'", "nothing");
	GrammerProductionRuleUnit grammerProductionRuleUnit07("F", "( E )");
	GrammerProductionRuleUnit grammerProductionRuleUnit08("F", "i");
	
	//构造文法产生式的存储表
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit01);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit02);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit03);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit04);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit05);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit06);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit07);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit08);

	//遍历文法产生式并打印出来
	cout << "文法产生式列表：" << endl;
	for (vector<GrammerProductionRuleUnit>::iterator i = this->grammerProductionRuleVector.begin(); i != this->grammerProductionRuleVector.end(); i++) {
		GrammerProductionRuleUnit currentGrammerProductionRuleUnit = *i;
		cout << currentGrammerProductionRuleUnit.getLeftsideVn() << "->";
		vector<string> rightsideVnVtVector = currentGrammerProductionRuleUnit.getRightsideVnVtVector();
		for (vector<string>::iterator j = rightsideVnVtVector.begin(); j != rightsideVnVtVector.end(); j++) {
			cout << (*j);
		}
		cout << endl;
	}

	/*********************************构造预测分析表************************************************
			预测分析表：
			_________________________________________________________________________________________________________________________________________
			|		|		i		|		+		|		-		|		*		|		/		|		（		|		）		|		=		|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	E	|	E->FE'		|				|				|				|				|	E->FE'		|				|				|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	E'	|				|	E'->+FE'	|	E'->-FE'	|	E'->*FE'	|	E'->/FE'	|				|	E'->空		|	E'->空		|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	F	|	F->i		|				|				|				|				|	F->(E)		|				|				|
			-----------------------------------------------------------------------------------------------------------------------------------------
	***************************************************************************************************/
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E,i", grammerProductionRuleUnit01));	//相当于M[E,i]=E->FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("F,i", grammerProductionRuleUnit08));	//相当于M[F,i]=F->i
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',+", grammerProductionRuleUnit02));	//相当于M[E',+]=E'->+FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',-", grammerProductionRuleUnit03));	//相当于M[E',-]=E'->-FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',*", grammerProductionRuleUnit04));	//相当于M[E',*]=E'->*FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',/", grammerProductionRuleUnit05));	//相当于M[E',/]=E'->/FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E,(", grammerProductionRuleUnit01));	//相当于M[E,(]=E->FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("F,(", grammerProductionRuleUnit07));	//相当于M[F,(]=F->(E)
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',)", grammerProductionRuleUnit06));	//相当于M[E',)]=E'->nothing
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',=", grammerProductionRuleUnit06));	//相当于M[E',=]=E'->nothing

	//遍历预测分析表并打印出来
	cout << "遍历预测分析表：" << endl;
	for (map<string, GrammerProductionRuleUnit>::iterator i = this->predictAnalyticalMatrix.begin(); i != this->predictAnalyticalMatrix.end(); i++) {
		string key = i->first;
		GrammerProductionRuleUnit currentGrammerProductionRuleUnit = i->second;
		string leftsideVn = currentGrammerProductionRuleUnit.getLeftsideVn();
		vector<string> rightsideVnVtVector = currentGrammerProductionRuleUnit.getRightsideVnVtVector();
		
		cout << "M[" << key << "]=" << leftsideVn << "->";
		for (vector<string>::iterator j = rightsideVnVtVector.begin(); j != rightsideVnVtVector.end(); j++) {
			cout << (*j);
		}
		cout << endl;
	}
}

SyntaxAnalysisPhase2::~SyntaxAnalysisPhase2() {

}

SyntaxAnalysisPhase2::SyntaxAnalysisPhase2(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList) {
	this->reencapsulateWordAndType(lexicalAnalysisResultList);
	this->initGrammerProductionRuleVectorAndPredictAnalyticalMatrix();
	this->initVtVector();
	this->initVnVector();
}

/**
	语法检测大致过程：
		初始化一个分析栈，先把"="压栈，再把文法开始符压栈。
		从词法分析结果中读取一个word（从wordPtr中读取）。如果读取的是操作数，则word="i"
		while (分析栈不空) {
			读取栈顶符号并赋值给x
			if ("=" == word == x) {
				return true;
			}
			if (x属于Vt) {
				if (x=="nothing") { 
					pop出x
					continue;
				}
				if (x==word) {
					将x从栈中弹出并从wordPtr中读取下一个word
				} else {
					error()
				}
			} else {
				//分析栈顶符号如果是非终结符
				从预测分析表predictAnalyticalMatrix中读取key为 非终结符,word 对应的值并赋值给matrixVal
				if (matrixVal is not null) {
					pop出x
					读取出产生式右边的各符号，按照倒序将各符号压入分析栈
				} else {
					error()
				}
			}
		}

*/
bool SyntaxAnalysisPhase2::checkSyntax() {
	stack<string> analyticalStack;			//初始化一个分析栈
	analyticalStack.push("=");
	analyticalStack.push("E");

	//首先把wordPtr中的第一个word读出来
	string word = "";
	string type = "";
	if (lengthOfWordAndTypePtr > 0) {
		word = wordPtr[0];
		type = typePtr[0];
	}
	if (word != "" && type == "OPERAND") {
		word = "i";
	}

	int i = 1;
	while (!analyticalStack.empty()) {
		string x = analyticalStack.top();
		if (x == "=" && word == x) {
			return true;
		}
		if (this->isVt(x)) {
			if (x == "nothing") {
				analyticalStack.pop();	//将x从栈中弹出
				continue;
			}
			//如果栈顶是终结符的情况进行以下处理
			if (x == word) {
				analyticalStack.pop();	//将x从栈中弹出

				//从wordPtr中读取下一个word
				if (i < lengthOfWordAndTypePtr) {
					word = wordPtr[i];
					type = typePtr[i];
				}
				if (word != "" && type == "OPERAND") {
					word = "i";
				}
				i++;
			}
			else {
				cout << "Syntax error: " << wordPtr[i - 1] << " at index of " << i << " in lexical analysis result." << endl;
				return false;
			}
		}
		else {
			//如果栈顶是非终结符的话则得到其产生式右边的符号串，按倒序入栈
			analyticalStack.pop();	//将x从栈中弹出
			//从预测分析表中获取表达式
			string key = x + "," + word;
			map<string, GrammerProductionRuleUnit>::iterator iter = this->predictAnalyticalMatrix.find(key);
			if (iter != this->predictAnalyticalMatrix.end()) {
				//表示已经找到了
				GrammerProductionRuleUnit currentGrammerProductionRuleUnit = iter->second;
				string leftsideVn = currentGrammerProductionRuleUnit.getLeftsideVn();
				vector<string> rightsideVnVtVector = currentGrammerProductionRuleUnit.getRightsideVnVtVector();
				//倒序遍历产生式右边的元素，然后元素压入分析栈
				for (vector<string>::reverse_iterator j = rightsideVnVtVector.rbegin(); j != rightsideVnVtVector.rend(); j++) {
					analyticalStack.push((*j));
				}
			}
			else {
				//如果在预测分析表中找不到对应key的内容，则报错
				cout << "Syntax error: " << wordPtr[i - 1] << " at index of " << i << " in lexical analysis result." << endl;
				return false;

			}
		}
	}
	return false;
	
}

void SyntaxAnalysisPhase2::initVtVector() {
	this->vtVector.push_back("i");
	this->vtVector.push_back("+");
	this->vtVector.push_back("-");
	this->vtVector.push_back("*");
	this->vtVector.push_back("/");
	this->vtVector.push_back("(");
	this->vtVector.push_back(")");
	this->vtVector.push_back("nothing");
	this->vtVector.push_back("=");
}

void SyntaxAnalysisPhase2::initVnVector() {
	this->vnVector.push_back("E");
	this->vnVector.push_back("E'");
	this->vnVector.push_back("F");
}

bool SyntaxAnalysisPhase2::isVt(string s) {
	for (vector<string>::iterator i = this->vtVector.begin(); i != this->vtVector.end(); i++) {
		if ((*i) == s) {
			return true;
		}
	}
	return false;
}

bool SyntaxAnalysisPhase2::isVn(string s) {
	for (vector<string>::iterator i = this->vnVector.begin(); i != this->vnVector.end(); i++) {
		if ((*i) == s) {
			return true;
		}
	}
	return false;
}

