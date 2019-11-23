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
	*	���ڹ淶�����ı���Ϊ��-���������ڸ����Ŀ�ͷ���֣�Ҳ�����ǲ�������
	*	Ϊ������DFA�ܷ���ʶ��-�����������ڲ������Ŀ�ͷ���ֻ��ǲ����������ǲ�ȡ���·�ʽ�淶���ʽ��
	*	��������p1ָ��ָ��'-'֮ǰ�Ľڵ㣬p2ָ��ָ����֮��Ľڵ㡣
	*	���һ�����p1�ڵ��ֵ��'('����p2�ڵ��ֵ��')'�����Ա��ʽ���κβ�����������p2�ڵ��ֵ����')'������p1֮�����'('����p2֮ǰ����')'��
	*	����������p1�ڵ��ֵ��{����,')'}�е�һ�֣�����p1֮�����'+('����p2֮ǰ����')'��
	*	����������p1�ڵ��ֵ��{'+','-','*','/'}�е�һ�֣�����p1֮�����'('����p2֮ǰ����')'��
	*
	*	ǰ�������ĸ�����������жϵ��أ������ǣ���ͷΪ'-'��֮������������֣���֮�������ֻ�С���㲢��ֻ����һ��С���㣬���һ��һ�������֡�
	*	����dblLinkListObj�������һ���Ѿ�ȥ�����еĿո���������ʽ��
	*
	*	������ʽ�����һ��Ԫ��ֵ����'='�����ڱ��ʽ��������Ԫ��'='
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
		int pos_p = 1;		//pָ��ָ����ǵڼ���Ԫ��
		int pos_p1 = 0;		//p1ָ��ָ����ǵڼ���Ԫ��
		int pos_p2 = 0;		//p2ָ��ָ����ǵڼ���Ԫ��
		NumberUtil numberUtil;
		while (p && p != head) {	//�������нڵ�
			if (p->data == '-') {
				//����p1��λ��
				p1 = p->prior;
				pos_p1 = pos_p - 1;
				p = p->next;
				pos_p++;
				//�˴���������ֵ����ȷ�Լ��飬��ȷ�Լ����������DFA�зִʵ�ʱ��ִ�С�����ֻҪ�Ǹ����ֻ�'.'������ˡ�
				bool hasNumber = false;	//'-'�����Ƿ�������
				while (numberUtil.isIn0_9(p->data) || '.' == p->data) {
					p = p->next;
					pos_p++;
					hasNumber = true;
				}
				if (hasNumber) {	//���ǰ���Ǹ�������������p2��λ��
					p2 = p;
					pos_p2 = pos_p;
				}
				if (pos_p1 >= 0 && pos_p2 > pos_p1) {
					//���һ�����p1�ڵ��ֵ��'('����p2�ڵ��ֵ����')'������p1֮�����'('����p2֮ǰ����')'��
					if ((p1->data == '(' && p2->data != ')') || p1->data == ')') {
						dblLinkListObj.insert(pos_p1 + 1, '(');
						pos_p2 = pos_p2 + 1;		//����ǰ�������1���ڵ㣬����pos_p2��λ�þͻ�����1
						dblLinkListObj.insert(pos_p2, ')');
						p = p->next;
						pos_p = pos_p + 3;	//����ǰ���ܹ�������2���ڵ㣬����p��λ���ֺ�����1λ������pos_pҪ��ԭ�ȵĻ����ϼ�3
					}
					else if (numberUtil.isIn0_9(p1->data)) {
						//����������p1�ڵ��ֵ��{����,')'}�е�һ�֣�����p1֮�����'+('����p2֮ǰ����')'��
						dblLinkListObj.insert(pos_p1 + 1, '+');
						dblLinkListObj.insert(pos_p1 + 2, '(');
						pos_p2 = pos_p2 + 2;		//����ǰ�������2���ڵ㣬����pos_p2��λ�þͻ�����2
						dblLinkListObj.insert(pos_p2, ')');
						p = p->next;
						pos_p = pos_p + 4;	//����ǰ���ܹ�������3���ڵ㣬����p��λ���ֺ�����1λ������pos_pҪ��ԭ�ȵĻ����ϼ�4
					}
					else if (p1->data == '+' || p1->data == '-' || p1->data == '*' || p1->data == '/') {
						//����������p1�ڵ��ֵ��{'+','-','*','/'}�е�һ�֣�����p1֮�����'('����p2֮ǰ����')'��
						dblLinkListObj.insert(pos_p1 + 1, '(');
						pos_p2 = pos_p2 + 1;	//����ǰ�������1���ڵ㣬����pos_p2��λ�þͻ�����1
						dblLinkListObj.insert(pos_p2, ')');
						p = p->next;
						pos_p = pos_p + 3;	//����ǰ���ܹ�������2���ڵ㣬����p��λ���ֺ�����1λ������pos_pҪ��ԭ�ȵĻ����ϼ�3
					}
				}
			}
			else {
				p = p->next;
				pos_p++;
			}
		}
		//������ʽ�����һ��Ԫ��ֵ����'='�����ڱ��ʽ��������Ԫ��'='
		size = dblLinkListObj.getSize();
		if (dblLinkListObj.getVal(size) != '=') {
			dblLinkListObj.insert(size + 1, '=');
		}
	}
}