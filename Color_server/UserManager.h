#pragma once
#include "Define.h"
#include "User.h"

class CUserManager
{
	DWORD				m_maxUserCount;
	DWORD				m_currentUserCount;

public:
	std::vector<CUser*>	m_vUser;
public:
	CUserManager();
	~CUserManager();

	void Begin(DWORD _maxUserCount);
	void End();

	CUser* AddUser(SOCKET client_sock);
	bool DeleteUser(CUser *user);

	DWORD  GetCurrentUserCount()
	{
		COLOR_CS->EnterCS();
		DWORD count = m_currentUserCount;
		COLOR_CS->LeaveCS();
		return count;
	};	// ���� �������� �� ���
	DWORD  GetMaxUserCount(){ return m_maxUserCount; };			// �ִ� �������� �� ���

	CUser* GetUser(unsigned int num);				// ���� ���

};

