#pragma once
#include <iostream>
#include <cstring>

using namespace std;

class ExpressionAutomation {
private:
	int rowCountOfResultTable;		//����������
public:
	/*
	*	���ɽ����
	*	��������ַ������ʽΪ��1+(22+(3.4+4))�����ɱ�����ӣ�
	*	1	OPERAND
	*	+	OPERATOR
	*	(	OPERATOR
	*	22	OPERAND
	*	+	OPERATOR
	*	(	OPERATOR
	*	3.4	OPERAND
	*	+	OPERATOR
	*	4	OPERAND
	*	)	OPERATOR
	*	)	OPERATOR
	*/
	char** generate(const char* expression);
};

