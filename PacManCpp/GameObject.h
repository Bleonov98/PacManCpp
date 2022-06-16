#pragma once
#include "Buffer.h"

class GameObject
{
public:

	GameObject(wd* wData, int x, int y) {
		_x = x, _y = y;
	};

	int GetX();

	int GetY();
	
	void SetX(int x);

	void SetY(int y);

private:

	int _x, _y;

};

