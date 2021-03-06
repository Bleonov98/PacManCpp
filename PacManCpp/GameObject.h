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

	bool IsObjectDelete();

	void DeleteObject();

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

	void FindPath(pair <int, int> targetPos);

	enum dir {
		UP,
		RIGHT,
		DOWN,
		LEFT,
		STOP
	};

	int _direction = STOP;

	vector <pair <int, int>> pathToGoal;

	bool _algMove = false;

};


class Player : public DynamicObject
{
public: 
	Player(wd* wData, int x, int y, int speed, int color) :DynamicObject(wData, x, y, speed, color) {};

	void DrawObject() override;

	void MoveObject() override;

	void Death(bool& worldIsRun);

	void Immortal(bool& immortal);

	int GetLifes();

protected:
	
	int _playerAnimation = 0, lifes = 3;

	bool changeDir = false;

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
				u"@"
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
				u"@"
			}
		}
	};

};


class Enemies : public DynamicObject 
{
public:

	Enemies(wd* wData, int x, int y, int speed, int color) :DynamicObject(wData, x, y, speed, color) {
		RefreshVisibleArea();
		FindPath(make_pair(65,20));
	};

	void DrawObject() override;

	void MoveObject() override;

	void IsInVisArea(Player* player);

	bool IsDeath();

	void EnemyDeath();

private:

	const int VISIBLE_RADIUS = 20;

	bool _death = false;

	vector <pair<int, int>> visibleArea;



	void ChangeDirection() override;

	void RefreshVisibleArea();

};


class FruitBonus : public DynamicObject
{
public:

	FruitBonus(wd* wData, int x, int y, int speed, int color) : DynamicObject(wData, x, y, speed, color) {
		ChangeDirection();
	};

	void DrawObject() override;

	int GetType();

private:

	void MoveObject() override;

	void ChangeDirection() override;

	int _type = rand() % 4, _tick = 0;
};


// ----------------------------------------- Static Objects ----------------------------------------------------- 

class Wall : public GameObject
{
public:

	Wall(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {};

	void DrawObject() override;

	void SetType(int type);

	vector <pair<int, int>> wallCoord;

private:

	static const int GIANT_WALL_WIDTH = 56;
	static const int MAIN_WALL_WIDTH = 36;
	static const int BIG_WALL_WIDTH = 30;
	static const int LIL_WALL_WIDTH = 20;
	static const int REG_WALL_WIDTH = 10;
	static const int THIN_WALL_WIDTH = 8;

	static const int BIG_WALL_HEIGHT = 10;
	static const int LIL_WALL_HEIGHT = 8;

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

	char16_t BigWallSprite[BIG_WALL_HEIGHT][BIG_WALL_WIDTH]{
		u"#############################",
		u"#############################",
		u"####                     ####",
		u"####                     ####",
		u"####                     ####",
		u"####                     ####",
		u"####                     ####",
		u"####                     ####",
		u"#############################",
		u"#############################"
	};

	char16_t LilWallSprite[LIL_WALL_HEIGHT][LIL_WALL_WIDTH]{
		u"###################",
		u"###################",
		u"####           ####",
		u"####           ####",
		u"####           ####",
		u"####           ####",
		u"###################",
		u"###################"
	};

	char16_t GiantWallSprite[LIL_WALL_HEIGHT][GIANT_WALL_WIDTH]{
		u"#######################################################",
		u"#######################################################",
		u"#####                                             #####",
		u"#####                                             #####",
		u"#####                                             #####",
		u"#####                                             #####",
		u"#######################################################",
		u"#######################################################"
	};

	char16_t ThinWallSprite[LIL_WALL_HEIGHT][THIN_WALL_WIDTH]{
		u"#######",
		u"##   ##",
		u"##   ##",
		u"##   ##",
		u"##   ##",
		u"##   ##",
		u"##   ##",
		u"#######"
	};

	char16_t RegWallSprite[BIG_WALL_HEIGHT][REG_WALL_WIDTH]{
		u"#########",
		u"##     ##",
		u"##     ##",
		u"##     ##",
		u"##     ##",
		u"##     ##",
		u"##     ##",
		u"##     ##",
		u"##     ##",
		u"#########"
	};



	void FillCoord();

};

class Coin :public GameObject
{
public:
	Coin(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};

	void SetType(int type);

	void DrawObject() override;

	int GetType();

private:

	int _type = MINI;

};