#pragma once
#include "Iocp.h"
#include "State.h"
#include "Object.h"
#include "Monster.h"
#include "NPC.h"

CState::CState()
{
	my_field = NULL;
}

CState::~CState()
{
}

void CState::Begin(CObject *_pObj, DWORD _time)
{
}

void CState::Update(CObject *_pObj, CField *field, DWORD _time)
{
}

void CState::End(CObject *_pObj, DWORD _time)
{
}
CField* CState::SendAllUser(char *_packet, CField *field)
{
	for (auto user : field->m_vUser)
	{
		if (user)
		{
			user->SendPacket(_packet);
		}
	}
	return NULL;
}

// CStateMonsterMove
void CStateMonsterMove::Begin(CObject *_pObj, DWORD _time)
{
	CMonster *pMon = (CMonster *)_pObj;
	pMon->m_state = MON_NORMAL;
	switch (pMon->dir)
	{
	case 0: // UP
		pMon->m_rotX = 0;
		pMon->m_rotY = 0;
		pMon->m_rotZ = 1;
		break;
	case 1: // DOWN
		pMon->m_rotX = 0;
		pMon->m_rotY = 0;
		pMon->m_rotZ = -1;
		break;
	case 2: // LEFT
		pMon->m_rotX = -1;
		pMon->m_rotY = 0;
		pMon->m_rotZ = 0;
		break;
	case 3: // RIGHT
		pMon->m_rotX = 1;
		pMon->m_rotY = 0;
		pMon->m_rotZ = 0;
		break;
	}
}

void CStateMonsterMove::Update(CObject *_pObj, CField *field, DWORD _time)
{
	CMonster *pMon = (CMonster *)_pObj;
	CMapObject obj;

	switch (pMon->dir)
	{
	case 0: // UP
		pMon->m_posZ += pMon->m_speed;
		break;
	case 1: // DOWN
		pMon->m_posZ -= pMon->m_speed;
		break;
	case 2: // LEFT
		pMon->m_posX -= pMon->m_speed;
		break;
	case 3: // RIGHT
		pMon->m_posX += pMon->m_speed;
		break;
	}
	// 몬스터가 노말상태일때 이동하면서 충돌이 생기면 방향전환
	obj.SetBox(0, 0, 1, CPoint3D(pMon->m_posX, 0, pMon->m_posZ), CPoint3D(3, 0, 3));
	if (pMon->m_Map1->CollisionCheck(obj))
	{
		switch (pMon->dir)
		{
		case 0: // UP
			pMon->dir = 1;
			pMon->m_rotX = 0;
			pMon->m_rotY = 0;
			pMon->m_rotZ = 1;
			break;
		case 1: // DOWN
			pMon->dir = 0;
			pMon->m_rotX = 0;
			pMon->m_rotY = 0;
			pMon->m_rotZ = -1;
			break;
		case 2: // LEFT
			pMon->dir = 3;
			pMon->m_rotX = -1;
			pMon->m_rotY = 0;
			pMon->m_rotZ = 0;
			break;
		case 3: // RIGHT
			pMon->dir = 2;
			pMon->m_rotX = 1;
			pMon->m_rotY = 0;
			pMon->m_rotZ = 0;
			break;
		}
	}
}

void CStateMonsterMove::End(CObject *_pObj, DWORD _time)
{
}


// CStateMonsterVigilance
void CStateMonsterVigilance::Begin(CObject *_pObj, DWORD _time)
{
	CMonster *pMon = (CMonster *)_pObj;
	pMon->m_state = MON_VIGILANCE;
}

void CStateMonsterVigilance::Update(CObject *_pObj, CField *field, DWORD _time)
{
	CMonster *pMon = (CMonster *)_pObj;
	if (pMon->m_target == NULL) return;
	CPoint3D dir = CPoint3D(pMon->m_target->m_posX, 0, pMon->m_target->m_posZ);
	dir = dir - CPoint3D(pMon->m_posX, 0, pMon->m_posZ);
	float length = dir.Length();
	pMon->m_rotX = (dir.x / length);
	pMon->m_rotY = 0;
	pMon->m_rotZ = (dir.z / length);
}

void CStateMonsterVigilance::End(CObject *_pObj, DWORD _time)
{
}


// CStateMonsterCombat
void CStateMonsterCombat::Begin(CObject *_pObj, DWORD _time)
{
	CMonster *pMon = (CMonster *)_pObj;
	pMon->m_state = MON_COMBAT;
}

void CStateMonsterCombat::Update(CObject *_pObj, CField *field, DWORD _time)
{
	CMonster *pMon = (CMonster *)_pObj;
	CField *my_room = new CField;
	CUser *pUser = new CUser;
	if (pMon->m_state == MON_COMBAT)	// 전투모드 
	{ 
		if (pMon->m_target == NULL)
		{
			pMon->ChangeState(STATE.m_pMonsterMove);
			cout << pMon->GetNumber() << "몬스터 Error('전투모드인데 타겟이 없음')" << endl;
			return;
		}
		else if (pMon->m_target->m_characterState == PSTATE_DEAD)
		{
			pMon->ChangeState(STATE.m_pMonsterMove);
			return;
		}
		CPoint3D dir = CPoint3D(pMon->m_target->m_posX, 0, pMon->m_target->m_posZ);
		dir = dir - CPoint3D(pMon->m_posX, 0, pMon->m_posZ);
		float length = dir.Length();
		pMon->m_rotX = (dir.x / length);
		pMon->m_rotY = 0;
		pMon->m_rotZ = (dir.z / length);

		CPoint3D mon_pos(pMon->m_posX, 0, pMon->m_posZ);
		CPoint3D target_pos(pMon->m_target->m_posX, 0, pMon->m_target->m_posZ);

		length = Point3Dlength(&(target_pos - mon_pos));
		if (length < 6)
		{
			pMon->m_state = MON_ATK;
			// 현재 공격딜레이가 완료상태면
			if (pMon->m_atkDelayTime < _time)
			{
				pMon->m_target->m_curHp -= pMon->m_atk;
				sc_monster_atk mon_atk_packet;
				mon_atk_packet.size = sizeof(mon_atk_packet);
				mon_atk_packet.type = SC_MONSTER_ATK;
				mon_atk_packet.target = pMon->m_target->GetNumber();
				mon_atk_packet.hp = pMon->m_target->m_curHp;
				mon_atk_packet.maxhp = pMon->m_target->m_maxHp;
				SendAllUser((char *)(&mon_atk_packet), field);
				MonsterSkill(pMon);
				// 플레이어가 죽었을 경우
				if (pMon->m_target->m_curHp <= 0)
				{
					field->SendDead(pMon->m_target);
				}
				// 딜레이를 준다.
				pMon->m_atkDelayTime = _time + pMon->m_atkDelay;
				//cout << "몬스터 :  공격!" << endl;
				if (pMon->m_target != NULL)
				{
					cout << pMon->GetNumber() << "몬스터 공격!  타겟 : " << pMon->m_target->GetNumber() << "번 유저 HP : " << pMon->m_target->m_curHp << endl;
				}
			}

		}
		else{
			pMon->m_posX += pMon->m_rotX * pMon->m_speed;
			pMon->m_posZ += pMon->m_rotZ * pMon->m_speed;
		}
	}
	else if (pMon->m_state == MON_ATK)
	{
		pMon->m_state = MON_COMBAT;
	}
}

void CStateMonsterCombat::End(CObject *_pObj, DWORD _time)
{
}

void CStateMonsterCombat::MonsterSkill(CObject *_pObj)
{
	CMonster *pMon = (CMonster *)_pObj;
	CField *my_room = new CField;
	CUser *pUser = new CUser;

	if (pMon->m_curHp > 0 && pMon->m_curHp <= 50)
	{
		pMon->m_skillDelayTime = GetTickCount();
		if ((rand() % 3) == 0)
		{
			pMon->m_target->m_characterState = PSTATE_STUN;
			pMon->m_target->m_stateTimer = GetTickCount();
			cout << pMon->m_num << " 몬스터 : " << pMon->m_target->GetNumber() << " 유저에게 스턴 공격!" << endl;
			sc_hit monskill_packet;
			monskill_packet.size = sizeof(monskill_packet);
			monskill_packet.type = SC_HIT;
			monskill_packet.action_type = ACTION_SKILL;
			monskill_packet.skill_type = MONSTERSKILLTYPE_STUN;
			monskill_packet.attacker_type = HIT_TYPE_MONSTER;
			monskill_packet.target_type = HIT_TYPE_PLAYER;
			monskill_packet.attacker_num = pMon->GetNumber();
			monskill_packet.target_num = pMon->m_target->GetNumber();
			monskill_packet.curhp = pMon->m_target->m_curHp;
			monskill_packet.maxHp = pMon->m_target->m_maxHp;
			monskill_packet.posX = pMon->m_target->m_posX;
			monskill_packet.posZ = pMon->m_target->m_posZ;
			SendAllUser((char *)(&monskill_packet), my_room);
		}
	}
	else if (pMon->m_curHp > 50 && pMon->m_curHp <= 150)
	{
		// 체력이 어느정도 있는 상황이면 공격당할 시 스킬을 사용한다.
		// 5초 딜레이
		pMon->m_skillDelay = GetTickCount();
		if (pMon->m_skillDelay - pMon->m_skillDelayTime > 5000)
		{
			pMon->m_skillDelayTime = GetTickCount();
			pMon->m_target->m_characterState = PSTATE_BLIND;
			pMon->m_target->m_stateTimer = GetTickCount();
			cout << pMon->m_num << " 몬스터 : " << pMon->m_target->GetNumber() << " 유저에게 블라인드 공격!" << endl;
			sc_hit monskill_packet;
			monskill_packet.size = sizeof(monskill_packet);
			monskill_packet.type = SC_HIT;
			monskill_packet.action_type = ACTION_SKILL;
			monskill_packet.skill_type = MONSTERSKILLTYPE_BLIND;
			monskill_packet.attacker_type = HIT_TYPE_MONSTER;
			monskill_packet.target_type = HIT_TYPE_PLAYER;
			monskill_packet.attacker_num = pMon->GetNumber();
			monskill_packet.target_num = pMon->m_target->GetNumber();
			monskill_packet.curhp = pMon->m_target->m_curHp;
			monskill_packet.maxHp = pMon->m_target->m_maxHp;
			monskill_packet.posX = pMon->m_target->m_posX;
			monskill_packet.posZ = pMon->m_target->m_posZ;
			SendAllUser((char *)(&monskill_packet), my_room);
		}
	}
}


// CStateMonsterDead
void CStateMonsterDead::Begin(CObject *_pObj, DWORD _time)
{
	CMonster *pMon = (CMonster *)_pObj;
	pMon->m_state = MON_DEAD;
}

void CStateMonsterDead::Update(CObject *_pObj, CField *field, DWORD _time)
{
	CMonster *pMon = (CMonster *)_pObj;
	
	if (pMon->m_state == MON_DEAD)
	{
		DWORD Revive_time = GetTickCount();
		if (Revive_time - pMon->m_reviveTimer > 6000)
		{
			pMon->m_reviveTimer = 0;
			cout << "부활 시간이 되었습니다" << endl;
			pMon->m_state = MON_NORMAL;
		}
		pMon->m_curHp = pMon->m_maxHp;
		sc_revive Revive_packet;
		Revive_packet.size = sizeof(Revive_packet);
		Revive_packet.type = SC_REVIVE;
		Revive_packet.target_type = REVIVE_MONSTER;
		Revive_packet.num = pMon->GetNumber();
		Revive_packet.x = pMon->m_posX;
		Revive_packet.y = pMon->m_posY;
		Revive_packet.z = pMon->m_posZ;
		field->SendAllUser((char *)(&Revive_packet));
	}
}

void CStateMonsterDead::End(CObject *_pObj, DWORD _time)
{
}
// CStateMonsterRoaming
void CStateMonsteRoaming::Begin(CObject *_pObj, DWORD _time)
{
	CFieldMonster *pMon = (CFieldMonster *)_pObj;
	pMon->m_state = MON_NORMAL;

	CPoint3D dir = CPoint3D(pMon->m_curWaypointPosX, 0, pMon->m_curWaypointPosZ);
	dir = dir - CPoint3D(pMon->m_posX, 0, pMon->m_posZ);
	float length = dir.Length();
	pMon->m_rotX = (dir.x / length);
	pMon->m_rotY = 0;
	pMon->m_rotZ = (dir.z / length);
	
}

void CStateMonsteRoaming::Update(CObject *_pObj, CField *field, DWORD _time)
{
	CFieldMonster *pMon = (CFieldMonster *)_pObj;
	CMapObject obj;
	float posX = pMon->m_posX + (pMon->m_rotX * pMon->m_speed);
	float posZ = pMon->m_posZ + (pMon->m_rotZ * pMon->m_speed);


	
	if (In_range(posX, posZ, pMon->m_curWaypointPosX, pMon->m_curWaypointPosZ, 3))
	{
		++pMon->m_waypoint;
		if (pMon->m_waypoint > 3) pMon->m_waypoint = 0;

		pMon->m_curWaypointPosX = pMon->m_waypointPosX[pMon->m_waypoint];
		pMon->m_curWaypointPosZ = pMon->m_waypointPosZ[pMon->m_waypoint];

		CPoint3D dir = CPoint3D(pMon->m_curWaypointPosX, 0, pMon->m_curWaypointPosZ);
		dir = dir - CPoint3D(pMon->m_posX, 0, pMon->m_posZ);
		float length = dir.Length();
		pMon->m_rotX = (dir.x / length);
		pMon->m_rotY = 0;
		pMon->m_rotZ = (dir.z / length);
	}
	pMon->m_posX = posX;
	pMon->m_posZ = posZ;
}

void CStateMonsteRoaming::End(CObject *_pObj, DWORD _time)
{
}



// NPC 이동

void CStateNPCMove::Begin(CObject *_pObj, DWORD _time)
{
	CNpc *pNpc = (CNpc *)_pObj;
	pNpc->m_state = NPC_NORMAL;
	switch (pNpc->dir)
	{
	case 0: // UP
		pNpc->m_rotX = 0;
		pNpc->m_rotY = 0;
		pNpc->m_rotZ = 1;
		break;
	case 1: // DOWN
		pNpc->m_rotX = 0;
		pNpc->m_rotY = 0;
		pNpc->m_rotZ = -1;
		break;
	case 2: // LEFT
		pNpc->m_rotX = -1;
		pNpc->m_rotY = 0;
		pNpc->m_rotZ = 0;
		break;
	case 3: // RIGHT
		pNpc->m_rotX = 1;
		pNpc->m_rotY = 0;
		pNpc->m_rotZ = 0;
		break;
	}
}

void CStateNPCMove::Update(CObject *_pObj, CField *field, DWORD _time)
{
	CNpc *pNpc = (CNpc *)_pObj;
	CMapObject obj;

	switch (pNpc->dir)
	{
	case 0: // UP
		pNpc->m_posZ += pNpc->m_speed;
		break;
	case 1: // DOWN
		pNpc->m_posZ -= pNpc->m_speed;
		break;
	case 2: // LEFT
		pNpc->m_posX -= pNpc->m_speed;
		break;
	case 3: // RIGHT
		pNpc->m_posX += pNpc->m_speed;
		break;
	}
	// NPC가 노말상태일때 이동하면서 충돌이 생기면 방향전환
	obj.SetBox(0, 0, 1, CPoint3D(pNpc->m_posX, 0, pNpc->m_posZ), CPoint3D(3, 0, 3));

}

void CStateNPCMove::End(CObject *_pObj, DWORD _time)
{
}

// NPC 경계

// CStateMonsterVigilance
void CStateNPCRange::Begin(CObject *_pObj, DWORD _time)
{
	CNpc *pNpc = (CNpc *)_pObj;
	pNpc->m_state = NPC_COMBAT;
}

void CStateNPCRange::Update(CObject *_pObj, CField *field, DWORD _time)
{
	cout << " 경계 업데이트 " << endl;
	CNpc *pNpc = (CNpc *)_pObj;
	if (pNpc->m_target == NULL) return;
	CPoint3D dir = CPoint3D(pNpc->m_target->m_posX, 0, pNpc->m_target->m_posZ);
	dir = dir - CPoint3D(pNpc->m_posX, 0, pNpc->m_posZ);
	float length = dir.Length();
	pNpc->m_rotX = (dir.x / length);
	pNpc->m_rotY = 0;
	pNpc->m_rotZ = (dir.z / length);
}

void CStateNPCRange::End(CObject *_pObj, DWORD _time)
{
}


// NPC 로밍(지정위치 배회)
void CStateNPCRoaming::Begin(CObject *_pObj, DWORD _time)
{
	CFieldNpc *pNpc = (CFieldNpc *)_pObj;
	pNpc->m_state = NPC_NORMAL;

	CPoint3D dir = CPoint3D(pNpc->m_curWaypointPosX, 0, pNpc->m_curWaypointPosZ);
	dir = dir - CPoint3D(pNpc->m_posX, 0, pNpc->m_posZ);
	float length = dir.Length();
	pNpc->m_rotX = (dir.x / length);
	pNpc->m_rotY = 0;
	pNpc->m_rotZ = (dir.z / length);

}

void CStateNPCRoaming::Update(CObject *_pObj, CField *field, DWORD _time)
{
	CFieldNpc *pNpc = (CFieldNpc *)_pObj;
	CMapObject obj;
	float posX = pNpc->m_posX + (pNpc->m_rotX * pNpc->m_speed);
	float posZ = pNpc->m_posZ + (pNpc->m_rotZ * pNpc->m_speed);

	
	if (In_range(posX, posZ, pNpc->m_curWaypointPosX, pNpc->m_curWaypointPosZ, 3))
	{
		++pNpc->m_waypoint;
		if (pNpc->m_waypoint > 3) pNpc->m_waypoint = 0;

		pNpc->m_curWaypointPosX = pNpc->m_waypointPosX[pNpc->m_waypoint];
		pNpc->m_curWaypointPosZ = pNpc->m_waypointPosZ[pNpc->m_waypoint];

		CPoint3D dir = CPoint3D(pNpc->m_curWaypointPosX, 0, pNpc->m_curWaypointPosZ);
		dir = dir - CPoint3D(pNpc->m_posX, 0, pNpc->m_posZ);
		float length = dir.Length();
		pNpc->m_rotX = (dir.x / length);
		pNpc->m_rotY = 0;
		pNpc->m_rotZ = (dir.z / length);
	}
	pNpc->m_posX = posX;
	pNpc->m_posZ = posZ;

}

void CStateNPCRoaming::End(CObject *_pObj, DWORD _time)
{
}

