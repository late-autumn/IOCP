#pragma once
#include "Monster.h"

class CMonsterManager : public CMonster
{
public:
	CMonsterManager();
	~CMonsterManager();

	void Begin();
	void End();

	virtual bool EnemySearch(CUser* _target); // Àû Å½»ö
};

