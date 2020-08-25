#pragma once
#include "Define.h"

class CSkill
{

public:
	CSkill();
	~CSkill();

private:
	bool	m_SkillReady;					// 스킬 준비 상태 검사 : 사용 가능 = true, 사용 불가능 = false

	BYTE	m_SkillType;				// 스킬의 종류 : 공격, 도움
	bool	m_SkillDurability;		// 스킬의 지속성 : 지속 = true, 단발 = false
	bool	m_SkillRange;			// 범위 스킬 여부 : 범위 = true, 단일 = false
	BYTE	m_SkillTargetType;			// 스킬을 받는 대상의 종류 : 적 대상, 아군 플레이어
	float	m_SkillDamage;				// 스킬의 배율 : 1회 당 피해량/회복량 등
	float	m_SkillDistance;			// 스킬의 적용 범위 : 범위 스킬 = 반지름, 그 외 = 거리

	BYTE	m_MaxRepeatingNumber;		// 최대 반복 횟수
	BYTE	m_RemainingRepeatingNumber;	// 남아있는 반복 횟수

	DWORD	m_MaxSkillCoolDown;			// 스킬의 최대 쿨타임 : 1초 = 1000ms
	DWORD	m_RemainingSkillCoolDown;	// 스킬의 남아있는 쿨타임 : 1초 = 1000ms

public:
	bool	Begin(int num);
	void	End();

	BYTE	GetRemainingRepeatingNumber()	{ return m_RemainingRepeatingNumber; };
	float	GetDistance()					{ return m_SkillDistance; }
	float	GetPower()						{ return m_SkillDamage; }
	bool	IsReady()						{ return m_SkillReady; }
	bool	IsRangeSkill()					{ return m_SkillRange; }

};

