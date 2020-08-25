#pragma once
#include "Define.h"

class CSkill
{

public:
	CSkill();
	~CSkill();

private:
	bool	m_SkillReady;					// ��ų �غ� ���� �˻� : ��� ���� = true, ��� �Ұ��� = false

	BYTE	m_SkillType;				// ��ų�� ���� : ����, ����
	bool	m_SkillDurability;		// ��ų�� ���Ӽ� : ���� = true, �ܹ� = false
	bool	m_SkillRange;			// ���� ��ų ���� : ���� = true, ���� = false
	BYTE	m_SkillTargetType;			// ��ų�� �޴� ����� ���� : �� ���, �Ʊ� �÷��̾�
	float	m_SkillDamage;				// ��ų�� ���� : 1ȸ �� ���ط�/ȸ���� ��
	float	m_SkillDistance;			// ��ų�� ���� ���� : ���� ��ų = ������, �� �� = �Ÿ�

	BYTE	m_MaxRepeatingNumber;		// �ִ� �ݺ� Ƚ��
	BYTE	m_RemainingRepeatingNumber;	// �����ִ� �ݺ� Ƚ��

	DWORD	m_MaxSkillCoolDown;			// ��ų�� �ִ� ��Ÿ�� : 1�� = 1000ms
	DWORD	m_RemainingSkillCoolDown;	// ��ų�� �����ִ� ��Ÿ�� : 1�� = 1000ms

public:
	bool	Begin(int num);
	void	End();

	BYTE	GetRemainingRepeatingNumber()	{ return m_RemainingRepeatingNumber; };
	float	GetDistance()					{ return m_SkillDistance; }
	float	GetPower()						{ return m_SkillDamage; }
	bool	IsReady()						{ return m_SkillReady; }
	bool	IsRangeSkill()					{ return m_SkillRange; }

};

