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
*	����ʷ������׶�Ҫ��������
*/
class LexicalAnalysisPhase {
public:
	/*
*	�淶����ѧ���ʽ����3�����飺
*	1���������пո��ַ�
*	2�������ж�'-'���ڲ������Ŀ�ͷ���ֻ��ǲ������������ʵ��ĵط����'+','(',')'��Ŀ����Ϊ�˷���DFA״̬��ʶ��
*	3�����������ʽ�������'='��Ϊ�˷���DFA״̬��ʶ��
*	���磺-2*(5.5 - 2/-1  )���淶��Ϊ��-2*(5.5+(-2)/(-1))=
*	��һ�������������ԭʼ�������ʽ
*	�ڶ��������������ѹ淶�����������ʽ
*	���أ��ѹ淶�����������ʽ�ĳ��ȡ�
*/
	int normalizeMathExpression(const char* const expression, char*& resultExpression);

	/**
*	���ɴʷ������Ľ����
*	������ʽ��1.2*(1+3.5/0.5-2)*(-2.5)=�����Ϊһ�����Ͷ�ά���Ķ�����
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
*	���˵����
*		��һ��������	�����Ѿ��淶��֮����������ʽ
*		�ڶ���������	���ڴ�Ŵʷ�����������б�
*/
	Status generateLexicalAnalysisResultTableList(const char* const expression, list<LexicalAnalysisWordAndType>& wordAndTypeResultList);
	
	/**
	*	�������ֻ�Ǽ򵥼����ʽ�е�����ƥ�������
	*	��������������������ϵ��﷨��⡣�������﷨������﷨�����׶ε������ж��塣
	*	���Ϊ�������ʽ�ַ���
	*/
	Status checkParenthesesPair(const char* expression);

};

int LexicalAnalysisPhase::normalizeMathExpression(const char* const expression, char*& resultExpression) {
	char* tempChars = new char[strlen(expression)];
	strcpy(tempChars, expression);
	DoubleLinkListForExpression<char> doubleLinkListForExpressionObj(' ');
	int len = strlen(expression);
	if (ERROR == doubleLinkListForExpressionObj.createList(tempChars, len, doubleLinkListForExpressionObj)) {
		cout << "����˫��ѭ������ʧ��" << endl;
		return 0;
	}

	//cout << "ѭ��˫������ԭʼ���ݣ�"; 
	//doubleLinkListForExpressionObj.traverse();
	doubleLinkListForExpressionObj.delAll(' ');
	//cout << "ɾ�����пո�����ݣ�";
	//doubleLinkListForExpressionObj.traverse();
	//��������ʽ���й淶������
	doubleLinkListForExpressionObj.normalizeExpression(doubleLinkListForExpressionObj);
	//cout << "�淶����ʽ�����ݣ�";
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
			//nextStatus==0��ʾ��״̬�����Ҳ������ϴ˶����״̬ӳ�䣬������Ϊ���ǲ��Ϸ��ı��ʽ��
			//�������һ���̶���'='��'='��״̬��״̬����Ҳû���壬����Ҳ�᷵��0��Ȼ�������������������Ӧ����Ϊ��������������Ҫ�С�i < length - 1�������ơ�
			//��֮����������ʾ������'='֮ǰ��������в�����״̬�������״̬����ʱ����ζ�ű��ʽ����ͨ���ʷ����������潫��ӡ���ڱ��ʽ���ĸ�λ���ϳ��ִ���Ȼ��ʷ�����������ֹ��
			cout << "�ʷ������������󡣴���λ���ڹ淶������������ʽ�еĵ� " << (i + 1) << " ���ַ���" << readChar << "����" << endl;
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
	//�������wordAndTypeResultList��׷��{"=",""}������ں���������������Ϊ���ʽ�Ľ�����־��
	wordAndType.setWord("=");
	wordAndType.setType("");
	wordAndTypeResultList.push_back(wordAndType);

	return OK;
}

Status LexicalAnalysisPhase::checkParenthesesPair(const char* expression) {
	int len = strlen(expression);
	SqStack<char> myStack;
	char popedChar = NULL;
	//��ͷ��β�����������ʽ�ַ����е�ÿ���ַ�������ַ���'('����ջ������ַ���')'ʱ��ȡ��ջ��Ԫ�ؿ����ǲ���'('�������������������������
	for (int i = 0; i < len; i++) {
		if ('(' == expression[i]) {
			myStack.Push(expression[i]);
		}
		if (')' == expression[i]) {
			if (myStack.StackEmpty()) {
				cout << "����" << i + 1 << "���ַ�ʱ������ջ�ѿ��ˣ�������pop���������ˡ�" << endl;
				return ERROR;
			}
			myStack.Pop(popedChar);
			if (popedChar != '(') {
				cout << "����" << i + 1 << "���ַ�ʱ�������������'" << expression[i] << "', ��ջ����Ԫ����'" << popedChar << "'�����Ų�ƥ��." << endl;
				return ERROR;
			}
		}
	}
	return OK;

}