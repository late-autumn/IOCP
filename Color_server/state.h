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

// ���� �̵�
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

// ���� ���
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

// ���� ����
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
// ���� ���
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

// ���� �ι�(������ġ ��ȸ)
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

// NPC �̵�
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

// NPC ���
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

// NPC �ι�(������ġ ��ȸ)
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
