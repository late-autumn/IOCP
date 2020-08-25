#pragma once
#include "StateManager.h"


CStateManager::CStateManager()
{
	m_pMonsterMove = NULL;
	m_pMonsterVigilance = NULL;
	m_pMonsterCombat = NULL;
	m_pMonsterDead = NULL;
	m_pMonsterRoaming = NULL;
	m_pNpcMove = NULL;
	m_pNpcRange = NULL;
	m_pNPCRoaming = NULL;
}


CStateManager::~CStateManager()
{
}

bool CStateManager::Begin()
{
	if (!m_pMonsterMove) m_pMonsterMove = new CStateMonsterMove;
	if (!m_pMonsterVigilance) m_pMonsterVigilance = new CStateMonsterVigilance;
	if (!m_pMonsterCombat) m_pMonsterCombat = new CStateMonsterCombat;
	if (!m_pMonsterDead) m_pMonsterDead = new CStateMonsterDead;
	if (!m_pMonsterRoaming) m_pMonsterRoaming = new CStateMonsteRoaming;
	if (!m_pNpcMove) m_pNpcMove = new CStateNPCMove;
	if (!m_pNpcRange) m_pNpcRange = new CStateNPCRange;
	if (!m_pNPCRoaming) m_pNPCRoaming = new CStateNPCRoaming;

	return true;
}

bool CStateManager::End()
{
	if (m_pMonsterMove) delete m_pMonsterMove;
	if (m_pMonsterVigilance) delete m_pMonsterVigilance;
	if (m_pMonsterCombat) delete m_pMonsterCombat;
	if (m_pMonsterDead) delete m_pMonsterDead;
	if (m_pMonsterRoaming) delete m_pMonsterRoaming;
	if (!m_pNpcMove)   delete m_pNpcMove;
	if (!m_pNpcRange)   delete m_pNpcRange;
	if (!m_pNPCRoaming)  delete m_pNPCRoaming;
	return true;
}
