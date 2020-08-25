#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Field.h"
#include "Iocp.h"

CField::CField()
{
	m_MaxUserCount = 100;
	for (int i = 0; i < 100; i++)
	{
		m_paUser[i] = NULL;
	}
	
	m_currentUserCount = 0;

	// ��������
	m_FieldState = false;
	m_gameState = GAME_WAIT;
	m_mapNum = -1;

	// ����
	m_maxMonsterCount = 300;
	m_currentMonsterCount = 0;
	m_deadMonsterCount = 0;

	// NPC
	m_maxNpcCount = 4;
	m_currentNpcCount = 0;
	m_deadNpcCount = 0;

	BMPmap m_BMPmap;
	m_BMPmap.LoadBMP("Data\\Map1.bmp");
	int BMPiter = 0;
	RGB rgbTemp;

	for (int i = 0; i < 32 * 32; i++)
	{
		rgbTemp = m_BMPmap.GetPixel(BMPiter);

		//�»� : ��
		if (rgbTemp.Red == 255 && rgbTemp.Green == 0 && rgbTemp.Blue == 0) {m_ColorMap[i] = 1;}
		//��� : û
		else if (rgbTemp.Red == 0 && rgbTemp.Green == 0 && rgbTemp.Blue == 255){m_ColorMap[i] = 2;}
		//���� : ��
		else if (rgbTemp.Red == 0 && rgbTemp.Green == 255 && rgbTemp.Blue == 0){m_ColorMap[i] = 3;}
		//���� : Ȳ
		else if (rgbTemp.Red == 255 && rgbTemp.Green == 255 && rgbTemp.Blue == 0){m_ColorMap[i] = 4;}
		//��ֹ� : ���ӻ�
		else{m_ColorMap[i] = 0;}
		++BMPiter;
	}
}


CField::~CField()
{
	m_vMonster.clear();
	m_vNpc.clear();
}

bool CField::Begin(DWORD _num)
{
	m_num = _num;
	for (int i = 0; i < m_MaxUserCount; i++)
	{
		m_paUser[i] = NULL;
	}
	m_MaxUserCount = 100;
	m_currentUserCount = 0;

	// ��������
	m_FieldState = false;
	m_gameState = GAME_WAIT;
	m_mapNum = -1;

	// ����
	m_maxMonsterCount = 300;
	m_currentMonsterCount = 0;
	m_deadMonsterCount = 0;
	m_vMonster.clear();

	// NPC
	m_maxNpcCount = 4;
	m_currentNpcCount = 0;
	m_deadNpcCount = 0;
	m_vNpc.clear();

	// ���� �ʱ�ȭ
	int ran = rand() % 100;
	for (int i = 0; i < 100; ++i)
	{
		CBuff *buff = new CBuff();
		buff->Begin(i);

		srand((unsigned int)time(NULL));
		if ((i + ran) % 3 != 0)
		{
			buff->m_atk = 1.5;
		}
		else
		{
			buff->m_hp = 20;
		}
		m_vBuff.push_back(buff);
	}

	return true;
}

void CField::End()
{
	m_FieldState = false;
	m_gameState = GAME_WAIT;
	// ��
	COLOR_CS->EnterCS();

	for (DWORD i = 0; i < m_vMonster.size(); ++i)
	{
		CMonster *Mon = m_vMonster[i];
		Mon->End();
		delete Mon;
	}
	m_vMonster.clear();
	m_currentMonsterCount = 0;
	m_deadMonsterCount = 0;
	for (DWORD i = 0; i < m_vNpc.size(); ++i)
	{
		CNpc *Npc = m_vNpc[i];
		Npc->End();
		delete Npc;
	}
	m_vNpc.clear();
	m_currentNpcCount = 0;
	m_deadNpcCount = 0;
	// �� ����
	COLOR_CS->LeaveCS();
}

// �ش� ��Ŷ�� �濡 �ִ� ��� �������� ������.
void CField::SendAllUser(char *_packet)
{
	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL)
		{
			continue;
		}
		m_paUser[i]->SendPacket(_packet);
		
	}
}
void CField::SendAllMove()
{
	sc_pos player_pos_packet;
	player_pos_packet.size = sizeof(player_pos_packet);
	player_pos_packet.type = SC_POS;
	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL)
		{
			continue;
		}
		player_pos_packet.num = m_paUser[i]->GetNumber();
		player_pos_packet.character = m_paUser[i]->m_selectCharacter;
		player_pos_packet.state = m_paUser[i]->m_characterState;
		player_pos_packet.posX = m_paUser[i]->m_posX;
		player_pos_packet.posY = m_paUser[i]->m_posY;
		player_pos_packet.posZ = m_paUser[i]->m_posZ;
		player_pos_packet.rotX = m_paUser[i]->m_rotX;
		player_pos_packet.rotY = m_paUser[i]->m_rotY;
		player_pos_packet.rotZ = m_paUser[i]->m_rotZ;
		SendAllUser((char*)&player_pos_packet);
		cout << " �ǽ� ���� 2 " << endl;
	}
}


// ��� ������ ������ ��� �������� ������.
void CField::SendAllMonster()
{
	for (auto monIter = m_vMonster.begin(); monIter != m_vMonster.end(); ++monIter)
	{
		sc_monster monster_packet;
		monster_packet.size = sizeof(monster_packet);
		monster_packet.type = SC_MONSTER;
		monster_packet.num = (*monIter)->m_num;
		monster_packet.montype = (*monIter)->m_type;
		monster_packet.state = (*monIter)->m_state;
		monster_packet.posX = (*monIter)->m_posX;
		monster_packet.posY = 2;
		monster_packet.posZ = (*monIter)->m_posZ;
		monster_packet.rotX = (*monIter)->m_rotX;
		monster_packet.rotY = (*monIter)->m_rotY;
		monster_packet.rotZ = (*monIter)->m_rotZ;
		SendAllUser((char*)(&monster_packet));
	}
}


void CField::SendAllMonster(CUser *_user)
{
	int id = -1;
	for (int i = 0; i < m_MaxUserCount; ++i)
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
		if (m_paUser[i] == NULL)
		{
			continue;
		}
		if (m_paUser[i] != _user)
		{
			continue;
		}
		id = i;
	}
	if (id == -1)
	{
		return;
	}
	for (auto monIter = m_vMonster.begin(); monIter != m_vMonster.end(); ++monIter)
	{
		sc_monster monster_packet;
		monster_packet.size = sizeof(monster_packet);
		monster_packet.type = SC_MONSTER;
		monster_packet.num = (*monIter)->m_num;
		monster_packet.montype = (*monIter)->m_type;
		monster_packet.state = (*monIter)->m_state;
		monster_packet.posX = (*monIter)->m_posX;
		monster_packet.posY = 2;
		monster_packet.posZ = (*monIter)->m_posZ;
		monster_packet.rotX = (*monIter)->m_rotX;
		monster_packet.rotY = (*monIter)->m_rotY;
		monster_packet.rotZ = (*monIter)->m_rotZ;

		m_paUser[id]->SendPacket((char *)(&monster_packet));
	}

}

void CField::SetMonster(vector<CMonster*>& v)
{
	m_vMonster = v;
}

// ��� ������ ������ ��� �������� ������.
void CField::SendAllNpc()
{
	for (auto npcIter = m_vNpc.begin(); npcIter != m_vNpc.end(); ++npcIter)
	{
		sc_npc npc_packet;
		npc_packet.size = sizeof(npc_packet);
		npc_packet.type = SC_NPC;
		npc_packet.num = (*npcIter)->m_num;
		npc_packet.npctype = (*npcIter)->m_type;
		npc_packet.state = (*npcIter)->m_state;
		npc_packet.posX = (*npcIter)->m_posX;
		npc_packet.posY = 2;
		npc_packet.posZ = (*npcIter)->m_posZ;
		npc_packet.rotX = (*npcIter)->m_rotX;
		npc_packet.rotY = (*npcIter)->m_rotY;
		npc_packet.rotZ = (*npcIter)->m_rotZ;
		SendAllUser((char*)(&npc_packet));
	}
}


void CField::SendAllNpc(CUser *_user)
{
	int id = -1;
	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL)
		{
			continue;
		}
		if (m_paUser[i] != _user)
		{
			continue;
		}
		id = i;
	}
	if (id == -1)
	{
		return;
	}
	for (auto npcIter = m_vNpc.begin(); npcIter != m_vNpc.end(); ++npcIter)
	{
		sc_npc npc_packet;
		npc_packet.size = sizeof(npc_packet);
		npc_packet.type = SC_NPC;
		npc_packet.num = (*npcIter)->m_num;
		npc_packet.npctype = (*npcIter)->m_type;
		npc_packet.state = (*npcIter)->m_state;
		npc_packet.posX = (*npcIter)->m_posX;
		npc_packet.posY = 2;
		npc_packet.posZ = (*npcIter)->m_posZ;
		npc_packet.rotX = (*npcIter)->m_rotX;
		npc_packet.rotY = (*npcIter)->m_rotY;
		npc_packet.rotZ = (*npcIter)->m_rotZ;
		m_paUser[id]->SendPacket((char *)(&npc_packet));
	}
}
void CField::SetNpc(vector<CNpc*>& n)
{
	m_vNpc = n;
}

bool CField::JoinUser(CUser* user)
{
	if (user == NULL)
	{
		return false;
	}
	// �濡 �ο��� �� á���� ����
	if (m_currentUserCount >= m_MaxUserCount)
	{
		return false;
	}

	for (int i = 0; i < m_MaxUserCount; i++)
	{
		if (NULL == m_paUser[i])
		{
			m_paUser[i] = user;
			break;
		}
	}
	user->SetField(this);
	user->m_FieldNumber = m_num;

	m_currentUserCount = min(m_currentUserCount + 1, m_MaxUserCount);

	return true;
}

bool CField::LeaveUser(CUser *user)
{
	if (NULL == user)
	{
		return false;
	}
	//�濡 �ο��� ������
	COLOR_CS->EnterCS();
	if (m_currentUserCount <= 0)
	{
		COLOR_CS->LeaveCS();
		return false;
	}

	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (user == m_paUser[i])
		{
			user->SetField(NULL);
			user->m_FieldNumber = -1;
			m_paUser[i] = NULL;
			break;
		}
	}
	m_currentUserCount = max(m_currentUserCount - 1, 0);

	// �� ���ο� �ִ� �ٸ� Ŭ���̾�Ʈ�鿡�� �ش� ���� �������� �˸�
	// ���� ����
	sc_player_disconnect remove_packet;
	remove_packet.size = sizeof(remove_packet);
	remove_packet.type = SC_REMOVE_PLAYER;
	remove_packet.num = user->GetNumber();

	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL)
		{
			continue;
		}
		if (m_paUser[i] == user)
		{
			continue;
		}

		m_paUser[i]->SendPacket((char*)(&remove_packet));

		printf("%d�� %d�� Ŭ�� �������� %d�� %d�� Ŭ�󿡰� ����\n",
			m_num, user->GetNumber(), m_paUser[i]->m_FieldNumber, m_paUser[i]->GetNumber());
	}
	COLOR_CS->LeaveCS();
	// �濡 �ο��� ������ �ʱ�ȭ
	if (m_currentUserCount <= 0)
	{
		End();
		COLOR_CS->EnterCS();
		Begin(m_num);
		COLOR_CS->LeaveCS();

	}
	return true;
}


bool CField::RoomStart()
{
	m_FieldState = true;
	return true;
}



bool CField::GameStart()
{
	m_gameState = GAME_PLAY;
	return true;
}

void CField::Update()
{
	COLOR_CS->EnterCS();
	DWORD curTime = GetTickCount();

	// ���� ������ ������Ʈ
	for (int i = 0; i < m_MaxUserCount; ++i)
	{
		if (m_paUser[i] == NULL)
		{
			continue;
		}
		if (m_paUser[i]->m_buff.m_timer == 0)
		{
			continue;
		}
		
		// ������ ���� Ÿ�̸Ӹ� �����Ѵ�.
		if (m_paUser[i]->m_buff.m_timer < curTime)
		{
			//cout << m_paUser[i]->m_buff.m_type << "�� ���� " << endl;
			//cout << m_paUser[i]->m_buff.m_timer << "���� Ÿ����" << endl;
			m_paUser[i]->m_buff.Begin(0);
			cout << m_paUser[i]->GetNumber() << " ���� ����ȿ�� �����" << endl;
		}
	}


	// ���� ������Ʈ
	for (auto mon : m_vMonster)
	{
		if (mon->m_state == MON_NON)
		{
			continue;
		}
		else if (mon->m_state == MON_DEAD)
		{
			mon->Update(curTime, this);
			continue;
		}
		// ���Ͱ� �Ϲݸ���̰ų� ������� ��� Ž��
		else if (mon->m_state == MON_NORMAL || mon->m_state == MON_VIGILANCE)
		{
			for (int i = 0; i < m_MaxUserCount; ++i)
			{
				if (m_paUser[i] == NULL)
				{
					continue;
				}
				// ���Ͱ� ������ ã���� ����
				if (true == mon->EnemySearch(m_paUser[i]))
				{
					break;;
				}
			}
		}
		else if (mon->m_state == MON_COMBAT)
		{
			for (int i = 0; i < m_MaxUserCount; ++i)
			{
				if (m_paUser[i] == NULL)
				{
					continue;
				}
				if (mon->m_target != m_paUser[i])
				{
					continue;
				}
				if (true == mon->EnemySearch(m_paUser[i]))
				{
					continue;
				}
			}
		}
		mon->Update(curTime,this);
	}
	// NPC ������Ʈ

	for (auto npc : m_vNpc)
	{
		if (npc->m_state == NPC_NON)
		{
			continue;
		}
		if (npc->m_state == NPC_DEAD)
		{
			continue;
		}
		// NPC�� �Ϲݸ���̰ų� ������� ��� Ž��
		if (npc->m_state == NPC_NORMAL)
		{
			for (int i = 0; i < m_MaxUserCount; ++i)
			{
				if (m_paUser[i] == NULL)
				{
					continue;
				}
				// ���Ͱ� ������ ã���� ����
				if (true == npc->EnemySearch(m_paUser[i]))
				{
					break;;
				}
			}
		}
		else if (npc->m_state == NPC_COMBAT )
		{
			for (int i = 0; i < m_MaxUserCount; ++i)
			{
				if (m_paUser[i] == NULL)
				{
					continue;
				}
				if (npc->m_target != m_paUser[i])
				{
					continue;
				}
				if (true == npc->EnemySearch(m_paUser[i]))
				{
					continue;
				}
			}
		}
		npc->Update(curTime, this);
	}
	COLOR_CS->LeaveCS();
	COLOR_CS->EnterCS();
	// ��� ���� ������ ��� �������� ������.
	SendAllMonster();
//	view();
	// ��� NPC ������ ��� �������� ������.
	SendAllNpc();
		// ��� ������ �̵������� ��� �������� ������.
	SendAllMove();
	cout << "�ǽ����� " << endl;
	COLOR_CS->LeaveCS();
}

bool CField::GameLoading()
{
	m_currentMonsterCount = m_vMonster.size();
	m_maxMonsterCount = m_currentMonsterCount;	
	m_currentNpcCount = m_vNpc.size();
	m_maxNpcCount = m_currentNpcCount;
	return true;
}
 
CBuff* CField::Buff(CUser *user)
{
	// ���� �����Ѵ�.
	for (auto buff : m_vBuff)
	{
		if (buff->m_state == 0)
		{
			buff->m_state = 1;
			buff->m_posX = user->m_posX;
			buff->m_posY = user->m_posY;
			buff->m_posZ = user->m_posZ;

			sc_put_buff putbuff_packet;
			putbuff_packet.size = sizeof(putbuff_packet);
			putbuff_packet.type = SC_BUFF;
			putbuff_packet.num = buff->m_num;
			putbuff_packet.x = buff->m_posX;
			putbuff_packet.y = buff->m_posY;
			putbuff_packet.z = buff->m_posZ;
			SendAllUser((char *)(&putbuff_packet));
			return buff;
		}
	}
	return NULL;
}

bool CField::UseBuff(CUser *_user)
{
	if (_user == NULL)
	{
		return false;
	}

	for (auto buff : m_vBuff)
	{
		if (buff->m_state == 1)
		{
			if (false == In_range(buff->m_posX, buff->m_posZ, _user->m_posX, _user->m_posZ, 2))
			{
				continue;
			}
			// �����ȿ� ������ �ش� �������� ���ش�.
			buff->m_state = 2;

			// �������� �����ִ� ȿ���� �������� ��������ְ� �ð� ����
			
			if (buff->m_atk > 1.1)
			{
				_user->m_buff.m_atk = buff->m_atk;
				_user->m_buff.SetTime(GetTickCount());
				cout << _user->m_buff.m_timer << "�ð� " << endl;
				cout << _user->GetNumber() << "���� ������ ȹ��! ���ݷ� ����!" << endl;
				
			}
			if (buff->m_hp > 0)
			{
				_user->m_curHp = min(_user->m_curHp + buff->m_hp, _user->m_maxHp);
				cout << _user->m_buff.m_timer << "�ð� " << endl;
				cout << _user->GetNumber() << "���� ������ ȹ��! hp 20 ȸ��!" << endl;
			}
			sc_remove_buff buff_packet;
			buff_packet.size = sizeof(buff_packet);
			buff_packet.type = SC_REMOVE_BUFF;
			buff_packet.num = buff->m_num;
			SendAllUser((char *)(&buff_packet));
		}
	}
	return true;
}

void CField::ToAttack(CUser *user)
{
	CPoint3D attack_pos;
	CPoint3D mon_pos;
	CPoint3D Touser_pos;
	attack_pos.SetXYZ(user->m_posX + user->m_dirX * 2,
		0,
		user->m_posZ + user->m_dirZ * 2);

	WORD attackRange = 2;
	WORD CanSize = 5;
	CPoint3D dir = CPoint3D(user->m_posX, 0, user->m_posZ);
	float length = 0; // ������ ���ݹ��� �� ���ͰŸ� 

	float minLength = attackRange + CanSize;
	float tempLength = 0; // ���� �� ���� üũ�� �ӽ� ����
	CMonster *monHit = NULL; // �ǰݴ��� ����
	CUser *userHit = NULL;	//�ǰݴ��� ���� 
	DWORD time = GetTickCount();

	for (auto mons : m_vMonster)
	{
		mon_pos.SetXYZ(mons->m_posX, 0, mons->m_posZ);
		length = Point3Dlength(&(attack_pos - mon_pos));

		// ������ ���ݹ��� �ȿ� ���Ͱ� ������ 
		if (length < attackRange + CanSize)
		{
			// ���� �Ӹ� �ƴ϶�, ���⵵ �˻��Ѵ�. 
			// ������ �����´�.
			float userDirX = user->m_dirX;
			float userDirZ = user->m_dirZ;

			// ������ ���� ������ ������ �˱� ����, '���� ��ġ - ���� ��ġ'�� ���� ���� ������ �Ѵ�. �׸��� ���� ����ȭ �Ѵ�. hgi
			float UserToMonX = mon_pos.x - user->m_posX;
			float UserToMonZ = mon_pos.z - user->m_posZ;
			UserToMonX = UserToMonX / sqrtf(UserToMonX * UserToMonX + UserToMonZ * UserToMonZ);
			UserToMonZ = UserToMonZ / sqrtf(UserToMonX * UserToMonX + UserToMonZ * UserToMonZ);

			//������ ���̸� �̿��ؼ� �����Ѵ�. ������ ���� ���� ������ ���̴�, 1�� �����Ѵ�. hgi
			float Angle_UserToMon = ((userDirX * UserToMonX) + (userDirZ * UserToMonZ)) / sqrtf((UserToMonX * UserToMonX) + (UserToMonZ * UserToMonZ));

			if (Angle_UserToMon < 0.4f)
			{
				continue;
			}

			// ���Ϳ� ������ �Ÿ� �ľ�
			tempLength = Point3Dlength(&(dir - mon_pos));
			// ���� ����� ���� Ȯ��
			if (tempLength < minLength)
			{
				minLength = tempLength;
				monHit = mons;
			}
		}
	}

	// Ÿ������ ���Ͱ� ���� ���
	if (monHit != NULL)
	{
		monHit->m_target = user;
		monHit->m_curHp = max(monHit->m_curHp - (user->m_atk * user->m_buff.m_atk), 0);

		// ���ݴ��ϸ� ���͸� �������·� �ٲ�
		if (monHit->m_state != MON_COMBAT)
		{
			monHit->ChangeState(STATE.m_pMonsterCombat);
		}
		if (monHit->m_curHp <= 0)
		{
			//cout << monHit->m_num << "�� ���� Dead" << endl;
			monHit->m_state = MON_DEAD;
			monHit->ChangeState(STATE.m_pMonsterDead);
			user->m_curExp += 100;
			SendEXPandLVUP(user);
			if (rand() % 5 > 1)
			{
				Buff(user);
			}
			monHit->m_curHp = 0;
			monHit->m_target = NULL;
			m_deadMonsterCount = min(m_deadMonsterCount + 1, m_maxMonsterCount);
			m_currentMonsterCount = max(m_currentMonsterCount - 1, 0);
		}
		else
		{
			cout << monHit->m_num << "�� ���� �ǰ�! ����hp : " << monHit->m_curHp << endl;
		}

	}
	//���� ���Ͱ� Ÿ������ �������� ������ �����ٸ�
	else if (monHit == NULL)
	{
		for (int i = 0; i < m_MaxUserCount; i++)
		{
			if (m_paUser[i] != NULL)
			{
				if (m_paUser[i]->m_num == user->m_num)
				{
					continue;
				}

				if (m_paUser[i]->m_color == user->m_color)
				{
					continue;
				}

				Touser_pos.SetXYZ(m_paUser[i]->m_posX, 0, m_paUser[i]->m_posZ);			//���� ������ ��ġ�� ���� �´�.
				length = Point3Dlength(&(attack_pos - Touser_pos));

				if (length < attackRange + CanSize)
				{
					// ������ ������ �Ÿ� �ľ�
					tempLength = Point3Dlength(&(dir - Touser_pos));

					// ���� ����� ���� Ȯ��
					if (tempLength < minLength)
					{
						minLength = tempLength;
						userHit = m_paUser[i];
						//cout << "����� ���� Ȯ��" << userHit->m_id << endl;
					}
				}
			}
		}
		// Ÿ������ ������ �ִ� ���
		if (userHit != NULL)
		{
			userHit->m_curHp = max(userHit->m_curHp - (user->m_atk * user->m_buff.m_atk), 0);

			// �÷��̾ �׾��� ���
			if (userHit->m_curHp <= 0)
			{
				SendDead(userHit);
				user->m_curExp += 200;
				SendEXPandLVUP(user);
			}
			else
			{
				cout << userHit->m_num << "�� ���� �ǰ�! ����hp : " << userHit->m_curHp << endl;
			}
		}
	}

}

void CField::Respon(CUser *user)
{
	CMonster *monHit = NULL; // �ǰݴ��� ����
	CUser *userHit = NULL;	//�ǰݴ��� ���� 
	DWORD time = GetTickCount();
	CUser *User = user;
	// Ÿ������ ���Ͱ� ���� ���
	for (auto mon : m_vMonster)
	{
		monHit = mon;
	}

	if (monHit!= NULL)
	{
		monHit->m_target = user;
		// Ÿ������ ���Ͱ� ���� ���
		switch (monHit->m_target->m_color)
		{
		case 1:
			monHit->m_target->m_posX = -320.0f;
			monHit->m_target->m_posZ = -320.0f;
			break;
		case 2:
			monHit->m_target->m_posX = 320.0f - 30.0f;
			monHit->m_target->m_posZ = -320.0f;
			break;
		case 3:
			monHit->m_target->m_posX = -320.0f;
			monHit->m_target->m_posZ = 320.0f;
			break;
		case 4:
			monHit->m_target->m_posX = 320.0f - 30.0f;
			monHit->m_target->m_posZ = 320.0f;
			break;
		default:
			break;
		}
		sc_revive Revive_packet;
		Revive_packet.size = sizeof(Revive_packet);
		Revive_packet.type = SC_REVIVE;
		Revive_packet.target_type = REVIVE_PLAYER;
		Revive_packet.num = user->GetNumber();
		Revive_packet.x = user->m_posX;
		Revive_packet.y = user->m_posY;
		Revive_packet.z = user->m_posZ;
		SendAllUser((char *)(&Revive_packet));
	}
	else if (monHit == NULL)
	{
		for (int i = 0; i < MAX_USER; i++)
		{
			if (m_paUser[i] != NULL)
			{
				if (m_paUser[i]->m_num == user->m_num)
				{
					continue;
				}

				if (m_paUser[i]->m_color == user->m_color)
				{
					continue;
				}
				userHit = m_paUser[i];
			}
		}
		userHit->user_target = user;
		// Ÿ������ ������ �ִ� ���
		if (userHit != NULL)
		{
			switch (user->m_color)
			{
			case 1:
				user->m_posX = -320.0f;
				user->m_posZ = -320.0f;
				break;
			case 2:
				user->m_posX = 320.0f - 30.0f;
				user->m_posZ = -320.0f;
				break;
			case 3:
				user->m_posX = -320.0f;
				user->m_posZ = 320.0f;
				break;
			case 4:
				user->m_posX = 320.0f - 30.0f;
				user->m_posZ = 320.0f;
				break;
			default:
				break;
			}
			sc_revive Revive_packet;
			Revive_packet.size = sizeof(Revive_packet);
			Revive_packet.type = SC_REVIVE;
			Revive_packet.target_type = REVIVE_PLAYER;
			Revive_packet.num = user->GetNumber();
			Revive_packet.x = user->m_posX;
			Revive_packet.y = user->m_posY;
			Revive_packet.z = user->m_posZ;
			SendAllUser((char *)(&Revive_packet));
		}
	}
}


void CField::UseSkillToMonster(CUser* user, BYTE slotnum, float dirX, float dirY, float dirZ, int StartNum)
{
	CMonster* mon = NULL;
	CSkill* skill = user->GetSkill(slotnum);
	float dist = skill->GetDistance();

	for (int num = StartNum; num < m_vMonster.size(); ++num)
	{
		if (m_vMonster[num])
		{
			//������ ���°� ������ �ƴ϶��
			//if (mon->m_state != PSTATE_DEAD)

			mon = m_vMonster[num];

			if (In_range(user->m_posX, user->m_posZ, mon->m_posX, mon->m_posZ, dist))
			{
				mon->m_curHp = max(mon->m_curHp - (user->m_atk * user->GetSkill(slotnum)->GetPower()), 0);
			}
		}
	}
}

void CField::UseSkillToUser(CUser* user, BYTE slotnum, float dirX, float dirY, float dirZ, int StartNum)
{
	CUser* t_user = NULL;
	CSkill* skill = user->GetSkill(slotnum);
	float dist = skill->GetDistance();

	for (int num = StartNum; num < m_vUser.size(); ++num)
	{
		if (m_vUser[num])
		{
			t_user = m_vUser[num];

			// �� �ڽ��� �ƴϰ�, �����̶��
			if ((user->GetNumber() != t_user->GetNumber()) && (user->m_color != t_user->m_color))
			{
				//������ ���°� ������ �ƴ϶��
				//if (mon->m_state != PSTATE_DEAD)

				if (In_range(user->m_posX, user->m_posZ, t_user->m_posX, t_user->m_posZ, dist))
				{
					t_user->m_curHp = max(t_user->m_curHp - (user->m_atk * user->GetSkill(slotnum)->GetPower()), 0);
				}
			}
		}
	}

}


void CField::SendEXPandLVUP(CUser *_user)
{
	sc_exp exp_packet;
	exp_packet.size = sizeof(exp_packet);
	exp_packet.type = SC_EXP;
	exp_packet.num = _user->GetNumber();
	exp_packet.curExp = _user->m_curExp;
	exp_packet.maxExp = _user->m_maxExp;
	SendAllUser((char *)(&exp_packet));
	if (_user->m_maxExp <= _user->m_curExp)
	{
		_user->m_Lv += 1;
		_user->m_atk += 5;
		_user->m_atksp += 0;
		_user->m_maxHp += 50;
		_user->m_curHp = _user->m_maxHp;
		_user->m_curExp = 0;
		_user->m_maxExp += 1000;
		sc_Lvup lvup_packet;
		lvup_packet.size = sizeof(lvup_packet);
		lvup_packet.type = SC_LVUP;
		lvup_packet.m_atk = _user->m_atk;
		lvup_packet.m_atksp = _user->m_atksp;
		lvup_packet.m_curExp = _user->m_curExp;
		lvup_packet.m_curHp = _user->m_curHp;
		lvup_packet.m_Lv = _user->m_Lv;
		lvup_packet.m_maxExp = _user->m_maxExp;
		lvup_packet.m_maxHp = _user->m_maxHp;
		SendAllUser((char *)(&lvup_packet));
	}
}

void CField::SendDead(CUser *user)
{
	CMonster *monHit = NULL; // ����
	CUser *userHit = NULL;	//�ǰݴ��� ���� 
	DWORD time = GetTickCount();
	CUser *User = user;
	// Ÿ������ ���Ͱ� ���� ���
	for (auto mon : m_vMonster)
	{
		monHit = mon;
	}
	if (monHit != NULL)
	{
		cout << "�÷��̾��� �ǰ� 0�� �Ǿ� ����߽��ϴ� " << endl;
		user->m_characterState = PSTATE_DEAD;
		user->m_stateTimer = GetTickCount();
		sc_dead dead_packet;
		dead_packet.size = sizeof(dead_packet);
		dead_packet.type = SC_DEAD;
		dead_packet.num = user->GetNumber();
		dead_packet.target_type = DEAD_PLAYER;
		SendAllUser((char *)(&dead_packet));
		monHit->m_target = NULL;
		monHit->ChangeState(STATE.m_pMonsterMove);
	}
	else if (monHit == NULL || userHit != NULL)
		{
			cout << userHit->m_num << "�� ���� Dead" << endl;
			userHit->m_curHp = 0;
			user->m_characterState = PSTATE_DEAD;
			user->m_stateTimer = GetTickCount();
			sc_dead dead_packet;
			dead_packet.size = sizeof(dead_packet);
			dead_packet.type = SC_DEAD;
			dead_packet.num = user->GetNumber();
			dead_packet.target_type = DEAD_PLAYER;
			SendAllUser((char *)(&dead_packet));
		}
}
	//void CField::view()
	//{
	//	CUser *User = NULL;
	//	// ���� ������ �ٸ��������
	//
	//	for (int i = 0; i < GetCurrentUserCount(); i++)
	//	{
	//		set<int> old_list = m_paUser[i]->view_list;
	//
	//		set<int> new_list;
	//		new_list.insert(i);
	//
	//		for (int obj : new_list)
	//		{
	//			if (0 == old_list.count(obj))
	//			{
	//				COLOR_CS->EnterCS();
	//				m_paUser[i]->view_list.insert(obj);
	//				COLOR_CS->LeaveCS();
	//				sc_put_player put_player_packet;
	//				put_player_packet.id = obj;
	//				put_player_packet.size = sizeof(sc_put_player);
	//				put_player_packet.type = SC_PUT_PLAYER;
	//				put_player_packet.x = m_paUser[obj]->m_posX;
	//				put_player_packet.y = m_paUser[obj]->m_posY;
	//				put_player_packet.z = m_paUser[obj]->m_posZ;
	//				put_player_packet.dirX = m_paUser[obj]->m_dirX;
	//				put_player_packet.dirY = m_paUser[obj]->m_dirY;
	//				put_player_packet.dirZ = m_paUser[obj]->m_dirZ;
	//				SendAllUser((char*)&put_player_packet);
	//				COLOR_CS->EnterCS();
	//				if (0 != m_paUser[obj]->view_list.count(i))
	//				{
	//					cout << " �������� ���� " << endl;
	//					SendAllMove();
	//				}
	//				else
	//				{
	//					m_paUser[obj]->view_list.insert(i);
	//					sc_put_player put_player_packet;
	//					put_player_packet.id = i;
	//					put_player_packet.size = sizeof(sc_put_player);
	//					put_player_packet.type = SC_PUT_PLAYER;
	//					put_player_packet.x = m_paUser[i]->m_posX;
	//					put_player_packet.y = m_paUser[i]->m_posY;
	//					put_player_packet.z = m_paUser[i]->m_posZ;
	//					SendAllUser((char*)&put_player_packet);
	//					cout << "���ο� �÷��̾� ���� ��Ŷ ���� " << endl;
	//				}
	//				COLOR_CS->LeaveCS();
	//			}
	//			else  // �� �丮��Ʈ�� obj�� �̹� �־��ٸ�
	//			{
	//				COLOR_CS->EnterCS();
	//				if (0 != m_paUser[obj]->view_list.count(i))
	//				{
	//					//SendAllUser((char*)(&pos_packet));
	//					SendAllMove();
	//				}
	//				else
	//				{
	//					m_paUser[obj]->view_list.insert(i);
	//					sc_player_view put_view_packet;
	//					put_view_packet.id = i;
	//					put_view_packet.size = sizeof(sc_player_view);
	//					//put_view_packet.type = SC_PUT_PLAYER;
	//					put_view_packet.type = SC_VIEW;
	//					put_view_packet.x = m_paUser[i]->m_posX;
	//					put_view_packet.y = m_paUser[i]->m_posY;
	//					put_view_packet.z = m_paUser[i]->m_posZ;
	//					SendAllUser((char*)&put_view_packet);
	//					cout << "�� ��Ŷ ���� " << endl;
	//				}
	//				COLOR_CS->LeaveCS();
	//			}
	//		}  // ���� ���ο� �丮��Ʈ�� �ִ� objectó��
	//
	//		for (int obj : old_list)
	//		{
	//			// new_list�� �������� �ʴ� ��ü ó��
	//			if (0 != new_list.count(obj))
	//			{
	//				continue;
	//			}
	//			cout << " �õ� ����Ʈ���� ���� ���� ���� " << endl;
	//			LeaveUser(m_paUser[i]);
	//			COLOR_CS->EnterCS();
	//			m_paUser[i]->view_list.erase(obj);
	//			COLOR_CS->LeaveCS();
	//			if (0 == m_paUser[obj]->view_list.count(i))
	//			{
	//				COLOR_CS->LeaveCS();
	//				continue;
	//			}
	//			m_paUser[obj]->view_list.erase(i);
	//			cout << "�����ٴ°� �̰ɷ� ������ �Ŵ� " << endl;
	//			COLOR_CS->LeaveCS();
	//		}
	//			//for (int i = 0; i < GetMaxUserCount(); i++)
	//			//{
	//			//	if (m_paUser[i] != NULL)
	//			//	{
	//			//		//if (false == field->m_paUser[i]->m_Login)
	//			//		//{
	//			//		//	continue;
	//			//		//}
	//			//		//if (user == field->m_paUser[i])
	//			//		//{
	//			//		//	continue;
	//			//		//}
	//			//		cout << "�� ���� �����ֱ� " << endl;
	//			//		//SendPacketType(SC_PUT_PLAYER, user, field->m_paUser[i], field);
	//			//		//SendPacketType(SC_CHARACTERSELECT, user, field->m_paUser[i], field);
	//			//	}
	//			//}
	//	}
	//	// �ٸ������ ������ ������
	//	//for (int i = 0; i < GetMaxUserCount(); i++)
	//	//{
	//	//	if (m_paUser[i] != NULL)
	//	//	{
	//	//		if (false == m_paUser[i]->m_Login)
	//	//		{
	//	//			continue;
	//	//		}
	//	//		if (User == m_paUser[i])
	//	//		{
	//	//			continue;
	//	//		}
	//	//		cout << " �ٸ� ����� ���� �����ֱ� " << endl;
	//	//		//SendPacketType(SC_PUT_PLAYER, m_paUser[i], user);
	//	//		//SendPacketType(SC_CHARACTERSELECT, m_paUser[i], user);
	//	//	}
	//	//}
	//}
















































