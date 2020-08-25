#define WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <set>
#include <time.h>
#include <map>
#include <mutex>
#include <sys/timeb.h>
#include <chrono>
#include "CriticalSection.h"
#include "protocol.h"

// 싱글톤 매크로
class CIocp;
#define SERVER CIocp::GetInst()
#define STATE CIocp::GetInst()->m_StateManager
#define COLOR_CS cCriticalSection::GetInst()
///////////////

#define MAX_BUF_SIZE		512  
using namespace std;

enum enumType
{
	TYPE_RECV,
	TYPE_SEND,
	TYPE_ROOM
};

enum SkillSlotnum
{
	SKILL_SLOT_1,	// 스킬 슬롯 1번
	SKILL_SLOT_2	// 스킬 슬롯 2번
};

// 스킬의 종류를 나타낸다.
enum SkillType
{
	SKILL_TYPE_ATTACK,	// 상대를 공격하는 스킬
	SKILL_TYPE_HELPING	// 상대에게 도움을 주는 스킬
};

// 스킬의 대상 종류를 나타낸다.
enum SkillTargetType
{
	SKILL_TARGET_FOE,	// 상대는 적이다.
	SKILL_TARGET_FRIEND	// 상대는 아군이다.
};

enum Color
{
	COLOR_GRAY,		// 무소속
	COLOR_RED,		// 적색 소속
	COLOR_BLUE,		// 청색 소속
	COLOR_GREEN,	// 녹색 소속
	COLOR_YELLOW	// 황색 소속
};

enum BuffType
{
	BUFF_TYPE_NONE,			// 버프 없음
	BUFF_TYPE_RECOVER_HP,	// 체력 회복 버프
	BUFF_TYPE_ATK_POWER_UP	// 공격력 증가 버프
};

enum HitType
{
	HIT_TYPE_MONSTER,		// HIT 패킷 타입 몬스터
	HIT_TYPE_PLAYER			// HIT 패킷 타입 플레이어
};

enum ActionType
{
	ACTION_BASIC_ATTACK,	// HIT 패킷 행동 평타
	ACTION_SKILL			// HIT 패킷 행동 스킬
};

enum DeadType
{
	DEAD_MONSTER,			// Dead 몬스터
	DEAD_PLAYER				// Dead 플레이어
};

enum ReviveType
{
	REVIVE_MONSTER,			// Revive 몬스터
	REVIVE_PLAYER			// Revive 플레이어
};

struct stOverlapped
{
	WSAOVERLAPPED		m_wsaOverlapped;
	WSABUF				m_wsaBuf;
	char				m_IocpBuf[MAX_BUF_SIZE];		// IOCP send/recv 버퍼
	char				m_packetBuf[MAX_BUF_SIZE];		// recv되는 패킷이 조립되는 버퍼
	enumType			m_eType;						// TYPE_RECV / TYPE_SEND / TYPE_ROOM
	int					m_nRemainLen;					// 미완료된 패킷크기를 저장
	int					recv_packet_size;			    // 지금 조립중인 패킷의 크기
};

class CPoint3D
{
public:
	float x;
	float y;
	float z;
public:
	CPoint3D(){};
	CPoint3D(float x1, float y1, float z1){ x = x1; y = y1; z = z1; };
	~CPoint3D(){};

	void SetXYZ(float x1, float y1, float z1){ x = x1; y = y1; z = z1; };
	void GetXYZ(float &out_x, float &out_y, float &out_z){ out_x = x; out_y = y; out_z = z; };
	float GetX(){ return x; };
	float GetY(){ return y; };
	float GetZ(){ return z; };

	void AddXYZ(float x1, float y1, float z1){ x += x1; y += y1; z += z1; };

	float Length(){ return sqrt(x*x + y*y + z*z); };
	CPoint3D operator - (const CPoint3D& p){
		CPoint3D temp(x - p.x, y - p.y, z - p.z);
		return temp;
	};
};

static float Point3Dlength(CPoint3D *p)
{
	return p->Length();
};

static bool In_range(float _x1, float _z1, float _x2, float _z2, float _radius)
{
	float x_dist;
	float z_dist;

	x_dist = _x1 - _x2;
	x_dist = x_dist * x_dist;
	z_dist = _z1 - _z2;
	z_dist = z_dist * z_dist;


	if ((x_dist + z_dist) < _radius * _radius) return true;
	else return false;
};
