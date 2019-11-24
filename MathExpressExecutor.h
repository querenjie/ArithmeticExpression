#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "GlobalConstVars.h"
#include <stack>
#include "LexicalAnalysisWordAndType.h"
#include <list>
using namespace std;

/**
*	������Ƕ����˼�����ʽֵ��ʱ��ı�Ҫ���ݺͲ�����
*	��ν�ļ�����ʽֵ��ʱ����ָ�ڴʷ��������﷨����֮���ʱ�����Դ����в������ʷ��������﷨�����Ĺ����еĺ������塣
*	
*/
class MathExpressExecutor {
private:
	string operators[7] = { "+", "-", "*", "/", "(", ")", "=" };	//���ʽ�еĲ������������������ȱ��ж�Ӧ����ͷ����ͷ��������ȱ��Ǹ���ά�ı��
	/**
	*	������ȹ�ϵ�����к��о���operators���顣���������ϵ������β�������Ҫ�ο�����ԭ����������������ȱ�����ݣ����̱Ƚϸ��ӣ��ڴ˲������͡�
	*	������ȱ������������ʾ��
	*			| "+", "-", "*", "/", "(", ")", "=" |
	*			|-----------------------------------|
	*		"+" | '>', '>', '<', '<', '<', '>', '>'	|
	*		"-"	| '>', '>', '<', '<', '<', '>', '>' |
	*		"*"	| '>', '>', '>', '>', '<', '>', '>' |
	*		"/"	| '>', '>', '>', '>', '<', '>', '>' |
	*		"("	| '<', '<', '<', '<', '<', '=', ' ' |
	*		")"	| '>', '>', '>', '>', ' ', '>', '>' |
	*		"="	| '<', '<', '<', '<', '<', ' ', '=' |
	*			|-----------------------------------|
	*/
	const char precedences[7][7] = { 
									{'>', '>', '<', '<', '<', '>', '>'}, 
									{'>', '>', '<', '<', '<', '>', '>'},
									{'>', '>', '>', '>', '<', '>', '>'},
									{'>', '>', '>', '>', '<', '>', '>'},
									{'<', '<', '<', '<', '<', '=', ' '},
									{'>', '>', '>', '>', ' ', '>', '>'},
									{'<', '<', '<', '<', '<', ' ', '='}
									};

	int locationOfCharInOperators(string operatorWord);			//�����ַ���operators�е�����λ��

	/*
	*	��������ȹ�ϵ���л�ö�Ӧ������ֵ��������ջ��ջ��Ԫ�غ��������ʽ�еĵ�ǰ�ַ��Ƚϵõ��������ַ���
	*	��һ��������������ջ��ջ��Ԫ�ء���Ӧ�����ߵ���һ���е�ĳ������������һ�е�ÿ����������Ӧһ�����ݡ�
	*	�ڶ����������������ʽ�еĵ�ǰ�ַ�����Ӧ����������һ���е�ĳ������������һ�е�ÿ����������Ӧһ�����ݡ�
	*	��õ�����ͨ�����¾���˵����
	*		����1��operatorWord1="(", operatorWord2=")", return '='
	*/
	char getPrecede(string operatorWord1, string operatorWord2);				//��������ȹ�ϵ���л�ö�Ӧ������ֵ
	/*
	*	��2��������ָ���Ĳ������������㣬�õ��µ�ֵ��
	*	a�ڱ��ʽ���Ǳ�b��ǰ����Ǹ���������������ʽΪ��3-4=����ôa����3��b����4
	*	���ĸ�����hasFirstOperandInStack��ʾ������ջ���Ƿ����a����Ϊһ������������һ�����Ӧ2������������ʱֻ�ܶ�Ӧһ�������������磺-4
	*/
	double operate(double a, string operatorWord, double b, bool hasFirstOperandInStack);

	/*
	*	stringת��ֵ����
	*/
	template<class T>
	T stringToNum(const string& str);


public:
	/*
	*	���ݴʷ������Ľ����������ʽ��ֵ
	*	
	*/
	string evaluateExpression(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);

};

int MathExpressExecutor::locationOfCharInOperators(string operatorWord) {
	for (int i = 0; i < sizeof(this->operators) / sizeof(char); i++) {
		if (this->operators[i] == operatorWord) {
			return i;
		}
	}
	return -1;
}

char MathExpressExecutor::getPrecede(string operatorWord1, string operatorWord2) {
	int indexOfOperatorWord1 = this->locationOfCharInOperators(operatorWord1);
	int indexOfOperatorWord2 = this->locationOfCharInOperators(operatorWord2);
	return this->precedences[indexOfOperatorWord1][indexOfOperatorWord2];
}

double MathExpressExecutor::operate(double a, string operatorWord, double b, bool hasFirstOperandInStack) {
	double result = 0;
	if (!hasFirstOperandInStack) {
		//���û�в�����a���������´���
		if (operatorWord == "+") {
			result = b;
		} else if (operatorWord == "-") {
			result = 0 - b;
		}
		else {
			cout << "���ʽ��ͷ�д�" << endl;
		}
		return result;
	}

	if (operatorWord == "+") {
		result = a + b;
	}
	if (operatorWord == "-") {
		result = a - b;
	}
	if (operatorWord == "*") {
		result = a * b;
	}
	if (operatorWord == "/") {
		if (b == 0.0) {
			cout << "�������ܵ���0" << endl;
			return NULL;
		}
		result = a / b;
	}

	return result;
}

template<class T>
T MathExpressExecutor::stringToNum(const string& str) {
	istringstream iss(str);
	T num;
	iss >> num;
	return num;
}

string MathExpressExecutor::evaluateExpression(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList) {
	stack<string> operandStack;		//��ʼ��������ջ
	stack<string> operatorStack;	//��ʼ��������ջ
	string result;

	operatorStack.push("=");
	//cout << "operatorStack <== =" << endl;
	for (list<LexicalAnalysisWordAndType>::iterator i = lexicalAnalysisResultList.begin(); i != lexicalAnalysisResultList.end(); i++) {
		//cout << i->getWord() << "\t" << i->getType() << endl;
		if (i->getWord() == "=" && operatorStack.top() == "=") {
			//�������������ʾ���ʽ�ļ����Ѿ�����β�����Ѳ�����ջ�еĽ������������
			result = operandStack.top();
			//cout << "result=" << result << endl;
			return result;
		}
		if (i->getType() == "OPERAND") {
			//����ʷ�����������е�ǰԪ���ǲ��������������ѹ�������ջ��
			operandStack.push(i->getWord());
			//cout << "operandStack <== " << i->getWord() << endl;
		}
		else {
			//����ʷ�����������е�ǰԪ���ǲ�����������ݲ�����ջ���Ĳ������͵�ǰ�Ĳ������Ƚ����ȼ����ٸ������ȼ�����Ӧ����
			char precedence = this->getPrecede(operatorStack.top(), i->getWord());
			if (precedence == '<') {
				//������ջ���Ĳ��������ȼ�<��ǰ�Ĳ��������ѵ�ǰ������ѹ�������ջ��
				operatorStack.push(i->getWord());
				//cout << "operatorStack <== " << i->getWord() << endl;

			}
			if (precedence == '=') {
				//cout << operatorStack.top() << " pop from operatorStack" << endl;
				//˵����������ƥ�䣬��������ջ��������ȥ����
				operatorStack.pop();
			}
			if (precedence == '>') {
				//������ջ���Ĳ��������ȼ��Ƚϸߣ���ȡջ���������Ͳ�����ջ��2�����ݽ������㣬������ѹ�������ջ��
				string operatorWord = operatorStack.top();
				//cout << operatorWord << " pop from operatorStack" << endl;
				operatorStack.pop();

				string data1 = "";
				if (!operandStack.empty()) {
					data1 = operandStack.top();
					//cout << data1 << " pop from operandStack" << endl;
					operandStack.pop();
				}
				string data2 = "";
				if (!operandStack.empty()) {
					data2 = operandStack.top();
					//cout << data2 << " pop from operandStack" << endl;
					operandStack.pop();
				}

				if (data1 == "") {
					//˵��������ջ�Ѿ�û�����ˣ���˵�����ʽ�����⡣
					return "ERROR";
				}
				double dblData1 = this->stringToNum<double>(data1);
				double dblData2 = 0;
				bool hasData2 = false;	//data2�Ƿ����
				if (data2 != "") {
					dblData2 = this->stringToNum<double>(data2);
					hasData2 = true;
				}
				//ֵ��ע����ǣ�����ջ���Ƚ�����ģ��ڱ��ʽ��data2��ǰ�棬data1�ں��档���������������ʱ��һ��Ҫע��˳��
				double tempResult = this->operate(dblData2, operatorWord, dblData1, hasData2);
				operandStack.push(to_string(tempResult));
				//cout << "operandStack <== " << to_string(tempResult) << endl;
				i--;
			}
			if (precedence == ' ') {
				//�����ȼ����еõ������ȼ�Ϊ' '˵�����ʽ�����⣬Ӧ�÷��ش������Ӧ�ü���ֵ��
				return "ERROR";
			}
		}
	}

	return "";
}
