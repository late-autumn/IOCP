#pragma once
#include "Define.h"
#include "Buff.h"
#include "Object.h"
#include "Skill.h"
class CLobby;
class CField;
class CSkill;

class CUser : public PlayerObject
{
	CLobby*				m_pLobby;
	CField*             m_pField;
	CSkill*				m_pSkill[2];

public:
	bool				m_Login;
	SOCKET				m_socket;
	stOverlapped		m_stRecvOverlapped;		// Recv Overlapped I/O 작업을 위한 변수

	// 로비정보
	char				m_id[20];				// 아이디
	WORD				m_FromState;			// 현재접속위치(로비, 방, 게임중)
	DWORD				m_FieldNumber;			// 방번호

	//게임정보
	WORD				m_GameState;
	WORD				m_selectCharacter;		// 선택한 케릭터
	WORD				m_characterState;		// 케릭터 상태(상태이상 유무)  
	DWORD               m_stateTimer = 0;
	WORD				m_color;
	CUser*              user_target;
	CBuff               m_buff;
	
	set<int>			view_list;
	bool				in_use;

public:
	CUser();
	~CUser();

	void				Begin(int _num);
	void				End();

	void				GameInit();
	
	SOCKET				GetSocket()						{ return m_socket; };
	void				SetSocket(SOCKET _socket)		{ m_socket = _socket; };

	void				SetLobby(CLobby *lobby)			{ m_pLobby = lobby; };
	CLobby*				GetLobby()						{ return m_pLobby; };

	void				SetField(CField *field)			{ m_pField = field; };
	CField*				GetField()						{ return m_pField; };

	void				SetSkill(int num, CSkill *skill){ m_pSkill[num] = skill; };
	CSkill*				GetSkill(int num)				{ return m_pSkill[num]; };

	void				SendPacketToThisUser(char *buf);
	void				SendPacket(char *buf);
};

