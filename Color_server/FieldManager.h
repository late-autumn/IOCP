#pragma once
#include "Define.h"
#include "Field.h"
//#include "Lobby.h"
#include "MapObject.h"
#include "BMPmap.h" //bmpmap 추가

class CFieldManager
{
	DWORD				m_maxFieldCount;     // 최대 방 수
	DWORD				m_currentFieldCount; // 유저가 접속된 방 수 ( 대기중인방 x )

public:
	cCriticalSection	m_Lock;
	mutex				m_lock;
	vector<CField*>     m_vField;
	//vector<CLobby*>		m_vLobby;
	// 맵 데이터
	CMapObject*         m_Map1;
	// 맵 별로 몬스터 정보를 저장해둘 벡터
	vector<CMonster*>	m_vMonster;
	//  맵 별로 NPC 정보를 저장해둘 벡터
	vector<CNpc*>		m_vNpc;
public:
	CFieldManager();
	~CFieldManager();

	bool Begin(DWORD _maxFieldCount);
	bool End();

	// 서버 실행시 미리 읽어둔다.
	bool MonsterFileLoadAndSet(char *buf);	// 맵 번호에 따른 몬스터 위치정보 파일로드
	bool MapFileLoadAndSet(char *buf);	// 맵 생성 파일로드
	bool NpcFileLoadAndSet(char *buf);	// NPC 생성 파일 로드
	int  GetCurrentFieldCount(){ return m_currentFieldCount; };		// 현재 방 갯수 얻기

	CField* QuickJoin(CUser* user);     // 방 입장
	CField* GetField(unsigned int num);		// 방 얻기
//	CLobby* QuickJoinLobby(CUser* user);	// 로비 입장
//	CLobby* getLobby(unsigned int num);     // 로비 얻기 
};

