#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma once
#include <iostream>
#include "DoubleLinkListForExpression.h"
#include "LexicalAnalysisWordAndType.h"
#include <list>
#include "GlobalConstVars.h"
#include "MathExpressDFA.h"
#include "SqStack.h"

/**
*	定义词法分析阶段要做的事情
*/
class LexicalAnalysisPhase {
public:
	/*
*	规范化数学表达式，做3件事情：
*	1、消除所有空格字符
*	2、初步判定'-'属于操作数的开头部分还是操作符，会在适当的地方添加'+','(',')'，目的是为了方便DFA状态机识别。
*	3、在算术表达式后面加上'='，为了方便DFA状态机识别。
*	例如：-2*(5.5 - 2/-1  )，规范化为：-2*(5.5+(-2)/(-1))=
*	第一个参数：传入的原始算术表达式
*	第二个参数：接手已规范化的算术表达式
*	返回：已规范化的算术表达式的长度。
*/
	int normalizeMathExpression(const char* const expression, char*& resultExpression);

	/**
*	生成词法分析的结果表
*	例如表达式：1.2*(1+3.5/0.5-2)*(-2.5)=，结果为一个类型二维表格的东西：
*		1.2		OPERAND
*		*		OPERATOR
*		(		OPERATOR
*		1		OPERAND
*		+		OPERATOR
*		3.5		OPERAND
*		/		OPERATOR
*		0.5		OPERAND
*		-		OPERATOR
*		2		OPERAND
*		)		OPERATOR
*		*		OPERATOR
*		(		OPERATOR
*		-2.5	OPERAND
*		)		OPERATOR
*		=
*	入参说明：
*		第一个参数：	传入已经规范化之后的算术表达式
*		第二个参数：	用于存放词法分析结果的列表。
*/
	Status generateLexicalAnalysisResultTableList(const char* const expression, list<LexicalAnalysisWordAndType>& wordAndTypeResultList);
	
	/**
	*	这个方法只是简单检查表达式中的括号匹配情况。
	*	这个还不能算真正意义上的语法检测。真正的语法检测在语法分析阶段的类中有定义。
	*	入参为算术表达式字符串
	*/
	Status checkParenthesesPair(const char* expression);

};

int LexicalAnalysisPhase::normalizeMathExpression(const char* const expression, char*& resultExpression) {
	char* tempChars = new char[strlen(expression)];
	strcpy(tempChars, expression);
	DoubleLinkListForExpression<char> doubleLinkListForExpressionObj(' ');
	int len = strlen(expression);
	if (ERROR == doubleLinkListForExpressionObj.createList(tempChars, len, doubleLinkListForExpressionObj)) {
		cout << "创建双向循环链表失败" << endl;
		return 0;
	}

	//cout << "循环双向链表原始内容："; 
	//doubleLinkListForExpressionObj.traverse();
	doubleLinkListForExpressionObj.delAll(' ');
	//cout << "删除所有空格后，内容：";
	//doubleLinkListForExpressionObj.traverse();
	//对算术公式进行规范化改造
	doubleLinkListForExpressionObj.normalizeExpression(doubleLinkListForExpressionObj);
	//cout << "规范化公式后，内容：";
	//doubleLinkListForExpressionObj.traverse();

	int resultSize = doubleLinkListForExpressionObj.getSize();
	resultExpression = new char[resultSize];
	for (int i = 0; i < resultSize; i++) {
		resultExpression[i] = doubleLinkListForExpressionObj.getVal(i + 1);
	}
	return resultSize;

}

Status LexicalAnalysisPhase::generateLexicalAnalysisResultTableList(const char* const expression, list<LexicalAnalysisWordAndType>& wordAndTypeResultList) {
	LexicalAnalysisWordAndType wordAndType;
	int length = strlen(expression);
	int i = 0;
	int startStatus = 0;
	string tempStr;
	MathExpressDFA mathExpressDFA;
	while (i < length) {
		char readChar = expression[i];
		int nextStatus = mathExpressDFA.statusMap[to_string(startStatus).append(1, ',').append(1, readChar)];
		if (nextStatus == 0 && i < length - 1) {
			//nextStatus==0表示在状态机中找不到符合此定义的状态映射，于是认为这是不合法的表达式。
			//由于最后一个固定是'='，'='的状态在状态机中也没定义，所以也会返回0，然而它是正常的情况而不应当作为错误。所以条件中要有‘i < length - 1’的限制。
			//总之，此条件表示在最后的'='之前如果发现有不符合状态机定义的状态出现时就意味着表达式不能通过词法分析。下面将打印出在表达式的哪个位置上出现错误，然后词法分析过程中止。
			cout << "词法分析检测出错误。错误位置在规范化后的算术表达式中的第 " << (i + 1) << " 个字符（" << readChar << "）上" << endl;
			return ERROR;
		}
		if (nextStatus != mathExpressDFA.OPERAND && nextStatus != mathExpressDFA.OPERATOR) {
			tempStr.append(1, readChar);
			i++;
			startStatus = nextStatus;
		}
		if (nextStatus == mathExpressDFA.OPERAND) {
			wordAndType.setWord(tempStr);
			wordAndType.setType("OPERAND");
			wordAndTypeResultList.push_back(wordAndType);
			tempStr = "";
			startStatus = 0;
			continue;
		}
		if (nextStatus == mathExpressDFA.OPERATOR) {
			wordAndType.setWord(tempStr);
			wordAndType.setType("OPERATOR");
			wordAndTypeResultList.push_back(wordAndType);
			tempStr = "";
			startStatus = 0;
			continue;
		}
	}
	//最后再向wordAndTypeResultList中追加{"=",""}。这个在后面的运算过程中作为表达式的结束标志。
	wordAndType.setWord("=");
	wordAndType.setType("");
	wordAndTypeResultList.push_back(wordAndType);

	return OK;
}

Status LexicalAnalysisPhase::checkParenthesesPair(const char* expression) {
	int len = strlen(expression);
	SqStack<char> myStack;
	char popedChar = NULL;
	//从头到尾遍历算术表达式字符串中的每个字符，如果字符是'('则入栈，如果字符是')'时则取出栈顶元素看看是不是'('，如果是则正常，否则不正常。
	for (int i = 0; i < len; i++) {
		if ('(' == expression[i]) {
			myStack.Push(expression[i]);
		}
		if (')' == expression[i]) {
			if (myStack.StackEmpty()) {
				cout << "检测第" << i + 1 << "个字符时，发现栈已空了，不能再pop出东西来了。" << endl;
				return ERROR;
			}
			myStack.Pop(popedChar);
			if (popedChar != '(') {
				cout << "检测第" << i + 1 << "个字符时，发现输入的是'" << expression[i] << "', 而栈顶的元素是'" << popedChar << "'，括号不匹配." << endl;
				return ERROR;
			}
		}
	}
	return OK;

}