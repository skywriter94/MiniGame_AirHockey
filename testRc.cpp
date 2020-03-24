#include "stdafx.h"
#include "testRc.h"


testRc::testRc()
{
}


testRc::~testRc()
{
}

HRESULT testRc::init()
{
	_rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 100, 200);

	return S_OK;
}

void testRc::release()
{
}

void testRc::update()
{
	_rc.left += 1;
	_rc.right += 1;
}

void testRc::render(HDC hdc)
{
	Rectangle(hdc, _rc);
}
