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
	//cout << "Map1 트리 생성 완료" << endl;
	// 맵 정보 읽어두기
	//MapFileLoadAndSet("Data\\map1_object.txt");
	MapFileLoadAndSet("Data\\Map1.bmp"); //bmp파일을 읽는 것으로 임시 처리 
	// 몬스터 정보 읽어두기
	//MonsterFileLoadAndSet("Data\\map4_monster.txt");
	MonsterFileLoadAndSet("Data\\map1_monster.bmp");
	// NPC 정보 읽어두기
	NpcFileLoadAndSet("Data\\map1_npc.bmp");



	// 방 생성
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
	// 락
		COLOR_CS->EnterCS();
		for (DWORD i = 0; i < m_vField.size(); ++i)
	{
			CField *Field = m_vField[i];
			Field->End();
			delete Field;
	}
		m_vField.clear();
		m_currentFieldCount = 0;

	// 락 해제
	COLOR_CS->LeaveCS();
	return true;
}

// 몬스터 위치 파일 데이터를 미리 읽어온다.
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

		//보스 : 보라색
		if (rgbTemp.Red == 255 && rgbTemp.Green == 0 && rgbTemp.Blue == 255)
		{
			CFieldMonster *monster = new CFieldMonster;
			monster->SetType(MON_BOSS);
			monster->Begin();
			monster->SetNum(-1);
			monster->SetPos(posX, 0.0f, posZ);
			m_vMonster.push_back(monster);
		}
		//버프 몬스터1 : 적색
		else if (rgbTemp.Red == 255 && rgbTemp.Green == 0 && rgbTemp.Blue == 0)
		{
			CFieldMonster *monster = new CFieldMonster;
			monster->SetType(MON_BUFF1);
			monster->Begin();
			monster->SetNum(-1);
			monster->SetPos(posX, 0.0f, posZ);
			m_vMonster.push_back(monster);
		}
		//버프 몬스터2 : 청색
		else if (rgbTemp.Red == 0 && rgbTemp.Green == 0 && rgbTemp.Blue == 255)
		{
			CFieldMonster *monster = new CFieldMonster;
			monster->SetType(MON_BUFF2);
			monster->Begin();
			monster->SetNum(-1);
			monster->SetPos(posX, 0.0f, posZ);
			m_vMonster.push_back(monster);
		}
		//버프 몬스터1 부하: 녹색
		else if(rgbTemp.Red == 0 && rgbTemp.Green == 255 && rgbTemp.Blue == 0)
		{
			CFieldMonster *monster = new CFieldMonster;
			monster->SetType(MON_LBF1);
			monster->Begin();
			monster->SetNum(-1);
			monster->SetPos(posX, 0.0f, posZ);
			m_vMonster.push_back(monster);
		}
		//버프 몬스터2 부하 : 황색
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

		// 타일 중앙 좌표에서 일정하게 분포하도록, 뒤에 난수 인자를 두었다.
		posX = ((posX - 16) * 32) + ((rand() % 20) - 10);
		posZ = ((posZ - 16) * 32) + ((rand() % 20) - 10);

		monster->SetPos(posX, 0.0f, posZ);
		m_vMonster.push_back(monster);
	}

	for (int i = 0; i < 300; i++)
	{
		//번호 부여
		m_vMonster[i]->SetNum(i);
	}
	return true;
}


// 몬스터 위치 파일 데이터를 미리 읽어온다.
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

		//NPC : 적색
		if (rgbTemp.Red == 255 && rgbTemp.Green == 0 && rgbTemp.Blue == 0)
		{
			CFieldNpc *npc = new CFieldNpc;
			npc->SetType(NPC_RED);
			npc->Begin();
			npc->SetNum(-1);
			npc->SetPos(posX, 0.0f, posZ);
			m_vNpc.push_back(npc);
		}
		//NPC: 청색
		else if (rgbTemp.Red == 0 && rgbTemp.Green == 0 && rgbTemp.Blue == 255)
		{
			CFieldNpc *npc = new CFieldNpc;
			npc->SetType(NPC_BLUE);
			npc->Begin();
			npc->SetNum(-1);
			npc->SetPos(posX, 0.0f, posZ);
			m_vNpc.push_back(npc);
		}
		//버프 몬스터1 부하: 녹색
		else if (rgbTemp.Red == 0 && rgbTemp.Green == 255 && rgbTemp.Blue == 0)
		{
			CFieldNpc *npc = new CFieldNpc;
			npc->SetType(NPC_GREEN);
			npc->Begin();
			npc->SetNum(-1);
			npc->SetPos(posX, 0.0f, posZ);
			m_vNpc.push_back(npc);
		}
		//버프 몬스터2 부하 : 황색
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
		//번호 부여
		m_vNpc[i]->SetNum(i);
	}
	return true;
}


// 맵 오브젝트 위치 파일 데이터를 읽고 세팅한다.
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

		//장애물 : 라임색
		if (rgbTemp.Red == 0 && rgbTemp.Green == 255 && rgbTemp.Blue == 255)
		{
			CMapObject *obj = new CMapObject;
			obj->SetBox(0, 0, 1, CPoint3D(posX, posY, posZ), CPoint3D(32 / 2, 32 / 2, 32 / 2));
			// 충돌체크를 위해 트리구조에 추가하는 오브젝트
			m_Map1->AddObject(*obj);
			// 맵에 어떤 오브젝트들이 들어가있나 확인용
			m_Map1->v_AddObj.push_back(obj);
		}

		++BMPiter;
	}

	//주석 아래가 원본이었음. 문제 생기면 삭제 바람. hgi
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
			//printf("%d 읽음 : %d %d %f %f %f\n", retval, num, type, x, y, z);
			//CMapObject *obj = new CMapObject;
			//obj->SetBox(0, 0, type, CPoint3D(posX, posY, posZ), CPoint3D(sizeX, sizeY, sizeZ));
			// 충돌체크를 위해 트리구조에 추가하는 오브젝트
			//m_Map1->AddObject(*obj);
			// 맵에 어떤 오브젝트들이 들어가있나 확인용
			//m_Map1->v_AddObj.push_back(obj);
			//cout << "Obj pos : " << posX << ", " << posY << ", " << posZ << " 삽입 완료" << endl;
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
	cout << "이거 문제 되나 >" << endl;
	// 락
	COLOR_CS->EnterCS();
	if (user == NULL) return NULL;
	CField *Field = NULL;
	for (DWORD i = 0; i < m_vField.size(); ++i)
	{
		Field = m_vField[i];
		//Field->m_lock.lock();
		COLOR_CS->EnterCS();
		// 사람이 꽉 차있지 않고, 사람이 있고, 게임시작하지 않은 방
		// 지금은 게임 실행중에도 입장하도록 함 
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
	// 사람이 있는 방이 없거나 모두 꽉 찼을 경우
	// 빈방을 찾는다.
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
	// 락 해제
	//m_lock.unlock();
	COLOR_CS->LeaveCS();
	// 빈방이 없다.
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
//	// 락
//	m_lock.lock();
//	//COLOR_CS->EnterCS();
//	if (user == NULL) return NULL;
//	CLobby *Lobby = NULL;
//	for (DWORD i = 0; i < m_vLobby.size(); ++i)
//	{
//		Lobby->m_lock.lock();
//		//COLOR_CS->EnterCS();
//		// 사람이 꽉 차있지 않고, 사람이 있고, 게임시작하지 않은 방
//		// 지금은 게임 실행중에도 입장하도록 함 
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
//	// 사람이 있는 방이 없거나 모두 꽉 찼을 경우
//	// 빈방을 찾는다.
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
//	// 락 해제
//	m_lock.unlock();
//	//COLOR_CS->LeaveCS();
//	// 빈방이 없다.
//	return NULL;
//}
//CLobby* CFieldManager::getLobby(unsigned int num)
//{
//	std::vector<CLobby*>::iterator iter = m_vLobby.begin();
//	if (num >= m_vLobby.size()) return NULL;
//	iter += num;
//	return (*iter);
//}