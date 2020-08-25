#define WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include "Iocp.h"

DWORD WINAPI CallWorkerThread(LPVOID arg)
{
	CIocp* pCIocp = (CIocp*)arg;
	pCIocp->WorkerThread();
	return 0;
}

DWORD WINAPI CallTimerThread(LPVOID arg)
{
	CIocp* pCIocp = (CIocp*)arg;
	pCIocp->TimerThread();
	return 0;
}

CIocp::CIocp(void)
{
	// ��� ��� ������ �ʱ�ȭ
	m_listenSock = INVALID_SOCKET;
	m_hIocp = NULL;
	
	m_dwWorkerThreadCnt = 0;
	m_vWorkerThreadHandle.clear();
}


CIocp::~CIocp(void)
{
	COLOR_CS->DeleteCS();
	// ������ ����� ������.
	WSACleanup();

	// ��ü�� ����
	m_UserManager.End();
}

// ------------------------------------------ 
// ���� �Լ� ���� ��� �� ����
// ------------------------------------------ 
void CIocp::Err_Quit(LPWSTR msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

// ------------------------------------------ 
// ���� �Լ� ���� ���
// ------------------------------------------ 
void CIocp::Err_Display(LPWSTR msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}

bool CIocp::Begin()
{
	m_StateManager.Begin();
	m_UserManager.Begin(MAX_USER);
	m_FieldManager.Begin(MAX_ROOM);
	// ���� �ʱ�ȭ
	if (!InitSocket())	return false;
	// ���� ���� �� ���
	if (!BindandListen(9000))	return false;
	// �۾��� ������ ����
	if (!CreateWorkerThread())	return false;
	// �ð����� ������ ����
	if (!CreateTimerThread())	return false;

	return true;
}

bool CIocp::End()
{
	m_FieldManager.End();
	m_StateManager.End();
	// Thread ����
	DestroyThread();
	// ��ü�� �ʱ�ȭ
	for (auto i = m_UserManager.m_vUser.begin(); i != m_UserManager.m_vUser.end(); ++i)
	{
		if ((*i) == NULL) continue;

		if ((*i)->m_socket != INVALID_SOCKET)
		{
			CloseSocket((*i)->m_socket);
		}
	}

	// ������ ����� ������.
	WSACleanup();

	return true;
}

bool CIocp::InitSocket()
{
	COLOR_CS->InitCS();
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return false;

	// TCP ���� ����
	m_listenSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_listenSock) return false;
	cout << "���� �ʱ�ȭ �� TCP ���� ���� ����" << endl;
	return true;

}

bool CIocp::BindandListen(int nBindPort)
{
	int retval;

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(nBindPort);
	retval = ::bind(m_listenSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) return false;

	// listen()
	retval = listen(m_listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR) return false;

	return true;
}

bool CIocp::CreateWorkerThread(int workerThreadCnt)
{
	// ����� �Ϸ� ��Ʈ ����
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_hIocp == NULL) return false;

	if (workerThreadCnt == 0)
	{
		// CPU ���� Ȯ��
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		// (CPU ���� * 2)��
		m_dwWorkerThreadCnt = si.dwNumberOfProcessors * 2;
	}
	else
	{
		m_dwWorkerThreadCnt = workerThreadCnt;
	}
	// �۾��� ������ ����
	HANDLE hThread;
	for (DWORD i = 0; i < m_dwWorkerThreadCnt; ++i)
	{
		hThread = CreateThread(NULL, 0, CallWorkerThread, this, 0, NULL);
		if (hThread == NULL) return false;
		m_vWorkerThreadHandle.push_back(hThread);
	}
	cout << "������ WorkerThread �� : " << m_dwWorkerThreadCnt << endl;
	return true;
}

bool CIocp::CreateTimerThread()
{
	// Timer ������ ����
	m_hTimerThreadHandle = CreateThread(NULL, 0, CallTimerThread, this, 0, NULL);
	if (m_hTimerThreadHandle == NULL) return false;
	cout << "TimerThread �����Ϸ� " << endl;
	return true;
}

void CIocp::DestroyThread()
{
	if (m_hIocp != NULL)
	{
		for (auto i = m_vWorkerThreadHandle.begin(); i != m_vWorkerThreadHandle.end(); ++i)
		{
			// WaitingThread Queue���� ������� �����忡 ����� ���� �޼����� ������
			PostQueuedCompletionStatus(m_hIocp, 0, 0, NULL);
		}

		for (auto i = m_vWorkerThreadHandle.begin(); i != m_vWorkerThreadHandle.end(); ++i)
		{
			// ������ �ڵ��� �ݰ� �����尡 ����� ������ ��ٸ���.
			CloseHandle((*i));
			WaitForSingleObject((*i), INFINITE);
		}
	}
	closesocket(m_listenSock);
}

void CIocp::CloseSocket(SOCKET &client_sock)
{
	struct linger stLinger = { 0, 0 };		// SO_DONTLINGER �� ����
	// socketClose������ ������ �ۼ����� ��� �ߴ� ��Ų��.
	shutdown(client_sock, SD_BOTH);
	// ���� �ɼ��� �����Ѵ�.
	setsockopt(client_sock, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
	// ���� ������ ���� ��Ų��.
	closesocket(client_sock);
}

// Accept() Ŭ���̾�Ʈ ���� �ޱ� 
bool CIocp::StartServer()
{
	int retval;
	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD flags;
	CUser* user = NULL;
	CField* Field = NULL;
	CLobby* Lobby = NULL;
	

	while (1){

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = WSAAccept(m_listenSock, (SOCKADDR *)&clientaddr, &addrlen, NULL, NULL);
		if (client_sock == INVALID_SOCKET)
		{
			Err_Display(L"accept()");
			break;
		}
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		// �������� ���� �� �ʱ�ȭ
		user = m_UserManager.AddUser(client_sock);
		if (NULL == user)
		{
			// �α��� ���� ��Ŷ ����
			cout << "�����ο� �ʰ�!";
			cout << "  �����ο�: " << m_UserManager.GetCurrentUserCount() << "��  �ִ��ο� : " << m_UserManager.GetMaxUserCount() << "��" << endl;
			closesocket(client_sock);

			continue;
		}
		// ���ϰ� �������� ����
		CreateIoCompletionPort((HANDLE)user->m_socket, m_hIocp, user->GetNumber(), 0);

		BOOL optval = TRUE;
		setsockopt(user->m_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&optval, sizeof(optval));
		// �񵿱� ����� ����
		flags = 0;
		retval = WSARecv(user->m_socket, &user->m_stRecvOverlapped.m_wsaBuf, 1, NULL,
			&flags, &user->m_stRecvOverlapped.m_wsaOverlapped, NULL);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				Err_Display(L"WSARecv()");
				cout << "Accept(WSARecv) Error" << endl;
			}
		}
	}

	return true;
}


void CIocp::WorkerThread()
{
	while (1)
	{
		// �񵿱� ����� �Ϸ� ��ٸ���
		BOOL retval = TRUE;
		DWORD iosize = 0;
		int num = -1;
		CUser *user = NULL;
		stOverlapped *overlapped = NULL;
		retval = GetQueuedCompletionStatus(m_hIocp, &iosize,
			(LPDWORD)&num, (LPOVERLAPPED *)&overlapped, INFINITE);
		// ������ ����̹Ƿ� ������ ����
		if (overlapped->m_eType == TYPE_SEND || overlapped->m_eType == TYPE_RECV)
		{
			user = m_UserManager.GetUser(num);
			if (false == user->m_Login)
			{
				continue;
			}
		}

		// Ŭ���̾�Ʈ�� ����Ǿ��� ���(���ϰ��� FALSE �̰ų� ���۵� �����Ͱ� 0�� ���)
		if (retval == FALSE || iosize == 0)
		{
			if (false == user->m_Login)
			{
				continue;
			}
			// Ŭ���̾�Ʈ ���� ���
			SOCKADDR_IN clientaddr;
			int addrlen = sizeof(clientaddr);
			getpeername(user->m_socket, (SOCKADDR *)&clientaddr, &addrlen);
			printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

			user->m_Login = false;
			CloseSocket(user->m_socket);
			// ������ �濡 ������ ��� �濡�� ������ �� �� �ٸ� �������� ���� ���� ������.
			//CLobby* Lobby = user->GetLobby();
			//if (Lobby != NULL)
			//{
			//	Lobby->LeaveUser(user);
			//}
			CField *Field = user->GetField();
			if (Field != NULL)
			{
				Field->LeaveUser(user);
			}
			m_UserManager.DeleteUser(user); // ���� ����
			printf("���� �������� Ŭ���̾�Ʈ ��(m_nClientCnt) : %d\n", m_UserManager.GetCurrentUserCount());
			continue;
		}
		if (overlapped->m_eType == TYPE_RECV)
		{
			int restDataSize = iosize;
			char *packet_start = overlapped->m_IocpBuf;
			while (restDataSize != 0)
			{
				//�̿ϼ� ��Ŷ�� �������� �ʰ�, ��Ŷ�� ó������ ���۵�
				if (overlapped->recv_packet_size == 0)
					overlapped->recv_packet_size = (int)packet_start[0];

				int required = overlapped->recv_packet_size - overlapped->m_nRemainLen;

				// ��Ŷ�� ���̻� ����� ����
				if (restDataSize < required)
				{
					memcpy(overlapped->m_packetBuf + overlapped->m_nRemainLen,
						packet_start, restDataSize);
					overlapped->m_nRemainLen += restDataSize;
					break;
				}
				else // ��Ŷ�� �ϼ��� �� ����
				{
					memcpy(overlapped->m_packetBuf + overlapped->m_nRemainLen,
						packet_start, required);

					Process_packet(user, overlapped->m_packetBuf);

					overlapped->m_nRemainLen = 0;
					restDataSize -= required;
					packet_start += required;
					overlapped->recv_packet_size = 0;
				}
			}

			DWORD flags = 0;
			retval = WSARecv(user->m_socket, &overlapped->m_wsaBuf, 1, NULL, &flags, &overlapped->m_wsaOverlapped, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					//Err_Display(L"WSARecv()");
				}
				continue;
			}
		}
		else if (overlapped->m_eType == TYPE_SEND)
		{
			
			delete overlapped;
		}
		else if (overlapped->m_eType == TYPE_ROOM)
		{
			// �� ������Ʈ TimerThread()���� ȣ��ȴ�.
			//Lock(m_RoomManager.m_Lock);
			CField* field = m_FieldManager.GetField(num - ROOM_NUM_START);
			//UnLock(m_RoomManager.m_lock);
			if (field == NULL)
			{
				delete overlapped;
				continue;
			}
				delete overlapped;
		}
		else
		{
			printf("default overlapped Ÿ��\n");
			delete overlapped;
		}
	}
}

void CIocp::Process_packet(VOID *object, char *buf)
{
	CUser* user = (CUser*)object;

	if (user == NULL)	return; // ���޵� ���� ������ ���
	CLobby *lobby = NULL;
	CField *field = NULL;
	CMonster *mon = NULL;
	CMapObject obj;
	CUser* DamagedUser = NULL;
	CNpc * npc = NULL;
	DWORD hitnum = 0;

	switch (buf[1])
	{
		////// �κ� ��Ŷ
	case CS_LOGIN: // ���ӹ�ȣ ������
		cs_login login_packet;
		memcpy(&login_packet, buf, sizeof(login_packet));
		memcpy(user->m_id, login_packet.LoginID, sizeof(user->m_id));
		SendPacketType(SC_LOGIN_OK, user);
		//cout << " �α��� ������ " << endl;
		//break;
		//if (!lobby->GetLobbyState())
		//{
		//	lobby->FieldStart();

		//}
		// ���� �ڵ� ����� �� ���ӵǰ� �ص� �����̹Ƿ�
		// �̺κ��� ���߿� ������ ��Ŷ���� �Űܾ��Ѵ�.
		// ������ �ڵ�����, ������ ����ġ ��ȣ�� �����Ѵ�.
	case CS_LOBBYJOIN: //�κ� ����
		cs_lobbyjoin lobby_pakcet;
		memcpy(&lobby_pakcet, buf, sizeof(lobby_pakcet));
		memcpy(user->m_id, lobby_pakcet.LoginID, sizeof(user->m_id));
		SendPacketType(SC_LOBBYJOIN_OK, user);
		//cout << "�κ� ���� �Ϸ� " << endl;
		break;

	case CS_CHARACTERSELECT:
		cs_characterselect character_packet;
		memcpy(&character_packet, buf, sizeof(character_packet));
		memcpy(&user->m_color, &character_packet.color, sizeof(user->m_color));
		memcpy(&user->m_selectCharacter, &character_packet.character, sizeof(user->m_selectCharacter));
		memcpy(user->m_id, character_packet.LoginID, sizeof(user->m_id));
		//cout << " ĳ���� ���� �Ϸ� : ���̵� - " << character_packet.LoginID << " �� " << (int)(character_packet.color) << " Ÿ�� " << (int)(character_packet.character) << endl;
		
		user->GameInit();			// ĳ�� ���� �Լ�
		//lobby = m_FieldManager.QuickJoinLobby(user);

		field = m_FieldManager.QuickJoin(user);
		//if (NULL == lobby)
		//{
		//	cout << "�κ����� �߰� ���� " << endl;
		//}
		//cout << "���� ���̵� :" << user->m_id << "���� ��ȣ : " << user->GetNumber() << "�� ��ȣ : " << user->m_FieldNumber << endl;

		//case CS_READY:
		if (NULL == field)
		{
			cout << "������ �߰� ���� " << endl;
		}
		sc_start start_packet;
		start_packet.size = sizeof(start_packet);
		start_packet.type = SC_START;
		SendPacket(user,(char*)&start_packet);
		// ���� �� �̺κ��� ��������� ������ �����.
		// ���߿� �����غ�ɶ� �θ����� ��ġ �����ؾߵʱ��� ����
		COLOR_CS->EnterCS();
		if (!field->GetFieldState())
		{
			field->m_Map1 = m_FieldManager.m_Map1;
			field->SetMonster(m_FieldManager.m_vMonster);
			field->SetNpc(m_FieldManager.m_vNpc);
			field->GameLoading();
			field->RoomStart();
			field->GameStart();
		}
		COLOR_CS->LeaveCS();

		for (int i = 0; i < field->GetMaxUserCount(); i++)
			{
				if (field->m_paUser[i] != NULL)
				{
					if (false == field->m_paUser[i]->m_Login)
					{
						continue;
					}
					if (user == field->m_paUser[i])
					{
						continue;
					}
					cout << "�� ���� �����ֱ� " << endl;
					SendPacketType(SC_PUT_PLAYER, user, field->m_paUser[i], field);
					SendPacketType(SC_CHARACTERSELECT, user, field->m_paUser[i], field);
				}
			}
		// �ٸ������ ������ ������
		for (int i = 0; i < field->GetMaxUserCount(); i++)
		{
			if (field->m_paUser[i] != NULL)
			{
				if (false == field->m_paUser[i]->m_Login)
				{
					continue;
				}
				if (user == field->m_paUser[i])
				{
					continue;
				}
				cout << " �ٸ� ����� ���� �����ֱ� " << endl;
				SendPacketType(SC_PUT_PLAYER, field->m_paUser[i], user, field);
				SendPacketType(SC_CHARACTERSELECT, field->m_paUser[i], user, field);
			}
		}
		cout << "�ٸ���� ���� �����ֱ� �� " << endl;
		// ��� ������ ���� ������
		field->SendAllMonster(user);
		// ��� NPC�� ���� ������
		field->SendAllNpc(user);
		break;
		////// ������Ŷ
	case CS_MOVE: // �̵�
		cs_move move_packet;
		memcpy(&move_packet, buf, sizeof(move_packet));
		user->m_posX = move_packet.posx;
		user->m_posY = move_packet.posy;
		user->m_posZ = move_packet.posz;
		user->m_dirX = move_packet.dirx;
		user->m_dirY = move_packet.diry;
		user->m_dirZ = move_packet.dirz;
		user->m_rotX = move_packet.rotx;
		user->m_rotY = move_packet.roty;
		user->m_rotZ = move_packet.rotz;
		SendPacketAllUserInSameField(SC_POS, user);
		cout << "���� ��Ŷ ���� " << endl;
		break;

	case CS_ATTACK: // �Ϲݰ���
		cs_attack atk_packet;
		memcpy(&atk_packet, buf, sizeof(atk_packet));
		user->m_dirX = atk_packet.x;
		user->m_dirY = atk_packet.y;
		user->m_dirZ = atk_packet.z;
		field = user->GetField();
		// ���� ���ݸ�� �ٸ� �������� ����
		sc_player_atk sc_atk_packet;
		sc_atk_packet.size = sizeof(sc_atk_packet);
		sc_atk_packet.type = SC_ATK;
		sc_atk_packet.num = user->GetNumber();
		SendPacketAllUserInSameField(SC_ATK, user, (char *)(&sc_atk_packet));
		
		// �ǰݵ� ���� 
		field->ToAttack(user);
		
		if (mon != NULL)
		{
			sc_hit hit_packet;
			hit_packet.size = sizeof(hit_packet);
			hit_packet.type = SC_HIT;
			hit_packet.action_type = ACTION_BASIC_ATTACK;
			hit_packet.skill_type = -1;
			hit_packet.attacker_type = HIT_TYPE_PLAYER;
			hit_packet.target_type = HIT_TYPE_MONSTER;
			hit_packet.attacker_num = user->GetNumber();
			hit_packet.target_num = mon->GetNumber();
			hit_packet.curhp = mon->m_curHp;
			hit_packet.maxHp = mon->m_maxHp;
			hit_packet.posX = mon->m_posX;
			hit_packet.posZ = mon->m_posZ;

			SendPacketAllUserInSameField(SC_HIT, user, (char*)(&hit_packet));
		}
		if (DamagedUser != NULL)
		{

			sc_hit hit_packet;
			hit_packet.size = sizeof(hit_packet);
			hit_packet.type = SC_HIT;
			hit_packet.action_type = ACTION_BASIC_ATTACK;
			hit_packet.skill_type = -1;
			hit_packet.attacker_type = HIT_TYPE_PLAYER;
			hit_packet.target_type = HIT_TYPE_PLAYER;
			hit_packet.attacker_num = user->GetNumber();
			hit_packet.target_num = DamagedUser->GetNumber();
			hit_packet.curhp = DamagedUser->m_curHp;
			hit_packet.maxHp = DamagedUser->m_maxHp;
			hit_packet.posX = DamagedUser->m_posX;
			hit_packet.posZ = DamagedUser->m_posZ;

			SendPacketAllUserInSameField(SC_HIT, user, (char*)(&hit_packet));
		}
		break;
	case CS_SKILL:
		//cs_skill skill_packet;
		//memcpy(&skill_packet, buf, sizeof(skill_packet));
		//field = user->GetField();
	
		/*
		if (mon != NULL)
		{
			sc_hit hit_packet;
			hit_packet.size = sizeof(hit_packet);
			hit_packet.type = SC_HIT;
			hit_packet.action_type = ACTION_SKILL;
			hit_packet.skill_type = skill_packet.skilltype;
			hit_packet.attacker_type = HIT_TYPE_PLAYER;
			hit_packet.target_type = HIT_TYPE_MONSTER;
			hit_packet.attacker_num = user->GetNumber();
			hit_packet.target_num = mon->GetNumber();
			hit_packet.curhp = mon->m_curHp;
			hit_packet.maxHp = mon->m_maxHp;
			hit_packet.posX = mon->m_posX;
			hit_packet.posZ = mon->m_posZ;

			SendPacketAllUserInSameField(SC_HIT, user, (char*)(&hit_packet));

		}
		if (DamagedUser != NULL)
		{
			sc_hit hit_packet;
			hit_packet.size = sizeof(hit_packet);
			hit_packet.type = SC_HIT;
			hit_packet.action_type = ACTION_SKILL;
			hit_packet.skill_type = skill_packet.skilltype;
			hit_packet.attacker_type = HIT_TYPE_PLAYER;
			hit_packet.target_type = HIT_TYPE_PLAYER;
			hit_packet.attacker_num = user->GetNumber();
			hit_packet.target_num = DamagedUser->GetNumber();
			hit_packet.curhp = DamagedUser->m_curHp;
			hit_packet.maxHp = DamagedUser->m_maxHp;
			hit_packet.posX = DamagedUser->m_posX;
			hit_packet.posZ = DamagedUser->m_posZ;

			SendPacketAllUserInSameField(SC_HIT, user, (char*)(&hit_packet));*/
		//while (mon!=NULL)
		//{
		//	cout << "���� ù��° " << endl;
			//field->UseSkillToMonster(user, skill_packet.skillslot, skill_packet.dirX, skill_packet.dirY, skill_packet.dirZ, hitnum);

			//mon = field->m_vMonster[hitnum];

			cs_skill cs_skill_packet;
			memcpy(&cs_skill_packet, buf, sizeof(cs_skill_packet));
			user->m_dirX = cs_skill_packet.dirX;
			user->m_dirY = cs_skill_packet.dirY;
			user->m_dirZ = cs_skill_packet.dirZ;
			field = user->GetField();
			// ���� ���ݸ�� �ٸ� �������� ����
			sc_player_skill_atk sc_skill_packet;
			sc_skill_packet.size = sizeof(sc_skill_packet);
			sc_skill_packet.type = SC_SKILL;
			sc_skill_packet.skilltype = SC_HIT;
			sc_skill_packet.dirX = user->m_dirX;
			sc_skill_packet.dirY = user->m_dirY;
			sc_skill_packet.dirZ = user->m_dirZ;
			SendPacketAllUserInSameField(SC_SKILL, user, (char *)(&sc_skill_packet));
			// �ǰݵ� ���� 
			field->UseSkillToMonster(user, cs_skill_packet.skillslot, cs_skill_packet.dirX, cs_skill_packet.dirY, cs_skill_packet.dirZ, hitnum);
			cout << "��ų ���� " << endl;

			if (mon != NULL)
			{
				sc_hit hit_packet;
				hit_packet.size = sizeof(hit_packet);
				hit_packet.type = SC_HIT;
				hit_packet.action_type = ACTION_SKILL;
				hit_packet.skill_type = -1;
				hit_packet.attacker_type = HIT_TYPE_PLAYER;
				hit_packet.target_type = HIT_TYPE_MONSTER;
				hit_packet.attacker_num = user->GetNumber();
				hit_packet.target_num = mon->GetNumber();
				hit_packet.curhp = mon->m_curHp;
				hit_packet.maxHp = mon->m_maxHp;
				hit_packet.posX = mon->m_posX;
				hit_packet.posZ = mon->m_posZ;

				SendPacketAllUserInSameField(SC_HIT, user, (char*)(&hit_packet));
				cout << " SC_HIT ���� " << endl;
			}
			if (DamagedUser != NULL)
			{

				sc_hit hit_packet;
				hit_packet.size = sizeof(hit_packet);
				hit_packet.type = SC_HIT;
				hit_packet.action_type = ACTION_BASIC_ATTACK;
				hit_packet.skill_type = -1;
				hit_packet.attacker_type = HIT_TYPE_PLAYER;
				hit_packet.target_type = HIT_TYPE_PLAYER;
				hit_packet.attacker_num = user->GetNumber();
				hit_packet.target_num = DamagedUser->GetNumber();
				hit_packet.curhp = DamagedUser->m_curHp;
				hit_packet.maxHp = DamagedUser->m_maxHp;
				hit_packet.posX = DamagedUser->m_posX;
				hit_packet.posZ = DamagedUser->m_posZ;

				SendPacketAllUserInSameField(SC_HIT, user, (char*)(&hit_packet));
			}
		break;
	case CS_COLOR:
		cs_color color_packet;
		field = user->GetField();
		memcpy(&color_packet, buf, sizeof(color_packet));
		field->SetColorNumInColorMap(color_packet.num, color_packet.color);	//Ŭ�󿡼� ���� ������, �÷��̾ �ִ� ���� ���� �� ������ �����Ѵ�.

		sc_color_change change_packet;
		change_packet.size = sizeof(change_packet);
		change_packet.type = SC_COLOR_CHANGE;
		change_packet.num = color_packet.num;
		change_packet.color = color_packet.color;
		SendPacketAllUserInSameField(SC_COLOR_CHANGE, user, (char*)(&change_packet)); 
		break;
	case CS_BUFF:
		//GameItemUse(user);
		break;
	case CS_DEFAULT_PACKET: // �ӽ÷� �����صа� ���￹��
		break;

	default:
		printf("Default packet\n");
	}
}

// �������� �渶�� �ð��� �����Ͽ� WorkerThread�� ȣ���ϰ� ������ AI�� �۵���Ų��.
void CIocp::TimerThread()
{
	cout << "TimerThread() ������ " << endl;
	std::vector<CField*>::iterator fielditer;
	std::multimap<DWORD, int>::iterator timerQueiter;
	CField *field;
	DWORD time = 0;
	DWORD startTime, endTime;
	DWORD delay = 200;
	_ftime(&start);
	int num;
	while (1)
	{
		cout << "Ÿ�̸� ������ ���� " << endl;
		startTime = GetTickCount();
		for (fielditer = m_FieldManager.m_vField.begin(); fielditer != m_FieldManager.m_vField.end(); ++fielditer)
		{
			if ((*fielditer) == NULL)
			{
				continue;
			}
			time = delay + GetTickCount();
			// �������϶��� �����ϵ��� �Ѵ�.
			if ((*fielditer)->GetGameState() == GAME_PLAY)
			{
				m_start_time = (finish.time * 1000 + finish.millitm) - (start.time * 1000 + start.millitm);
				m_Max_time = 10000;
				m_curtime = m_Max_time - m_start_time / 1000;

				//���� �ð� ������ ��� �������� ������.
				m_timerQueue.insert(pair<DWORD, int>(time, (*fielditer)->GetNumber() + ROOM_NUM_START));
				_ftime(&finish);
				//printf("->�ҿ� �ð� : %.2f seconds.\n", m_curtime);
				SendTimePacket(SC_TIME, (*fielditer));
				(*fielditer)->Update();
				cout << "Ÿ�̸� �����忡�� �ʵ� ������Ʈ " << endl;
				SendPlayerStatePacket(SC_DEBUFF, (*fielditer));
				SendPlayerStatePacket(SC_REVIVE, (*fielditer));
				// ��������
				if (m_curtime == 0)
				{
					if (MON_BOSS == NULL)
						{
							cout << "��������" << endl;
							for (int i = 0; i < MAX_USER; ++i)
							{
								if ((*fielditer)->m_paUser[i] == NULL) continue;

								sc_gameset gamePacket;
								gamePacket.size = sizeof(gamePacket);
								gamePacket.type = SC_WIN;
								(*fielditer)->m_paUser[i]->SendPacket((char*)&gamePacket);
								//LeaveUser(m_paUser[i]);
							}
						}
					}
				}
		}
		while (!m_timerQueue.empty())
		{
			timerQueiter = m_timerQueue.begin();
			if (timerQueiter->first > GetTickCount())
			{
				break;
			}
			num = timerQueiter->second;
			m_timerQueue.erase(timerQueiter);
			stOverlapped *overlapped = new stOverlapped;
			ZeroMemory(&overlapped->m_wsaOverlapped, sizeof(OVERLAPPED));
			overlapped->m_eType = TYPE_ROOM;
			PostQueuedCompletionStatus(m_hIocp, 1, num, (LPOVERLAPPED)(overlapped));
		}

		// ������ 1�ʵ� �Ȱɸ���� �����̸� �ش�.
		endTime = GetTickCount() - startTime;
		if (endTime < delay)
		{
			DWORD result = WaitForSingleObject(m_hTimerThreadHandle, delay - endTime);
			if (result == WAIT_OBJECT_0)
			{
				return;
			}
		}
	}

}
	
void CIocp::SendPacket(CUser *user, char *buf)
{
	if (user == NULL)
	{
		cout << "���� : ������ NULL" << endl;
		return;
	}
	if (user->m_socket == INVALID_SOCKET)
	{
		cout << "���� : ���� �� " << endl;
		return;
	}
	if (user->m_Login == false)
	{
		//cout << "���� : ��������� ����" << endl;
		return;
	}
	stOverlapped *overlapped = new stOverlapped;
	DWORD io_size;

	ZeroMemory(&overlapped->m_wsaOverlapped, sizeof(OVERLAPPED));

	overlapped->m_eType = TYPE_SEND;
	overlapped->m_wsaBuf.buf = overlapped->m_IocpBuf;
	overlapped->m_wsaBuf.len = buf[0];
	memcpy(overlapped->m_IocpBuf, buf, buf[0]);

	int retval = WSASend(user->m_socket, &overlapped->m_wsaBuf, 1, &io_size, 0, &overlapped->m_wsaOverlapped, NULL);

	if (retval == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			user->m_Login = false;
			//cout << "IOCP WSASend Error : " << error << endl;
		}
	}
}

void CIocp::SendPacketType(BYTE type, CUser *user, CUser *to, CField *field, char* buf)
{
	// ���� ����� ���� ��� ���ο���
	// user�� NULL �ϰ�� �Ʒ� if������ ���ѵȴ�.
	if (to == NULL)					{ to = user; }
	// ���޵� user�� ��������� ���� ��� ����
	if (user == NULL && to == NULL)	{ return; }
	// ���� �̸� ���޹��� ��Ŷ ���۰� ������, type �˻縦 ���� �ʰ� �ٷ� ������.
	if (buf != NULL)				{ SendPacket(to, buf); return; }

	switch (type){
	case SC_LOGIN_OK: // �α��� ����(���� ���ӹ�ȣ�� �����Ѵ�)
		sc_login login_packet;
		login_packet.size = sizeof(login_packet);
		login_packet.type = type;
		login_packet.num = user->GetNumber();
		SendPacket(to, (char *)(&login_packet));
		break;
	case SC_LOBBYJOIN_OK:
		sc_lobbyjoin lobbyok_packet;
		lobbyok_packet.size = sizeof(lobbyok_packet);
		lobbyok_packet.type = type;
		lobbyok_packet.num = user->GetNumber();
		SendPacket(to, (char *)(&lobbyok_packet));
		break;

	case SC_CHARACTERSELECT:
		sc_characterselect character_packet;
		character_packet.size = sizeof(character_packet);
		character_packet.type = type;
		character_packet.num = user->GetNumber();
		character_packet.character = user->m_selectCharacter;
		character_packet.color = user->m_color;
		SendPacket(to, (char *)(&character_packet));
		break;

	case SC_PUT_PLAYER:
		sc_put_player put_player_packet;
		put_player_packet.size = sizeof(put_player_packet);
		put_player_packet.type = SC_PUT_PLAYER;
		put_player_packet.id = user->GetNumber();
		put_player_packet.x = user->m_posX;
		put_player_packet.y = user->m_posY;
		put_player_packet.z = user->m_posZ;
		put_player_packet.dirX = user->m_rotX;
		put_player_packet.dirY = user->m_rotY;
		put_player_packet.dirZ = user->m_rotZ;
		SendPacket(to, (char *)(&put_player_packet));
		SendColorPacketToUser(user);	//�� ������ �������� '���' ���� ������ �����Ѵ�.
		break;
	case SC_POS: // �÷��̾� ��ġ �� ȸ����
		sc_pos pos_packet;
		pos_packet.size = sizeof(pos_packet);
		pos_packet.type = SC_POS;
		pos_packet.num = user->GetNumber();
		pos_packet.state = user->m_characterState;
		pos_packet.posX = user->m_posX;
		pos_packet.posY = user->m_posY;
		pos_packet.posZ = user->m_posZ;
		pos_packet.rotX = user->m_rotX;
		pos_packet.rotY = user->m_rotY;
		pos_packet.rotZ = user->m_rotZ;
		SendPacketAllUserInSameField(SC_POS, user);
		break;
	default: // �ӽ� ����ó��
		printf("Default Type Packet\n");
	}
}

// ������ ��� �������� �����ϴ� �Լ�(���� �Ⱦ���)
void CIocp::SendPacketAllUser(BYTE type, CUser *user, char* buf)
{
	for (auto user_i : m_UserManager.m_vUser)
	{
		if (user_i )
		{
			SendPacketType(type, user, user_i, NULL, buf);
		}
	}

}

// �濡 �ִ� ��� �������� �����ϴ� �Լ�

void CIocp::SendPacketAllUserInSameField(BYTE type, CUser *user, char* buf)
{
	CField* field = user->GetField();

	if (field)
	{
		for (int i = 0; i < field->GetMaxUserCount(); i++)
		{
			if (field->m_paUser[i] != NULL)
			{
				if (field->m_paUser[i]->m_socket != INVALID_SOCKET && field->m_paUser[i]->m_Login == true)
					SendPacketType(type, user, field->m_paUser[i], field, buf);
			}
		}
	}
}

//���Ƿ� ���� ���ο� �Լ�
void CIocp::SendColorPacketToUser(CUser* user)
{
	sc_color_change change_packet;
	change_packet.size = sizeof(change_packet);
	change_packet.type = SC_COLOR_CHANGE;

	for (int i = 0; i < 32 * 32; i++)
	{
		if (user->GetField()->GetColorNumInColorMap(i) > 0)
		{
			//����� ���� ��ȣ
			change_packet.num = i;
			//����� ���� ����
			change_packet.color = user->GetField()->GetColorNumInColorMap(i);
			//������
			SendPacket(user, (char *)(&change_packet));
		}
	}
}

void CIocp::SendTimePacket(BYTE type, CField* field)
{
	sc_time time_packet;
	//��Ŷ ũ��
	time_packet.size = sizeof(time_packet);
	//��Ŷ ��ȣ
	time_packet.type = SC_TIME;
	time_packet.Cur_time = m_curtime;
	time_packet.Max_time = m_Max_time;
	time_packet.Start_time = m_start_time;
	for (int i = 0; i < field->GetMaxUserCount(); i++)
	{
		if (field->m_paUser[i] != NULL)
		{
			SendPacket(field->m_paUser[i], (char *)(&time_packet));
		}
	}

}

void CIocp::SendPlayerStatePacket(BYTE type, CField* field)
{
	CMonster *monHit = NULL; // �ǰݴ��� ����
	for (auto mons : field->m_vMonster)
	{
		monHit = mons;
	}
	for (int i = 0; i < MAX_USER; ++i)  // MAX_USER
	{
		if (field->m_paUser[i] != NULL)
		{
			switch (field->m_paUser[i]->m_characterState)
			{
			// ����ε�� 3���� ���� 
			case PSTATE_BLIND:
			{
				 sc_debuff debuff_packet;
				 debuff_packet.size = sizeof(debuff_packet);
				 debuff_packet.type = SC_DEBUFF;
				 debuff_packet.target_type = PSTATE_BLIND;
				 debuff_packet.num = field->m_paUser[i]->GetNumber();
				 SendPacket(field->m_paUser[i], (char *)(&debuff_packet));
				 DWORD start = GetTickCount();
				 if (start - field->m_paUser[i]->m_stateTimer > 3000)
				 {
					 field->m_paUser[i]->m_stateTimer = 0;
					 field->m_paUser[i]->m_characterState = PSTATE_IDLE;
				 }
			}
				break;
				// ���� �� 4���� ����
			case PSTATE_STUN:
			{
				sc_debuff debuff_packet;
				debuff_packet.size = sizeof(debuff_packet);
				debuff_packet.type = SC_DEBUFF;
				debuff_packet.target_type = PSTATE_STUN;
				debuff_packet.num = field->m_paUser[i]->GetNumber();
				SendPacket(field->m_paUser[i], (char *)(&debuff_packet));
				DWORD Stun_T = GetTickCount();
				if (Stun_T - field->m_paUser[i]->m_stateTimer > 4000)
				{
					field->m_paUser[i]->m_stateTimer = 0;
					field->m_paUser[i]->m_characterState = PSTATE_IDLE;
				}
			}
				break;
				// �׾��� �� 3���� ��Ȱ 
			case PSTATE_DEAD:
			{
				DWORD Die_T = GetTickCount();
				if (Die_T - field->m_paUser[i]->m_stateTimer > 3000)
				{
					field->m_paUser[i]->m_stateTimer = 0;
					field->m_paUser[i]->m_characterState = PSTATE_IDLE;
					field->Respon(field->m_paUser[i]);
					field->m_paUser[i]->m_curHp = field->m_paUser[i]->m_maxHp;
				}
			}
				break;
			}
		}
	}
}