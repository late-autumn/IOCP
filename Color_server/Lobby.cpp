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

	// ��������
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

	// ��������
	m_LobbyState = true;
	m_gameState = LOBBY_WAIT;

	return true;
}

void CLobby::End()
{
	m_LobbyState = true;
	m_gameState = LOBBY_WAIT;

}

// ���� ������ �ٸ� �������� ������.

// �ٸ� ������ ������ ������ ������.


// �ش� ��Ŷ�� �濡 �ִ� ��� �������� ������.
void CLobby::SendAllUser(char *_packet)
{
	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL) continue;
		m_paUser[i]->SendPacket(_packet);

	}
}
// �ð� ������ ��� �������� �����ش�.
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
	// �濡 �ο��� �� á���� ����
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
	//�κ� �ο��� ������
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

	// �κ� ���ο� �ִ� �ٸ� Ŭ���̾�Ʈ�鿡�� �ش� ���� �������� �˸�
	// ���� ����
	sc_player_disconnect remove_packet;
	remove_packet.size = sizeof(remove_packet);
	remove_packet.type = SC_REMOVE_PLAYER;
	remove_packet.num = user->GetNumber();

	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL) continue;
		if (m_paUser[i] == user) continue;

		m_paUser[i]->SendPacket((char*)(&remove_packet));

		printf("%d�� %d�� Ŭ�� �������� %d�� %d�� Ŭ�󿡰� ����\n",
			m_num, user->GetNumber(), m_paUser[i]->m_FieldNumber, m_paUser[i]->GetNumber());
	}
	m_lock.unlock();
	//COLOR_CS->LeaveCS();
	// �濡 �ο��� ������ �ʱ�ȭ
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
	// ���� ������ ������Ʈ
	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL) continue;
		if (m_paUser[i]->m_buff.m_timer == 0) continue;

	}
	_ftime(&finish);
	m_start_time = (finish.time * 1000 + finish.millitm) - (start.time * 1000 + start.millitm);
	m_Max_time = 10000;
	m_curtime = m_Max_time - m_start_time / 1000;
	//printf("->�ҿ� �ð� : %.2f seconds.\n", m_curtime);
	//���� �ð� ������ ��� �������� ������.
	SendAllTime();

	//	m_lock.unlock();
	COLOR_CS->LeaveCS();

}



















































