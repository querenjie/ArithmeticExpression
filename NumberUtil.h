#pragma once
#include <iostream>

class NumberUtil {
private:
	char numarrIn0_9[10] = { '0','1','2','3','4','5','6','7','8','9' };
	char numarrIn1_9[9] = { '1','2','3','4','5','6','7','8','9' };
public:
	bool isIn0_9(char c);		//�ַ�����c��ֵ�Ƿ�����'0'��'9'֮���ֵ
	bool isIn1_9(char c);		//�ַ�����c��ֵ�Ƿ�����'1'��'9'֮���ֵ
};

bool NumberUtil::isIn0_9(char c) {
	for (int i = 0; i < sizeof(numarrIn0_9) / sizeof(char); i++) {
		if (c == numarrIn0_9[i]) {
			return true;
		}
	}
	return false;
}

bool NumberUtil::isIn1_9(char c) {
	for (int i = 0; i < sizeof(numarrIn1_9) / sizeof(char); i++) {
		if (c == numarrIn1_9[i]) {
			return true;
		}
	}
	return false;
}