#pragma once
#include "Buffer.h"

#define WIDTH 2
#define HEIGHT 1
#define DIRECTION 4
#define ANIMATION 2

class GameObject
{
public:

	wd* wData;

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

	~GameObject() {
		delete this;
	}

protected:

	int _x, _y, _speed, _color;

	bool _deleteObject = false;

	vector <pair<int, int>> objectCoord;

	void EraseObject();
};



class DynamicObject : public GameObject
{
public:

	DynamicObject(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};

	enum dir {
		TOP,
		BOT,
		LEFT,
		RIGHT,
		STOP
	};

	void MoveObject(int direction);

	void CheckNextStep();

protected:

};




class Player : public DynamicObject, public GameObject
{
public:
	Player(wd* wData, int x, int y, int speed, int color) :DynamicObject(wData, x, y, speed, color), GameObject(wData, x, y, speed, color) 
	{};

	void DrawObject() override;

protected:

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
				u"<"
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