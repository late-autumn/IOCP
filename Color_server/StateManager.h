#pragma once
#include "State.h"

class CStateManager
{
public:
	CStateMonsterMove *m_pMonsterMove;
	CStateMonsterVigilance *m_pMonsterVigilance;
	CStateMonsterCombat *m_pMonsterCombat;
	CStateMonsterDead *m_pMonsterDead;
	CStateMonsteRoaming *m_pMonsterRoaming;
	CStateNPCMove *m_pNpcMove;
	CStateNPCRange *m_pNpcRange;
	CStateNPCRoaming *m_pNPCRoaming;
	

public:
	CStateManager();
	~CStateManager();

	bool Begin();
	bool End();
};

