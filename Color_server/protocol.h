#define WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once

#define MAX_ROOM			1		// 최대 방 제한
#define MAX_USER			MAX_ROOM*100  // 최대 접속인원 제한 

#define ROOM_NUM_START      MAX_USER

#define LOGIN_FAIL			-1   // class CUser의 m_num에 쓰임(초기 생성 및 UserManager 추가 실패시)

// NON = 아무것도 아님, DEAD = 죽음, NORMAL = 평화, VIGILANCE = 경계(주변에 유저 인식), COMBAT = 전투중, FLEE = 도주중, ATK = 공격
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
	USER_LOGIN, // 유저가 없는상태 (로비 대기)
	USER_LOBBY,// 유저가 로비로(게임진행x)
	USER_FIELD, // 유저가 게임중
	FIELD_END  // 상태 체크용
};
enum Game_state
{
	LOBBY_WAIT, // 유저가 없는 상태 (로비대기)
	GAME_WAIT, // 유저가 로비 접속한 상태 ( 게임 진행 x ) 대기 상태 
	GAME_PLAY, // 게임중
	GAME_PLAYEND, // 게임 종료 ( 플레이 종료 후 로비로 이동하기 위함)
	GAME_END  // 상태 체크용 (추후의 완전 종료)
};

enum UserSkillType			// 유저 스킬 타입
{ 
	USERSKILLTYPE_SMASH,	// 강타 : 캐릭터1 스킬1
	USERSKILLTYPE_BASH,		// 범위 : 캐릭터1 스킬2
	USERSKILLTYPE_B1,		// 캐릭터2 스킬1
	USERSKILLTYPE_B2,		// 캐릭터2 스킬2
	USERSKILLTYPE_HEALING,	// 힐 : 캐릭터3 스킬1
	USERSKILLTYPE_BLOOD,	// 흡혈 : 캐릭터3 스킬2
	USERSKILLTYPE_C1
};

enum MonsterSkillType		// 몬스터 스킬 타입
{	
	MONSTERSKILLTYPE_STUN, 	// 일시 정지
	MONSTERSKILLTYPE_BLIND, // 블라인드
	MONSTERSKILLTYPE_SLOW	// 슬로우
};

// 클라이언트 -> 서버
enum CS_Packet_Type  		// 클라이언트 패킷타입
{	
	CS_DEFAULT_PACKET,		// 디폴트
	CS_LOGIN, 				// 로그인
	CS_LOBBYJOIN,			// 로비 방 참여
	CS_LOGOUT,				// 로비 게임종료
	CS_CHARACTERSELECT, 	// 방 내부 캐릭터 선택
	CS_READY,				// 방 내부 게임준비
	CS_OUT, 				// 방에서 나가기(로비이동)
	CS_MOVE,				// 이동
	CS_ATTACK,				// 공격
	CS_SKILL, 				// 스킬
	CS_COLOR,				// 점령
	CS_BUFF					// 버프
};

// 서버 -> 클라이언트
enum SC_Packet_Type  		// 서버 패킷타입
{	
	SC_DEFAULT_PACKET,		// 디폴트
	SC_LOGIN_OK, 			// 로그인 성공
	SC_LOGIN_FAIL, 			// 로그인 실패
	SC_LOBBYJOIN_OK, 		// 로비 방 참여 성공
	SC_LOBBYJOIN_FAIL, 		// 로비 방 참여 실패
	SC_CHARACTERSELECT, 	// 방 내부 캐릭터 선택
	SC_READY, 				// 방 내부 준비
	SC_START, 				// 방 내부 게임시작
	SC_USER_HIT,			// 유저가 피격
	SC_MONSTER, 			// 몬스터
	SC_MONSTER_ATK,			// 몬스터가 유저를 공격 
	SC_MONSTER_HIT,			// 몬스터가 피격 당했을 시
	SC_POS,                 // 플레이어 위치, 회전
	SC_PUT_PLAYER,          // 다른 플레이어 추가
	SC_ATK,					// 공격 모션
	SC_SKILL,				// 스킬 모션
	SC_REMOVE_PLAYER, 		// 다른 플레이어 제거
	SC_COLOR_CHANGE,		// 진영 색 변경
	SC_EXP,					// 경험치 
	SC_LVUP,				// 레벨업
	SC_WIN, 				// 플레이어 승리
	SC_LOSE,				// 플레이어 패배
	SC_TIME,				// 시간
	SC_DEAD,				// 사망
	SC_BUFF,				// 버프 
	SC_REMOVE_BUFF,			// 버프 제거
	SC_REVIVE,				// 부활
	SC_MONSTER_SKILL,		// 몬스터 스킬
	SC_NPC,					// NPC
	SC_HIT,					// 공용타격
	SC_DEBUFF,				// 상태이상
	SC_VIEW
};


#pragma pack (push, 1)
// 클라이언트 -> 서버
struct cs_login // 로그인
{
	BYTE size;
	BYTE type;
	char LoginID[20];
};
struct cs_lobbyjoin // 로비 방 참여
{
	BYTE size;
	BYTE type;
	char LoginID[20];
	BYTE num;

};

struct cs_logout // 로비 게임종료
{
	BYTE size;
	BYTE type;
};

struct cs_characterselect // 캐릭터 선택
{
	BYTE size;
	BYTE type;
	int num;
	char LoginID[20];
	WORD color;
	WORD character;
};

struct cs_ready // 준비
{
	BYTE size;
	BYTE type;
};

struct cs_out // 방 나가기
{
	BYTE size;
	BYTE type;
};

struct cs_move // 이동
{
	BYTE size;
	BYTE type;
	float posx, posy, posz;
	float dirx, diry, dirz;	// 플레이어가 보는 방향
	float rotx, roty, rotz;	// 플레이어의 회전 각도
};

struct cs_attack // 공격
{
	BYTE size;
	BYTE type;
	float x, y, z; // 플레이어 보고있는 방향
};

struct cs_skill // 스킬
{
	BYTE size;
	BYTE type;
	BYTE skilltype;
	BYTE skillslot;
	float dirX, dirY, dirZ; // 플레이어 보고있는 방향
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

// 서버 -> 클라이언트
struct sc_login // 로그인 성공, 실패
{
	BYTE size;
	BYTE type;
	int  num;   // 로그인 성공시 접속번호 // 실패시 LOGIN_FAIL (-1)
};

struct sc_fieldcreate // 방 생성 성공, 실패
{
	BYTE size;
	BYTE type;
};

struct sc_lobbyjoin // 방 참여 성공, 실패
{
	BYTE size;
	BYTE type;
	int num;
};

struct sc_characterselect // 캐릭터 선택
{
	BYTE size;
	BYTE type;
	int num;
	WORD color;
	WORD character;
};
struct sc_start // 게임 시작
{
	BYTE size;
	BYTE type;
};

struct sc_monster // 몬스터
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
struct sc_monster_atk  // 몬스터 공격
{
	BYTE size;
	BYTE type;
	int target;
	int hp;
	int maxhp;
};

struct sc_monster_hit // 몬스터가 공격당했을 시 SC_MONSTER_HIT
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
	float dirX, dirY, dirZ; // 플레이어 보고있는 방향
};
struct sc_exp // 경험치 
{
	BYTE size;
	BYTE type;
	int num;
	int curExp;
	int maxExp;
};
struct sc_pos // 플레이어 번호, 위치, 회전
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

struct sc_color_change // 영역 
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

struct sc_player_disconnect // 플레이어 번호 종료된 다른 클라이언트 제거용
{
	BYTE size;
	BYTE type;
	int  num;
};

struct sc_gameset // 게임 끝 ( 승리 or 패배 )
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
	float x, y, z; // 플레이어 위치

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
struct sc_npc // 몬스터
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

struct sc_hit	// 공용 타격
{
	BYTE size;
	BYTE type;
	BYTE action_type;	// 행동 타입 : 일반 공격, 스킬
	BYTE skill_type;	// 스킬 타입 : 스킬이 아닐 경우에는 -1
	BYTE attacker_type;	// 공격자 타입 : 플레이어, 몬스터
	BYTE target_type;	// 피격자 타입 : 플레이어, 몬스터
	int attacker_num;	// 공격자 서버 번호
	int target_num;		// 피격자 서버 번호
	int curhp;			// 피격자 현재 체력
	int maxHp;			// 피격자 최대 체력
	float posX;			// 타격이 발생한 좌표 X축
	float posZ;			// 타격이 발생한 좌표 Z축
};
struct sc_dead
{
	BYTE	size;
	BYTE	type;
	BYTE	target_type;	// 피격자 타입 : 플레이어, 몬스터
	int		num;			// 피격자 서버 번호
};
struct sc_revive
{
	BYTE	size;
	BYTE	type;
	BYTE	target_type;	// 부활 객체 타입 : 플레이어, 몬스터
	int		num;			// 부활 객체 서버 번호
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