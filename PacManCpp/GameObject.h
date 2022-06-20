#pragma once
#include "Buffer.h"

#define CREATURE_WIDTH 2
#define CREATURE_HEIGHT 1
#define DIRECTION 5
#define PLAYER_ANIMATION 2

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



// ---------------------------------------- Dynamic Objects ----------------------------------------------


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

	int _direction = STOP;
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

	char16_t sprite[PLAYER_ANIMATION][DIRECTION][CREATURE_HEIGHT][CREATURE_WIDTH]
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
			},
			{
				u"S"
			},
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
			},
			{
				u"S"
			}
		}
	};

};




class Enemies : public DynamicObject 
{
public:

	Enemies(wd* wData, int x, int y, int speed, int color) :DynamicObject(wData, x, y, speed, color) {};

	void DrawObject() override;

	void MoveObject() override;

};




class FruitBonus : public DynamicObject
{
public:
	FruitBonus(wd* wData, int x, int y, int speed, int color) : DynamicObject(wData, x, y, speed, color) {};
private:
};



// ----------------------------------------- Static Objects ----------------------------------------------------- 


class Wall : public GameObject
{
public:

	Wall(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {};

	void DrawObject() override;

	void SetType(int type);

private:

	vector <pair<int, int>> wallCoord;

	enum WallType {
		MAIN,
		BIG,
		LIL
	};

	static const int BIG_WALL_WIDTH = 30;
	static const int BIG_WALL_HEIGHT = 10;
	static const int LIL_WALL_WIDTH = 20;
	static const int LIL_WALL_HEIGHT = 8;
	static const int MAIN_WALL_WIDTH = 36;

	int _type = MAIN;

	char16_t MainWallSprite[BIG_WALL_HEIGHT][MAIN_WALL_WIDTH]{
		u"################# #################",
		u"################# #################",
		u"####                           ####",
		u"####                           ####",
		u"####                           ####",
		u"####                           ####",
		u"####                           ####",
		u"####                           ####",
		u"###################################",
		u"###################################"
	};

	void FillCoord();

};



class Coin :public GameObject
{
public:
	Coin(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};
private:
};



class BigCoin :public GameObject
{
public:
	BigCoin(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};
private:
};