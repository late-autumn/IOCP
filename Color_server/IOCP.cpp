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
	// 모든 멤버 변수들 초기화
	m_listenSock = INVALID_SOCKET;
	m_hIocp = NULL;
	
	m_dwWorkerThreadCnt = 0;
	m_vWorkerThreadHandle.clear();
}


CIocp::~CIocp(void)
{
	COLOR_CS->DeleteCS();
	// 윈속의 사용을 끝낸다.
	WSACleanup();

	// 객체를 삭제
	m_UserManager.End();
}

// ------------------------------------------ 
// 소켓 함수 오류 출력 후 종료
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
// 소켓 함수 오류 출력
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
	// 소켓 초기화
	if (!InitSocket())	return false;
	// 소켓 연결 및 대기
	if (!BindandListen(9000))	return false;
	// 작업자 쓰레드 생성
	if (!CreateWorkerThread())	return false;
	// 시간관리 쓰레드 생성
	if (!CreateTimerThread())	return false;

	return true;
}

bool CIocp::End()
{
	m_FieldManager.End();
	m_StateManager.End();
	// Thread 종료
	DestroyThread();
	// 객체를 초기화
	for (auto i = m_UserManager.m_vUser.begin(); i != m_UserManager.m_vUser.end(); ++i)
	{
		if ((*i) == NULL) continue;

		if ((*i)->m_socket != INVALID_SOCKET)
		{
			CloseSocket((*i)->m_socket);
		}
	}

	// 윈속의 사용을 끝낸다.
	WSACleanup();

	return true;
}

bool CIocp::InitSocket()
{
	COLOR_CS->InitCS();
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return false;

	// TCP 소켓 생성
	m_listenSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_listenSock) return false;
	cout << "윈속 초기화 및 TCP 소켓 생성 성공" << endl;
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
	// 입출력 완료 포트 생성
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_hIocp == NULL) return false;

	if (workerThreadCnt == 0)
	{
		// CPU 개수 확인
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		// (CPU 개수 * 2)개
		m_dwWorkerThreadCnt = si.dwNumberOfProcessors * 2;
	}
	else
	{
		m_dwWorkerThreadCnt = workerThreadCnt;
	}
	// 작업자 스레드 생성
	HANDLE hThread;
	for (DWORD i = 0; i < m_dwWorkerThreadCnt; ++i)
	{
		hThread = CreateThread(NULL, 0, CallWorkerThread, this, 0, NULL);
		if (hThread == NULL) return false;
		m_vWorkerThreadHandle.push_back(hThread);
	}
	cout << "생성된 WorkerThread 수 : " << m_dwWorkerThreadCnt << endl;
	return true;
}

bool CIocp::CreateTimerThread()
{
	// Timer 스레드 생성
	m_hTimerThreadHandle = CreateThread(NULL, 0, CallTimerThread, this, 0, NULL);
	if (m_hTimerThreadHandle == NULL) return false;
	cout << "TimerThread 생성완료 " << endl;
	return true;
}

void CIocp::DestroyThread()
{
	if (m_hIocp != NULL)
	{
		for (auto i = m_vWorkerThreadHandle.begin(); i != m_vWorkerThreadHandle.end(); ++i)
		{
			// WaitingThread Queue에서 대기중인 쓰레드에 사용자 종료 메세지를 보낸다
			PostQueuedCompletionStatus(m_hIocp, 0, 0, NULL);
		}

		for (auto i = m_vWorkerThreadHandle.begin(); i != m_vWorkerThreadHandle.end(); ++i)
		{
			// 쓰레드 핸들을 닫고 쓰레드가 종료될 때까지 기다린다.
			CloseHandle((*i));
			WaitForSingleObject((*i), INFINITE);
		}
	}
	closesocket(m_listenSock);
}

void CIocp::CloseSocket(SOCKET &client_sock)
{
	struct linger stLinger = { 0, 0 };		// SO_DONTLINGER 로 설정
	// socketClose소켓의 데이터 송수신을 모두 중단 시킨다.
	shutdown(client_sock, SD_BOTH);
	// 소켓 옵션을 설정한다.
	setsockopt(client_sock, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
	// 소켓 연결을 종료 시킨다.
	closesocket(client_sock);
}

// Accept() 클라이언트 접속 받기 
bool CIocp::StartServer()
{
	int retval;
	// 데이터 통신에 사용할 변수
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
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		// 접속유저 생성 및 초기화
		user = m_UserManager.AddUser(client_sock);
		if (NULL == user)
		{
			// 로그인 실패 패킷 전송
			cout << "접속인원 초과!";
			cout << "  현재인원: " << m_UserManager.GetCurrentUserCount() << "명  최대인원 : " << m_UserManager.GetMaxUserCount() << "명" << endl;
			closesocket(client_sock);

			continue;
		}
		// 소켓과 접속유저 연결
		CreateIoCompletionPort((HANDLE)user->m_socket, m_hIocp, user->GetNumber(), 0);

		BOOL optval = TRUE;
		setsockopt(user->m_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&optval, sizeof(optval));
		// 비동기 입출력 시작
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
		// 비동기 입출력 완료 기다리기
		BOOL retval = TRUE;
		DWORD iosize = 0;
		int num = -1;
		CUser *user = NULL;
		stOverlapped *overlapped = NULL;
		retval = GetQueuedCompletionStatus(m_hIocp, &iosize,
			(LPDWORD)&num, (LPOVERLAPPED *)&overlapped, INFINITE);
		// 유저의 경우이므로 유저를 얻음
		if (overlapped->m_eType == TYPE_SEND || overlapped->m_eType == TYPE_RECV)
		{
			user = m_UserManager.GetUser(num);
			if (false == user->m_Login)
			{
				continue;
			}
		}

		// 클라이언트가 종료되었을 경우(리턴값이 FALSE 이거나 전송된 데이터가 0일 경우)
		if (retval == FALSE || iosize == 0)
		{
			if (false == user->m_Login)
			{
				continue;
			}
			// 클라이언트 정보 얻기
			SOCKADDR_IN clientaddr;
			int addrlen = sizeof(clientaddr);
			getpeername(user->m_socket, (SOCKADDR *)&clientaddr, &addrlen);
			printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

			user->m_Login = false;
			CloseSocket(user->m_socket);
			// 유저가 방에 들어가있을 경우 방에서 나가고 방 안 다른 유저에게 나간 것을 보낸다.
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
			m_UserManager.DeleteUser(user); // 유저 제거
			printf("현재 접속중인 클라이언트 수(m_nClientCnt) : %d\n", m_UserManager.GetCurrentUserCount());
			continue;
		}
		if (overlapped->m_eType == TYPE_RECV)
		{
			int restDataSize = iosize;
			char *packet_start = overlapped->m_IocpBuf;
			while (restDataSize != 0)
			{
				//미완성 패킷이 존재하지 않고, 패킷이 처음부터 전송됨
				if (overlapped->recv_packet_size == 0)
					overlapped->recv_packet_size = (int)packet_start[0];

				int required = overlapped->recv_packet_size - overlapped->m_nRemainLen;

				// 패킷을 더이상 만들수 없음
				if (restDataSize < required)
				{
					memcpy(overlapped->m_packetBuf + overlapped->m_nRemainLen,
						packet_start, restDataSize);
					overlapped->m_nRemainLen += restDataSize;
					break;
				}
				else // 패킷을 완성할 수 있음
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
			// 방 업데이트 TimerThread()에서 호출된다.
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
			printf("default overlapped 타입\n");
			delete overlapped;
		}
	}
}

void CIocp::Process_packet(VOID *object, char *buf)
{
	CUser* user = (CUser*)object;

	if (user == NULL)	return; // 전달된 값이 없으면 취소
	CLobby *lobby = NULL;
	CField *field = NULL;
	CMonster *mon = NULL;
	CMapObject obj;
	CUser* DamagedUser = NULL;
	CNpc * npc = NULL;
	DWORD hitnum = 0;

	switch (buf[1])
	{
		////// 로비 패킷
	case CS_LOGIN: // 접속번호 보내기
		cs_login login_packet;
		memcpy(&login_packet, buf, sizeof(login_packet));
		memcpy(user->m_id, login_packet.LoginID, sizeof(user->m_id));
		SendPacketType(SC_LOGIN_OK, user);
		//cout << " 로그인 오케이 " << endl;
		//break;
		//if (!lobby->GetLobbyState())
		//{
		//	lobby->FieldStart();

		//}
		// 현재 자동 방생성 및 접속되게 해둔 상태이므로
		// 이부분은 나중에 방입장 패킷으로 옮겨야한다.
		// 방으로 자동입장, 유저의 방위치 번호를 저장한다.
	case CS_LOBBYJOIN: //로비 참가
		cs_lobbyjoin lobby_pakcet;
		memcpy(&lobby_pakcet, buf, sizeof(lobby_pakcet));
		memcpy(user->m_id, lobby_pakcet.LoginID, sizeof(user->m_id));
		SendPacketType(SC_LOBBYJOIN_OK, user);
		//cout << "로비 접속 완료 " << endl;
		break;

	case CS_CHARACTERSELECT:
		cs_characterselect character_packet;
		memcpy(&character_packet, buf, sizeof(character_packet));
		memcpy(&user->m_color, &character_packet.color, sizeof(user->m_color));
		memcpy(&user->m_selectCharacter, &character_packet.character, sizeof(user->m_selectCharacter));
		memcpy(user->m_id, character_packet.LoginID, sizeof(user->m_id));
		//cout << " 캐릭터 선택 완료 : 아이디 - " << character_packet.LoginID << " 색 " << (int)(character_packet.color) << " 타입 " << (int)(character_packet.character) << endl;
		
		user->GameInit();			// 캐릭 지정 함수
		//lobby = m_FieldManager.QuickJoinLobby(user);

		field = m_FieldManager.QuickJoin(user);
		//if (NULL == lobby)
		//{
		//	cout << "로비입장 추가 실패 " << endl;
		//}
		//cout << "유저 아이디 :" << user->m_id << "접속 번호 : " << user->GetNumber() << "방 번호 : " << user->m_FieldNumber << endl;

		//case CS_READY:
		if (NULL == field)
		{
			cout << "방입장 추가 실패 " << endl;
		}
		sc_start start_packet;
		start_packet.size = sizeof(start_packet);
		start_packet.type = SC_START;
		SendPacket(user,(char*)&start_packet);
		// 지금 이 이부분을 세팅해줘야 게임이 실행됨.
		// 나중에 게임준비될때 부르도록 위치 변경해야됨구역 생성
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
					cout << "내 정보 보내주기 " << endl;
					SendPacketType(SC_PUT_PLAYER, user, field->m_paUser[i], field);
					SendPacketType(SC_CHARACTERSELECT, user, field->m_paUser[i], field);
				}
			}
		// 다른사람의 정보를 나에게
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
				cout << " 다른 사람의 정보 보여주기 " << endl;
				SendPacketType(SC_PUT_PLAYER, field->m_paUser[i], user, field);
				SendPacketType(SC_CHARACTERSELECT, field->m_paUser[i], user, field);
			}
		}
		cout << "다른사람 정보 보내주기 끝 " << endl;
		// 모든 몬스터의 정보 나에게
		field->SendAllMonster(user);
		// 모든 NPC의 정보 나에게
		field->SendAllNpc(user);
		break;
		////// 게임패킷
	case CS_MOVE: // 이동
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
		cout << "무브 패킷 전송 " << endl;
		break;

	case CS_ATTACK: // 일반공격
		cs_attack atk_packet;
		memcpy(&atk_packet, buf, sizeof(atk_packet));
		user->m_dirX = atk_packet.x;
		user->m_dirY = atk_packet.y;
		user->m_dirZ = atk_packet.z;
		field = user->GetField();
		// 유저 공격모션 다른 유저에게 보냄
		sc_player_atk sc_atk_packet;
		sc_atk_packet.size = sizeof(sc_atk_packet);
		sc_atk_packet.type = SC_ATK;
		sc_atk_packet.num = user->GetNumber();
		SendPacketAllUserInSameField(SC_ATK, user, (char *)(&sc_atk_packet));
		
		// 피격된 몬스터 
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
		//	cout << "와일 첫번째 " << endl;
			//field->UseSkillToMonster(user, skill_packet.skillslot, skill_packet.dirX, skill_packet.dirY, skill_packet.dirZ, hitnum);

			//mon = field->m_vMonster[hitnum];

			cs_skill cs_skill_packet;
			memcpy(&cs_skill_packet, buf, sizeof(cs_skill_packet));
			user->m_dirX = cs_skill_packet.dirX;
			user->m_dirY = cs_skill_packet.dirY;
			user->m_dirZ = cs_skill_packet.dirZ;
			field = user->GetField();
			// 유저 공격모션 다른 유저에게 보냄
			sc_player_skill_atk sc_skill_packet;
			sc_skill_packet.size = sizeof(sc_skill_packet);
			sc_skill_packet.type = SC_SKILL;
			sc_skill_packet.skilltype = SC_HIT;
			sc_skill_packet.dirX = user->m_dirX;
			sc_skill_packet.dirY = user->m_dirY;
			sc_skill_packet.dirZ = user->m_dirZ;
			SendPacketAllUserInSameField(SC_SKILL, user, (char *)(&sc_skill_packet));
			// 피격된 몬스터 
			field->UseSkillToMonster(user, cs_skill_packet.skillslot, cs_skill_packet.dirX, cs_skill_packet.dirY, cs_skill_packet.dirZ, hitnum);
			cout << "스킬 전송 " << endl;

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
				cout << " SC_HIT 전송 " << endl;
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
		field->SetColorNumInColorMap(color_packet.num, color_packet.color);	//클라에서 색을 받으면, 플레이어가 있는 방의 색상 맵 정보를 변경한다.

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
	case CS_DEFAULT_PACKET: // 임시로 설정해둔것 지울예정
		break;

	default:
		printf("Default packet\n");
	}
}

// 게임중인 방마다 시간을 측정하여 WorkerThread를 호출하고 몬스터의 AI를 작동시킨다.
void CIocp::TimerThread()
{
	cout << "TimerThread() 가동중 " << endl;
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
		cout << "타이머 스레드 시작 " << endl;
		startTime = GetTickCount();
		for (fielditer = m_FieldManager.m_vField.begin(); fielditer != m_FieldManager.m_vField.end(); ++fielditer)
		{
			if ((*fielditer) == NULL)
			{
				continue;
			}
			time = delay + GetTickCount();
			// 게임중일때만 동작하도록 한다.
			if ((*fielditer)->GetGameState() == GAME_PLAY)
			{
				m_start_time = (finish.time * 1000 + finish.millitm) - (start.time * 1000 + start.millitm);
				m_Max_time = 10000;
				m_curtime = m_Max_time - m_start_time / 1000;

				//위에 시간 정보를 모든 유저에게 보낸다.
				m_timerQueue.insert(pair<DWORD, int>(time, (*fielditer)->GetNumber() + ROOM_NUM_START));
				_ftime(&finish);
				//printf("->소요 시간 : %.2f seconds.\n", m_curtime);
				SendTimePacket(SC_TIME, (*fielditer));
				(*fielditer)->Update();
				cout << "타이머 스레드에서 필드 업데이트 " << endl;
				SendPlayerStatePacket(SC_DEBUFF, (*fielditer));
				SendPlayerStatePacket(SC_REVIVE, (*fielditer));
				// 게임종료
				if (m_curtime == 0)
				{
					if (MON_BOSS == NULL)
						{
							cout << "게임종료" << endl;
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

		// 루프당 1초도 안걸릴경우 딜레이를 준다.
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
		cout << "에러 : 유저값 NULL" << endl;
		return;
	}
	if (user->m_socket == INVALID_SOCKET)
	{
		cout << "에러 : 소켓 빔 " << endl;
		return;
	}
	if (user->m_Login == false)
	{
		//cout << "에러 : 접속종료된 유저" << endl;
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
	// 보낼 대상이 없을 경우 본인에게
	// user도 NULL 일경운 아래 if문으로 제한된다.
	if (to == NULL)					{ to = user; }
	// 전달된 user와 보낼대상이 없을 경우 에러
	if (user == NULL && to == NULL)	{ return; }
	// 만약 미리 전달받은 패킷 버퍼가 있으면, type 검사를 하지 않고 바로 보낸다.
	if (buf != NULL)				{ SendPacket(to, buf); return; }

	switch (type){
	case SC_LOGIN_OK: // 로그인 성공(유저 접속번호를 전송한다)
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
		SendColorPacketToUser(user);	//갓 접속한 유저에게 '모든' 색상 정보를 제공한다.
		break;
	case SC_POS: // 플레이어 위치 및 회전값
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
	default: // 임시 예외처리
		printf("Default Type Packet\n");
	}
}

// 접속한 모든 유저에게 전송하는 함수(아직 안쓰임)
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

// 방에 있는 모든 유저에게 전송하는 함수

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

//임의로 만든 개인용 함수
void CIocp::SendColorPacketToUser(CUser* user)
{
	sc_color_change change_packet;
	change_packet.size = sizeof(change_packet);
	change_packet.type = SC_COLOR_CHANGE;

	for (int i = 0; i < 32 * 32; i++)
	{
		if (user->GetField()->GetColorNumInColorMap(i) > 0)
		{
			//변경된 땅의 번호
			change_packet.num = i;
			//변경된 땅의 색상
			change_packet.color = user->GetField()->GetColorNumInColorMap(i);
			//보내기
			SendPacket(user, (char *)(&change_packet));
		}
	}
}

void CIocp::SendTimePacket(BYTE type, CField* field)
{
	sc_time time_packet;
	//패킷 크기
	time_packet.size = sizeof(time_packet);
	//패킷 번호
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
	CMonster *monHit = NULL; // 피격당한 몬스터
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
			// 블라인드시 3초후 정상 
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
				// 스턴 시 4초후 정상
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
				// 죽었을 시 3초후 부활 
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