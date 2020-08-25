#define WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once

#include "Iocp.h"

int main(){
	cout << "IOCP 서버 초기화" << endl;
	SERVER->Begin();
	cout << "IOCP 서버 실행" << endl;
	SERVER->StartServer();
	SERVER->End();
	return 0;
}
