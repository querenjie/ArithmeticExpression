#pragma once
#include <iostream>
#include "MathExpressExecutor.h"
#include "LexicalAnalysisPhase.h"
#include "LexicalAnalysisWordAndType.h"
#include <list>
#include <string>
#include "GlobalConstVars.h"
#include "SyntaxAnalysisPhase.h"
#include "SyntaxAnalysisPhase2.h"

using namespace std;

class MathExpressService {
public:
	/**
	*	获取规范化后的算术表达式
	*/
	string getNormalizedExpression(string mathExpression);

	/**
	*	获取词法分析的结果
	*	入参是规范化之后的算术表达式字符串
	*/
	list<LexicalAnalysisWordAndType> getLexicalAnalysisResult(string normalizedExpression);

	/**
	*	这个方法只是简单检查表达式中的括号匹配情况。
	*	这个还不能算真正意义上的语法检测。真正的语法检测在语法分析阶段的类中有定义。
	*	入参为算术表达式字符串
	*/
	bool checkParenthesesPair(string expression);

	/*
	*	根据词法分析的输出结果计算表达式的值
	*/
	string calculateExpression(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);

	/**
	*	检查语法是否有问题
	*/
	bool checkSyntax(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);
};

string MathExpressService::getNormalizedExpression(string mathExpression) {
	const char* ccMathExpression = mathExpression.c_str();
	LexicalAnalysisPhase lexicalAnalysisPhase;
	char* normalizedExpression;
	int lenOfNormalizedExpression = lexicalAnalysisPhase.normalizeMathExpression(ccMathExpression, normalizedExpression);
	/**************************************************************************
	cout << "规范化后的算术表达式：" << endl;
	for (int i = 0; i < lenOfNormalizedExpression; i++) {
		cout << normalizedExpression[i];
	}
	cout << endl;
	**************************************************************************/

	string strNormalizedExpression(normalizedExpression, lenOfNormalizedExpression);
	return strNormalizedExpression;
}

list<LexicalAnalysisWordAndType> MathExpressService::getLexicalAnalysisResult(string normalizedExpression) {
	const char* ccNormalizedExpression = normalizedExpression.c_str();
	list<LexicalAnalysisWordAndType> wordAndTypeResultList;
	LexicalAnalysisPhase lexicalAnalysisPhase;

	Status isLexicalAnalysisProcessOK = lexicalAnalysisPhase.generateLexicalAnalysisResultTableList(ccNormalizedExpression, wordAndTypeResultList);
	if (isLexicalAnalysisProcessOK == ERROR) {
		cout << "未能通过词法分析阶段" << endl;
		list<LexicalAnalysisWordAndType> wordAndTypeResultListEmpty;
		return wordAndTypeResultListEmpty;
	}
	return wordAndTypeResultList;

}

bool MathExpressService::checkParenthesesPair(string expression) {
	LexicalAnalysisPhase lexicalAnalysisPhase;
	const char* ccExpression = expression.c_str();
	if (lexicalAnalysisPhase.checkParenthesesPair(ccExpression) == ERROR) {
		cout << "检测到括号不匹配" << endl;
		return false;
	}
	return true;
}

string MathExpressService::calculateExpression(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList) {
	MathExpressExecutor mathExpressExecutor;
	return mathExpressExecutor.evaluateExpression(lexicalAnalysisResultList);
}

bool MathExpressService::checkSyntax(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList) {
	cout << "以下是执行有递归的预测分析：" << endl;
	SyntaxAnalysisPhase syntaxAnalysisPhase(lexicalAnalysisResultList);
	bool isSyntaxOK = syntaxAnalysisPhase.checkSyntax();
	if (isSyntaxOK) {
		cout << "通过有递归的预测分析的语法分析" << endl;
	}
	else {
		cout << "未能通过有递归的预测分析的语法分析" << endl;
	}

	cout << "以下是执行非递归的预测分析：" << endl;
	SyntaxAnalysisPhase2 syntaxAnalysisPhase2(lexicalAnalysisResultList);
	bool isSyntaxOK2 = syntaxAnalysisPhase2.checkSyntax();
	if (isSyntaxOK2) {
		cout << "通过非递归的预测分析的语法分析" << endl;
	}
	else {
		cout << "未能通过非递归的预测分析的语法分析" << endl;
	}

	return isSyntaxOK && isSyntaxOK2;
}