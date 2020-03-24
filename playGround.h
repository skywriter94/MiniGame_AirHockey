#pragma once
#include "gameNode.h"
#define PI 3.14f
#define PI2 PI * 2
#define PLAYERMAXSPEED 6.5f // �÷��̾� �ִ� �ӷ� 
#define PLAYERSPEEDINCREASE 0.15f // �÷��̾� �̵��� �����Ǵ� �ӵ� ��
#define AIR_RESISTANCE 0.08f // ���� ����
#define BALLMAXSPEED 15 // ���� �ִ� ���ǵ�
#define CENTERSTUCKINTERVAL 300 // ���� ����� �ӹ��� �� �ִ� �ִ� �ð�
#define GOALININTERVAL 150 // �� �ְ� ���� �ٽ� ���� ������Ǳ������ �ð�

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

	virtual HRESULT init();			//�ʱ�ȭ ���� �Լ�
	virtual void release();			//�޸� ���� �Լ�
	virtual void update();			//���� ���� �Լ�
	virtual void render(HDC hdc);	//�׸��� ���� �Լ�

	void init_SetObjs();
	void update_Ball();
	void update_KeyManager();
	void update_PlayerMovement(Player* player);
	void render_RenderObjs(HDC backDC);
	void render_PlayGround(HDC backDC);
};