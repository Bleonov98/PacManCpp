#pragma once
#include "Buffer.h"

#define WIDTH 2
#define HEIGHT 1
#define DIRECTION 4
#define ANIMATION 2

class GameObject
{
public:

	GameObject(wd* wData, int x, int y, int speed, int color) {
		_x = x, _y = y, _speed = speed, _color = color;
		this->wData = wData;
	};

	int GetX();

	int GetY();

	void SetX(int x);

	void SetY(int y);

	virtual void DrawObject() = 0;

	bool DeleteObject();

protected:

	wd* wData;

	int _x, _y, _speed, _color;

	bool _deleteObject = false;

	void EraseObject();

	virtual ~GameObject() {
		delete this;
	}
};



class DynamicObject : public GameObject
{
public:

	DynamicObject(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};

	virtual void MoveObject() = 0;

protected:

	void CheckNextStep();

	virtual void ChangeDirection() = 0;

	enum dir {
		UP,
		RIGHT,
		DOWN,
		LEFT,
		STOP
	};

	int _direction = UP;
};




class Player : public DynamicObject
{
public: 
	Player(wd* wData, int x, int y, int speed, int color) :DynamicObject(wData, x, y, speed, color) {};

	void DrawObject() override;

	void MoveObject() override;

protected:
	
	int _playerAnimation = 0;

	void ChangeDirection() override;

	char16_t sprite[ANIMATION][DIRECTION][HEIGHT][WIDTH]
	{
		{
			{
				u"V"
			},
			{
				u"<"
			},
			{
				u"^"
			},
			{
				u">"
			}
		},
		{
			{
				u"|"
			},
			{
				u"-"
			},
			{
				u"|"
			},
			{
				u"-"
			}
		}
	};

};














class Enemies :DynamicObject 
{
public:

	Enemies(wd* wData, int x, int y, int speed, int color) :DynamicObject(wData, x, y, speed, color) {};


};





class Coin : GameObject
{
public:
	Coin(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};
private:
};

class BigCoin : GameObject
{
public:
	BigCoin(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};
private:
};

class FruitBonus :GameObject
{
public:
	FruitBonus(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};
private:
};