#pragma once
#include <iostream>
#include <cstring>

using namespace std;

class ExpressionAutomation {
private:
	int rowCountOfResultTable;		//结果表的行数
public:
	/*
	*	生成结果表。
	*	如果输入字符串表达式为：1+(22+(3.4+4))，生成表的样子：
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

