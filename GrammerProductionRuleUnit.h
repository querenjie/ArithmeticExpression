#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
	�����װ�ķ��е�һ������ʽ�����磺E->FE'
	���õķ�ʽ��
		GrammerProductionRuleUnit gpru("E", "F E'");
*/
class GrammerProductionRuleUnit {
private:
	string leftsideVn;		//����ʽ��ߵķ��ս��������ע��������ֻ��һ�����ս��������û�������ս������ս����
	vector<string> rightsideVnVtVector;		//����ʽ�ұߵ��ս������ս���ļ��ϣ�����ǰ���˳���ŵġ�

public:
	/*
		���캯��
		��һ��������	����ʽ����ߵ�Ψһ�ķ��ս��������ֻ����ôһ�����ս����������������Ԫ�ش��ڡ�
		�ڶ���������	����ʽ�ұߵı��ʽ�����еĸ�����֮����һ���ո�������Ա�ʶ���ս�����������ʽ��һ���ַ�����
		���ã�			����һ������ֱ�Ӹ�ֵ��˽������leftsideVn���ڶ�������Ҫ���ݿո��з��ַ�����ȡ�����еĸ������ţ����η���˽������rightsideVnVts�����У�ͬʱ����Ԫ�ظ������浽˽������lengthOfRightsideVnVtsArray�С�
	*/
	GrammerProductionRuleUnit(string leftsideVn, string rightsideVnVtStatement);
	
	~GrammerProductionRuleUnit();

	string getLeftsideVn();		//��ȡ����ʽ��ߵķ��ս����

	vector<string> getRightsideVnVtVector();	//��ȡ����ʽ�ұߵ��ս������ս���ļ���
};

/*
	string leftsideVn��	����ʽ����ߵ�Ψһ�ķ��ս��������ֻ����ôһ�����ս����������������Ԫ�ش��ڡ�
	string rightsideVnVtStatement�� ����ʽ�ұߵı��ʽ�����еĸ�����֮����һ���ո�������Ա�ʶ���ս�����������ʽ��һ���ַ��������磺F E��
*/
GrammerProductionRuleUnit::GrammerProductionRuleUnit(string leftsideVn, string rightsideVnVtStatement) {
	this->leftsideVn = leftsideVn;

	//���´����Ƕ��ұߵı��ʽ�����зַ��ţ����ΰѷ���д��rightsideVnVtVector
	int lengthOfRightsideVnVtStatement = rightsideVnVtStatement.size();
	string currentSymbol = "";
	for (int i = 0; i < lengthOfRightsideVnVtStatement; i++) {
		char c = rightsideVnVtStatement.at(i);
		if (c != ' ') {
			currentSymbol = currentSymbol.append(1, c);
		}
		else {
			if (currentSymbol != "") {
				this->rightsideVnVtVector.push_back(currentSymbol);
				currentSymbol = "";
			}
		}
	}
	if (currentSymbol != "") {
		this->rightsideVnVtVector.push_back(currentSymbol);
		currentSymbol = "";
	}

}

GrammerProductionRuleUnit::~GrammerProductionRuleUnit() {

}

string GrammerProductionRuleUnit::getLeftsideVn() {
	return this->leftsideVn;
}

vector<string> GrammerProductionRuleUnit::getRightsideVnVtVector() {
	return this->rightsideVnVtVector;
}