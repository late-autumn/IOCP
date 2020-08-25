#pragma once
#include "UserManager.h"


CUserManager::CUserManager()
{
	m_maxUserCount = MAX_USER;
	m_currentUserCount = 0;
}


CUserManager::~CUserManager()
{
}

void CUserManager::Begin(DWORD _maxUserCount)
{
	m_maxUserCount = _maxUserCount;
	m_currentUserCount = 0;

	for (auto i = 0; i < m_maxUserCount; ++i)
	{
		CUser *user = new CUser;
		user->Begin(i);
		m_vUser.push_back(user);
	}
}

void CUserManager::End(){
	// 락
	COLOR_CS->EnterCS();
	for (DWORD i = 0; i < m_vUser.size() ; ++i)
	{
		CUser *User = m_vUser[i];
		m_vUser[i]->view_list.clear();
		User->End();
		delete User;
	}
	m_vUser.clear();
	m_currentUserCount = 0;

	// 락 해제
	COLOR_CS->LeaveCS();
}


CUser* CUserManager::AddUser(SOCKET client_sock)
{
	if (NULL == client_sock) return NULL;
	if (INVALID_SOCKET == client_sock) return NULL;
	COLOR_CS->EnterCS();
	if (m_currentUserCount >= m_maxUserCount)
	{
		COLOR_CS->LeaveCS();
		return NULL;
	}

	for (auto iter = m_vUser.begin(); iter != m_vUser.end(); ++iter)
	{
		// 중간에 비어있는 위치가 있을 경우 해당 위치에 추가
		if (false == (*iter)->m_Login)
		{
			(*iter)->m_socket = client_sock;
			m_currentUserCount = min(m_currentUserCount + 1, MAX_USER);
			(*iter)->m_Login = true;
			printf("유저매니저로 와서 다음과 같은 메세지 출력 :접속 성공! 위치(%d)\n", (*iter)->GetNumber());
			printf("현재 접속중인 클라이언트 수 : %d\n", m_currentUserCount);
			COLOR_CS->LeaveCS();
			return (*iter); // 추가 성공
		}
	}
	cout << "접속인원 초과!";
	cout << "  현재인원: " << m_currentUserCount << "명  최대인원 : " << m_maxUserCount << "명" << endl;
	COLOR_CS->LeaveCS();
	return NULL;
}

bool CUserManager::DeleteUser(CUser *user)
{
	if (NULL == user) return false;

	COLOR_CS->EnterCS();
	printf("접속번호 %d번 유저 종료\n", user->GetNumber());
	user->Begin(user->GetNumber());
	m_currentUserCount = max((int)(m_currentUserCount - 1), 0);
	COLOR_CS->LeaveCS();

	return true; // 삭제 성공
}

CUser* CUserManager::GetUser(unsigned int num)
{
	auto iter = m_vUser.begin();
	if (num < 0 && num >= m_vUser.size()) return NULL;
	iter += num;
	return (*iter);
}
