#pragma once
#include "iostream"

/********************************
	双向循环链表节点类
********************************/
template<class T>
class DoubleLinkNode {
	template<class T>
	friend class DoubleLinkList;

	template<class T>
	friend class DoubleLinkListForExpression;
private:
	T	data;		//节点中的数据
	DoubleLinkNode<T>* prior;		//前驱指针
	DoubleLinkNode<T>* next;		//后驱指针
public:
	DoubleLinkNode(const T data) {
		this->data = data;
		this->prior = this;
		this->next = this;
	}
};