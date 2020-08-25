#pragma once
#include "Iocp.h"

void CIocp::GameItemUse(CUser *user)
{
	if (user == NULL) return;
	if (user->GetSocket() == INVALID_SOCKET) return;
	if (user->GetField() == NULL) return;

	// 아이템이 사용중일 경우
	if (user->m_buff.m_timer != 0) return;

	user->m_buff.Begin(0);

	srand((unsigned int)time(NULL));
	if (rand() % 3 != 0)
	{
		user->m_buff.m_atk = 1.5;
		cout << user->GetNumber() << "번 유저 공격력증가 아이템 획득! 공격력 : " << 46 * user->m_buff.m_atk << endl;
	}
	else
	{
		user->m_curHp = min(user->m_curHp + 50, user->m_maxHp);
		cout << user->GetNumber() << "번 유저 회복 아이템 획득! 현재 체력 : " << user->m_curHp << endl;
	}

	user->m_buff.SetTime(GetTickCount());
}