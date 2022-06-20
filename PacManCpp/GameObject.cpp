#include "GameObject.h"

int GameObject::GetX()
{
    return _x;
}

int GameObject::GetY()
{
    return _y;
}

void GameObject::SetX(int x)
{
    _x = x;
}

void GameObject::SetY(int y)
{
    _y = y;
}

void GameObject::EraseObject()
{
    wData->vBuf[_y][_x] = u' ';
}

bool GameObject::DeleteObject()
{
    return _deleteObject;
}

void DynamicObject::CheckNextStep()
{
    if (
        (_direction == UP) && (wData->vBuf[_y - 1][_x] == u'#')
       ) {
        _direction = STOP;
    }
    else if (
        (_direction == RIGHT) && (wData->vBuf[_y][_x + 1] == u'#')
        ) {
        _direction = STOP;
    }
    else if (_direction == DOWN && wData->vBuf[_y + 1][_x] == u'#') {
        _direction = STOP;
    }
    else if (_direction == LEFT && wData->vBuf[_y][_x - 1] == u'#') {
        _direction = STOP;
    }
}

void Player::ChangeDirection()
{
    if (GetAsyncKeyState(VK_UP)) {
        _direction = UP;
    }
    else if (GetAsyncKeyState(VK_RIGHT)) {
        _direction = RIGHT;
    }
    else if (GetAsyncKeyState(VK_DOWN)) {
        _direction = DOWN;
    }
    else if (GetAsyncKeyState(VK_LEFT)) {
        _direction = LEFT;
    }
}

void Player::MoveObject()
{
    ChangeDirection();

    CheckNextStep();

    if (_direction != STOP) {

        EraseObject();

        if (_direction == UP) {
            _y -= _speed;
        }
        else if (_direction == RIGHT) {
            _x += _speed;
        }
        else if (_direction == DOWN) {
            _y += _speed;
        }
        else if (_direction == LEFT) {
            _x -= _speed;
        }

        if (_playerAnimation == 0) {
            _playerAnimation++;
        }
        else _playerAnimation = 0;

    }
}

void Player::DrawObject()
{
    for (int i = 0; i < CREATURE_HEIGHT; i++)
    {
        for (int j = 0; j < CREATURE_WIDTH - 1; j++)
        {
            wData->vBuf[_y + i][_x + j] = sprite[_playerAnimation][_direction][i][j] | (_color << 8);
        }
    }
}

void Wall::SetType(int type)
{
    _type = type;

    FillCoord();
}

void Wall::FillCoord()
{
    if (_type == MAIN) 
    {
        for (int y = 0; y < BIG_WALL_HEIGHT; y++)
        {
            for (int x = 0; x < MAIN_WALL_WIDTH - 1; x++)
            {
                if (MainWallSprite[y][x] != 0 || MainWallSprite[y][x] != u' ') {
                    wallCoord.push_back(make_pair(x, y));
                }
            }
        }
    }
    
}

void Wall::DrawObject()
{
    if (_type == MAIN)
    {
        for (int i = 0; i < wallCoord.size(); i++)
        {
            wData->vBuf[wallCoord[i].second + _y][wallCoord[i].first + _x] = MainWallSprite[wallCoord[i].second][wallCoord[i].first];
        }
    }


}