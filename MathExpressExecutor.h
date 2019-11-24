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
*	这个类是定义了计算表达式值的时候的必要数据和操作。
*	所谓的计算表达式值的时候是指在词法分析和语法分析之后的时候，所以此类中不包括词法分析和语法分析的过程中的函数定义。
*	
*/
class MathExpressExecutor {
private:
	string operators[7] = { "+", "-", "*", "/", "(", ")", "=" };	//表达式中的操作符。这就是算符优先表中对应的行头和列头，算符优先表是个二维的表格。
	/**
	*	算符优先关系表，其行和列就是operators数组。至于这个关系表是如何产生的这要参考编译原理的相关制作算符优先表的内容，过程比较复杂，在此不作解释。
	*	算符优先表的内容如下所示：
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

	int locationOfCharInOperators(string operatorWord);			//返回字符在operators中的索引位置

	/*
	*	从算符优先关系表中获得对应的优先值。操作符栈的栈顶元素和算术表达式中的当前字符比较得到的优先字符。
	*	第一个参数：操作符栈的栈顶元素。对应表格左边的那一列中的某个操作符。这一列的每个操作符对应一行内容。
	*	第二个参数：算术表达式中的当前字符。对应表格上面的那一行中的某个操作符。这一行的每个操作符对应一列内容。
	*	获得的内容通过以下举例说明：
	*		例子1：operatorWord1="(", operatorWord2=")", return '='
	*/
	char getPrecede(string operatorWord1, string operatorWord2);				//从算符优先关系表中获得对应的优先值
	/*
	*	对2个数根据指定的操作符进行运算，得到新的值。
	*	a在表达式中是比b在前面的那个操作数。例如表达式为：3-4=，那么a就是3，b就是4
	*	第四个参数hasFirstOperandInStack表示操作数栈中是否存在a，因为一个操作符并不一定会对应2个操作数，有时只能对应一个操作数。比如：-4
	*/
	double operate(double a, string operatorWord, double b, bool hasFirstOperandInStack);

	/*
	*	string转数值类型
	*/
	template<class T>
	T stringToNum(const string& str);


public:
	/*
	*	根据词法分析的结果来计算表达式的值
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
		//如果没有操作数a，进行如下处理
		if (operatorWord == "+") {
			result = b;
		} else if (operatorWord == "-") {
			result = 0 - b;
		}
		else {
			cout << "表达式开头有错。" << endl;
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
			cout << "除数不能等于0" << endl;
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
	stack<string> operandStack;		//初始化操作数栈
	stack<string> operatorStack;	//初始化操作符栈
	string result;

	operatorStack.push("=");
	//cout << "operatorStack <== =" << endl;
	for (list<LexicalAnalysisWordAndType>::iterator i = lexicalAnalysisResultList.begin(); i != lexicalAnalysisResultList.end(); i++) {
		//cout << i->getWord() << "\t" << i->getType() << endl;
		if (i->getWord() == "=" && operatorStack.top() == "=") {
			//进入此条件语句表示表达式的计算已经到了尾声，把操作数栈中的结果弹出就是了
			result = operandStack.top();
			//cout << "result=" << result << endl;
			return result;
		}
		if (i->getType() == "OPERAND") {
			//如果词法分析结果表中当前元素是操作数，则操作数压入操作数栈。
			operandStack.push(i->getWord());
			//cout << "operandStack <== " << i->getWord() << endl;
		}
		else {
			//如果词法分析结果表中当前元素是操作符，则根据操作符栈顶的操作符和当前的操作符比较优先级，再根据优先级作相应处理。
			char precedence = this->getPrecede(operatorStack.top(), i->getWord());
			if (precedence == '<') {
				//操作符栈顶的操作符优先级<当前的操作符，把当前操作符压入操作符栈。
				operatorStack.push(i->getWord());
				//cout << "operatorStack <== " << i->getWord() << endl;

			}
			if (precedence == '=') {
				//cout << operatorStack.top() << " pop from operatorStack" << endl;
				//说明左右括号匹配，将操作符栈顶的括号去除。
				operatorStack.pop();
			}
			if (precedence == '>') {
				//操作符栈顶的操作符优先级比较高，提取栈顶操作符和操作数栈的2个数据进行运算，运算结果压入操作数栈。
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
					//说明操作数栈已经没数据了，这说明表达式有问题。
					return "ERROR";
				}
				double dblData1 = this->stringToNum<double>(data1);
				double dblData2 = 0;
				bool hasData2 = false;	//data2是否存在
				if (data2 != "") {
					dblData2 = this->stringToNum<double>(data2);
					hasData2 = true;
				}
				//值得注意的是：由于栈是先进后出的，在表达式中data2在前面，data1在后面。在做减法或除法的时候一定要注意顺序。
				double tempResult = this->operate(dblData2, operatorWord, dblData1, hasData2);
				operandStack.push(to_string(tempResult));
				//cout << "operandStack <== " << to_string(tempResult) << endl;
				i--;
			}
			if (precedence == ' ') {
				//从优先级表中得到的优先级为' '说明表达式有问题，应该返回错误而不应该计算值。
				return "ERROR";
			}
		}
	}

	return "";
}
