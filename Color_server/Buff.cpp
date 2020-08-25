#pragma once
#include "Buff.h"


CBuff::CBuff()
{
	m_num = 0;
	m_type = 0;
	m_state = 0;

	m_atk = 1;
	m_speed = 1;
	m_hp = 0;

	m_posX = 0;
	m_posY = 0;
	m_posZ = 0;
	m_size = 2;
	m_timer = 0;

}


CBuff::~CBuff()
{
}

bool CBuff::Begin(int _num)
{
	m_num = _num;
	m_type = 0;
	m_state = 0;

	m_atk = 1;
	m_speed = 1;
	m_hp = 0;

	m_posX = 0;
	m_posY = 0;
	m_posZ = 0;
	m_size = 2;
	m_timer = 0;

	return true;



}
