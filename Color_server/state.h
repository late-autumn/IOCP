#pragma once
#include "Define.h"
#include "User.h"
#include "Field.h"

class CObject;
class CMonster;
class CUser;
class CField;
class CNpc;

class CState
{

public:
	CState();
	~CState();
	CField *my_field;
	virtual void Begin(CObject *_pObj, DWORD _time = NULL);
	virtual void Update(CObject *_pObj, CField *field, DWORD _time = NULL);
	virtual void End(CObject *_pObj, DWORD _time = NULL);
	CField* SendAllUser(char *_packet, CField *field);
	virtual void SetField(CField *field){ my_field = field; };
};

// 몬스터 이동
class CStateMonsterMove : public CState
{
public:
	CStateMonsterMove(){};
	~CStateMonsterMove(){};

	virtual void Begin(CObject *_pObj, DWORD _time = NULL);
	virtual void Update(CObject *_pObj, CField *field, DWORD _time = NULL);
	virtual void End(CObject *_pObj, DWORD _time = NULL);
	virtual void SetField(CField *field){ my_field = field; };
};

// 몬스터 경계
class CStateMonsterVigilance : public CState
{
public:
	CStateMonsterVigilance(){};
	~CStateMonsterVigilance(){};

	virtual void Begin(CObject *_pObj, DWORD _time = NULL);
	virtual void Update(CObject *_pObj, CField *field, DWORD _time = NULL);
	virtual void End(CObject *_pObj, DWORD _time = NULL);
	virtual void SetField(CField *field){ my_field = field; };
};

// 몬스터 전투
class CStateMonsterCombat : public CState
{
public:
	CStateMonsterCombat(){};
	~CStateMonsterCombat(){};

	virtual void Begin(CObject *_pObj, DWORD _time = NULL);
	virtual void Update(CObject *_pObj, CField *field, DWORD _time = NULL);
	virtual void End(CObject *_pObj, DWORD _time = NULL);
	virtual void SetField(CField *field){ my_field = field; };
	void MonsterSkill(CObject *_pObj);
};
// 몬스터 사망
class CStateMonsterDead : public CState
{
public:
	CStateMonsterDead(){};
	~CStateMonsterDead(){};

	virtual void Begin(CObject *_pObj, DWORD _time = NULL);
	virtual void Update(CObject *_pObj, CField *field, DWORD _time = NULL);
	virtual void End(CObject *_pObj, DWORD _time = NULL);
	virtual void SetField(CField *field){ my_field = field; };
};

// 몬스터 로밍(지정위치 배회)
class CStateMonsteRoaming : public CState
{
public:
	CStateMonsteRoaming(){  };
	~CStateMonsteRoaming(){};

	virtual void Begin(CObject *_pObj, DWORD _time = NULL);
	virtual void Update(CObject *_pObj, CField *field, DWORD _time = NULL);
	virtual void End(CObject *_pObj, DWORD _time = NULL);
	virtual void SetField(CField *field){ my_field = field; };

};

// NPC 이동
class CStateNPCMove : public CState
{
public:
	CStateNPCMove(){};
	~CStateNPCMove(){};

	virtual void Begin(CObject *_pObj, DWORD _time = NULL);
	virtual void Update(CObject *_pObj, CField *field, DWORD _time = NULL);
	virtual void End(CObject *_pObj, DWORD _time = NULL);
	virtual void SetField(CField *field){ my_field = field; };
};

// NPC 경계
class CStateNPCRange : public CState
{
public:
	CStateNPCRange(){};
	~CStateNPCRange(){};

	virtual void Begin(CObject *_pObj, DWORD _time = NULL);
	virtual void Update(CObject *_pObj, CField *field, DWORD _time = NULL);
	virtual void End(CObject *_pObj, DWORD _time = NULL);
	virtual void SetField(CField *field){ my_field = field; };
};

// NPC 로밍(지정위치 배회)
class CStateNPCRoaming : public CState
{
public:
	CStateNPCRoaming(){};
	~CStateNPCRoaming(){};

	virtual void Begin(CObject *_pObj, DWORD _time = NULL);
	virtual void Update(CObject *_pObj, CField *field, DWORD _time = NULL);
	virtual void End(CObject *_pObj, DWORD _time = NULL);
	virtual void SetField(CField *field){ my_field = field; };

};
