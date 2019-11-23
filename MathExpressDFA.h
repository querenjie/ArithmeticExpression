#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;

/**
*	数学表达式的确定的有向自动机
*/
class MathExpressDFA {
private:
	const char charArray1[6] = { '+', '-', '*', '/', ')', '=' };
	const char charArray2[13] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '(', ')' };
	const char charArray3[11] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(' };

	/*
	*	判断指定的字符是否在指定的集合中存在
	*/
	bool isInCharArray(char c, const char* charArray, const int size) {
		for (int i = 0; i < size; i++) {
			if (c == charArray[i]) {
				return true;
			}
		}
		return false;
	}

public:
	MathExpressDFA();			//构造方法，初始化statusMap
	~MathExpressDFA();			//析构方法

	const int OPERAND = 100;		//定义操作数的状态编号
	const int OPERATOR = 101;		//定义操作符的状态编号
	map<string, int> statusMap;		//存放状态机的各个状态映射。其中的key的格式是 ({状态编号}+','+{读入的字符}),例如：0,1。映射的值是新的状态编号。

};

/**
*	在构造函数中构造DFA
*	DFA结构描述如下：
*		有穷字母表：{'0..9', '+', '-', '*', '/', '(', ')', '='}
*		有限状态集：{0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, OPERAND=>(100), OPERATOR=>(101)}
*		初态：0
*		终态：{OPERAND=>(100), OPERATOR=>(101)}
*		单值映射关系：
*			(0, '1..9')-->1		(0, '-')-->4	(0, '0')-->5
*			(0, '(')-->6
*			(0, ')')-->7
*			(0, '+')-->8
*			(0, '*')-->10
*			(0, '/')-->11
*			(1, '0..9)-->1
*			(1, '.')-->2
*			(1, {'+','-','*','/',')','='})-->OPERAND
*			(2, '0..9')-->3
*			(3, '0..9')-->3
*			(3, {'+','-','*','/',')','='})-->OPERAND
*			(4, '1..9')-->1
*			(4, '(')-->OPERATOR
*			(4, '0')-->5
*			(5, '.')-->2
*			(5, {'+','-','*','/',')','='})-->OPERAND
*			(6, {'0..9', '-', '(', ')'})-->OPERATOR
*			(7, {'+','-','*','/',')','='})-->OPERATOR
*			(8, {'0..9', '('})-->OPERATOR
*			(10, {'0..9', '('})-->OPERATOR
*			(11, {'0..9', '('})-->OPERATOR
*/
MathExpressDFA::MathExpressDFA() {
	int sizeOfCharArray1 = sizeof(charArray1);
	int sizeOfCharArray2 = sizeof(charArray2);
	int sizeOfCharArray3 = sizeof(charArray3);

	//初始化statusMap
	//从状态0开始定义状态映射
	for (int i = 1; i < 10; i++) {
		statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(to_string(i)), 1));	//从状态0开始，读取'1'..'9'中的任何一个字符，进入状态1
	}
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '-'), 4));			//从状态0开始，读取'-'，进入状态4
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '0'), 5));			//从状态0开始，读取'0'，进入状态5
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '('), 6));			//从状态0开始，读取'('，进入状态6
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, ')'), 7));			//从状态0开始，读取')'，进入状态7
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '+'), 8));			//从状态0开始，读取'+'，进入状态8
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '*'), 10));		//从状态0开始，读取'*'，进入状态10
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '/'), 11));		//从状态0开始，读取'/'，进入状态11

	//从状态1开始定义状态映射
	for (int i = 0; i < 10; i++) {
		statusMap.insert(pair<string, int>(to_string(1).append(1, ',').append(to_string(i)), 1));	//从状态1开始，读取'0'..'9'中的任何一个字符，进入状态1
	}
	statusMap.insert(pair<string, int>(to_string(1).append(1, ',').append(1, '.'), 2));			//从状态1开始，读取'.'，进入状态2
	for (int i = 0; i < sizeOfCharArray1; i++) {
		//从状态1开始，读取{ '+', '-', '*', '/', ')', '=' }中的任何一个字符，进入状态OPERAND
		statusMap.insert(pair<string, int>(to_string(1).append(1, ',').append(1, charArray1[i]), OPERAND));			
	}

	//从状态2开始定义状态映射
	for (int i = 0; i < 10; i++) {
		statusMap.insert(pair<string, int>(to_string(2).append(1, ',').append(to_string(i)), 3));	//从状态2开始，读取'0'..'9'中的任何一个字符，进入状态3
	}

	//从状态3开始定义状态映射
	for (int i = 0; i < 10; i++) {
		statusMap.insert(pair<string, int>(to_string(3).append(1, ',').append(to_string(i)), 3));	//从状态3开始，读取'0'..'9'中的任何一个字符，进入状态3
	}
	for (int i = 0; i < sizeOfCharArray1; i++) {
		//从状态3开始，读取{ '+', '-', '*', '/', ')', '=' }中的任何一个字符，进入状态OPERAND
		statusMap.insert(pair<string, int>(to_string(3).append(1, ',').append(1, charArray1[i]), OPERAND));
	}

	//从状态4开始定义状态映射
	for (int i = 1; i < 10; i++) {
		statusMap.insert(pair<string, int>(to_string(4).append(1, ',').append(to_string(i)), 1));	//从状态4开始，读取'1'..'9'中的任何一个字符，进入状态1
	}
	statusMap.insert(pair<string, int>(to_string(4).append(1, ',').append(1, '0'), 5));					//从状态4开始，读取'0'，进入状态5
	statusMap.insert(pair<string, int>(to_string(4).append(1, ',').append(1, '('), OPERATOR));			//从状态4开始，读取'('，进入状态OPERATOR

	//从状态5开始定义状态映射
	statusMap.insert(pair<string, int>(to_string(5).append(1, ',').append(1, '.'), 2));			//从状态5开始，读取'.'，进入状态2
	for (int i = 0; i < sizeOfCharArray1; i++) {
		//从状态5开始，读取{ '+', '-', '*', '/', ')', '=' }中的任何一个字符，进入状态OPERAND
		statusMap.insert(pair<string, int>(to_string(5).append(1, ',').append(1, charArray1[i]), OPERAND));
	}

	//从状态6开始定义状态映射
	for (int i = 0; i < sizeOfCharArray2; i++) {
		//从状态6开始，读取{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '(', ')' }中的任何一个字符，进入状态OPERATOR
		statusMap.insert(pair<string, int>(to_string(6).append(1, ',').append(1, charArray2[i]), OPERATOR));
	}

	//从状态7开始定义状态映射
	for (int i = 0; i < sizeOfCharArray1; i++) {
		//从状态7开始，读取{ '+', '-', '*', '/', ')', '=' }中的任何一个字符，进入状态OPERATOR
		statusMap.insert(pair<string, int>(to_string(7).append(1, ',').append(1, charArray1[i]), OPERATOR));
	}

	//从状态8开始定义状态映射
	for (int i = 0; i < sizeOfCharArray3; i++) {
		//从状态8开始，读取{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(' }中的任何一个字符，进入状态OPERATOR
		statusMap.insert(pair<string, int>(to_string(8).append(1, ',').append(1, charArray3[i]), OPERATOR));
	}

	//从状态10开始定义状态映射
	for (int i = 0; i < sizeOfCharArray3; i++) {
		//从状态10开始，读取{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(' }中的任何一个字符，进入状态OPERATOR
		statusMap.insert(pair<string, int>(to_string(10).append(1, ',').append(1, charArray3[i]), OPERATOR));
	}

	//从状态11开始定义状态映射
	for (int i = 0; i < sizeOfCharArray3; i++) {
		//从状态11开始，读取{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(' }中的任何一个字符，进入状态OPERATOR
		statusMap.insert(pair<string, int>(to_string(11).append(1, ',').append(1, charArray3[i]), OPERATOR));
	}

	/*
	cout << "状态机定义：" << endl;
	for (map<string, int>::iterator i = statusMap.begin(); i != statusMap.end(); i++) {
		cout << i->first << "-->" << i->second << endl;
	}
	*/
}

MathExpressDFA::~MathExpressDFA() {

}

