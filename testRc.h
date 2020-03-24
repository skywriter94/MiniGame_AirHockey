#pragma once
#include "gameNode.h"

class testRc : public gameNode
{
private:
	RECT _rc;

public:
	testRc();
	~testRc();

	virtual HRESULT init();			//초기화 전용 함수
	virtual void release();			//메모리 해제 함수
	virtual void update();			//연산 전용 함수
	virtual void render(HDC hdc);	//그리기 전용 함수

};

