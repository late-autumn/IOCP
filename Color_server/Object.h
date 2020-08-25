#pragma once

class CState;

class CObject
{
public:
	int m_num; // ��ȣ

	int m_type;
	int m_state;
	// ��ġ
	float m_posX;
	float m_posY;
	float m_posZ;
	// ȸ��
	float m_rotX;
	float m_rotY;
	float m_rotZ;
	// ����
	float m_dirX;
	float m_dirY;
	float m_dirZ;

	int m_maxHp;
	int m_curHp;
	float m_atk; // ���ݷ�
	float m_speed; // �̵��ӵ�

	CState *m_pState;
public:
	CObject(){};
	~CObject(){};

	int  GetNumber(){ return m_num; };
	void ChangeState(CState *_pState);
};


class PlayerObject
{
public:
	int m_num; // ��ȣ

	int m_type;
	int m_state;
	// ��ġ
	float m_posX;
	float m_posY;
	float m_posZ;
	// ȸ��
	float m_rotX;
	float m_rotY;
	float m_rotZ;
	// ����
	float m_dirX;
	float m_dirY;
	float m_dirZ;

	int m_maxHp;
	int m_curHp;
	float m_atk; // ���ݷ�
	float m_speed; // �̵��ӵ�
	float m_atksp; //���ݼӵ� 
	int m_maxExp;
	int m_curExp;
	int m_Lv;
	int m_skillDelay;
	int m_skillDelayTime;
	int m_stateTime;
public:
	PlayerObject(){};
	~PlayerObject(){};

	int  GetNumber(){ return m_num; };
};