#define WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once

#define MAX_ROOM			1		// �ִ� �� ����
#define MAX_USER			MAX_ROOM*100  // �ִ� �����ο� ���� 

#define ROOM_NUM_START      MAX_USER

#define LOGIN_FAIL			-1   // class CUser�� m_num�� ����(�ʱ� ���� �� UserManager �߰� ���н�)

// NON = �ƹ��͵� �ƴ�, DEAD = ����, NORMAL = ��ȭ, VIGILANCE = ���(�ֺ��� ���� �ν�), COMBAT = ������, FLEE = ������, ATK = ����
enum Monster_State
{
	MON_NON,
	MON_DEAD,
	MON_NORMAL,
	MON_VIGILANCE,
	MON_COMBAT,
	MON_ATK
};
enum Monster_Type
{
	MON_BOSS,
	MON_BUFF1,
	MON_BUFF2,
	MON_LBF1,
	MON_LBF2,
	MON_BASE
};
enum Npc_State
{
	NPC_NON,
	NPC_DEAD,
	NPC_NORMAL,
	NPC_COMBAT
};
enum Npc_Type
{
	NPC_RED,
	NPC_BLUE,
	NPC_GREEN,
	NPC_YELLOW
};
enum Player_State
{
	PSTATE_IDLE = 0,
	PSTATE_WALK,
	PSTATE_ATTACKSKILL,
	PSTATE_STUN,
	PSTATE_BLIND,
	PSTATE_DEAD,
	PSTATE_END
};
enum User_State
{
	USER_LOGIN, // ������ ���»��� (�κ� ���)
	USER_LOBBY,// ������ �κ��(��������x)
	USER_FIELD, // ������ ������
	FIELD_END  // ���� üũ��
};
enum Game_state
{
	LOBBY_WAIT, // ������ ���� ���� (�κ���)
	GAME_WAIT, // ������ �κ� ������ ���� ( ���� ���� x ) ��� ���� 
	GAME_PLAY, // ������
	GAME_PLAYEND, // ���� ���� ( �÷��� ���� �� �κ�� �̵��ϱ� ����)
	GAME_END  // ���� üũ�� (������ ���� ����)
};

enum UserSkillType			// ���� ��ų Ÿ��
{ 
	USERSKILLTYPE_SMASH,	// ��Ÿ : ĳ����1 ��ų1
	USERSKILLTYPE_BASH,		// ���� : ĳ����1 ��ų2
	USERSKILLTYPE_B1,		// ĳ����2 ��ų1
	USERSKILLTYPE_B2,		// ĳ����2 ��ų2
	USERSKILLTYPE_HEALING,	// �� : ĳ����3 ��ų1
	USERSKILLTYPE_BLOOD,	// ���� : ĳ����3 ��ų2
	USERSKILLTYPE_C1
};

enum MonsterSkillType		// ���� ��ų Ÿ��
{	
	MONSTERSKILLTYPE_STUN, 	// �Ͻ� ����
	MONSTERSKILLTYPE_BLIND, // ����ε�
	MONSTERSKILLTYPE_SLOW	// ���ο�
};

// Ŭ���̾�Ʈ -> ����
enum CS_Packet_Type  		// Ŭ���̾�Ʈ ��ŶŸ��
{	
	CS_DEFAULT_PACKET,		// ����Ʈ
	CS_LOGIN, 				// �α���
	CS_LOBBYJOIN,			// �κ� �� ����
	CS_LOGOUT,				// �κ� ��������
	CS_CHARACTERSELECT, 	// �� ���� ĳ���� ����
	CS_READY,				// �� ���� �����غ�
	CS_OUT, 				// �濡�� ������(�κ��̵�)
	CS_MOVE,				// �̵�
	CS_ATTACK,				// ����
	CS_SKILL, 				// ��ų
	CS_COLOR,				// ����
	CS_BUFF					// ����
};

// ���� -> Ŭ���̾�Ʈ
enum SC_Packet_Type  		// ���� ��ŶŸ��
{	
	SC_DEFAULT_PACKET,		// ����Ʈ
	SC_LOGIN_OK, 			// �α��� ����
	SC_LOGIN_FAIL, 			// �α��� ����
	SC_LOBBYJOIN_OK, 		// �κ� �� ���� ����
	SC_LOBBYJOIN_FAIL, 		// �κ� �� ���� ����
	SC_CHARACTERSELECT, 	// �� ���� ĳ���� ����
	SC_READY, 				// �� ���� �غ�
	SC_START, 				// �� ���� ���ӽ���
	SC_USER_HIT,			// ������ �ǰ�
	SC_MONSTER, 			// ����
	SC_MONSTER_ATK,			// ���Ͱ� ������ ���� 
	SC_MONSTER_HIT,			// ���Ͱ� �ǰ� ������ ��
	SC_POS,                 // �÷��̾� ��ġ, ȸ��
	SC_PUT_PLAYER,          // �ٸ� �÷��̾� �߰�
	SC_ATK,					// ���� ���
	SC_SKILL,				// ��ų ���
	SC_REMOVE_PLAYER, 		// �ٸ� �÷��̾� ����
	SC_COLOR_CHANGE,		// ���� �� ����
	SC_EXP,					// ����ġ 
	SC_LVUP,				// ������
	SC_WIN, 				// �÷��̾� �¸�
	SC_LOSE,				// �÷��̾� �й�
	SC_TIME,				// �ð�
	SC_DEAD,				// ���
	SC_BUFF,				// ���� 
	SC_REMOVE_BUFF,			// ���� ����
	SC_REVIVE,				// ��Ȱ
	SC_MONSTER_SKILL,		// ���� ��ų
	SC_NPC,					// NPC
	SC_HIT,					// ����Ÿ��
	SC_DEBUFF,				// �����̻�
	SC_VIEW
};


#pragma pack (push, 1)
// Ŭ���̾�Ʈ -> ����
struct cs_login // �α���
{
	BYTE size;
	BYTE type;
	char LoginID[20];
};
struct cs_lobbyjoin // �κ� �� ����
{
	BYTE size;
	BYTE type;
	char LoginID[20];
	BYTE num;

};

struct cs_logout // �κ� ��������
{
	BYTE size;
	BYTE type;
};

struct cs_characterselect // ĳ���� ����
{
	BYTE size;
	BYTE type;
	int num;
	char LoginID[20];
	WORD color;
	WORD character;
};

struct cs_ready // �غ�
{
	BYTE size;
	BYTE type;
};

struct cs_out // �� ������
{
	BYTE size;
	BYTE type;
};

struct cs_move // �̵�
{
	BYTE size;
	BYTE type;
	float posx, posy, posz;
	float dirx, diry, dirz;	// �÷��̾ ���� ����
	float rotx, roty, rotz;	// �÷��̾��� ȸ�� ����
};

struct cs_attack // ����
{
	BYTE size;
	BYTE type;
	float x, y, z; // �÷��̾� �����ִ� ����
};

struct cs_skill // ��ų
{
	BYTE size;
	BYTE type;
	BYTE skilltype;
	BYTE skillslot;
	float dirX, dirY, dirZ; // �÷��̾� �����ִ� ����
};

struct cs_buff
{
	BYTE size;
	BYTE type;
};
struct cs_color
{
	BYTE size;
	BYTE type;
	float x, y, z;
	int num;
	int color;
};

// ���� -> Ŭ���̾�Ʈ
struct sc_login // �α��� ����, ����
{
	BYTE size;
	BYTE type;
	int  num;   // �α��� ������ ���ӹ�ȣ // ���н� LOGIN_FAIL (-1)
};

struct sc_fieldcreate // �� ���� ����, ����
{
	BYTE size;
	BYTE type;
};

struct sc_lobbyjoin // �� ���� ����, ����
{
	BYTE size;
	BYTE type;
	int num;
};

struct sc_characterselect // ĳ���� ����
{
	BYTE size;
	BYTE type;
	int num;
	WORD color;
	WORD character;
};
struct sc_start // ���� ����
{
	BYTE size;
	BYTE type;
};

struct sc_monster // ����
{
	BYTE size;
	BYTE type;
	int num;
	int montype;
	int state;
	float posX;
	float posY;
	float posZ;
	float rotX;
	float rotY;
	float rotZ;
};
struct sc_monster_atk  // ���� ����
{
	BYTE size;
	BYTE type;
	int target;
	int hp;
	int maxhp;
};

struct sc_monster_hit // ���Ͱ� ���ݴ����� �� SC_MONSTER_HIT
{
	BYTE size;
	BYTE type;
	int num;
	int target;
	int state;
	int hp;
	int maxHp;
	float posX;
	float posZ;
};

struct sc_user_hit
{
	BYTE size;
	BYTE type;
	int num;
	int target;
	int state;
	int hp;
	int maxHp;
};
struct sc_player_skill_atk
{
	BYTE size;
	BYTE type;
	BYTE skilltype;
	float dirX, dirY, dirZ; // �÷��̾� �����ִ� ����
};
struct sc_exp // ����ġ 
{
	BYTE size;
	BYTE type;
	int num;
	int curExp;
	int maxExp;
};
struct sc_pos // �÷��̾� ��ȣ, ��ġ, ȸ��
{
	BYTE size;
	BYTE type;
	int  num;
	WORD  state;
	WORD  character;
	float posX;
	float posY;
	float posZ;
	float rotX;
	float rotY;
	float rotZ;
};
struct sc_player_atk
{
	BYTE size;
	BYTE type;
	int num;
};
struct sc_player_view 
{
	BYTE size;
	BYTE type;
	WORD id;
	float x;
	float y;
	float z;
};

struct sc_color_change // ���� 
{
	BYTE size;
	BYTE type;
	int num;
	int color;
};

struct sc_put_player
{
	BYTE size;
	BYTE type;
	int id;
	float x;
	float y;
	float z;
	float dirX;
	float dirY;
	float dirZ;
};

struct sc_player_disconnect // �÷��̾� ��ȣ ����� �ٸ� Ŭ���̾�Ʈ ���ſ�
{
	BYTE size;
	BYTE type;
	int  num;
};

struct sc_gameset // ���� �� ( �¸� or �й� )
{
	BYTE size;
	BYTE type;
};

struct sc_player_dead
{
	BYTE			size;
	BYTE			type;
	int				num;
};

struct sc_player_revive
{
	BYTE			size;
	BYTE			type;
	int				num;
	float x, y, z; // �÷��̾� ��ġ

};
struct sc_put_buff
{
	BYTE size;
	BYTE type;
	BYTE num;
	float x;
	float y;
	float z;
};
struct sc_remove_buff
{
	BYTE size;
	BYTE type;
	BYTE num;
};
struct sc_time
{
	BYTE size;
	BYTE type;
	double Start_time;
	double Max_time;
	double Cur_time;
};
struct sc_npc // ����
{
	BYTE size;
	BYTE type;
	int num;
	int npctype;
	int state;
	float posX;
	float posY;
	float posZ;
	float rotX;
	float rotY;
	float rotZ;
};

struct sc_hit	// ���� Ÿ��
{
	BYTE size;
	BYTE type;
	BYTE action_type;	// �ൿ Ÿ�� : �Ϲ� ����, ��ų
	BYTE skill_type;	// ��ų Ÿ�� : ��ų�� �ƴ� ��쿡�� -1
	BYTE attacker_type;	// ������ Ÿ�� : �÷��̾�, ����
	BYTE target_type;	// �ǰ��� Ÿ�� : �÷��̾�, ����
	int attacker_num;	// ������ ���� ��ȣ
	int target_num;		// �ǰ��� ���� ��ȣ
	int curhp;			// �ǰ��� ���� ü��
	int maxHp;			// �ǰ��� �ִ� ü��
	float posX;			// Ÿ���� �߻��� ��ǥ X��
	float posZ;			// Ÿ���� �߻��� ��ǥ Z��
};
struct sc_dead
{
	BYTE	size;
	BYTE	type;
	BYTE	target_type;	// �ǰ��� Ÿ�� : �÷��̾�, ����
	int		num;			// �ǰ��� ���� ��ȣ
};
struct sc_revive
{
	BYTE	size;
	BYTE	type;
	BYTE	target_type;	// ��Ȱ ��ü Ÿ�� : �÷��̾�, ����
	int		num;			// ��Ȱ ��ü ���� ��ȣ
	float	x;
	float	y;
	float	z;
};
struct sc_Lvup
{
	BYTE	size;
	BYTE	type;
	int     m_Lv;
	int		m_atk;
	int		m_atksp;
	int		m_maxHp;
	int		m_curHp;
	int		m_curExp;
	int		m_maxExp;
};
struct sc_debuff
{
	BYTE size;
	BYTE type;
	BYTE target_type;
	int num;
};
#pragma pack (pop)