#include <iostream>
#include "MathExpressService.h"
#include <string>
using namespace std;

int main()
{
	MathExpressService mathExpressService;

	//算术表达式字符串
	string expression = "-0.5-2*((-1.25+6)*2-(3-2)*-2)";			//这个可以正常运算
	//string expression = "-0.5-2*(-1.25+6)*2-(3-2)*-2)";		//这会引起语法分析错误
	//string expression = "-0.5//-2*(-1.25)";						//这个通不过词法分析

	/**********************************************************************************************
	//检查括号是否匹配。然而这个没必要调用，在后面的语法分析阶段也能检测出来，而且可扩展性更强。
	bool isExpressOK = mathExpressService.checkParenthesesPair(expression);
	if (!isExpressOK) {
		cout << "---------------------------------------------------------" << endl;
		cout << "表达式中括号不匹配。" << endl;
		exit(1);
	}
	**********************************************************************************************/

	//规范化算术表达式
	string normalizedExpression = mathExpressService.getNormalizedExpression(expression);
	cout << "---------------------------------------------------------" << endl;
	cout << "原先的算术表达式：" << expression << endl;
	cout << "规范化后的算术表达式：" << normalizedExpression << endl;

	//获取词法分析之后的结果
	list<LexicalAnalysisWordAndType> lexicalAnalysisWordAndTypeList = mathExpressService.getLexicalAnalysisResult(normalizedExpression);
	cout << "---------------------------------------------------------" << endl;
	cout << "词法分析的结果：" << endl;
	if (lexicalAnalysisWordAndTypeList.size() == 0) {
		cout << "因为有错无法所以词法分析无结果。" << endl;
	} else {
		for (list<LexicalAnalysisWordAndType>::iterator i = lexicalAnalysisWordAndTypeList.begin(); i != lexicalAnalysisWordAndTypeList.end(); i++) {
			cout << i->getWord() << "\t" << i->getType() << endl;
		}
	}

	//检查语法是否正常。（根据编译原理语法分析阶段的基于文法产生式分析语法）
	cout << "-------------------开始语法分析--------------------------------------" << endl;
	if (lexicalAnalysisWordAndTypeList.size() == 0) {
		cout << "因为词法分析无结果，所以无需进行语法分析。" << endl;
	}
	else {
		bool isSyntaxOK = mathExpressService.checkSyntax(lexicalAnalysisWordAndTypeList);
		if (isSyntaxOK) {
			cout << "语法检测正确" << endl;
		}
		else {
			cout << "语法错误" << endl;
		}
	}


	//表达式求值
	string strResult = mathExpressService.calculateExpression(lexicalAnalysisWordAndTypeList);
	cout << "---------------------------------------------------------" << endl;
	cout << "表达式求值的结果：" << normalizedExpression << strResult << endl;

}
