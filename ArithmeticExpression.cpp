#include <iostream>
#include "MathExpressService.h"
#include <string>
using namespace std;

int main()
{
	MathExpressService mathExpressService;

	//算术表达式字符串
	string expression = "-0.5-2*(-1.25+6)-(3-2)*-2";
	bool isExpressOK = mathExpressService.checkParenthesesPair(expression);
	if (!isExpressOK) {
		cout << "---------------------------------------------------------" << endl;
		cout << "表达式中括号不匹配。" << endl;
		exit(1);
	}

	//规范化算术表达式
	string normalizedExpression = mathExpressService.getNormalizedExpression(expression);
	cout << "---------------------------------------------------------" << endl;
	cout << "原先的算术表达式：" << expression << endl;
	cout << "规范化后的算术表达式：" << normalizedExpression << endl;

	//获取词法分析之后的结果
	list<LexicalAnalysisWordAndType> lexicalAnalysisWordAndTypeList = mathExpressService.getLexicalAnalysisResult(normalizedExpression);
	cout << "---------------------------------------------------------" << endl;
	cout << "词法分析的结果：" << endl;
	for (list<LexicalAnalysisWordAndType>::iterator i = lexicalAnalysisWordAndTypeList.begin(); i != lexicalAnalysisWordAndTypeList.end(); i++) {
		cout << i->getWord() << "\t" << i->getType() << endl;
	}

	//表达式求值
	string strResult = mathExpressService.calculateExpression(lexicalAnalysisWordAndTypeList);
	cout << "---------------------------------------------------------" << endl;
	cout << "表达式求值的结果：" << normalizedExpression << strResult << endl;
}
