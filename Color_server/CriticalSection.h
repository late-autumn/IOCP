#pragma once
#include "Singleton.h"

class cCriticalSection
	:public CSingleTon<cCriticalSection>
{
public:
	cCriticalSection(void)
	{
	}
	~cCriticalSection(void)
	{
	}
private:
	CRITICAL_SECTION m_cs;

public:
	void InitCS(void)
	{
		InitializeCriticalSection(&m_cs);
	}
	void DeleteCS(void)
	{
		DeleteCriticalSection(&m_cs);
	}
	void EnterCS(void)
	{
		EnterCriticalSection(&m_cs);
	}
	void LeaveCS(void)
	{
		LeaveCriticalSection(&m_cs);
	}


};
