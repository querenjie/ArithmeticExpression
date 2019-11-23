#pragma once
#include "iostream"

/********************************
	˫��ѭ������ڵ���
********************************/
template<class T>
class DoubleLinkNode {
	template<class T>
	friend class DoubleLinkList;

	template<class T>
	friend class DoubleLinkListForExpression;
private:
	T	data;		//�ڵ��е�����
	DoubleLinkNode<T>* prior;		//ǰ��ָ��
	DoubleLinkNode<T>* next;		//����ָ��
public:
	DoubleLinkNode(const T data) {
		this->data = data;
		this->prior = this;
		this->next = this;
	}
};