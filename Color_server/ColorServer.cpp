#define WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once

#include "Iocp.h"

int main(){
	cout << "IOCP ���� �ʱ�ȭ" << endl;
	SERVER->Begin();
	cout << "IOCP ���� ����" << endl;
	SERVER->StartServer();
	SERVER->End();
	return 0;
}
