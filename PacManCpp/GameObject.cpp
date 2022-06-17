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
    for (int i = 0; i < objectCoord.size(); i++)
    {
        wData->vBuf[objectCoord[i].second][objectCoord[i].first] = u' ';
    }
}

bool GameObject::DeleteObject()
{
    return _deleteObject;
}

void DynamicObject::MoveObject(int direction)
{

}

void Player::DrawObject()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH - 1; j++)
        {
            GameObject::wData->vBuf[GameObject::GetY()][GameObject::GetX()] = sprite[1][1][1][1] | (GameObject::_color << 8);
        }
    }
}


