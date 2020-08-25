#pragma once
#include "MonsterManager.h"
#include "IOCP.h"

CMonsterManager::CMonsterManager()
{
	m_num = -1;
	m_type = -1;
	m_state = MON_NORMAL;
	m_curHp = 100;
	m_maxHp = 100;
	m_posX = 0;
	m_posY = 0;
	m_posZ = 0;
	m_rotX = 0;
	m_rotY = 0;
	m_rotZ = 0;
	m_dirX = 0;
	m_dirY = 0;
	m_dirZ = 0;
	m_atk = 2;
	m_speed = 1.0f;
	m_atkDelay = 1500;
	m_atkDelayTime = 0;
	m_skillDelay = 10000;
	m_skillDelayTime = 0;
	m_range = 20;

	m_target = NULL;
	m_pState = NULL;
}

CMonsterManager::~CMonsterManager()
{
}

void CMonsterManager::Begin()
{
	m_num = -1;
	m_type = -1;
	m_state = MON_NORMAL;
	m_curHp = 100;
	m_maxHp = 100;
	m_posX = 0;
	m_posY = 0;
	m_posZ = 0;
	m_rotX = 0;
	m_rotY = 0;
	m_rotZ = 0;
	m_dirX = 0;
	m_dirY = 0;
	m_dirZ = 0;
	m_atk = 2;
	m_speed = 1.0f;
	m_atkDelay = 1500;
	m_atkDelayTime = 0;
	m_skillDelay = 10000;
	m_skillDelayTime = 0;
	m_range = 20;

	m_target = NULL;

	m_pState = NULL;
	ChangeState(STATE.m_pMonsterMove);
}

void CMonsterManager::End()
{

}


// Àû Å½»ö
bool CMonsterManager::EnemySearch(CUser* _target)
{
	return CMonster::EnemySearch(_target);
}