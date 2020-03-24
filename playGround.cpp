#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}
playGround::~playGround()
{
}

HRESULT playGround::init()
{
	gameNode::init();
	init_SetObjs();
	return S_OK;
}

void playGround::release()
{
	gameNode::release();
}


void playGround::update()
{
	gameNode::update();
	if (gameStart)
	{
		update_Ball();
		update_KeyManager();
	}
}


void playGround::render(HDC hdc)
{
	HDC backDC = this->getBackBuffer()->getMemDC();
	PatBlt(backDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//==================================================
	render_PlayGround(backDC);
	render_RenderObjs(backDC);
	//===================================================
	this->getBackBuffer()->render(hdc, 0, 0);
}


void playGround::init_SetObjs()
{
	srand(time(NULL));
	player1.x = WINSIZEX / 4, player1.y = WINSIZEY / 2;
	player1.radius = 35;
	player1.score = 0;
	player1.speed = 0;
	player1.dir = false;
	player1.goalArea.x = 0, player1.goalArea.y = WINSIZEY / 2;
	player1.goalArea.radius = 50;

	player2.x = WINSIZEX / 1.33, player2.y = WINSIZEY / 2;
	player2.radius = 35;
	player2.score = 0;
	player2.speed = 0;
	player2.dir = true;
	player2.goalArea.x = WINSIZEX, player2.goalArea.y = WINSIZEY / 2;
	player2.goalArea.radius = 50;

	ball.x = WINSIZEX / 2, ball.y = WINSIZEY / 2;
	ball.radius = 20;
	ball.speed = 6;

	chooseBallDir = rand() % 2;
	gameStart = true;

	player1_handle = new image;
	player1_handle->init("p1_handle.bmp", player1.radius * 2, player1.radius * 2, true, RGB(255, 0, 255));

	player2_handle = new image;
	player2_handle->init("p2_handle.bmp", player2.radius * 2, player2.radius * 2, true, RGB(255, 0, 255));

	ballImg = new image;
	ballImg->init("puck.bmp", ball.radius * 2, ball.radius * 2, true, RGB(255, 0, 255));
	
	hockeyBG = new image;
	hockeyBG->init("hockeyBG.bmp", WINSIZEX, WINSIZEY, false, RGB(0, 0, 0));

	hockeyLine = new image;
	hockeyLine->init("hockeyLine.bmp", 3, 768, false, RGB(0, 0, 0));

	centerCircle = new image;
	centerCircle->init("CenterCircle.bmp", 150, 150, true, RGB(255, 0, 255));

	goalArea = new image;
	goalArea->init("GoalArea.bmp", 100, 200, true, RGB(255, 0, 255));

	if (chooseBallDir) ball.angle = PI;
	else ball.angle = PI2;
}


void playGround::update_Ball()
{
	// 공 이동
	ball.x += cosf(ball.angle) * ball.speed;
	ball.y -= sinf(ball.angle) * ball.speed;

	// 공 속도 감속
	if (ball.speed > 0)
	{
		ball.speed -= AIR_RESISTANCE;
		if (ball.speed < 0) ball.speed = 0;
	}

	// 플레이어 속도 감속
	if (player1.speed > 0)
	{
		player1.speed -= AIR_RESISTANCE;
		if (player1.speed < 0) player1.speed = 0;
	}
	if (player2.speed > 0)
	{
		player2.speed -= AIR_RESISTANCE;
		if (player2.speed < 0) player2.speed = 0;
	}

	// 공 반사
	if (ball.radius + player1.radius > getDistance(ball.x, ball.y, player1.x, player1.y))
	{
		ball.angle = getAngle(player1.x, player1.y, ball.x, ball.y);
		if (ball.speed < BALLMAXSPEED) ball.speed += (player1.speed * 0.75);
		if (ball.x <= ball.radius * 2 || ball.radius * 2 > ball.y || ball.y >= WINSIZEY - (ball.radius * 2)) ball.speed /= 2.25f;
		if (player1.speed <= 1) ball.speed /= 4;
	}

	if (ball.radius + player2.radius > getDistance(ball.x, ball.y, player2.x, player2.y))
	{
		ball.angle = getAngle(player2.x, player2.y, ball.x, ball.y);
		if (ball.speed < BALLMAXSPEED) ball.speed += (player2.speed * 0.75);
		if (ball.x <= ball.radius * 2 || ball.radius * 2 > ball.y || ball.y >= WINSIZEY - (ball.radius * 2)) ball.speed /= 2.25f;
		if (player2.speed <= 1) ball.speed /= 4;
	}

	// 골대 영역
	if (ball.x > 0 && ball.x < player1.goalArea.radius - ball.radius)
	{
		if (ball.y > player1.goalArea.y - player1.goalArea.radius * 2 && ball.y < player1.goalArea.y + player1.goalArea.radius * 2)
		{
			ball.speed = 0;
			++ball.goalInInterVal;
			if (ball.goalInInterVal > GOALININTERVAL)
			{
				ball.goalInInterVal = 0;
				player2.score++;
				ball.x = WINSIZEX / 3.15, ball.y = WINSIZEY / 2;
			}
		}
	}
	if (ball.x < player2.goalArea.x && ball.x > player2.goalArea.x - ball.radius * 1.25)
	{
		if (ball.y > player2.goalArea.y - player2.goalArea.radius * 2 && ball.y < player2.goalArea.y + player2.goalArea.radius * 2)
		{
			ball.speed = 0;
			++ball.goalInInterVal;
			if (ball.goalInInterVal > GOALININTERVAL)
			{
				ball.goalInInterVal = 0;
				player1.score++;
				ball.x = WINSIZEX / 1.45f, ball.y = WINSIZEY / 2;
			}
		}
	}

	// 가운데 영역
	if (ball.x + ball.radius > WINSIZEX / 2.75f && ball.x - ball.radius < WINSIZEX / 1.57f)
	{
		++ball.centerStuckInterval;
		if (ball.centerStuckInterval > CENTERSTUCKINTERVAL)
		{
			ball.centerStuckInterval = 0;
			ball._centerStuckInterval = true;
		}
		if (ball._centerStuckInterval)
		{
			ball.speed = 10;
			ball._centerStuckInterval = false;
		}
	}

	// 벽 충돌
	if (ball.x - ball.radius < 0)
	{
		ball.x = ball.radius;
		ball.angle = PI - ball.angle;
	}
	else if (ball.x + ball.radius > WINSIZEX)
	{
		ball.x = WINSIZEX - ball.radius;
		ball.angle = PI - ball.angle;
	}
	if (ball.y - ball.radius < 0)
	{
		ball.y = ball.radius;
		ball.angle = PI2 - ball.angle;
	}
	else if (ball.y + ball.radius > WINSIZEY)
	{
		ball.y = WINSIZEY - ball.radius;
		ball.angle = PI2 - ball.angle;
	}
}

void playGround::update_KeyManager()
{
	// Player 1
	if (KEYMANAGER->isStayKeyDown('W')) player1.key = MoveUp, update_PlayerMovement(&player1);
	if (KEYMANAGER->isStayKeyDown('A')) player1.key = MoveLeft, update_PlayerMovement(&player1);
	if (KEYMANAGER->isStayKeyDown('S'))	player1.key = MoveDown, update_PlayerMovement(&player1);
	if (KEYMANAGER->isStayKeyDown('D')) player1.key = MoveRight, update_PlayerMovement(&player1);

	// Player 2
	if (KEYMANAGER->isStayKeyDown(VK_UP)) player2.key = MoveUp, update_PlayerMovement(&player2);
	if (KEYMANAGER->isStayKeyDown(VK_LEFT)) player2.key = MoveLeft, update_PlayerMovement(&player2);
	if (KEYMANAGER->isStayKeyDown(VK_DOWN)) player2.key = MoveDown, update_PlayerMovement(&player2);
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) player2.key = MoveRight, update_PlayerMovement(&player2);
}

void playGround::update_PlayerMovement(Player* player)
{
	float blockXpos;
	if (!player->dir) blockXpos = WINSIZEX / 2.75f;
	else blockXpos = WINSIZEX / 1.57f;

	if (player->speed < PLAYERMAXSPEED) player->speed += PLAYERSPEEDINCREASE;

	if (player->key == MoveUp)
	{
		if (player->y - player->radius > 0) player->y -= player->speed;
		else player->speed = 0;
	}
	if (player->key == MoveLeft)
	{
		if (player->dir)
		{
			if (player->x - player->radius > blockXpos) player->x -= player->speed;
			else player->speed = 0;
		}
		else
		{
			if (player->x - player->radius > 0) player->x -= player->speed;
			else player->speed = 0;
		}
	}
	if (player->key == MoveDown)
	{
		if (player->y + player->radius < WINSIZEY) player->y += player->speed;
		else player->speed = 0;
	}
	if (player->key == MoveRight)
	{
		if (player->dir)
		{
			if (player->x + player->radius < WINSIZEX) player->x += player->speed;
			else player->speed = 0;
		}
		else
		{
			if (player->x + player->radius < blockXpos) player->x += player->speed;
			else player->speed = 0;
		}
	}
}


void playGround::render_RenderObjs(HDC backDC)
{
	player1_handle->render(backDC, player1.x - player1.radius, player1.y - player1.radius);
	player2_handle->render(backDC, player2.x - player2.radius, player2.y - player2.radius);
	ballImg->render(backDC, ball.x - ball.radius, ball.y - ball.radius);

	char str1[128];
	sprintf_s(str1, sizeof(str1), "점수 : %d", player1.score);
	TextOut(backDC, player1.x - player1.radius, player1.y, str1, strlen(str1));

	char str2[128];
	sprintf_s(str2, sizeof(str2), "점수 : %d", player2.score);
	TextOut(backDC, player2.x - player2.radius, player2.y, str2, strlen(str2));
}

void playGround::render_PlayGround(HDC backDC)
{
	hockeyBG->render(backDC, 0, 0);
	hockeyLine->render(backDC, WINSIZEX / 2.75f, 0);
	hockeyLine->render(backDC, WINSIZEX / 2, 0);
	hockeyLine->render(backDC, WINSIZEX / 1.57f, 0);
	centerCircle->render(backDC, WINSIZEX / 2 - 75, WINSIZEY / 2 - 75);
	goalArea->render(backDC, player1.goalArea.x - 50, player1.goalArea.y - 100);
	goalArea->render(backDC, player2.goalArea.x - 50, player2.goalArea.y - 100);
}