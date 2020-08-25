#pragma once
#include "Define.h"
#include "Field.h"
//#include "Lobby.h"
#include "MapObject.h"
#include "BMPmap.h" //bmpmap �߰�

class CFieldManager
{
	DWORD				m_maxFieldCount;     // �ִ� �� ��
	DWORD				m_currentFieldCount; // ������ ���ӵ� �� �� ( ������ι� x )

public:
	cCriticalSection	m_Lock;
	mutex				m_lock;
	vector<CField*>     m_vField;
	//vector<CLobby*>		m_vLobby;
	// �� ������
	CMapObject*         m_Map1;
	// �� ���� ���� ������ �����ص� ����
	vector<CMonster*>	m_vMonster;
	//  �� ���� NPC ������ �����ص� ����
	vector<CNpc*>		m_vNpc;
public:
	CFieldManager();
	~CFieldManager();

	bool Begin(DWORD _maxFieldCount);
	bool End();

	// ���� ����� �̸� �о�д�.
	bool MonsterFileLoadAndSet(char *buf);	// �� ��ȣ�� ���� ���� ��ġ���� ���Ϸε�
	bool MapFileLoadAndSet(char *buf);	// �� ���� ���Ϸε�
	bool NpcFileLoadAndSet(char *buf);	// NPC ���� ���� �ε�
	int  GetCurrentFieldCount(){ return m_currentFieldCount; };		// ���� �� ���� ���

	CField* QuickJoin(CUser* user);     // �� ����
	CField* GetField(unsigned int num);		// �� ���
//	CLobby* QuickJoinLobby(CUser* user);	// �κ� ����
//	CLobby* getLobby(unsigned int num);     // �κ� ��� 
};

