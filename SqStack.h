#pragma once
#include "iostream"
#include "GlobalConstVars.h"

using namespace std;

/********************************
	˳��ջ�洢��
********************************/
const int STACK_INIT_SIZE = 1;	//�洢�ռ��ʼ������
const int STACKINCREMENT = 10;		//�洢�ռ��������
template<class SElemType>
class SqStack {
private:
	SElemType*	base;		//��ջ����֮ǰ������֮��base��ֵΪNULL
	SElemType*	top;		//ջ��ָ��
	int			stacksize;	//��ǰ�ѷ���Ĵ洢�ռ䣬��Ԫ��Ϊ��λ
public:
	SqStack();
	~SqStack();
	void		DestoryStack();			//����ջ
	void		ClearStack();			//��Ϊ��ջ
	bool		StackEmpty();			//��Ϊ��ջ����true�����򷵻�false��
	int			StackLength();			//����ջ��Ԫ�ظ�������ջ�ĳ��ȡ�
	Status		Pop(SElemType &e);		//ɾ��������ջ��Ԫ��
	Status		Push(SElemType e);		//Ԫ��ѹջ
	Status		GetTop(SElemType &e);	//��ջ���շ���ջ��Ԫ��

};

template<class SElemType>
SqStack<SElemType>::SqStack() {
	base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!base) {
		cout << "��ʼ��ջʧ��" << endl;
		stacksize = -1;
		top = NULL;
		base = NULL;
		return;
	}
	top = base;
	stacksize = STACK_INIT_SIZE;
}

//����ջ
template<class SElemType>
void SqStack<SElemType>::DestoryStack() {
	if (base) {
		delete base;
	}
	top = base = NULL;
	cout << "ջ�����١�" << endl;
}

//��Ϊ��ջ
template<class SElemType>
void SqStack<SElemType>::ClearStack() {
	if (base) {
		top = base;
	}
}

//��Ϊ��ջ����true�����򷵻�false��
template<class SElemType>
bool SqStack<SElemType>::StackEmpty() {
	if (base) {
		return top - base == 0 ? true : false;
	}
	return true;
}

//����ջ��Ԫ�ظ�������ջ�ĳ��ȡ�
template<class SElemType>
int SqStack<SElemType>::StackLength() {
	if (base) {
		return top - base;
	}
	return 0;
}

//ɾ��������ջ��Ԫ��
template<class SElemType>
Status SqStack<SElemType>::Pop(SElemType &e) {
	if (top && base && top - base > 0) {
		e = *--top;
		return OK;
	}
	cout << "ջ�����ڻ�գ��޷�����ջ��Ԫ��" << endl;
	return ERROR;
}

template<class SElemType>
Status SqStack<SElemType>::Push(SElemType e) {
	if (!base) {
		cout << "��δ��ʼ��ջ���޷���Ԫ��ѹջ" << endl;
		return ERROR;
	}
	if (top - base >= stacksize) {
		cout << "Ϊջ��չ�ڴ�ռ�..." << endl;
		SElemType* tempbase = (SElemType*)realloc(base, (stacksize + STACKINCREMENT) * sizeof(SElemType));
		if (!tempbase) {
			cout << "����洢�ռ�ʧ��" << endl;
			exit(OVERFLOW);
		}
		base = tempbase;
		top = base + stacksize;
		stacksize += STACKINCREMENT;
		cout << "Ŀǰջ�ռ��С��" << stacksize << endl;
	}
	*top++ = e;
	return OK;
}

template<class SElemType>
SqStack<SElemType>::~SqStack() {
	DestoryStack();
}

template<class SElemType>
Status SqStack<SElemType>::GetTop(SElemType &e) {
	if (top && base && top - base > 0) {
		e = *(top - 1);
		return OK;
	}
	cout << "ջ����Ԫ��" << endl;
	return ERROR;
}