#pragma once
#include "FieldManager.h"


CFieldManager::CFieldManager()
{
	m_maxFieldCount = MAX_ROOM;
	m_currentFieldCount = 0;
	m_Map1 = NULL;
	m_vField.clear();
	m_vMonster.clear();
	m_vNpc.clear();
}


CFieldManager::~CFieldManager()
{
}

bool CFieldManager::Begin(DWORD _maxRoomCount)
{
	if (_maxRoomCount <= 0) return false;
	m_maxFieldCount = _maxRoomCount;

	m_currentFieldCount = 0;

	m_Map1 = new CMapObject();
	m_Map1->SetBox(0, 2, 0, CPoint3D(0, 0, 0), CPoint3D(200, 0, 200));
	//m_Map1->SetBox(0, 2, 0, CPoint3D(0, 0, 0), CPoint3D(512, 0, 512));
	//cout << "Map1 Ʈ�� ���� �Ϸ�" << endl;
	// �� ���� �о�α�
	//MapFileLoadAndSet("Data\\map1_object.txt");
	MapFileLoadAndSet("Data\\Map1.bmp"); //bmp������ �д� ������ �ӽ� ó�� 
	// ���� ���� �о�α�
	//MonsterFileLoadAndSet("Data\\map4_monster.txt");
	MonsterFileLoadAndSet("Data\\map1_monster.bmp");
	// NPC ���� �о�α�
	NpcFileLoadAndSet("Data\\map1_npc.bmp");



	// �� ����
	for (auto i = 0; i < m_maxFieldCount; ++i)
	{
		if (m_vField.size() >= m_maxFieldCount)
		{
			break;
		}

		CField *Field = new CField();
		if (Field->Begin(i))
		{
			m_vField.push_back(Field);
		}

		else
		{
			End();
			return false;
		}
	}

	return true;
}

bool CFieldManager::End()
{
	// ��
		COLOR_CS->EnterCS();
		for (DWORD i = 0; i < m_vField.size(); ++i)
	{
			CField *Field = m_vField[i];
			Field->End();
			delete Field;
	}
		m_vField.clear();
		m_currentFieldCount = 0;

	// �� ����
	COLOR_CS->LeaveCS();
	return true;
}

// ���� ��ġ ���� �����͸� �̸� �о�´�.
bool CFieldManager::MonsterFileLoadAndSet(char *buf)
{
	BMPmap m_BMPmap;
	if (!m_BMPmap.LoadBMP(buf))
		return false;

	int BMPiter = 0;
	RGB rgbTemp;

	float posX, posZ;
	float posY = 32 / 2;

	for (int i = 0; i < 32 * 32; i++)
	{
		posX = (float)(((int)(i % 32) - 16) * 32);
		posZ = (float)(((int)(i / 32) - 16) * 32);

		rgbTemp = m_BMPmap.GetPixel(BMPiter);

		//���� : �����
		if (rgbTemp.Red == 255 && rgbTemp.Green == 0 && rgbTemp.Blue == 255)
		{
			CFieldMonster *monster = new CFieldMonster;
			monster->SetType(MON_BOSS);
			monster->Begin();
			monster->SetNum(-1);
			monster->SetPos(posX, 0.0f, posZ);
			m_vMonster.push_back(monster);
		}
		//���� ����1 : ����
		else if (rgbTemp.Red == 255 && rgbTemp.Green == 0 && rgbTemp.Blue == 0)
		{
			CFieldMonster *monster = new CFieldMonster;
			monster->SetType(MON_BUFF1);
			monster->Begin();
			monster->SetNum(-1);
			monster->SetPos(posX, 0.0f, posZ);
			m_vMonster.push_back(monster);
		}
		//���� ����2 : û��
		else if (rgbTemp.Red == 0 && rgbTemp.Green == 0 && rgbTemp.Blue == 255)
		{
			CFieldMonster *monster = new CFieldMonster;
			monster->SetType(MON_BUFF2);
			monster->Begin();
			monster->SetNum(-1);
			monster->SetPos(posX, 0.0f, posZ);
			m_vMonster.push_back(monster);
		}
		//���� ����1 ����: ���
		else if(rgbTemp.Red == 0 && rgbTemp.Green == 255 && rgbTemp.Blue == 0)
		{
			CFieldMonster *monster = new CFieldMonster;
			monster->SetType(MON_LBF1);
			monster->Begin();
			monster->SetNum(-1);
			monster->SetPos(posX, 0.0f, posZ);
			m_vMonster.push_back(monster);
		}
		//���� ����2 ���� : Ȳ��
		else if (rgbTemp.Red == 255 && rgbTemp.Green == 255 && rgbTemp.Blue == 0)
		{
			CFieldMonster *monster = new CFieldMonster;
			monster->SetType(MON_LBF2);
			monster->Begin();
			monster->SetNum(-1);
			monster->SetPos(posX, 0.0f, posZ);
			m_vMonster.push_back(monster);
		}

		++BMPiter;
	}

	for (int i = m_vMonster.size(); i < 300; ++i)
	{
		CFieldMonster *monster = new CFieldMonster;
		monster->SetType(MON_BASE);
		monster->Begin();
		monster->SetNum(-1);

		while(1)
		{
			int px = rand() % 32;
			int pz = rand() % 32;

			rgbTemp = m_BMPmap.GetPixel(pz * 32 + px);

			if (rgbTemp.Red == 255 && rgbTemp.Green == 255 && rgbTemp.Blue == 255)
			{
				posZ = pz;
				posX = px;
				break;
			}
		}

		// Ÿ�� �߾� ��ǥ���� �����ϰ� �����ϵ���, �ڿ� ���� ���ڸ� �ξ���.
		posX = ((posX - 16) * 32) + ((rand() % 20) - 10);
		posZ = ((posZ - 16) * 32) + ((rand() % 20) - 10);

		monster->SetPos(posX, 0.0f, posZ);
		m_vMonster.push_back(monster);
	}

	for (int i = 0; i < 300; i++)
	{
		//��ȣ �ο�
		m_vMonster[i]->SetNum(i);
	}
	return true;
}


// ���� ��ġ ���� �����͸� �̸� �о�´�.
bool CFieldManager::NpcFileLoadAndSet(char *buf)
{
	BMPmap m_BMPmap;
	if (!m_BMPmap.LoadBMP(buf))
		return false;

	int BMPiter = 0;
	RGB rgbTemp;

	float posX, posZ;
	float posY = 32 / 2;

	for (int i = 0; i < 32 * 32; i++)
	{
		posX = (float)(((int)(i % 32) - 16) * 32);
		posZ = (float)(((int)(i / 32) - 16) * 32);

		rgbTemp = m_BMPmap.GetPixel(BMPiter);

		//NPC : ����
		if (rgbTemp.Red == 255 && rgbTemp.Green == 0 && rgbTemp.Blue == 0)
		{
			CFieldNpc *npc = new CFieldNpc;
			npc->SetType(NPC_RED);
			npc->Begin();
			npc->SetNum(-1);
			npc->SetPos(posX, 0.0f, posZ);
			m_vNpc.push_back(npc);
		}
		//NPC: û��
		else if (rgbTemp.Red == 0 && rgbTemp.Green == 0 && rgbTemp.Blue == 255)
		{
			CFieldNpc *npc = new CFieldNpc;
			npc->SetType(NPC_BLUE);
			npc->Begin();
			npc->SetNum(-1);
			npc->SetPos(posX, 0.0f, posZ);
			m_vNpc.push_back(npc);
		}
		//���� ����1 ����: ���
		else if (rgbTemp.Red == 0 && rgbTemp.Green == 255 && rgbTemp.Blue == 0)
		{
			CFieldNpc *npc = new CFieldNpc;
			npc->SetType(NPC_GREEN);
			npc->Begin();
			npc->SetNum(-1);
			npc->SetPos(posX, 0.0f, posZ);
			m_vNpc.push_back(npc);
		}
		//���� ����2 ���� : Ȳ��
		else if (rgbTemp.Red == 255 && rgbTemp.Green == 255 && rgbTemp.Blue == 0)
		{
			CFieldNpc *npc = new CFieldNpc;
			npc->SetType(NPC_YELLOW);
			npc->Begin();
			npc->SetNum(-1);
			npc->SetPos(posX, 0.0f, posZ);
			m_vNpc.push_back(npc);
		}

		++BMPiter;
	}
	
	for (int i = 0; i < 4; i++)
	{
		//��ȣ �ο�
		m_vNpc[i]->SetNum(i);
	}
	return true;
}


// �� ������Ʈ ��ġ ���� �����͸� �а� �����Ѵ�.
bool CFieldManager::MapFileLoadAndSet(char *buf)
{
	m_Map1 = new CMapObject;
	m_Map1->SetBox(0, 2, 0, CPoint3D(0, 0, 0), CPoint3D(32 * 32, 0, 32 * 32));

	BMPmap m_BMPmap;
	if (!m_BMPmap.LoadBMP(buf))
		return false;

	int BMPiter = 0;
	RGB rgbTemp;

	float posX, posZ;
	float posY = 32 / 2;

	for (int i = 0; i < 32 * 32; i++)
	{
		posX = (float)(((int)(i % 32) - 16) * 32);
		posZ = (float)(((int)(i / 32) - 16) * 32);

		rgbTemp = m_BMPmap.GetPixel(BMPiter);

		//��ֹ� : ���ӻ�
		if (rgbTemp.Red == 0 && rgbTemp.Green == 255 && rgbTemp.Blue == 255)
		{
			CMapObject *obj = new CMapObject;
			obj->SetBox(0, 0, 1, CPoint3D(posX, posY, posZ), CPoint3D(32 / 2, 32 / 2, 32 / 2));
			// �浹üũ�� ���� Ʈ�������� �߰��ϴ� ������Ʈ
			m_Map1->AddObject(*obj);
			// �ʿ� � ������Ʈ���� ���ֳ� Ȯ�ο�
			m_Map1->v_AddObj.push_back(obj);
		}

		++BMPiter;
	}

	//�ּ� �Ʒ��� �����̾���. ���� ����� ���� �ٶ�. hgi
	/*
	FILE *fp;
	int retval;
	int temp1, temp2;
	int type;
	float posX, posY, posZ;
	float sizeX, sizeY, sizeZ;
	float dirX, dirY, dirZ;
	if (fopen_s(&fp, buf, "r+") != 0){
		printf("%s file open Error\n", buf);
		return false;
	}
	while (1)
	{
		retval = fscanf_s(fp, "%d %d %d %f %f %f %f %f %f %f %f %f\n", &temp1, &temp2, &type, &posX, &posY, &posZ, &sizeX, &sizeY, &sizeZ, &dirX, &dirY, &dirZ);
		if (retval > 0){
			//printf("%d ���� : %d %d %f %f %f\n", retval, num, type, x, y, z);
			//CMapObject *obj = new CMapObject;
			//obj->SetBox(0, 0, type, CPoint3D(posX, posY, posZ), CPoint3D(sizeX, sizeY, sizeZ));
			// �浹üũ�� ���� Ʈ�������� �߰��ϴ� ������Ʈ
			//m_Map1->AddObject(*obj);
			// �ʿ� � ������Ʈ���� ���ֳ� Ȯ�ο�
			//m_Map1->v_AddObj.push_back(obj);
			//cout << "Obj pos : " << posX << ", " << posY << ", " << posZ << " ���� �Ϸ�" << endl;
		}
		else{
			//printf("retval Error : %d\n", retval);
			break;
		}
	}
	fclose(fp);

	if (retval < 0) return false;
	*/

	return true;
}


CField* CFieldManager::QuickJoin(CUser* user)
{
	cout << "�̰� ���� �ǳ� >" << endl;
	// ��
	COLOR_CS->EnterCS();
	if (user == NULL) return NULL;
	CField *Field = NULL;
	for (DWORD i = 0; i < m_vField.size(); ++i)
	{
		Field = m_vField[i];
		//Field->m_lock.lock();
		COLOR_CS->EnterCS();
		// ����� �� ������ �ʰ�, ����� �ְ�, ���ӽ������� ���� ��
		// ������ ���� �����߿��� �����ϵ��� �� 
		if (!Field->GetIsFull() && !Field->GetIsEmpty())// && !Room->GetRoomState())
		{
			if (!Field->JoinUser(user))
			{
				//Field->m_lock.unlock();
				COLOR_CS->LeaveCS();
				continue;
			}
			else
			{
				//Field->m_lock.unlock();
				//m_lock.unlock();
				COLOR_CS->LeaveCS();
				return Field;
			}
		}
		//Field->m_lock.unlock();
		COLOR_CS->LeaveCS();
	}
	// ����� �ִ� ���� ���ų� ��� �� á�� ���
	// ����� ã�´�.
	for (DWORD i = 0; i < m_vField.size(); ++i)
	{
		Field = m_vField[i];
		//Field->m_lock.lock();
		COLOR_CS->EnterCS();
		if (Field->GetIsEmpty())
		{
			if (!Field->JoinUser(user)){
				//Field->m_lock.unlock();
				COLOR_CS->LeaveCS();
				continue;
			}
			else{
				//Field->m_lock.unlock();
				//m_lock.unlock();
	
				COLOR_CS->LeaveCS();
				return Field;
			}
		}
		//Field->m_lock.unlock();
		COLOR_CS->LeaveCS();
	}
	// �� ����
	//m_lock.unlock();
	COLOR_CS->LeaveCS();
	// ����� ����.
	return NULL;
}

CField* CFieldManager::GetField(unsigned int num)
{
	std::vector<CField*>::iterator iter = m_vField.begin();
	if (num >= m_vField.size()) return NULL;
	iter += num;
	return (*iter);
}

//CLobby* CFieldManager::QuickJoinLobby(CUser* user)
//{
//	// ��
//	m_lock.lock();
//	//COLOR_CS->EnterCS();
//	if (user == NULL) return NULL;
//	CLobby *Lobby = NULL;
//	for (DWORD i = 0; i < m_vLobby.size(); ++i)
//	{
//		Lobby->m_lock.lock();
//		//COLOR_CS->EnterCS();
//		// ����� �� ������ �ʰ�, ����� �ְ�, ���ӽ������� ���� ��
//		// ������ ���� �����߿��� �����ϵ��� �� 
//		if (!Lobby->GetIsFull() && !Lobby->GetIsEmpty())
//		{
//			if (!Lobby->JoinUser(user))
//			{
//				Lobby->m_lock.unlock();
//				//COLOR_CS->LeaveCS();
//				continue;
//			}
//			else
//			{
//				Lobby->m_lock.unlock();
//				m_lock.unlock();
//				//COLOR_CS->LeaveCS();
//				return Lobby;
//			}
//		}
//		Lobby->m_lock.unlock();
//		//COLOR_CS->LeaveCS();
//	}
//	// ����� �ִ� ���� ���ų� ��� �� á�� ���
//	// ����� ã�´�.
//	for (DWORD i = 0; i < m_vLobby.size(); ++i)
//	{
//		Lobby->m_lock.lock();
//		//COLOR_CS->EnterCS();
//		if (Lobby->GetIsEmpty())
//		{
//			if (!Lobby->JoinUser(user)){
//				Lobby->m_lock.unlock();
//				//COLOR_CS->LeaveCS();
//				continue;
//			}
//			else{
//				Lobby->m_lock.unlock();
//
//				//COLOR_CS->LeaveCS();
//				return Lobby;
//			}
//		}
//		Lobby->m_lock.unlock();
//		//COLOR_CS->LeaveCS();
//	}
//	// �� ����
//	m_lock.unlock();
//	//COLOR_CS->LeaveCS();
//	// ����� ����.
//	return NULL;
//}
//CLobby* CFieldManager::getLobby(unsigned int num)
//{
//	std::vector<CLobby*>::iterator iter = m_vLobby.begin();
//	if (num >= m_vLobby.size()) return NULL;
//	iter += num;
//	return (*iter);
//}