#pragma once
#include "User.h"
#include "Define.h"
#include "State.h"

#include "CriticalSection.h"
class CLobby
{
	// 방 정보 및 유저정보
	DWORD		m_num;				// 방번호
	bool		m_LobbyState;		// 로비 상태 (로비 true / 게임시작 false
	int			m_MaxUserCount;		// 최대 허용인원 수 
	int			m_currentUserCount;	// 현재 로비 입장인원 수

	// 게임정보
	int			m_gameState;		// 게임진행 여부

	// 시간 정보 
	double		 m_start_time;		// 게임 시작 시간
	double		 m_Max_time;		// 게임 종료 시간
	double		 m_curtime;			// 게임 진행 시간


public:
	CUser*				m_paUser[500];		// 유저데이터 주소
	mutex				m_lock;
	cCriticalSection	m_Lock;
	CRITICAL_SECTION	m_cs;
	vector<CUser*>		m_User;
	vector<CState*>		m_mstate;
	struct _timeb start, finish; //시간 측정 함수 (크로노 써도 상관은 없을거같기도함 ) 일단 써놓기만 함
public:
	CLobby();
	~CLobby();
	bool Begin(DWORD _num);
	void End();

	// SendType
	void SendType(BYTE type);
	// 로비에 있는 모든 유저에게 패킷 전송
	void SendAllUser(char *_packet);

	// 게임 전체 시간을 모든 유저에게 전송
	void SendAllTime();

	// 유저 입장 및 퇴장
	bool JoinUser(CUser* user);
	bool LeaveUser(CUser *user);

	bool FieldStart();

	// 방정보 Set Get
	int GetNumber(){ return m_num; };
	void SetNumber(int n){ m_num = n; };
	bool GetLobbyState(){ return m_LobbyState; }
	int GetMaxUserCount(){ return m_MaxUserCount; };
	int GetCurrentUserCount(){ return m_currentUserCount; };		// 현재 방 입장인원 수 얻기
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

	// 게임정보 Set Get
	int GetGameState(){ return m_gameState; };

	// 게임진행
	void Update();
	void EnterCS(){ EnterCriticalSection(&m_cs); }
	void LeaveCS(){ LeaveCriticalSection(&m_cs); }
};
