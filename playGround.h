#pragma once
#include "gameNode.h"
#define PI 3.14f
#define PI2 PI * 2
#define PLAYERMAXSPEED 6.5f // 플레이어 최대 속력 
#define PLAYERSPEEDINCREASE 0.15f // 플레이어 이동시 증가되는 속도 값
#define AIR_RESISTANCE 0.08f // 공기 저항
#define BALLMAXSPEED 15 // 퍽의 최대 스피드
#define CENTERSTUCKINTERVAL 300 // 공이 정가운데 머무를 수 있는 최대 시간
#define GOALININTERVAL 150 // 골 넣고 나서 다시 공이 재생성되기까지의 시간

class playGround : public gameNode
{
	enum KeyKind
	{
		MoveUp, MoveLeft, MoveDown, MoveRight
	};

	struct GoalArea
	{
		float x, y;
		float radius;
	};
	struct Ball
	{
		float x, y;
		float speed;
		float radius;
		float angle;
		float distance;
		int centerStuckInterval;
		bool _centerStuckInterval;
		int goalInInterVal;
		bool _goalInInterVal;
	};
	struct Player
	{
		float x, y;
		float speed;
		float radius;
		float angle;
		float distance;
		int score;
		bool dir;
		KeyKind key;
		GoalArea goalArea;
	};
private:
	Player player1, player2;
	Ball ball;
	bool chooseBallDir;
	bool gameStart;

	image* player1_handle;
	image* player2_handle;
	image* ballImg;
	image* hockeyBG;
	image* hockeyLine;
	image* centerCircle;
	image* goalArea;

public:
	playGround();
	~playGround();

	virtual HRESULT init();			//초기화 전용 함수
	virtual void release();			//메모리 해제 함수
	virtual void update();			//연산 전용 함수
	virtual void render(HDC hdc);	//그리기 전용 함수

	void init_SetObjs();
	void update_Ball();
	void update_KeyManager();
	void update_PlayerMovement(Player* player);
	void render_RenderObjs(HDC backDC);
	void render_PlayGround(HDC backDC);
};