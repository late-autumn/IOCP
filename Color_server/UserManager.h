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
	};	// 현재 접속유저 수 얻기
	DWORD  GetMaxUserCount(){ return m_maxUserCount; };			// 최대 접속유저 수 얻기

	CUser* GetUser(unsigned int num);				// 유저 얻기

};

