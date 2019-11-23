#pragma once
#include <iostream>
#include "DoubleLinkList.h"
#include "DoubleLinkNode.h"
#include "NumberUtil.h"

template<class T>
class DoubleLinkListForExpression : public DoubleLinkList<T> {
public:
	DoubleLinkListForExpression(T initData);
	~DoubleLinkListForExpression();
	/*
	*	用于规范负数的表达，因为‘-’可能属于负数的开头部分，也可能是操作符。
	*	为了能让DFA能方便识别‘-’到底是属于操作数的开头部分还是操作符，于是采取以下方式规范表达式：
	*	首先设置p1指针指向'-'之前的节点，p2指针指向负数之后的节点。
	*	情况一：如果p1节点的值是'('并且p2节点的值是')'，不对表达式做任何插入操作。如果p2节点的值不是')'，则在p1之后插入'('，在p2之前插入')'。
	*	情况二：如果p1节点的值是{数字,')'}中的一种，则在p1之后插入'+('，在p2之前插入')'。
	*	情况三：如果p1节点的值是{'+','-','*','/'}中的一种，则在p1之后插入'('，在p2之前插入')'。
	*
	*	前面所述的负数又是如何判断的呢，规则是：开头为'-'，之后紧跟的是数字，再之后是数字或小数点并且只能有一个小数点，最后一个一定是数字。
	*	参数dblLinkListObj传入的是一个已经去除其中的空格的算术表达式。
	*
	*	如果表达式的最后一个元素值不是'='，则在表达式的最后添加元素'='
	*/
	void normalizeExpression(DoubleLinkList<T>& dblLinkListObj);
};

template<class T>
DoubleLinkListForExpression<T>::DoubleLinkListForExpression(T initData):DoubleLinkList<T>(initData) {
}

template<class T>
DoubleLinkListForExpression<T>::~DoubleLinkListForExpression() {

}

template<class T>
void DoubleLinkListForExpression<T>::normalizeExpression(DoubleLinkList<T>& dblLinkListObj) {
	int size = dblLinkListObj.getSize();
	DoubleLinkNode<T>* head = dblLinkListObj.getHead();
	if (size > 0) {
		DoubleLinkNode<T>* p = head->next;
		DoubleLinkNode<T>* p1;
		DoubleLinkNode<T>* p2 = NULL;
		int pos_p = 1;		//p指针指向的是第几个元素
		int pos_p1 = 0;		//p1指针指向的是第几个元素
		int pos_p2 = 0;		//p2指针指向的是第几个元素
		NumberUtil numberUtil;
		while (p && p != head) {	//遍历所有节点
			if (p->data == '-') {
				//设置p1的位置
				p1 = p->prior;
				pos_p1 = pos_p - 1;
				p = p->next;
				pos_p++;
				//此处不进行数值的正确性检验，正确性检验过程留到DFA切分词的时候执行。现在只要是个数字或'.'就算过了。
				bool hasNumber = false;	//'-'后面是否有数字
				while (numberUtil.isIn0_9(p->data) || '.' == p->data) {
					p = p->next;
					pos_p++;
					hasNumber = true;
				}
				if (hasNumber) {	//如果前面是个负数，则设置p2的位置
					p2 = p;
					pos_p2 = pos_p;
				}
				if (pos_p1 >= 0 && pos_p2 > pos_p1) {
					//情况一：如果p1节点的值是'('并且p2节点的值不是')'，则在p1之后插入'('，在p2之前插入')'。
					if ((p1->data == '(' && p2->data != ')') || p1->data == ')') {
						dblLinkListObj.insert(pos_p1 + 1, '(');
						pos_p2 = pos_p2 + 1;		//由于前面添加了1个节点，所以pos_p2的位置就会增加1
						dblLinkListObj.insert(pos_p2, ')');
						p = p->next;
						pos_p = pos_p + 3;	//由于前面总共插入了2个节点，加上p的位置又后移了1位，所以pos_p要在原先的基础上加3
					}
					else if (numberUtil.isIn0_9(p1->data)) {
						//情况二：如果p1节点的值是{数字,')'}中的一种，则在p1之后插入'+('，在p2之前插入')'。
						dblLinkListObj.insert(pos_p1 + 1, '+');
						dblLinkListObj.insert(pos_p1 + 2, '(');
						pos_p2 = pos_p2 + 2;		//由于前面添加了2个节点，所以pos_p2的位置就会增加2
						dblLinkListObj.insert(pos_p2, ')');
						p = p->next;
						pos_p = pos_p + 4;	//由于前面总共插入了3个节点，加上p的位置又后移了1位，所以pos_p要在原先的基础上加4
					}
					else if (p1->data == '+' || p1->data == '-' || p1->data == '*' || p1->data == '/') {
						//情况三：如果p1节点的值是{'+','-','*','/'}中的一种，则在p1之后插入'('，在p2之前插入')'。
						dblLinkListObj.insert(pos_p1 + 1, '(');
						pos_p2 = pos_p2 + 1;	//由于前面添加了1个节点，所以pos_p2的位置就会增加1
						dblLinkListObj.insert(pos_p2, ')');
						p = p->next;
						pos_p = pos_p + 3;	//由于前面总共插入了2个节点，加上p的位置又后移了1位，所以pos_p要在原先的基础上加3
					}
				}
			}
			else {
				p = p->next;
				pos_p++;
			}
		}
		//如果表达式的最后一个元素值不是'='，则在表达式的最后添加元素'='
		size = dblLinkListObj.getSize();
		if (dblLinkListObj.getVal(size) != '=') {
			dblLinkListObj.insert(size + 1, '=');
		}
	}
}