#pragma once
#include "iostream"
#include "GlobalConstVars.h"
#include <stdlib.h>
#include "DoubleLinkNode.h"

using namespace std;

/********************************
	���Ա�Ĵ�ͷ�ڵ��ѭ��˫����洢��
********************************/
template <class T>
class DoubleLinkList {
private:
	DoubleLinkNode<T>*	head;		//ͷָ��
	int					size;		//��������ȣ�������ͷ�ڵ㣩
public:
	DoubleLinkList(T initData);		//���캯������ʼ��ͷ�ڵ㡣
	~DoubleLinkList();				//����������������������ʵ����

	int getSize();					//��ȡ˫��������Ԫ�ظ���
	void destoryList();				//��������˫������ʵ��
	void deleteAllDataNode();		//���ѭ��˫�������������ݽڵ�Ļ���ѭ��ɾ��ÿ��β�ͽڵ㣬ֱ��ֻʣ��ͷ�ڵ㡣
	Status insert(int i, T e);		//�ڵ�i��λ�ò���ֵe��i�Ǵ�1��ʼ��������
	void traverse();				//����ѭ��˫������
	Status del(int i);				//ɾ����i��λ�õĽڵ�
	/*
	*	����һ�����ݴ���ѭ��˫������
	*	arrData:	����
	*	size:		������Ԫ�ظ���
	*	doubleLinkListObj��	���ɵ�Ŀ��ѭ��˫������
	*/
	Status createList(T* arrData, int size, DoubleLinkList<T>& doubleLinkListObj);
	void delAll(T data);			//ɾ��ѭ��˫������������ָ�������ݡ�
	DoubleLinkNode<T>* getHead();
	void setSize(int size);
	T getVal(int i);				//��ȡ��i��λ���ϵ�ֵ
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
			//���ѭ��˫�������������ݽڵ�Ļ���ѭ��ɾ��ÿ��β�ͽڵ㣬ֱ��ֻʣ��ͷ�ڵ㡣
			deleteAllDataNode();
		}
		//Ȼ��ɾ��ͷ�ڵ�
		DoubleLinkNode<T>* p = this->head;
		p->next = NULL;
		p->prior = NULL;
		cout << "����ͷ�ڵ� p(" << p->data << ")" << endl;
		free(p);
	}
}

template<class T>
void DoubleLinkList<T>::deleteAllDataNode() {
	DoubleLinkNode<T>* p = this->head;
	while (size > 0) {
		p = this->head->prior;			//pָ�����һ���ڵ�
		DoubleLinkNode<T>* q = p;		//�½�qָ��Ҳָ��β�ڵ�
		p = p->prior;					//pָ��ǰһ���ڵ�
		q->next = NULL;					//�������һ���ڵ��nextָ��
		q->prior = NULL;				//�������һ���ڵ��priorָ��
		if (p != NULL) {
			p->next = this->head;			//���ڶ����ڵ��nextָ��ָ��ͷ�ڵ�
		}
		this->head->prior = p;			//ͷ�ڵ��priorָ��ָ�����ڶ����ڵ�
		cout << "����β�����ݽڵ� q(" << q->data << ")" << endl;
		free(q);
		this->size--;					//˫�������ȼ�1
	}
}

template<class T>
Status DoubleLinkList<T>::insert(int i, T e) {
	if (i < 1 || i > size + 1) {
		cout << "����λ�ó�����Χ����ȷ��Χ��1<=i<=" << (size + 1) << endl;
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
		cout << "ѭ��˫�������ǿյ�" << endl;
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
		cout << "ѭ��˫�������ǿյģ�����ɾ���κνڵ㡣" << endl;
		return ERROR;
	}
	if (i < 1 || i > size) {
		cout << "λ��i=" << i << "����������Χ�ڡ�λ��������Χ��[1," << size << "]" << endl;
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
	cout << "ɾ���˵� " << i << " ���ڵ�(" << q->data << ")" << endl;
	free(q);
	this->size--;
}

template<class T>
Status DoubleLinkList<T>::createList(T* arrData, int size, DoubleLinkList<T>& doubleLinkListObj) {
	if (!arrData) {
		cout << "�������ݲ���ΪNULL" << endl;
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
		cout << "ѭ��˫�������ǿյģ�û�����ݿ�ɾ��" << endl;
		return;
	}
	DoubleLinkNode<T>* p = this->head;
	DoubleLinkNode<T>* q = this->head->next;
	while (q != this->head) {
		if (q->data == data) {
			//ɾ��q�ڵ�
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
		cout << "ѭ��˫�������ǿյ�" << endl;
	}
	if (i < 1 || i > size) {
		cout << "λ��i=" << i << "����������Χ�ڡ�λ��������Χ��[1," << size << "]" << endl;
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