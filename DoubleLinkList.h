#pragma once
#include "iostream"
#include "GlobalConstVars.h"
#include <stdlib.h>
#include "DoubleLinkNode.h"

using namespace std;

/********************************
	线性表的带头节点的循环双链表存储类
********************************/
template <class T>
class DoubleLinkList {
private:
	DoubleLinkNode<T>*	head;		//头指针
	int					size;		//存放链表长度（不包括头节点）
public:
	DoubleLinkList(T initData);		//构造函数，初始化头节点。
	~DoubleLinkList();				//析构函数，彻底销毁链表实例。

	int getSize();					//获取双向链表中元素个数
	void destoryList();				//彻底销毁双向链表实例
	void deleteAllDataNode();		//如果循环双向链表中有数据节点的话，循环删除每个尾巴节点，直到只剩下头节点。
	Status insert(int i, T e);		//在第i个位置插入值e。i是从1开始的整数。
	void traverse();				//遍历循环双向链表
	Status del(int i);				//删除第i个位置的节点
	/*
	*	根据一串内容创建循环双向链表
	*	arrData:	内容
	*	size:		内容中元素个数
	*	doubleLinkListObj：	生成的目标循环双向链表
	*/
	Status createList(T* arrData, int size, DoubleLinkList<T>& doubleLinkListObj);
	void delAll(T data);			//删除循环双向链表中所有指定的内容。
	DoubleLinkNode<T>* getHead();
	void setSize(int size);
	T getVal(int i);				//获取第i个位置上的值
};

template<class T>
DoubleLinkList<T>::DoubleLinkList(T initData) {
	this->head = new DoubleLinkNode<T>(initData);
	if (!this->head) {
		exit(OVERFLOW);
	}
	this->size = 0;
}

template<class T>
DoubleLinkList<T>::~DoubleLinkList() {
	destoryList();
}

template<class T>
int DoubleLinkList<T>::getSize() {
	return size;
}

template <class T>
void DoubleLinkList<T>::destoryList() {
	if (this->head) {
		if (this->size > 0) {
			//如果循环双向链表中有数据节点的话，循环删除每个尾巴节点，直到只剩下头节点。
			deleteAllDataNode();
		}
		//然后删除头节点
		DoubleLinkNode<T>* p = this->head;
		p->next = NULL;
		p->prior = NULL;
		cout << "销毁头节点 p(" << p->data << ")" << endl;
		free(p);
	}
}

template<class T>
void DoubleLinkList<T>::deleteAllDataNode() {
	DoubleLinkNode<T>* p = this->head;
	while (size > 0) {
		p = this->head->prior;			//p指向最后一个节点
		DoubleLinkNode<T>* q = p;		//新建q指针也指向尾节点
		p = p->prior;					//p指向前一个节点
		q->next = NULL;					//砍断最后一个节点的next指针
		q->prior = NULL;				//砍断最后一个节点的prior指针
		if (p != NULL) {
			p->next = this->head;			//最后第二个节点的next指针指向头节点
		}
		this->head->prior = p;			//头节点的prior指针指向最后第二个节点
		cout << "销毁尾巴数据节点 q(" << q->data << ")" << endl;
		free(q);
		this->size--;					//双向链表长度减1
	}
}

template<class T>
Status DoubleLinkList<T>::insert(int i, T e) {
	if (i < 1 || i > size + 1) {
		cout << "插入位置超出范围。正确范围是1<=i<=" << (size + 1) << endl;
		return ERROR;
	}
	int j = 1;
	DoubleLinkNode<T>* p = this->head;
	DoubleLinkNode<T>* q = new DoubleLinkNode<T>(e);
	while (p && j < i) {
		p = p->next;
		j++;
	}
	if (p != NULL) {
		q->next = p->next;
		p->next->prior = q;
		q->prior = p;
		p->next = q;
		this->size++;
	}
	return OK;
}

template<class T>
void DoubleLinkList<T>::traverse() {
	if (size == 0) {
		cout << "循环双向链表是空的" << endl;
		return;
	}
	DoubleLinkNode<T>* p = this->head->next;
	while (p) {
		cout << p->data << ", ";
		p = p->next;
		if (p == this->head) {
			break;
		}
	}
	cout << endl;
}

template<class T>
Status DoubleLinkList<T>::del(int i) {
	if (size == 0) {
		cout << "循环双向链表是空的，无需删除任何节点。" << endl;
		return ERROR;
	}
	if (i < 1 || i > size) {
		cout << "位置i=" << i << "不在正常范围内。位置正常范围：[1," << size << "]" << endl;
		return ERROR;
	}

	DoubleLinkNode<T>* p = this->head;
	int j = 1;
	while (p && j < i) {
		p = p->next;
		j++;
	}
	DoubleLinkNode<T>* q = p->next;
	p->next = q->next;
	q->next->prior = p;
	q->next = NULL;
	q->prior = NULL;
	cout << "删除了第 " << i << " 个节点(" << q->data << ")" << endl;
	free(q);
	this->size--;
}

template<class T>
Status DoubleLinkList<T>::createList(T* arrData, int size, DoubleLinkList<T>& doubleLinkListObj) {
	if (!arrData) {
		cout << "传入数据不能为NULL" << endl;
		return ERROR;
	}

	DoubleLinkNode<T>* p = doubleLinkListObj.head;
	if (!p) {
		return ERROR;
	}
	for (int i = 0; i < size; i++) {
		DoubleLinkNode<T>* q = new DoubleLinkNode<T>(arrData[i]);
		if (!q) {
			return ERROR;
		}
		p->next = q;
		q->next = doubleLinkListObj.head;
		doubleLinkListObj.head->prior = q;
		q->prior = p;
		p = p->next;
		doubleLinkListObj.size++;
	}
	return OK;
}

template <class T>
void DoubleLinkList<T>::delAll(T data) {
	if (size == 0) {
		cout << "循环双向链表是空的，没有内容可删除" << endl;
		return;
	}
	DoubleLinkNode<T>* p = this->head;
	DoubleLinkNode<T>* q = this->head->next;
	while (q != this->head) {
		if (q->data == data) {
			//删除q节点
			p->next = q->next;
			q->next->prior = p;
			q->next = NULL;
			q->prior = NULL;
			free(q);
			q = p->next;
			size--;
		}
		else {
			q = q->next;
			p = p->next;
		}
	}
	
}

template<class T>
DoubleLinkNode<T>* DoubleLinkList<T>::getHead() {
	return this->head;
}

template<class T>
void DoubleLinkList<T>::setSize(int size) {
	this->size = size;
}

template<class T>
T DoubleLinkList<T>::getVal(int i) {
	if (size == 0) {
		cout << "循环双向链表是空的" << endl;
	}
	if (i < 1 || i > size) {
		cout << "位置i=" << i << "不在正常范围内。位置正常范围：[1," << size << "]" << endl;
		return NULL;
	}

	DoubleLinkNode<T>* p = this->head->next;
	int j = 1;
	while (p && j < i) {
		p = p->next;
		j++;
	}
	return p->data;
}