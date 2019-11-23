#pragma once
#include "iostream"
#include "GlobalConstVars.h"

using namespace std;

/********************************
	顺序栈存储类
********************************/
const int STACK_INIT_SIZE = 1;	//存储空间初始分配量
const int STACKINCREMENT = 10;		//存储空间分配增量
template<class SElemType>
class SqStack {
private:
	SElemType*	base;		//在栈构造之前和销毁之后，base的值为NULL
	SElemType*	top;		//栈顶指针
	int			stacksize;	//当前已分配的存储空间，以元素为单位
public:
	SqStack();
	~SqStack();
	void		DestoryStack();			//销毁栈
	void		ClearStack();			//置为空栈
	bool		StackEmpty();			//若为空栈返回true，否则返回false。
	int			StackLength();			//返回栈中元素个数，即栈的长度。
	Status		Pop(SElemType &e);		//删除并返回栈顶元素
	Status		Push(SElemType e);		//元素压栈
	Status		GetTop(SElemType &e);	//若栈不空返回栈顶元素

};

template<class SElemType>
SqStack<SElemType>::SqStack() {
	base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!base) {
		cout << "初始化栈失败" << endl;
		stacksize = -1;
		top = NULL;
		base = NULL;
		return;
	}
	top = base;
	stacksize = STACK_INIT_SIZE;
}

//销毁栈
template<class SElemType>
void SqStack<SElemType>::DestoryStack() {
	if (base) {
		delete base;
	}
	top = base = NULL;
	cout << "栈被销毁。" << endl;
}

//置为空栈
template<class SElemType>
void SqStack<SElemType>::ClearStack() {
	if (base) {
		top = base;
	}
}

//若为空栈返回true，否则返回false。
template<class SElemType>
bool SqStack<SElemType>::StackEmpty() {
	if (base) {
		return top - base == 0 ? true : false;
	}
	return true;
}

//返回栈中元素个数，即栈的长度。
template<class SElemType>
int SqStack<SElemType>::StackLength() {
	if (base) {
		return top - base;
	}
	return 0;
}

//删除并返回栈顶元素
template<class SElemType>
Status SqStack<SElemType>::Pop(SElemType &e) {
	if (top && base && top - base > 0) {
		e = *--top;
		return OK;
	}
	cout << "栈不存在或空，无法返回栈顶元素" << endl;
	return ERROR;
}

template<class SElemType>
Status SqStack<SElemType>::Push(SElemType e) {
	if (!base) {
		cout << "尚未初始化栈，无法将元素压栈" << endl;
		return ERROR;
	}
	if (top - base >= stacksize) {
		cout << "为栈扩展内存空间..." << endl;
		SElemType* tempbase = (SElemType*)realloc(base, (stacksize + STACKINCREMENT) * sizeof(SElemType));
		if (!tempbase) {
			cout << "补充存储空间失败" << endl;
			exit(OVERFLOW);
		}
		base = tempbase;
		top = base + stacksize;
		stacksize += STACKINCREMENT;
		cout << "目前栈空间大小：" << stacksize << endl;
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
	cout << "栈中无元素" << endl;
	return ERROR;
}