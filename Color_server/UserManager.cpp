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
	// ��
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

	// �� ����
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
		// �߰��� ����ִ� ��ġ�� ���� ��� �ش� ��ġ�� �߰�
		if (false == (*iter)->m_Login)
		{
			(*iter)->m_socket = client_sock;
			m_currentUserCount = min(m_currentUserCount + 1, MAX_USER);
			(*iter)->m_Login = true;
			printf("�����Ŵ����� �ͼ� ������ ���� �޼��� ��� :���� ����! ��ġ(%d)\n", (*iter)->GetNumber());
			printf("���� �������� Ŭ���̾�Ʈ �� : %d\n", m_currentUserCount);
			COLOR_CS->LeaveCS();
			return (*iter); // �߰� ����
		}
	}
	cout << "�����ο� �ʰ�!";
	cout << "  �����ο�: " << m_currentUserCount << "��  �ִ��ο� : " << m_maxUserCount << "��" << endl;
	COLOR_CS->LeaveCS();
	return NULL;
}

bool CUserManager::DeleteUser(CUser *user)
{
	if (NULL == user) return false;

	COLOR_CS->EnterCS();
	printf("���ӹ�ȣ %d�� ���� ����\n", user->GetNumber());
	user->Begin(user->GetNumber());
	m_currentUserCount = max((int)(m_currentUserCount - 1), 0);
	COLOR_CS->LeaveCS();

	return true; // ���� ����
}

CUser* CUserManager::GetUser(unsigned int num)
{
	auto iter = m_vUser.begin();
	if (num < 0 && num >= m_vUser.size()) return NULL;
	iter += num;
	return (*iter);
}
