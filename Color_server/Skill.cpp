#include "Define.h"
#include "Skill.h"

CSkill::CSkill()
{
}

CSkill::~CSkill()
{
	End();
}

bool CSkill::Begin(int num)
{
	m_SkillReady = true;

	switch (num)
	{
	case USERSKILLTYPE_SMASH:
		m_SkillType = SKILL_TYPE_ATTACK;
		m_SkillDurability = false;
		m_SkillRange = true;
		m_MaxRepeatingNumber = 1;
		m_RemainingRepeatingNumber = m_MaxRepeatingNumber;
		m_SkillTargetType = SKILL_TARGET_FOE;
		m_SkillDamage = 2.0f;
		m_SkillDistance = 10.0f;
		m_MaxSkillCoolDown = 5000;
		m_RemainingSkillCoolDown = 0;
		return true;
		break;

	case USERSKILLTYPE_BASH:
		m_SkillType = SKILL_TYPE_ATTACK;
		m_SkillDurability = false;
		m_SkillRange = true;
		m_MaxRepeatingNumber = 1;
		m_RemainingRepeatingNumber = m_MaxRepeatingNumber;
		m_SkillTargetType = SKILL_TARGET_FOE;
		m_SkillDamage = 5.0f;
		m_SkillDistance = 20.0f;
		m_MaxSkillCoolDown = 10000;
		m_RemainingSkillCoolDown = 0;
		return true;
		break;

	case USERSKILLTYPE_B1:
		m_SkillType = SKILL_TYPE_ATTACK;
		m_SkillDurability = false;
		m_SkillRange = true;
		m_MaxRepeatingNumber = 1;
		m_RemainingRepeatingNumber = m_MaxRepeatingNumber;
		m_SkillTargetType = SKILL_TARGET_FOE;
		m_SkillDamage = 2.0f;
		m_SkillDistance = 10.0f;
		m_MaxSkillCoolDown = 5000;
		m_RemainingSkillCoolDown = 0;
		return true;
		break;

	case USERSKILLTYPE_B2:
		m_SkillType = SKILL_TYPE_ATTACK;
		m_SkillDurability = false;
		m_SkillRange = true;
		m_MaxRepeatingNumber = 1;
		m_RemainingRepeatingNumber = m_MaxRepeatingNumber;
		m_SkillDamage = 5.0f;
		m_SkillDistance = 20.0f;
		m_MaxSkillCoolDown = 10000;
		m_RemainingSkillCoolDown = 0;
		return true;
		break;

	case USERSKILLTYPE_HEALING:
		m_SkillType = SKILL_TYPE_HELPING;
		m_SkillDurability = false;
		m_SkillRange = true;
		m_MaxRepeatingNumber = 1;
		m_RemainingRepeatingNumber = m_MaxRepeatingNumber;
		m_SkillTargetType = SKILL_TARGET_FRIEND;
		m_SkillDamage = 3.0f;
		m_SkillDistance = 5.0f;
		m_MaxSkillCoolDown = 5000;
		m_RemainingSkillCoolDown = 0;
		return true;
		break;

	case USERSKILLTYPE_BLOOD:
		m_SkillType = SKILL_TYPE_ATTACK;
		m_SkillDurability = false;
		m_SkillRange = true;
		m_MaxRepeatingNumber = 1;
		m_RemainingRepeatingNumber = m_MaxRepeatingNumber;
		m_SkillTargetType = SKILL_TARGET_FOE;
		m_SkillDamage = 2.0f;
		m_SkillDistance = 5.0f;
		m_MaxSkillCoolDown = 5000;
		m_RemainingSkillCoolDown = 0;
		return true;
		break;

	case USERSKILLTYPE_C1:
		m_SkillType = SKILL_TYPE_ATTACK;
		m_SkillDurability = false;
		m_SkillRange = true;
		m_MaxRepeatingNumber = 1;
		m_RemainingRepeatingNumber = m_MaxRepeatingNumber;
		m_SkillTargetType = SKILL_TARGET_FOE;
		m_SkillDamage = 2.0f;
		m_SkillDistance = 10.0f;
		m_MaxSkillCoolDown = 5000;
		m_RemainingSkillCoolDown = 0;
		return true;
		break;

	default:
		return false;
		break;
	}

	return false;
}

void CSkill::End()
{
}
