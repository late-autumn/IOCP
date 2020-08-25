#pragma once
#include "NPCManager.h"
#include "IOCP.h"

CNPCManager::CNPCManager()
{
	//m_num = -1;
	m_type = -1;
	m_state = NPC_NORMAL;
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
	m_range = 20;

	m_target = NULL;
	m_pState = NULL;
}

CNPCManager::~CNPCManager()
{
}

void CNPCManager::Begin()
{
	//m_num = -1;
	m_type = -1;
	m_state = NPC_NORMAL;
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
	m_range = 20;

	m_target = NULL;

	m_pState = NULL;
	ChangeState(STATE.m_pNpcMove);
}

void CNPCManager::End()
{

}


// Àû Å½»ö
bool CNPCManager::EnemySearch(CUser* _target)
{
	return CNpc::EnemySearch(_target);
}