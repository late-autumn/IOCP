#pragma once
#include "User.h"
#include "Define.h"
#include "State.h"

#include "CriticalSection.h"
class CLobby
{
	// �� ���� �� ��������
	DWORD		m_num;				// ���ȣ
	bool		m_LobbyState;		// �κ� ���� (�κ� true / ���ӽ��� false
	int			m_MaxUserCount;		// �ִ� ����ο� �� 
	int			m_currentUserCount;	// ���� �κ� �����ο� ��

	// ��������
	int			m_gameState;		// �������� ����

	// �ð� ���� 
	double		 m_start_time;		// ���� ���� �ð�
	double		 m_Max_time;		// ���� ���� �ð�
	double		 m_curtime;			// ���� ���� �ð�


public:
	CUser*				m_paUser[500];		// ���������� �ּ�
	mutex				m_lock;
	cCriticalSection	m_Lock;
	CRITICAL_SECTION	m_cs;
	vector<CUser*>		m_User;
	vector<CState*>		m_mstate;
	struct _timeb start, finish; //�ð� ���� �Լ� (ũ�γ� �ᵵ ����� �����Ű��⵵�� ) �ϴ� ����⸸ ��
public:
	CLobby();
	~CLobby();
	bool Begin(DWORD _num);
	void End();

	// SendType
	void SendType(BYTE type);
	// �κ� �ִ� ��� �������� ��Ŷ ����
	void SendAllUser(char *_packet);

	// ���� ��ü �ð��� ��� �������� ����
	void SendAllTime();

	// ���� ���� �� ����
	bool JoinUser(CUser* user);
	bool LeaveUser(CUser *user);

	bool FieldStart();

	// ������ Set Get
	int GetNumber(){ return m_num; };
	void SetNumber(int n){ m_num = n; };
	bool GetLobbyState(){ return m_LobbyState; }
	int GetMaxUserCount(){ return m_MaxUserCount; };
	int GetCurrentUserCount(){ return m_currentUserCount; };		// ���� �� �����ο� �� ���
	bool GetIsFull()
	{
		if (m_currentUserCount == m_MaxUserCount) return true;
		else return false;
	};
	bool GetIsEmpty()
	{
		if (m_currentUserCount > 0) return false;
		else return true;
	};

	// �������� Set Get
	int GetGameState(){ return m_gameState; };

	// ��������
	void Update();
	void EnterCS(){ EnterCriticalSection(&m_cs); }
	void LeaveCS(){ LeaveCriticalSection(&m_cs); }
};
