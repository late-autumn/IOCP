#pragma once
#include "NPC.h"

class CNPCManager : public CNpc
{
public:
	CNPCManager();
	~CNPCManager();

	void Begin();
	void End();

	virtual bool EnemySearch(CUser* _target); // Àû Å½»ö
};

