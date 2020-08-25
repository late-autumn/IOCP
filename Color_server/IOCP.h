#define WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once

#include "Define.h"
#include "SingleTon.h"
#include "UserManager.h"
#include "FieldManager.h"
#include "StateManager.h"
#include "MonsterManager.h"
#include "NPCManager.h"
#include "protocol.h"
#include "Object.h"
#include "CriticalSection.h"
#include "Lobby.h"

class CIocp : public CSingleTon<CIocp>
{
	SOCKET				m_listenSock;			// 클라이언트의 접속을 받기위한 Listen 소켓	

	HANDLE				m_hIocp;
	DWORD				m_dwWorkerThreadCnt;	// 생성된 WorkerThread 수
	std::vector<HANDLE> m_vWorkerThreadHandle;

	HANDLE				m_hTimerThreadHandle;
	std::multimap<DWORD, int> m_timerQueue;

	CUserManager		m_UserManager;			// 접속유저 관리
	CFieldManager		m_FieldManager;			// 방 관리

public:
	CStateManager       m_StateManager;         // 상태머신
	// 시간 정보 
	double		 m_start_time;					// 게임 시작 시간
	double		 m_Max_time;					// 게임 종료 시간
	double		 m_curtime;						// 게임 진행 시간
	struct _timeb start, finish;				//시간 측정 함수

public:
	CIocp(void);
	~CIocp(void);

	void				Err_Display(LPWSTR msg);
	void				Err_Quit(LPWSTR msg);

	bool				Begin();									  // 초기화 및 세팅
	bool				End();										  // 종료

	bool				InitSocket();								  // 소켓을 초기화하는 함수
	bool				BindandListen(int nBindPort);			      // Bind 및 Listen
	bool				StartServer();								  // 서버 시작

	bool				CreateWorkerThread(int workerThreadCnt = 0);  // WorkerThread 생성
	void				WorkerThread();								  // 작업자 쓰레드
	bool				CreateTimerThread();						  // TimerThread 생성
	void				TimerThread();								  // 시간관리 쓰레드

	void				DestroyThread();							  // 생성 쓰레드 제거
	void				CloseSocket(SOCKET &client_sock);             // 소켓 닫기

	void				Process_packet(VOID *object, char *buf);      // 패킷타입에 따른 처리

	void				SendPacket(CUser *user, char *buf);			  // 기본 패킷 전송

	void				SendPacketType(BYTE type, CUser *user, CUser *to = NULL, CField *field = NULL, char* buf = NULL);// 전송패킷 타입분류
	void				SendPacketAllUser(BYTE type, CUser *user = NULL, char* buf = NULL);	  // 모든 유저에게 보내기
	void				SendPacketAllUserInSameField(BYTE type, CUser *user, char* buf = NULL);

	// 게임 함수
	void				 GameSkillUse(CUser *user, char *_packet);	//  유저 스킬 패킷
	void				 GameItemUse(CUser *user);					//  유저 아이템 패킷
	void				 SendColorPacketToUser(CUser* user);		//  영역 색깔 변경 패킷
	void				 SendColorPacket(BYTE type, int num, int color, CField* field); // 필드 영역 색 변경 
	void				 SendTimePacket(BYTE type, CField* field);		// 게임 전체 시간 패킷
	void				 SendPlayerStatePacket(BYTE type, CField* field); 
};
