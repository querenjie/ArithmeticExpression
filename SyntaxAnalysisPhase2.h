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
	�﷨�����׶�Ҫ��������顣���﷨�������̲��õ��Ƿǵݹ��Ԥ��������õ���Ԥ�������
	ԭʼ����������˼ά���ķ�����ʽ��
		E->E+F|E-F|E*F|E/F|F
		F->(E)|i
	�����׵ó�������ݹ����ķ�����ʽ�������£�
		E->FE'
		E'->+FE'|-FE'|*FE'|/FE'|��
		F->(E)|i
	���з��ս��E�Ǳ��ʽ�����ս��F�Ǳ��ʽ�е����ӡ�
	�ս����{'+', '-', '*', '/', '(', ')'��'i'}������i�ǲ�������
	��ʼ����E
	
	�ǵݹ��Ԥ�����������Ҫ����Ԥ�������Ԥ�������Ĵ�����Ҫ�ȵó��ķ��и����ս����First����Follow����
	�ȷ���First����
		First(F)={'(', 'i'}
		First(E')={'+', '-', '*', '/', ��}
		First(E)=First(F)={'(', 'i'}
	Ȼ�����Follow����(���з��ս����Follow��������'=')
		Follow(E)={')', '='}
		First(E')-{��}����Follow(F)����Follow(F)={'+', '-', '*', '/'}
		����E'����0�������ɲ����Ƶ����ղ�����E->FE'������Follow(E)������Follow(E')����Follow(E')={')', '='}
	�������һ�½�����£�
		First(E)=First(F)={'(', 'i'}
		First(E')={'+', '-', '*', '/', ��}
		Follow(E)=Follow(E')={')', '='}
		Follow(F)={'+', '-', '*', '/'}
	�ڹ���Ԥ�������֮ǰ�ȼ��һ���ķ��Ƿ����LL(1)�ķ�
		��F->(E)|i���õ�First((E))��First(i)=�ռ�
		��E'->+FE'���õ�First(+FE')��Follow(E')={'+'}��{')', '='}=�ռ�
		��E'->-FE'���õ�First(-FE')��Follow(E')={'-'}��{')', '='}=�ռ�
		��E'->-FE'���õ�First(*FE')��Follow(E')={'*'}��{')', '='}=�ռ�
		��E'->-FE'���õ�First(/FE')��Follow(E')={'/'}��{')', '='}=�ռ�
		�����ķ���LL(1)�ķ�
	����Ԥ�������
		��E->FE'����First(FE')={'(', 'i'}����E->FE'����M[E,(]��M[E,i]
		��E'->+FE'����First(+FE')={'+'}����E'->+FE'����M[E',+]
		��E'->-FE'����First(-FE')={'-'}����E'->-FE'����M[E',-]
		��E'->*FE'����First(*FE')={'*'}����E'->*FE'����M[E',*]
		��E'->/FE'����First(/FE')={'/'}����E'->/FE'����M[E',/]
		��E'->�գ���Follow(E')={')', '='}����E'->�ռ�¼M[E',)]��M[E',=]
		��F->(E)����ȻF->(E)Ӧ����M[F,(]
		��F->i����ȻF->iӦ����M[F,i]
		����û����M�Ķ���Ҫ���д�����ġ�
		Ԥ�������
			_________________________________________________________________________________________________________________________________________
			|		|		i		|		+		|		-		|		*		|		/		|		��		|		��		|		=		|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	E	|	E->FE'		|				|				|				|				|	E->FE'		|				|				|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	E'	|				|	E'->+FE'	|	E'->-FE'	|	E'->*FE'	|	E'->/FE'	|				|	E'->��		|	E'->��		|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	F	|	F->i		|				|				|				|				|	F->(E)		|				|				|
			-----------------------------------------------------------------------------------------------------------------------------------------
*/
class SyntaxAnalysisPhase2 {
private:
	string* wordPtr;				//����ʷ���������еĴʵ����string�����С���������"="��Ҫȥ����
	string* typePtr;				//��ӦwordPtr�е�ÿ���ʵ����͡�
	int lengthOfWordAndTypePtr;		//wordPtr��typePtr�е�Ԫ�ظ���
	vector<GrammerProductionRuleUnit> grammerProductionRuleVector;	//���һϵ���ķ�����ʽ��ÿ��Ԫ�ض���GrammerProductionRuleUnit���͵Ķ������з�װ��һ������ʽ��
	map<string, GrammerProductionRuleUnit> predictAnalyticalMatrix;		//Ԥ�������key����ʽ�����ս��,����������磺E',+  ���ڲ�����ȫ����i���档
	vector<string> vtVector;	//������е��ս�����ڳ����п��Է����ж�ĳ�������Ƿ�Ϊ�ս����
	vector<string> vnVector;	//������еķ��ս�����ڳ����п��Է����ж�ĳ�������Ƿ�Ϊ���ս����

	/**
	*	���½��ʷ��������ȡ���ŵ�string�����У�Ŀ������������ȽϷ��㡣
	*	���������wordPtr��typePtr��lengthOfWordAndTypePtr��ֵ
	*/
	void reencapsulateWordAndType(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);	

	/**
		�����ķ�����ʽ�Ĵ洢�������grammerProductionRuleVector��
		����Ԥ������������predictAnalyticalMatrix��
	*/
	void initGrammerProductionRuleVectorAndPredictAnalyticalMatrix();

	void initVtVector();	//��ʼ�������ս���������vtVector�С�

	void initVnVector();	//��ʼ�����з��ս���������vnVector�С�

	bool isVt(string s);		//�ж�s�Ƿ�Ϊ�ս��

	bool isVn(string s);		//�ж�s�Ƿ�Ϊ���ս��
public:

	/**
	*	���캯�������ʷ������Ľ����Ϊ��δ�������
		���ܣ�
			1���ʷ����������ȥ��'='���ʺ����Էֱ𱣴浽wordPtr��typePtr�����������С�wordPtr��typePtr�������������ͬλ�õ�Ԫ���ж�Ӧ��ϵ���ֱ���ĳ�����ʺͶ�Ӧ�Ĵ��ԡ�
			2�������ķ�����ʽ�Ĵ洢�������grammerProductionRulesMap��
	*/
	SyntaxAnalysisPhase2(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);

	~SyntaxAnalysisPhase2();

	bool checkSyntax();		//����﷨
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
		E'->+FE'|-FE'|*FE'|/FE'|��
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
	
	//�����ķ�����ʽ�Ĵ洢��
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit01);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit02);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit03);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit04);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit05);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit06);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit07);
	this->grammerProductionRuleVector.push_back(grammerProductionRuleUnit08);

	//�����ķ�����ʽ����ӡ����
	cout << "�ķ�����ʽ�б�" << endl;
	for (vector<GrammerProductionRuleUnit>::iterator i = this->grammerProductionRuleVector.begin(); i != this->grammerProductionRuleVector.end(); i++) {
		GrammerProductionRuleUnit currentGrammerProductionRuleUnit = *i;
		cout << currentGrammerProductionRuleUnit.getLeftsideVn() << "->";
		vector<string> rightsideVnVtVector = currentGrammerProductionRuleUnit.getRightsideVnVtVector();
		for (vector<string>::iterator j = rightsideVnVtVector.begin(); j != rightsideVnVtVector.end(); j++) {
			cout << (*j);
		}
		cout << endl;
	}

	/*********************************����Ԥ�������************************************************
			Ԥ�������
			_________________________________________________________________________________________________________________________________________
			|		|		i		|		+		|		-		|		*		|		/		|		��		|		��		|		=		|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	E	|	E->FE'		|				|				|				|				|	E->FE'		|				|				|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	E'	|				|	E'->+FE'	|	E'->-FE'	|	E'->*FE'	|	E'->/FE'	|				|	E'->��		|	E'->��		|
			-----------------------------------------------------------------------------------------------------------------------------------------
			|	F	|	F->i		|				|				|				|				|	F->(E)		|				|				|
			-----------------------------------------------------------------------------------------------------------------------------------------
	***************************************************************************************************/
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E,i", grammerProductionRuleUnit01));	//�൱��M[E,i]=E->FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("F,i", grammerProductionRuleUnit08));	//�൱��M[F,i]=F->i
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',+", grammerProductionRuleUnit02));	//�൱��M[E',+]=E'->+FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',-", grammerProductionRuleUnit03));	//�൱��M[E',-]=E'->-FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',*", grammerProductionRuleUnit04));	//�൱��M[E',*]=E'->*FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',/", grammerProductionRuleUnit05));	//�൱��M[E',/]=E'->/FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E,(", grammerProductionRuleUnit01));	//�൱��M[E,(]=E->FE'
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("F,(", grammerProductionRuleUnit07));	//�൱��M[F,(]=F->(E)
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',)", grammerProductionRuleUnit06));	//�൱��M[E',)]=E'->nothing
	this->predictAnalyticalMatrix.insert(pair<string, GrammerProductionRuleUnit>("E',=", grammerProductionRuleUnit06));	//�൱��M[E',=]=E'->nothing

	//����Ԥ���������ӡ����
	cout << "����Ԥ�������" << endl;
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
	�﷨�����¹��̣�
		��ʼ��һ������ջ���Ȱ�"="ѹջ���ٰ��ķ���ʼ��ѹջ��
		�Ӵʷ���������ж�ȡһ��word����wordPtr�ж�ȡ���������ȡ���ǲ���������word="i"
		while (����ջ����) {
			��ȡջ�����Ų���ֵ��x
			if ("=" == word == x) {
				return true;
			}
			if (x����Vt) {
				if (x=="nothing") { 
					pop��x
					continue;
				}
				if (x==word) {
					��x��ջ�е�������wordPtr�ж�ȡ��һ��word
				} else {
					error()
				}
			} else {
				//����ջ����������Ƿ��ս��
				��Ԥ�������predictAnalyticalMatrix�ж�ȡkeyΪ ���ս��,word ��Ӧ��ֵ����ֵ��matrixVal
				if (matrixVal is not null) {
					pop��x
					��ȡ������ʽ�ұߵĸ����ţ����յ��򽫸�����ѹ�����ջ
				} else {
					error()
				}
			}
		}

*/
bool SyntaxAnalysisPhase2::checkSyntax() {
	stack<string> analyticalStack;			//��ʼ��һ������ջ
	analyticalStack.push("=");
	analyticalStack.push("E");

	//���Ȱ�wordPtr�еĵ�һ��word������
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
				analyticalStack.pop();	//��x��ջ�е���
				continue;
			}
			//���ջ�����ս��������������´���
			if (x == word) {
				analyticalStack.pop();	//��x��ջ�е���

				//��wordPtr�ж�ȡ��һ��word
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
			//���ջ���Ƿ��ս���Ļ���õ������ʽ�ұߵķ��Ŵ�����������ջ
			analyticalStack.pop();	//��x��ջ�е���
			//��Ԥ��������л�ȡ���ʽ
			string key = x + "," + word;
			map<string, GrammerProductionRuleUnit>::iterator iter = this->predictAnalyticalMatrix.find(key);
			if (iter != this->predictAnalyticalMatrix.end()) {
				//��ʾ�Ѿ��ҵ���
				GrammerProductionRuleUnit currentGrammerProductionRuleUnit = iter->second;
				string leftsideVn = currentGrammerProductionRuleUnit.getLeftsideVn();
				vector<string> rightsideVnVtVector = currentGrammerProductionRuleUnit.getRightsideVnVtVector();
				//�����������ʽ�ұߵ�Ԫ�أ�Ȼ��Ԫ��ѹ�����ջ
				for (vector<string>::reverse_iterator j = rightsideVnVtVector.rbegin(); j != rightsideVnVtVector.rend(); j++) {
					analyticalStack.push((*j));
				}
			}
			else {
				//�����Ԥ����������Ҳ�����Ӧkey�����ݣ��򱨴�
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

