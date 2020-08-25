#define WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once
#include "MapObject.h"
#include "MonsterManager.h"
#include "NPCManager.h"
#include "UserManager.h"
#include "Buff.h"
#include "Define.h"
#include "State.h"
#include "CriticalSection.h"

class CField
{
	// 방 정보 및 유저정보
	DWORD		m_num;				// 방번호
	bool		m_FieldState;		// 방 상태 (로비대기false/게임시작true)

	int			m_MaxUserCount;		// 최대 허용인원 수 
	int			m_currentUserCount;	// 현재 방 입장인원 수

	// 게임정보
	int			m_gameState;			// 게임진행 여부
	int			m_mapNum;				// 맵번호
	int			m_ColorMap[32 * 32];	// 색상 정보 맵
	
	// 시간 정보 
	double		 m_start_time;		// 게임 시작 시간
	double		 m_Max_time;		// 게임 종료 시간
	double		 m_curtime;			// 게임 진행 시간
	// 몬스터 추가
	CMonsterManager	m_CMonsterManager;
	int			m_maxMonsterCount;
	int			m_currentMonsterCount;
	int			m_deadMonsterCount;
	// NPC 추가
	CNPCManager m_CNpcManager;
	int			m_maxNpcCount;
	int			m_currentNpcCount;
	int			m_deadNpcCount;

public:
	CUser*				m_paUser[500];		// 유저데이터 주소
	vector<CUser*>		m_vUser;
	vector<CMonster*>	m_vMonster;
	vector<CNpc*>		m_vNpc;
	CMapObject*         m_Map1;
	vector<CBuff *>		m_vBuff;

	vector<CUser*>		m_User;


public:
	CField();
	~CField();

	bool				Begin(DWORD _num);
	void				End();

	// SendType
	void				SendAllUser(char *_packet);// 방에 있는 모든 유저에게 패킷 전송
	void				SendAllMonster();		  // 모든 몬스터의 정보 모든 유저에게 전송
	void				SendAllMonster(CUser *_user);// 모든 몬스터의 정보를 특정유저에게 전송
	void				SendAllNpc();	// 모든 NPC의 정보를 모든 유저에게 전송
	void				SendAllNpc(CUser *_user);	// 모든 NPC의 정보를 특정유저에게 전송
	void				SendAllMap();	// 모든 맵의 정보를 모든 유저에게 전송
	void				SendAllMap(CUser *_user);	// 모든 맵의 정보를 특정유저에게 전송
	void				SendAllMove();	// 모든 유저들의 움직임을 전송
	void				SendDead(CUser *_user);
	void				SendEXPandLVUP(CUser *_user); // 경험치 및 레벨업 
	// 유저 입장 및 퇴장
	bool				JoinUser(CUser* user);
	bool				LeaveUser(CUser *user);

	bool				RoomStart();
	bool				GameStart();

	// 방정보 Set Get
	int					GetNumber(){ return m_num; };
	void				SetNumber(int n){ m_num = n; };
	bool				GetFieldState(){ return m_FieldState; }
	void				SetMonster(vector<CMonster*>& v);
	void				SetNpc(vector<CNpc*>& n);
	int					GetMaxUserCount(){ return m_MaxUserCount; };
	int					GetCurrentUserCount(){ return m_currentUserCount; };		// 현재 방 입장인원 수 얻기
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
	int					GetGameState(){ return m_gameState; };
	int					GetColorNumInColorMap(int num){ return m_ColorMap[num]; }	//색상 갖고 오기 
	void				SetColorNumInColorMap(int num, int color){ m_ColorMap[num] = color; }//색상 정하기

	// 게임진행
	bool				GameLoading();					// 게임준비단계
	void				Update();
//	void				view();
	/////// 유저
	CBuff*				Buff(CUser *user);
	bool				DeleteItem(CUser *_user);
	bool				UseBuff(CUser *_user);
	void				ToAttack(CUser *user);
	void				Respon(CUser *user);
	////// 몬스터 
	int					GetMaxMonster(){ return m_maxMonsterCount; };
	void				CountUpDeadMonster(){ ++m_deadMonsterCount; }
	void				CountDownCurMonster(){ --m_currentMonsterCount; }
	void				UseSkillToMonster(CUser* user, BYTE slotnum, float dirX, float dirY, float dirZ, int StartNum);
	void				UseSkillToUser(CUser* user, BYTE slotnum, float dirX, float dirY, float dirZ, int StartNum);

};
 