#pragma once
#include "User.h"
#include "Define.h"

CUser::CUser()
{
	m_num = LOGIN_FAIL;
}

CUser::~CUser()
{
	End();
}


void CUser::Begin(int _num)
{
	m_socket = INVALID_SOCKET;

	ZeroMemory(&m_stRecvOverlapped.m_wsaOverlapped, sizeof(m_stRecvOverlapped.m_wsaOverlapped));
	m_stRecvOverlapped.m_nRemainLen = m_stRecvOverlapped.recv_packet_size = 0;
	m_stRecvOverlapped.m_wsaBuf.buf = m_stRecvOverlapped.m_IocpBuf;
	m_stRecvOverlapped.m_wsaBuf.len = MAX_BUF_SIZE;
	m_stRecvOverlapped.m_eType = TYPE_RECV;

	m_Login = false;
	m_num = _num;
	m_pField = NULL;
	m_pSkill[0] = NULL;
	m_pSkill[1] = NULL;
	// 로비정보
	m_FromState = USER_LOGIN;	// 현재접속위치(로비, 방, 게임중)
	m_FieldNumber = -1;			// 방번호

	//게임정보
	m_selectCharacter = 1;		// 선택한 케릭터
	m_characterState = PSTATE_IDLE;		// 케릭터 상태(상태이상 유무)
	m_atk = 20;
	m_atksp = 1;
	m_maxHp = 100;
	m_curHp = 100;
	m_curExp = 0;
	m_maxExp = 1000;
	m_Lv = 1;

	m_posX = 0.0f;			// 캐릭터 위치
	m_posY = -10000.0f;
	m_posZ = 0.0f;

	m_rotX = 0.0f;			// 캐릭터 회전 각도
	m_rotY = 0.0f;
	m_rotZ = 0.0f;

	m_dirX = 0.0f;			// 캐릭터 주시 방향
	m_dirY = 0.0f;
	m_dirZ = 1.0f;
}

void CUser::End()
{
	m_socket = INVALID_SOCKET;

	ZeroMemory(&m_stRecvOverlapped.m_wsaOverlapped, sizeof(m_stRecvOverlapped.m_wsaOverlapped));
	m_stRecvOverlapped.m_nRemainLen = m_stRecvOverlapped.recv_packet_size = 0;
	m_stRecvOverlapped.m_wsaBuf.buf = m_stRecvOverlapped.m_IocpBuf;
	m_stRecvOverlapped.m_wsaBuf.len = MAX_BUF_SIZE;
	m_stRecvOverlapped.m_eType = TYPE_RECV;

	m_Login = false;
	m_pField = NULL;
	m_pSkill[0] = NULL;
	m_pSkill[1] = NULL;

	// 로비정보
	m_FromState = USER_LOGIN;	// 현재접속위치(로비, 방, 게임중)
	m_FieldNumber = -1;			// 방번호

	//게임정보
	m_selectCharacter = 1;		// 선택한 캐릭터
	m_characterState = PSTATE_IDLE;	// 캐릭터 상태(상태이상 유무)
	m_Lv = 1;
	m_atk = 0;
	m_atksp = 0;
	m_maxHp = 0;
	m_curHp = 0;

	m_curExp = 0;
	m_maxExp = 0;

	m_posX = 0.0f;			// 캐릭터 위치
	m_posY = -10000.0f;
	m_posZ = 0.0f;

	m_rotX = 0.0f;			// 캐릭터 회전 각도
	m_rotY = 0.0f;
	m_rotZ = 0.0f;

	m_dirX = 0.0f;			// 캐릭터 주시 방향
	m_dirY = 0.0f;
	m_dirZ = 1.0f;


}

void CUser::SendPacketToThisUser(char * buf)
{
}

void CUser::SendPacket(char *buf)
{
	if (m_socket == INVALID_SOCKET)
	{
		cout << "에러 : 소켓 빔 " << endl;
		return;
	}
	if (m_Login == false)
	{
		cout << "에러 : 접속종료된 유저" << endl;
		return;
	}

	stOverlapped *overlapped = new stOverlapped;
	DWORD io_size;

	ZeroMemory(&overlapped->m_wsaOverlapped, sizeof(OVERLAPPED));

	overlapped->m_eType = TYPE_SEND;
	overlapped->m_wsaBuf.buf = overlapped->m_IocpBuf;
	overlapped->m_wsaBuf.len = buf[0];
	memcpy(overlapped->m_IocpBuf, buf, buf[0]);

	int retval = WSASend(m_socket, &overlapped->m_wsaBuf, 1, &io_size, 0, &overlapped->m_wsaOverlapped, NULL);

	if (retval == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			m_Login = false;
			cout << "User WSASend Error : " << error << endl;
		}
	}
}

void CUser::GameInit()
{
	m_FromState = USER_LOBBY;

	// 유저 정보 초기화
	if (m_selectCharacter < 0) { m_selectCharacter = 0; }		// 선택한 캐릭터

	CSkill* skill1 = new CSkill();
	CSkill* skill2 = new CSkill();

	switch (m_selectCharacter)
	{
	case 0:
	{
			  m_characterState = PSTATE_IDLE;
			  m_atk = 50;
			  m_atksp = 1;
			  m_maxHp = 200;
			  m_curHp = 200;
			  m_curExp = 0;
			  m_maxExp = 1000;
			  m_Lv = 1;

			  if (skill1->Begin(USERSKILLTYPE_SMASH))
				  m_pSkill[SKILL_SLOT_1] = skill1;

			  if (skill2->Begin(USERSKILLTYPE_BASH))
				  m_pSkill[SKILL_SLOT_2] = skill2;
	}
		break;

	case 1:
	{
			  m_characterState = PSTATE_IDLE;
			  m_atk = 20;
			  m_atksp = 2;
			  m_maxHp = 200;
			  m_curHp = 200;
			  m_curExp = 0;
			  m_maxExp = 1000;
			  m_Lv = 1;

			  if (skill1->Begin(USERSKILLTYPE_B1))
				  m_pSkill[SKILL_SLOT_1] = skill1;

			  if (skill2->Begin(USERSKILLTYPE_B2))
				  m_pSkill[SKILL_SLOT_2] = skill2;
	}
		break;

	case 2:
	{
			  m_characterState = PSTATE_IDLE;
			  m_atk = 20;
			  m_atksp = 1;
			  m_maxHp = 200;
			  m_curHp = 200;
			  m_curExp = 0;
			  m_maxExp = 1000;
			  m_Lv = 1;

			  if (skill1->Begin(USERSKILLTYPE_HEALING))
				  m_pSkill[SKILL_SLOT_1] = skill1;

			  if (skill2->Begin(USERSKILLTYPE_BLOOD))
				  m_pSkill[SKILL_SLOT_2] = skill2;
	}
		break;

	default:
		cout << "정의할 수 없는 캐릭터" << endl;
	}

	// 색상 초기화
	if (m_color <= COLOR_GRAY) { m_color = COLOR_GRAY; }

	m_dirX = 0.0f;
	m_dirY = 0.0f;
	m_dirZ = 1.0f;

	m_rotY = 0;

	switch (m_color)
	{
	case COLOR_RED:
	{
					  m_posX = -320.0f;
					  m_posY = 0.0f;
					  m_posZ = -320.0f;
	}
		break;

	case COLOR_BLUE:
	{
					   m_posX = 300.0f;
					   m_posY = 0.0f;
					   m_posZ = -320.0f;
	}
		break;

	case COLOR_GREEN:
	{
						m_posX = -320.0f;
						m_posY = 0.0f;
						m_posZ = 320.0f;
	}
		break;

	case COLOR_YELLOW:
	{
						 m_posX = 300.0f;
						 m_posY = 0.0f;
						 m_posZ = 320.0f;
	}
		break;

	default:
	{
			   cout << "정의할 수 없는 색" << endl;
	}
	}
}
