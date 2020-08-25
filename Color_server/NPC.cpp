#pragma once
#include "NPC.h"
#include "Iocp.h"

CNpc::CNpc()
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
	m_atk = 4;
	m_speed = 1.0f;
	m_range = 20;

	m_target = NULL;
	m_pState = NULL;

}

CNpc::~CNpc()
{
}

void CNpc::Begin()
{

	//m_num = -1;
	//m_type = -1;
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
	m_atk = 4;
	m_speed = 1.0f;
	m_range = 20;
	m_target = NULL;
	my_field = NULL;
}

void CNpc::End()
{
}

// ���Ǿ� ������Ʈ
void CNpc::Update(DWORD _curTime, CField *field)
{
	if (m_pState) m_pState->Update(this, field, _curTime);
}

// ���� Ž��
bool CNpc::EnemySearch(CUser* _target)
{

	if (_target == NULL) return false;

	if (m_state == NPC_NORMAL)
	{
		if (false == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))
		{
			//m_state = MON_NORMAL;
			ChangeState(STATE.m_pNPCRoaming);
			return false;
		}
		// 1�� �νĹ��� ����( ������ ��ó�� �ٰ����� ������ �ٲ۴�. )
		// �����
		m_target = _target;
		ChangeState(STATE.m_pNpcRange);
		//cout << m_num << "���� �����" << endl;
		return true;
	}// �����	
	else if (m_state == NPC_COMBAT)
	{
		if (true == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range / 2))
		{
			// 2�� �νĹ��� ����( ������ �� ������ ���� �������� �ٲ۴�. )
			// �����̿� ����� ������ �ν��Ѵ�.
			// �������
			m_target = _target;
			ChangeState(STATE.m_pNpcRange);
			//cout << " �������� ���� " << endl;
			//cout << m_num << "���� ������� Ÿ�� : " << _target->GetNumber() << "�� ����" << endl;
			return true;
		}
		else if (m_target != _target){
			m_target = NULL;
			ChangeState(STATE.m_pNPCRoaming);
			//cout << m_num << "���� Error('��������ε� Ÿ���� Ʋ��')" << endl;
			return false;
		}
		// ���� �ȿ� Ÿ���õ� ������ ���� ��� 
		else if (true == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))	return true;
	}// �������

	// ����� �� ������� �϶� ������ �����
	if (false == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))
	{
		m_target = NULL;
		_target = NULL;
		ChangeState(STATE.m_pNPCRoaming);
		//cout << m_num << "���� �������� �Ÿ��� �־��� ��ȭ���� ���ư�" << endl;
		//cout << m_target << ","<< _target <<  "�� Ÿ������ ���������ʴ´� " << endl;
		return false;
	}
	return true;
}






CFieldNpc::CFieldNpc()
{
}

CFieldNpc::~CFieldNpc()
{
}

void CFieldNpc::Begin()
{
	switch (m_type)
	{
	case NPC_RED:
	{
					 //m_num = -1;
					 m_state = NPC_NORMAL;
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
					 m_range = 15;
					 m_waypoint = 0;

					 m_target = NULL;

					 m_pState = NULL;
					 ChangeState(STATE.m_pNPCRoaming);
	}
		break;
	case NPC_BLUE:
	{
					 //m_num = -1;
					 m_state = NPC_NORMAL;
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
					 m_range = 30;
					 m_waypoint = 0;

					 m_target = NULL;

					 m_pState = NULL;
					 ChangeState(STATE.m_pNPCRoaming);
	}
		break;
	case NPC_GREEN:
	{
					 //m_num = -1;
					 m_state = NPC_NORMAL;
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
					 m_range = 30;
					 m_waypoint = 0;

					 m_target = NULL;

					 m_pState = NULL;
					 ChangeState(STATE.m_pNPCRoaming);
	}
		break;
	case NPC_YELLOW:
	{
					 //m_num = -1;
					 m_state = NPC_NORMAL;
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
					 m_range = 15;
					 m_waypoint = 0;

					 m_target = NULL;

					 m_pState = NULL;
					 ChangeState(STATE.m_pNPCRoaming);
	}
		break;
	
	}

}

void CFieldNpc::End()
{

}

void CFieldNpc::SetPos(float _x, float _y, float _z)
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

// ���Ǿ� ������Ʈ
void CFieldNpc::Update(DWORD _curTime, CField* field)
{
	if (m_pState) m_pState->Update(this, field,_curTime);
}

// ���� Ž��
bool CFieldNpc::EnemySearch(CUser* _target)
{

	if (_target == NULL) return false;

	if (m_state == NPC_NORMAL)
	{
		if (false == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))
		{
			//m_state = MON_NORMAL;
			ChangeState(STATE.m_pNPCRoaming);
			return false;
		}
		// 1�� �νĹ��� ����( ������ ��ó�� �ٰ����� ������ �ٲ۴�. )
		// �����
		m_target = _target;
		ChangeState(STATE.m_pNpcRange);
		cout << m_num << "NPC �����" << endl;
		return true;
	}// �����	
	else if (m_state == NPC_COMBAT)
	{
		if (true == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range / 2))
		{
			// 2�� �νĹ��� ����( ������ �� ������ ���� �������� �ٲ۴�. )
			// �����̿� ����� ������ �ν��Ѵ�.
			// �������
			m_target = _target;
			ChangeState(STATE.m_pNpcRange);
			cout << " NPC ���� ���� " << endl;
			
			return true;
		}
		else if (m_target != _target){
			m_target = NULL;
			ChangeState(STATE.m_pNPCRoaming);
			//cout << m_num << "���� Error('��������ε� Ÿ���� Ʋ��')" << endl;
			return false;
		}
		// ���� �ȿ� Ÿ���õ� ������ ���� ��� 
		else if (true == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))	return true;
	}// �������

	// ����� �� ������� �϶� ������ �����
	if (false == In_range(m_posX, m_posZ, _target->m_posX, _target->m_posZ, m_range))
	{
		m_target = NULL;
		_target = NULL;
		ChangeState(STATE.m_pNPCRoaming);
		cout << m_num << "NPC �������� �Ÿ��� �־��� ��ȭ���� ���ư�" << endl;
		return false;
	}
	return true;
}
