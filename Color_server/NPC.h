#pragma once
#include "Define.h"
#include "Object.h"
#include "MapObject.h"
#include "User.h"


class CNpc : public CObject
{
public:

	// �νĹ���
	int m_range;
	CField*				my_field;
	CMapObject*         m_Map1;
	CUser*              m_target;
	int dir;

public:
	CNpc();
	virtual ~CNpc();

	virtual void Begin();
	void End();

	virtual void SetField(CField *field){ my_field = field; };

	void SetNum(int n){ m_num = n; };
	void SetType(int type){ m_type = type; };
	virtual void SetPos(float x, float y, float z)
	{
		m_posX = x; m_posY = y; m_posZ = z;
	};

	virtual void Update(DWORD _curTime, CField *field);

	virtual bool EnemySearch(CUser* _target); // �� Ž��
};



class CFieldNpc : public CNpc
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
	CFieldNpc();
	~CFieldNpc();

	virtual void Begin();
	void End();
	virtual void SetField(CField *field){ my_field = field; };
	virtual void SetPos(float _x, float _y, float _z);
	virtual void Update(DWORD _curTime, CField *field);
	virtual bool EnemySearch(CUser* _target); // �� Ž��
};

