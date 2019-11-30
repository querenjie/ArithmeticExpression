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
	*	��ȡ�淶������������ʽ
	*/
	string getNormalizedExpression(string mathExpression);

	/**
	*	��ȡ�ʷ������Ľ��
	*	����ǹ淶��֮����������ʽ�ַ���
	*/
	list<LexicalAnalysisWordAndType> getLexicalAnalysisResult(string normalizedExpression);

	/**
	*	�������ֻ�Ǽ򵥼����ʽ�е�����ƥ�������
	*	��������������������ϵ��﷨��⡣�������﷨������﷨�����׶ε������ж��塣
	*	���Ϊ�������ʽ�ַ���
	*/
	bool checkParenthesesPair(string expression);

	/*
	*	���ݴʷ�������������������ʽ��ֵ
	*/
	string calculateExpression(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);

	/**
	*	����﷨�Ƿ�������
	*/
	bool checkSyntax(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList);
};

string MathExpressService::getNormalizedExpression(string mathExpression) {
	const char* ccMathExpression = mathExpression.c_str();
	LexicalAnalysisPhase lexicalAnalysisPhase;
	char* normalizedExpression;
	int lenOfNormalizedExpression = lexicalAnalysisPhase.normalizeMathExpression(ccMathExpression, normalizedExpression);
	/**************************************************************************
	cout << "�淶������������ʽ��" << endl;
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
		cout << "δ��ͨ���ʷ������׶�" << endl;
		list<LexicalAnalysisWordAndType> wordAndTypeResultListEmpty;
		return wordAndTypeResultListEmpty;
	}
	return wordAndTypeResultList;

}

bool MathExpressService::checkParenthesesPair(string expression) {
	LexicalAnalysisPhase lexicalAnalysisPhase;
	const char* ccExpression = expression.c_str();
	if (lexicalAnalysisPhase.checkParenthesesPair(ccExpression) == ERROR) {
		cout << "��⵽���Ų�ƥ��" << endl;
		return false;
	}
	return true;
}

string MathExpressService::calculateExpression(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList) {
	MathExpressExecutor mathExpressExecutor;
	return mathExpressExecutor.evaluateExpression(lexicalAnalysisResultList);
}

bool MathExpressService::checkSyntax(list<LexicalAnalysisWordAndType> lexicalAnalysisResultList) {
	cout << "������ִ���еݹ��Ԥ�������" << endl;
	SyntaxAnalysisPhase syntaxAnalysisPhase(lexicalAnalysisResultList);
	bool isSyntaxOK = syntaxAnalysisPhase.checkSyntax();
	if (isSyntaxOK) {
		cout << "ͨ���еݹ��Ԥ��������﷨����" << endl;
	}
	else {
		cout << "δ��ͨ���еݹ��Ԥ��������﷨����" << endl;
	}

	cout << "������ִ�зǵݹ��Ԥ�������" << endl;
	SyntaxAnalysisPhase2 syntaxAnalysisPhase2(lexicalAnalysisResultList);
	bool isSyntaxOK2 = syntaxAnalysisPhase2.checkSyntax();
	if (isSyntaxOK2) {
		cout << "ͨ���ǵݹ��Ԥ��������﷨����" << endl;
	}
	else {
		cout << "δ��ͨ���ǵݹ��Ԥ��������﷨����" << endl;
	}

	return isSyntaxOK && isSyntaxOK2;
}