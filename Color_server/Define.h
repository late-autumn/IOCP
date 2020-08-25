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

// �̱��� ��ũ��
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
	SKILL_SLOT_1,	// ��ų ���� 1��
	SKILL_SLOT_2	// ��ų ���� 2��
};

// ��ų�� ������ ��Ÿ����.
enum SkillType
{
	SKILL_TYPE_ATTACK,	// ��븦 �����ϴ� ��ų
	SKILL_TYPE_HELPING	// ��뿡�� ������ �ִ� ��ų
};

// ��ų�� ��� ������ ��Ÿ����.
enum SkillTargetType
{
	SKILL_TARGET_FOE,	// ���� ���̴�.
	SKILL_TARGET_FRIEND	// ���� �Ʊ��̴�.
};

enum Color
{
	COLOR_GRAY,		// ���Ҽ�
	COLOR_RED,		// ���� �Ҽ�
	COLOR_BLUE,		// û�� �Ҽ�
	COLOR_GREEN,	// ��� �Ҽ�
	COLOR_YELLOW	// Ȳ�� �Ҽ�
};

enum BuffType
{
	BUFF_TYPE_NONE,			// ���� ����
	BUFF_TYPE_RECOVER_HP,	// ü�� ȸ�� ����
	BUFF_TYPE_ATK_POWER_UP	// ���ݷ� ���� ����
};

enum HitType
{
	HIT_TYPE_MONSTER,		// HIT ��Ŷ Ÿ�� ����
	HIT_TYPE_PLAYER			// HIT ��Ŷ Ÿ�� �÷��̾�
};

enum ActionType
{
	ACTION_BASIC_ATTACK,	// HIT ��Ŷ �ൿ ��Ÿ
	ACTION_SKILL			// HIT ��Ŷ �ൿ ��ų
};

enum DeadType
{
	DEAD_MONSTER,			// Dead ����
	DEAD_PLAYER				// Dead �÷��̾�
};

enum ReviveType
{
	REVIVE_MONSTER,			// Revive ����
	REVIVE_PLAYER			// Revive �÷��̾�
};

struct stOverlapped
{
	WSAOVERLAPPED		m_wsaOverlapped;
	WSABUF				m_wsaBuf;
	char				m_IocpBuf[MAX_BUF_SIZE];		// IOCP send/recv ����
	char				m_packetBuf[MAX_BUF_SIZE];		// recv�Ǵ� ��Ŷ�� �����Ǵ� ����
	enumType			m_eType;						// TYPE_RECV / TYPE_SEND / TYPE_ROOM
	int					m_nRemainLen;					// �̿Ϸ�� ��Ŷũ�⸦ ����
	int					recv_packet_size;			    // ���� �������� ��Ŷ�� ũ��
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
