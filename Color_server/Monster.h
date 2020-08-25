#pragma once
#include "Define.h"
#include "Object.h"
#include "MapObject.h"
#include "User.h"


class CMonster : public CObject
{
public:
	DWORD               m_stateTimer;
	DWORD				m_reviveTimer;
	// �⺻���� ������ ���� �� ���� ����
	DWORD				m_atkDelay;
	DWORD				m_atkDelayTime;
	// ��ų���� ������ ���� �� ���� ����
	DWORD               m_skillDelay;
	DWORD               m_skillDelayTime;

	// �νĹ���
	int m_range;
	CField*				my_field;
	CMapObject*         m_Map1;
	CUser*              m_target;
	int dir;

public:
	CMonster();
	virtual ~CMonster();

	virtual void Begin();
	void End();

	virtual void SetField(CField *field) { my_field = field; };

	void SetNum(int n) { m_num = n; };
	void SetType(int type) { m_type = type; };
	virtual void SetPos(float x, float y, float z)
	{
		m_posX = x; m_posY = y; m_posZ = z;
	};

	virtual void Update(DWORD _curTime, CField *field);

	virtual bool EnemySearch(CUser* _target); // �� Ž��
};



class CFieldMonster : public CMonster
{
public:
	// ���� ���� ��ġ
	float m_startPosX;
	float m_startPosY;
	float m_startPosZ;

	// ���� ���� �ι� ��ġ
	float m_curWaypointPosX;
	float m_curWaypointPosY;
	float m_curWaypointPosZ;

	// ���� ���� �ι� ��ġ
	float m_waypointPosX[4];
	float m_waypointPosY[4];
	float m_waypointPosZ[4];

	int m_waypoint;

	DWORD dwTick;
public:
	CFieldMonster();
	~CFieldMonster();

	virtual void Begin();
	void End();
	virtual void SetField(CField *field) { my_field = field; };
	virtual void SetPos(float _x, float _y, float _z);
	virtual void Update(DWORD _curTime, CField *field);
	virtual bool EnemySearch(CUser* _target); // �� Ž��
};

