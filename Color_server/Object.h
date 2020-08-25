#pragma once

class CState;

class CObject
{
public:
	int m_num; // 번호

	int m_type;
	int m_state;
	// 위치
	float m_posX;
	float m_posY;
	float m_posZ;
	// 회전
	float m_rotX;
	float m_rotY;
	float m_rotZ;
	// 방향
	float m_dirX;
	float m_dirY;
	float m_dirZ;

	int m_maxHp;
	int m_curHp;
	float m_atk; // 공격력
	float m_speed; // 이동속도

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
	int m_num; // 번호

	int m_type;
	int m_state;
	// 위치
	float m_posX;
	float m_posY;
	float m_posZ;
	// 회전
	float m_rotX;
	float m_rotY;
	float m_rotZ;
	// 방향
	float m_dirX;
	float m_dirY;
	float m_dirZ;

	int m_maxHp;
	int m_curHp;
	float m_atk; // 공격력
	float m_speed; // 이동속도
	float m_atksp; //공격속도 
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