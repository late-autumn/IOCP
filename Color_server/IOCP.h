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
	SOCKET				m_listenSock;			// Ŭ���̾�Ʈ�� ������ �ޱ����� Listen ����	

	HANDLE				m_hIocp;
	DWORD				m_dwWorkerThreadCnt;	// ������ WorkerThread ��
	std::vector<HANDLE> m_vWorkerThreadHandle;

	HANDLE				m_hTimerThreadHandle;
	std::multimap<DWORD, int> m_timerQueue;

	CUserManager		m_UserManager;			// �������� ����
	CFieldManager		m_FieldManager;			// �� ����

public:
	CStateManager       m_StateManager;         // ���¸ӽ�
	// �ð� ���� 
	double		 m_start_time;					// ���� ���� �ð�
	double		 m_Max_time;					// ���� ���� �ð�
	double		 m_curtime;						// ���� ���� �ð�
	struct _timeb start, finish;				//�ð� ���� �Լ�

public:
	CIocp(void);
	~CIocp(void);

	void				Err_Display(LPWSTR msg);
	void				Err_Quit(LPWSTR msg);

	bool				Begin();									  // �ʱ�ȭ �� ����
	bool				End();										  // ����

	bool				InitSocket();								  // ������ �ʱ�ȭ�ϴ� �Լ�
	bool				BindandListen(int nBindPort);			      // Bind �� Listen
	bool				StartServer();								  // ���� ����

	bool				CreateWorkerThread(int workerThreadCnt = 0);  // WorkerThread ����
	void				WorkerThread();								  // �۾��� ������
	bool				CreateTimerThread();						  // TimerThread ����
	void				TimerThread();								  // �ð����� ������

	void				DestroyThread();							  // ���� ������ ����
	void				CloseSocket(SOCKET &client_sock);             // ���� �ݱ�

	void				Process_packet(VOID *object, char *buf);      // ��ŶŸ�Կ� ���� ó��

	void				SendPacket(CUser *user, char *buf);			  // �⺻ ��Ŷ ����

	void				SendPacketType(BYTE type, CUser *user, CUser *to = NULL, CField *field = NULL, char* buf = NULL);// ������Ŷ Ÿ�Ժз�
	void				SendPacketAllUser(BYTE type, CUser *user = NULL, char* buf = NULL);	  // ��� �������� ������
	void				SendPacketAllUserInSameField(BYTE type, CUser *user, char* buf = NULL);

	// ���� �Լ�
	void				 GameSkillUse(CUser *user, char *_packet);	//  ���� ��ų ��Ŷ
	void				 GameItemUse(CUser *user);					//  ���� ������ ��Ŷ
	void				 SendColorPacketToUser(CUser* user);		//  ���� ���� ���� ��Ŷ
	void				 SendColorPacket(BYTE type, int num, int color, CField* field); // �ʵ� ���� �� ���� 
	void				 SendTimePacket(BYTE type, CField* field);		// ���� ��ü �ð� ��Ŷ
	void				 SendPlayerStatePacket(BYTE type, CField* field); 
};
