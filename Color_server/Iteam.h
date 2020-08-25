#pragma once
#include "Define.h"

enum{ ITEM_ADD_ATK, ITEM_ADD_SPEED, ITEM_ADD_HEAL };
#define ITEM_TIME 5000

class CItem
{
public:
	BYTE m_num;
	BYTE m_state;
	BYTE m_type;
	float m_atk;
	float m_speed;
	float m_hp;
	float m_posX;
	float m_posY;
	float m_posZ;
	float m_size;

	DWORD m_timer;

public:
	CItem();
	~CItem();

	bool Begin(int _num);
	bool End();

	void SetTime(DWORD _time){ m_timer = _time + ITEM_TIME; };
};

