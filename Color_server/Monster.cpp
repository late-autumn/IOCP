#pragma once
#include "Monster.h"
#include "Iocp.h"

CMonster::CMonster()
{
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
	m_atk = 4;
	m_speed = 1.0f;
	m_atkDelay = 2000;
	m_atkDelayTime = 0;
	m_skillDelay = 10000;
	m_skillDelayTime = 0;
	m_range = 20;

	m_target = NULL;
	m_pState = NULL;

}


CMonster::~CMonster()
{
}

void CMonster::Begin()
{
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
	m_atk = 4;
	m_speed = 1.0f;
	m_atkDelay = 2000;
	m_atkDelayTime = 0;
	m_skillDelay = 10000;
	m_skillDelayTime = 0;
	m_range = 20;

	m_target = NULL;
	my_field = NULL;
}

void CMonster::End()
{
}

// 몬스터 업데이트
void CMonster::Update(DWORD _curTime, CField *field)
{
	if (m_pState)
	{
		m_pState->Update(this, field, _curTime);
	}
}

// 적 탐색
bool CMonster::EnemySearch(CUser* _target)
{

	if (_target == NULL)
	{
		return false;
	}

	if (m_state == MON_NORMAL)
	{
		if (false == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))
		{
			//m_state = MON_NORMAL;
			ChangeState(STATE.m_pMonsterMove);
			return false;
		}
		// 1차 인식범위 설정( 유저가 근처에 다가가면 경계모드로 바꾼다. )
		// 경계모드
		m_target = _target;
		ChangeState(STATE.m_pMonsterVigilance);
		//cout << m_num << "몬스터 경계모드" << endl;
		return true;
	}// 경계모드	
	else if (m_state == MON_VIGILANCE)
	{
		if (true == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range / 2))
		{
			// 2차 인식범위 설정( 유저가 더 가까이 오면 전투모드로 바꾼다. )
			// 가까이온 대상을 적으로 인식한다.
			// 전투모드
			m_target = _target;
			ChangeState(STATE.m_pMonsterCombat);
			//cout << m_num << "몬스터 전투모드 타겟 : " << _target->GetNumber() << "번 유저" << endl;
			return true;
		}
	}// 전투모드
	else if (m_state == MON_COMBAT)
	{
		if (m_target != _target)
		{
			m_target = NULL;
			ChangeState(STATE.m_pMonsterMove);
			cout << m_num << "몬스터 Error('전투모드인데 타겟이 틀림')" << endl;
			return false;
		}
		//cout << m_num << "몬스터 전투중 타겟 : " << _target->GetNumber() << "번 유저" << endl;
		// 범위 안에 타게팅된 유저가 있을 경우 
		if (true == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))
		{
			return true;
		}
	}
	// 경계모드 및 전투모드 일때 유저가 벗어나면
	if (false == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))
	{
		m_target = NULL;
		ChangeState(STATE.m_pMonsterMove);
		cout << m_num << "몬스터 유저와의 거리가 멀어져 평화모드로 돌아감" << endl;
		return false;
	}
	return true;
}






CFieldMonster::CFieldMonster()
{
}

CFieldMonster::~CFieldMonster()
{
}

void CFieldMonster::Begin()
{
	switch (m_type)
	{
	case MON_BASE:
	{
					 //m_num = -1;
					 m_state = MON_NORMAL;
					 m_curHp = 200;
					 m_maxHp = 200;
					 m_posX = 0;
					 m_posY = 0;
					 m_posZ = 0;
					 m_rotX = 0;
					 m_rotY = 0;
					 m_rotZ = 0;
					 m_dirX = 0;
					 m_dirY = 0;
					 m_dirZ = 0;
					 m_atk = 10;
					 m_speed = 1.0f;
					 m_atkDelay = 1000;
					 m_atkDelayTime = 0;
					 m_skillDelay = 10000;
					 m_skillDelayTime = 0;
					 m_range = 15;
					 m_waypoint = 0;

					 m_target = NULL;

					 m_pState = NULL;
					 ChangeState(STATE.m_pMonsterRoaming);
	}
		break;
	case MON_BUFF1:
	{
					 //m_num = -1;
					 m_state = MON_NORMAL;
					 m_curHp = 1000;
					 m_maxHp = 1000;
					 m_posX = 0;
					 m_posY = 0;
					 m_posZ = 0;
					 m_rotX = 0;
					 m_rotY = 0;
					 m_rotZ = 0;
					 m_dirX = 0;
					 m_dirY = 0;
					 m_dirZ = 0;
					 m_atk = 100;
					 m_speed = 1.0f;
					 m_atkDelay = 1000;
					 m_atkDelayTime = 0;
					 m_skillDelay = 10000;
					 m_skillDelayTime = 0;
					 m_range = 30;
					 m_waypoint = 0;

					 m_target = NULL;

					 m_pState = NULL;
					 ChangeState(STATE.m_pMonsterRoaming);
	}
		break;
	case MON_BUFF2:
	{
					 //m_num = -1;
					 m_state = MON_NORMAL;
					 m_curHp = 1000;
					 m_maxHp = 1000;
					 m_posX = 0;
					 m_posY = 0;
					 m_posZ = 0;
					 m_rotX = 0;
					 m_rotY = 0;
					 m_rotZ = 0;
					 m_dirX = 0;
					 m_dirY = 0;
					 m_dirZ = 0;
					 m_atk = 100;
					 m_speed = 1.0f;
					 m_atkDelay = 1000;
					 m_atkDelayTime = 0;
					 m_skillDelay = 10000;
					 m_skillDelayTime = 0;
					 m_range = 30;
					 m_waypoint = 0;

					 m_target = NULL;

					 m_pState = NULL;
					 ChangeState(STATE.m_pMonsterRoaming);
	}
		break;
	case MON_LBF1:
	{
					 //m_num = -1;
					 m_state = MON_NORMAL;
					 m_curHp = 500;
					 m_maxHp = 500;
					 m_posX = 0;
					 m_posY = 0;
					 m_posZ = 0;
					 m_rotX = 0;
					 m_rotY = 0;
					 m_rotZ = 0;
					 m_dirX = 0;
					 m_dirY = 0;
					 m_dirZ = 0;
					 m_atk = 50;
					 m_speed = 1.0f;
					 m_atkDelay = 1000;
					 m_atkDelayTime = 0;
					 m_skillDelay = 10000;
					 m_skillDelayTime = 0;
					 m_range = 15;
					 m_waypoint = 0;

					 m_target = NULL;

					 m_pState = NULL;
					 ChangeState(STATE.m_pMonsterRoaming);
	}
		break;
	case MON_LBF2:
	{
					 //m_num = -1;
					 m_state = MON_NORMAL;
					 m_curHp = 500;
					 m_maxHp = 500;
					 m_posX = 0;
					 m_posY = 0;
					 m_posZ = 0;
					 m_rotX = 0;
					 m_rotY = 0;
					 m_rotZ = 0;
					 m_dirX = 0;
					 m_dirY = 0;
					 m_dirZ = 0;
					 m_atk = 50;
					 m_speed = 1.0f;
					 m_atkDelay = 1000;
					 m_atkDelayTime = 0;
					 m_skillDelay = 10000;
					 m_skillDelayTime = 0;
					 m_range = 15;
					 m_waypoint = 0;

					 m_target = NULL;

					 m_pState = NULL;
					 ChangeState(STATE.m_pMonsterRoaming);
	}
		break;
	case MON_BOSS:
	{
					 //m_num = -1;
					 m_state = MON_NORMAL;
					 m_curHp = 10000;
					 m_maxHp = 10000;
					 m_posX = 0;
					 m_posY = 0;
					 m_posZ = 0;
					 m_rotX = 0;
					 m_rotY = 0;
					 m_rotZ = 0;
					 m_dirX = 0;
					 m_dirY = 0;
					 m_dirZ = 0;
					 m_atk = 300;
					 m_speed = 1.0f;
					 m_atkDelay = 1000;
					 m_atkDelayTime = 0;
					 m_skillDelay = 10000;
					 m_skillDelayTime = 0;
					 m_range = 30;
					 m_waypoint = 0;

					 m_target = NULL;

					 m_pState = NULL;
					 ChangeState(STATE.m_pMonsterRoaming);
	}
		break;
	}

}

void CFieldMonster::End()
{

}

void CFieldMonster::SetPos(float _x, float _y, float _z)
{
	m_posX = _x; m_posY = _y; m_posZ = _z;
	m_startPosX = _x; m_startPosY = _y; m_startPosZ = _z;

	m_waypointPosX[0] = m_startPosX - 15.0f;
	m_waypointPosY[0] = m_startPosY;
	m_waypointPosZ[0] = m_startPosZ;

	m_waypointPosX[1] = m_startPosX;
	m_waypointPosY[1] = m_startPosY;
	m_waypointPosZ[1] = m_startPosZ + 15.0f;

	m_waypointPosX[2] = m_startPosX + 15.0f;
	m_waypointPosY[2] = m_startPosY;
	m_waypointPosZ[2] = m_startPosZ;

	m_waypointPosX[3] = m_startPosX;
	m_waypointPosY[3] = m_startPosY;
	m_waypointPosZ[3] = m_startPosZ - 15.0f;

	m_curWaypointPosX = m_waypointPosX[0];
	m_curWaypointPosY = m_waypointPosY[0];
	m_curWaypointPosZ = m_waypointPosZ[0];
}

// 몬스터 업데이트
void CFieldMonster::Update(DWORD _curTime, CField* field)
{
	if (m_pState)
	{
		m_pState->Update(this, field, _curTime);
	}
}
// 적 탐색
bool CFieldMonster::EnemySearch(CUser* _target)
{

	if (_target == NULL)
	{
		return false;
	}
	if (m_state == MON_NORMAL)
	{
		if (false == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))
		{
			//m_state = MON_NORMAL;
			ChangeState(STATE.m_pMonsterRoaming);
			return false;
		}
		// 1차 인식범위 설정( 유저가 근처에 다가가면 경계모드로 바꾼다. )
		// 경계모드
		m_target = _target;
		ChangeState(STATE.m_pMonsterVigilance);
		//cout << m_num << "몬스터 경계모드" << endl;
		return true;
	}// 경계모드	
	else if (m_state == MON_VIGILANCE)
	{
		if (true == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range / 2))
		{
			// 2차 인식범위 설정( 유저가 더 가까이 오면 전투모드로 바꾼다. )
			// 가까이온 대상을 적으로 인식한다.
			// 전투모드
			m_target = _target;
			ChangeState(STATE.m_pMonsterCombat);
			//cout << m_num << "몬스터 전투모드 타겟 : " << _target->GetNumber() << "번 유저" << endl;
			return true;
		}
	}// 전투모드
	else if (m_state == MON_COMBAT)
	{
		if (m_target != _target)
		{
			m_target = NULL;
			ChangeState(STATE.m_pMonsterRoaming);
			//cout << m_num << "몬스터 Error('전투모드인데 타겟이 틀림')" << endl;
			return false;
		}
		// 범위 안에 타게팅된 유저가 있을 경우 
		if (true == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))
		{
			return true;
		}
	}
	// 경계모드 및 전투모드 일때 유저가 벗어나면
	if (false == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))
	{
		m_target = NULL;
		_target = NULL;
		ChangeState(STATE.m_pMonsterRoaming);
		//cout << m_num << "몬스터 유저와의 거리가 멀어져 평화모드로 돌아감" << endl;
		return false;
	}
	return true;
}