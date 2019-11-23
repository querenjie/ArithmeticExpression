#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;

/**
*	��ѧ���ʽ��ȷ���������Զ���
*/
class MathExpressDFA {
private:
	const char charArray1[6] = { '+', '-', '*', '/', ')', '=' };
	const char charArray2[13] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '(', ')' };
	const char charArray3[11] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(' };

	/*
	*	�ж�ָ�����ַ��Ƿ���ָ���ļ����д���
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
	MathExpressDFA();			//���췽������ʼ��statusMap
	~MathExpressDFA();			//��������

	const int OPERAND = 100;		//�����������״̬���
	const int OPERATOR = 101;		//�����������״̬���
	map<string, int> statusMap;		//���״̬���ĸ���״̬ӳ�䡣���е�key�ĸ�ʽ�� ({״̬���}+','+{������ַ�}),���磺0,1��ӳ���ֵ���µ�״̬��š�

};

/**
*	�ڹ��캯���й���DFA
*	DFA�ṹ�������£�
*		������ĸ��{'0..9', '+', '-', '*', '/', '(', ')', '='}
*		����״̬����{0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, OPERAND=>(100), OPERATOR=>(101)}
*		��̬��0
*		��̬��{OPERAND=>(100), OPERATOR=>(101)}
*		��ֵӳ���ϵ��
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

	//��ʼ��statusMap
	//��״̬0��ʼ����״̬ӳ��
	for (int i = 1; i < 10; i++) {
		statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(to_string(i)), 1));	//��״̬0��ʼ����ȡ'1'..'9'�е��κ�һ���ַ�������״̬1
	}
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '-'), 4));			//��״̬0��ʼ����ȡ'-'������״̬4
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '0'), 5));			//��״̬0��ʼ����ȡ'0'������״̬5
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '('), 6));			//��״̬0��ʼ����ȡ'('������״̬6
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, ')'), 7));			//��״̬0��ʼ����ȡ')'������״̬7
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '+'), 8));			//��״̬0��ʼ����ȡ'+'������״̬8
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '*'), 10));		//��״̬0��ʼ����ȡ'*'������״̬10
	statusMap.insert(pair<string, int>(to_string(0).append(1, ',').append(1, '/'), 11));		//��״̬0��ʼ����ȡ'/'������״̬11

	//��״̬1��ʼ����״̬ӳ��
	for (int i = 0; i < 10; i++) {
		statusMap.insert(pair<string, int>(to_string(1).append(1, ',').append(to_string(i)), 1));	//��״̬1��ʼ����ȡ'0'..'9'�е��κ�һ���ַ�������״̬1
	}
	statusMap.insert(pair<string, int>(to_string(1).append(1, ',').append(1, '.'), 2));			//��״̬1��ʼ����ȡ'.'������״̬2
	for (int i = 0; i < sizeOfCharArray1; i++) {
		//��״̬1��ʼ����ȡ{ '+', '-', '*', '/', ')', '=' }�е��κ�һ���ַ�������״̬OPERAND
		statusMap.insert(pair<string, int>(to_string(1).append(1, ',').append(1, charArray1[i]), OPERAND));			
	}

	//��״̬2��ʼ����״̬ӳ��
	for (int i = 0; i < 10; i++) {
		statusMap.insert(pair<string, int>(to_string(2).append(1, ',').append(to_string(i)), 3));	//��״̬2��ʼ����ȡ'0'..'9'�е��κ�һ���ַ�������״̬3
	}

	//��״̬3��ʼ����״̬ӳ��
	for (int i = 0; i < 10; i++) {
		statusMap.insert(pair<string, int>(to_string(3).append(1, ',').append(to_string(i)), 3));	//��״̬3��ʼ����ȡ'0'..'9'�е��κ�һ���ַ�������״̬3
	}
	for (int i = 0; i < sizeOfCharArray1; i++) {
		//��״̬3��ʼ����ȡ{ '+', '-', '*', '/', ')', '=' }�е��κ�һ���ַ�������״̬OPERAND
		statusMap.insert(pair<string, int>(to_string(3).append(1, ',').append(1, charArray1[i]), OPERAND));
	}

	//��״̬4��ʼ����״̬ӳ��
	for (int i = 1; i < 10; i++) {
		statusMap.insert(pair<string, int>(to_string(4).append(1, ',').append(to_string(i)), 1));	//��״̬4��ʼ����ȡ'1'..'9'�е��κ�һ���ַ�������״̬1
	}
	statusMap.insert(pair<string, int>(to_string(4).append(1, ',').append(1, '0'), 5));					//��״̬4��ʼ����ȡ'0'������״̬5
	statusMap.insert(pair<string, int>(to_string(4).append(1, ',').append(1, '('), OPERATOR));			//��״̬4��ʼ����ȡ'('������״̬OPERATOR

	//��״̬5��ʼ����״̬ӳ��
	statusMap.insert(pair<string, int>(to_string(5).append(1, ',').append(1, '.'), 2));			//��״̬5��ʼ����ȡ'.'������״̬2
	for (int i = 0; i < sizeOfCharArray1; i++) {
		//��״̬5��ʼ����ȡ{ '+', '-', '*', '/', ')', '=' }�е��κ�һ���ַ�������״̬OPERAND
		statusMap.insert(pair<string, int>(to_string(5).append(1, ',').append(1, charArray1[i]), OPERAND));
	}

	//��״̬6��ʼ����״̬ӳ��
	for (int i = 0; i < sizeOfCharArray2; i++) {
		//��״̬6��ʼ����ȡ{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '(', ')' }�е��κ�һ���ַ�������״̬OPERATOR
		statusMap.insert(pair<string, int>(to_string(6).append(1, ',').append(1, charArray2[i]), OPERATOR));
	}

	//��״̬7��ʼ����״̬ӳ��
	for (int i = 0; i < sizeOfCharArray1; i++) {
		//��״̬7��ʼ����ȡ{ '+', '-', '*', '/', ')', '=' }�е��κ�һ���ַ�������״̬OPERATOR
		statusMap.insert(pair<string, int>(to_string(7).append(1, ',').append(1, charArray1[i]), OPERATOR));
	}

	//��״̬8��ʼ����״̬ӳ��
	for (int i = 0; i < sizeOfCharArray3; i++) {
		//��״̬8��ʼ����ȡ{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(' }�е��κ�һ���ַ�������״̬OPERATOR
		statusMap.insert(pair<string, int>(to_string(8).append(1, ',').append(1, charArray3[i]), OPERATOR));
	}

	//��״̬10��ʼ����״̬ӳ��
	for (int i = 0; i < sizeOfCharArray3; i++) {
		//��״̬10��ʼ����ȡ{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(' }�е��κ�һ���ַ�������״̬OPERATOR
		statusMap.insert(pair<string, int>(to_string(10).append(1, ',').append(1, charArray3[i]), OPERATOR));
	}

	//��״̬11��ʼ����״̬ӳ��
	for (int i = 0; i < sizeOfCharArray3; i++) {
		//��״̬11��ʼ����ȡ{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(' }�е��κ�һ���ַ�������״̬OPERATOR
		statusMap.insert(pair<string, int>(to_string(11).append(1, ',').append(1, charArray3[i]), OPERATOR));
	}

	/*
	cout << "״̬�����壺" << endl;
	for (map<string, int>::iterator i = statusMap.begin(); i != statusMap.end(); i++) {
		cout << i->first << "-->" << i->second << endl;
	}
	*/
}

MathExpressDFA::~MathExpressDFA() {

}

