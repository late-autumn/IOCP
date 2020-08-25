#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Iocp.h"
#include "Lobby.h"


CLobby::CLobby()
{
	m_MaxUserCount = 100;
	for (int i = 0; i < 100; i++)
	{
		m_paUser[i] = NULL;
	}

	m_currentUserCount = 0;

	// 게임진행
	m_LobbyState = true;
	m_gameState = LOBBY_WAIT;
}


CLobby::~CLobby()
{
}

bool CLobby::Begin(DWORD _num)
{
	m_num = _num;
	for (int i = 0; i < m_MaxUserCount; i++)
	{
		m_paUser[i] = NULL;
	}
	m_MaxUserCount = 100;
	m_currentUserCount = 0;

	// 게임진행
	m_LobbyState = true;
	m_gameState = LOBBY_WAIT;

	return true;
}

void CLobby::End()
{
	m_LobbyState = true;
	m_gameState = LOBBY_WAIT;

}

// 나의 정보를 다른 유저에게 보낸다.

// 다른 유저의 정보를 나에게 보낸다.


// 해당 패킷을 방에 있는 모든 유저에게 보낸다.
void CLobby::SendAllUser(char *_packet)
{
	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL) continue;
		m_paUser[i]->SendPacket(_packet);

	}
}
// 시간 정보를 모든 유저에게 보내준다.
void CLobby::SendAllTime()
{
	sc_time time_packet;
	time_packet.size = sizeof(time_packet);
	time_packet.type = SC_TIME;
	time_packet.Start_time = m_start_time;
	time_packet.Max_time = m_Max_time;
	time_packet.Cur_time = m_curtime;
	SendAllUser((char*)(&time_packet));
}

bool CLobby::JoinUser(CUser* user)
{
	if (user == NULL) return false;

	m_gameState = GAME_WAIT;
	// 방에 인원이 다 찼으면 실패
	if (m_currentUserCount >= m_MaxUserCount) return false;

	for (int i = 0; i < m_MaxUserCount; i++)
	{
		if (NULL == m_paUser[i])
		{
			m_paUser[i] = user;
			break;
		}
	}
	user->SetLobby(this);
	user->m_FieldNumber = m_num;
	
	m_currentUserCount = min(m_currentUserCount + 1, m_MaxUserCount);

	return true;
}

bool CLobby::LeaveUser(CUser *user)
{
	if (NULL == user) return false;
	//로비에 인원이 없으면
	m_lock.lock();
	//COLOR_CS->EnterCS();
	if (m_currentUserCount <= 0)
	{
		m_lock.unlock();
		//COLOR_CS->LeaveCS();
		return false;
	}

	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (user == m_paUser[i])
		{
			user->SetLobby(NULL);
			user->m_FieldNumber = -1;
			m_paUser[i] = NULL;
			break;
		}
	}
	m_currentUserCount = max(m_currentUserCount - 1, 0);

	// 로비 내부에 있는 다른 클라이언트들에게 해당 유저 접속종료 알림
	// 본인 제외
	sc_player_disconnect remove_packet;
	remove_packet.size = sizeof(remove_packet);
	remove_packet.type = SC_REMOVE_PLAYER;
	remove_packet.num = user->GetNumber();

	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL) continue;
		if (m_paUser[i] == user) continue;

		m_paUser[i]->SendPacket((char*)(&remove_packet));

		printf("%d방 %d번 클라 접속종료 %d방 %d번 클라에게 전송\n",
			m_num, user->GetNumber(), m_paUser[i]->m_FieldNumber, m_paUser[i]->GetNumber());
	}
	m_lock.unlock();
	//COLOR_CS->LeaveCS();
	// 방에 인원이 없으면 초기화
	if (m_currentUserCount <= 0)
	{
		End();
		m_lock.lock();
		//COLOR_CS->EnterCS();
		Begin(m_num);
		//COLOR_CS->LeaveCS();
		m_lock.unlock();
	}
	return true;
}


bool CLobby::FieldStart()
{
	m_LobbyState = false;
	m_gameState = GAME_PLAY;
	_ftime(&start);
	return true;
}

void CLobby::Update()
{
	//	m_lock.lock();
	DWORD curTime = GetTickCount();
	// 유저 데이터 업데이트
	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL) continue;
		if (m_paUser[i]->m_buff.m_timer == 0) continue;

	}
	_ftime(&finish);
	m_start_time = (finish.time * 1000 + finish.millitm) - (start.time * 1000 + start.millitm);
	m_Max_time = 10000;
	m_curtime = m_Max_time - m_start_time / 1000;
	//printf("->소요 시간 : %.2f seconds.\n", m_curtime);
	//위에 시간 정보를 모든 유저에게 보낸다.
	SendAllTime();

	//	m_lock.unlock();
	COLOR_CS->LeaveCS();

}



















































