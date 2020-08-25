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
	// �� ���� �� ��������
	DWORD		m_num;				// ���ȣ
	bool		m_FieldState;		// �� ���� (�κ���false/���ӽ���true)

	int			m_MaxUserCount;		// �ִ� ����ο� �� 
	int			m_currentUserCount;	// ���� �� �����ο� ��

	// ��������
	int			m_gameState;			// �������� ����
	int			m_mapNum;				// �ʹ�ȣ
	int			m_ColorMap[32 * 32];	// ���� ���� ��
	
	// �ð� ���� 
	double		 m_start_time;		// ���� ���� �ð�
	double		 m_Max_time;		// ���� ���� �ð�
	double		 m_curtime;			// ���� ���� �ð�
	// ���� �߰�
	CMonsterManager	m_CMonsterManager;
	int			m_maxMonsterCount;
	int			m_currentMonsterCount;
	int			m_deadMonsterCount;
	// NPC �߰�
	CNPCManager m_CNpcManager;
	int			m_maxNpcCount;
	int			m_currentNpcCount;
	int			m_deadNpcCount;

public:
	CUser*				m_paUser[500];		// ���������� �ּ�
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
	void				SendAllUser(char *_packet);// �濡 �ִ� ��� �������� ��Ŷ ����
	void				SendAllMonster();		  // ��� ������ ���� ��� �������� ����
	void				SendAllMonster(CUser *_user);// ��� ������ ������ Ư���������� ����
	void				SendAllNpc();	// ��� NPC�� ������ ��� �������� ����
	void				SendAllNpc(CUser *_user);	// ��� NPC�� ������ Ư���������� ����
	void				SendAllMap();	// ��� ���� ������ ��� �������� ����
	void				SendAllMap(CUser *_user);	// ��� ���� ������ Ư���������� ����
	void				SendAllMove();	// ��� �������� �������� ����
	void				SendDead(CUser *_user);
	void				SendEXPandLVUP(CUser *_user); // ����ġ �� ������ 
	// ���� ���� �� ����
	bool				JoinUser(CUser* user);
	bool				LeaveUser(CUser *user);

	bool				RoomStart();
	bool				GameStart();

	// ������ Set Get
	int					GetNumber(){ return m_num; };
	void				SetNumber(int n){ m_num = n; };
	bool				GetFieldState(){ return m_FieldState; }
	void				SetMonster(vector<CMonster*>& v);
	void				SetNpc(vector<CNpc*>& n);
	int					GetMaxUserCount(){ return m_MaxUserCount; };
	int					GetCurrentUserCount(){ return m_currentUserCount; };		// ���� �� �����ο� �� ���
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
	int					GetGameState(){ return m_gameState; };
	int					GetColorNumInColorMap(int num){ return m_ColorMap[num]; }	//���� ���� ���� 
	void				SetColorNumInColorMap(int num, int color){ m_ColorMap[num] = color; }//���� ���ϱ�

	// ��������
	bool				GameLoading();					// �����غ�ܰ�
	void				Update();
//	void				view();
	/////// ����
	CBuff*				Buff(CUser *user);
	bool				DeleteItem(CUser *_user);
	bool				UseBuff(CUser *_user);
	void				ToAttack(CUser *user);
	void				Respon(CUser *user);
	////// ���� 
	int					GetMaxMonster(){ return m_maxMonsterCount; };
	void				CountUpDeadMonster(){ ++m_deadMonsterCount; }
	void				CountDownCurMonster(){ --m_currentMonsterCount; }
	void				UseSkillToMonster(CUser* user, BYTE slotnum, float dirX, float dirY, float dirZ, int StartNum);
	void				UseSkillToUser(CUser* user, BYTE slotnum, float dirX, float dirY, float dirZ, int StartNum);

};
 