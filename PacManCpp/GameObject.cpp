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
        (_direction == UP) && (wData->vBuf[_y - 1][_x] == u'#' || _y - 1 == 1)
       ) {
        _direction = STOP;
    }
    else if (
        (_direction == RIGHT) && (wData->vBuf[_y][_x + 1] == u'#' || _x + 1 == COLS - 1)
        ) {
        _direction = STOP;
    }
    else if (_direction == DOWN && (wData->vBuf[_y + 1][_x] == u'#' || _y + 1 == ROWS)) {
        _direction = STOP;
    }
    else if (_direction == LEFT && (wData->vBuf[_y][_x - 1] == u'#' || _x - 1 == 1)) {
        _direction = STOP;
    }
}

// -------------------- Player --------------

void Player::ChangeDirection()
{
    if (GetAsyncKeyState(VK_UP) && (wData->vBuf[_y - 1][_x] != u'#') && (_y - 1 != 1)) {
        _direction = UP;
    }
    else if (GetAsyncKeyState(VK_RIGHT) && (wData->vBuf[_y][_x + 1] != u'#') && (_x + 1 != COLS - 1)) {
        _direction = RIGHT;
    }
    else if (GetAsyncKeyState(VK_DOWN) && (wData->vBuf[_y + 1][_x] != u'#') && (_y + 1 != ROWS)) {
        _direction = DOWN;
    }
    else if (GetAsyncKeyState(VK_LEFT) && (wData->vBuf[_y][_x - 1] != u'#') && (_x - 1 != 1)) {
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


// -------------------- Enemy --------------- !!!!!!!!!!!!

void Enemies::ChangeDirection()
{
    int rnd = 1 + rand() % 2;

    if (_direction == STOP) {
        _direction = rand() % 4;
    }

    if (_direction == UP && wData->grid[_y][_x + 1] != -99
        || _direction == UP && wData->grid[_y][_x - 1] != -99) {
        if (rnd == 2) _direction = rand() % 4;
    }
 
    if (_direction == DOWN && wData->grid[_y][_x + 1] != -99
        || _direction == DOWN && wData->grid[_y][_x - 1] != -99) {
        if (rnd == 2) _direction = rand() % 4;
    }

    if (_direction == RIGHT && wData->grid[_y + 1][_x] != -99
        || _direction == RIGHT && wData->grid[_y - 1][_x] != -99) {
        if (_y != ROWS - 1) {
            if (rnd == 2) _direction = rand() % 4;
        }
    }

    if (_direction == LEFT && wData->grid[_y + 1][_x] != -99
        || _direction == LEFT && wData->grid[_y - 1][_x] != -99) {
        if (_y != ROWS - 1) {
            if (rnd == 2) _direction = rand() % 4;
        }
    }

}

void Enemies::RefreshVisibleArea()
{
    visibleArea.clear();

    for (int R = 1; R < VISIBLE_RADIUS; R++)
    {
        int x = 0;
        int y = R;

        int delta = 1 - 2 * R;
        int err = 0;

        while (y >= x) {
            visibleArea.push_back(make_pair(_x + x, _y + y));
            visibleArea.push_back(make_pair(_x + x, _y - y));
            visibleArea.push_back(make_pair(_x - x, _y + y));
            visibleArea.push_back(make_pair(_x - x, _y - y));
            visibleArea.push_back(make_pair(_x + y, _y + x));
            visibleArea.push_back(make_pair(_x + y, _y - x));
            visibleArea.push_back(make_pair(_x - y, _y + x));
            visibleArea.push_back(make_pair(_x - y, _y - x));

            err = 2 * (delta + y) - 1;

            if ((delta < 0) && (err <= 0)) {
                delta += 2 * ++x + 1;
                continue;
            }
            if ((delta > 0) && (err > 0)) {
                delta -= 2 * --y + 1;
                continue;
            }

            delta += 2 * (++x - --y);
        }
    }
}

void Enemies::IsInVisArea(Player* player)
{
    for (int i = 0; i < visibleArea.size(); i++)
    {
        if ((player->GetX() == visibleArea[i].first) && (player->GetY() == visibleArea[i].second)) {
            _algMove = true;
            FindPath(make_pair( player->GetX(), player->GetY() ));
            break;
        }
    }
}

void Enemies::MoveObject()
{
    EraseObject();

    if (pathToGoal.empty()) {
        _algMove = false;
    }

    if (!_algMove) {

        ChangeDirection();

        CheckNextStep();

        if (_direction == UP) {
            SetY(_y -= _speed);
        }
        else if (_direction == RIGHT) {
            SetX(_x += _speed);
        }
        else if (_direction == DOWN) {
            SetY(_y += _speed);
        }
        else if (_direction == LEFT) {
            SetX(_x -= _speed);
        }
    }
    else if (_algMove) {
        if (!pathToGoal.empty()) {
            SetX(pathToGoal.back().first);
            SetY(pathToGoal.back().second);

            pathToGoal.pop_back();
        }
    }

    RefreshVisibleArea();
}

void Enemies::DrawObject()
{
    wData->vBuf[_y][_x] = u'&' | (_color << 8);
}

void Enemies::FindPath(pair <int, int> targetPos)
{
    if (targetPos.first >= COLS || targetPos.second >= ROWS || targetPos.first <= 1 || targetPos.second <= 1) {
        return;
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (wData->vBuf[i][j] == u'#' || i <= 1 || j <= 1 || j >= COLS - 1 || i >= ROWS) {
                wData->grid[i][j] = -99;
            }
            else wData->grid[i][j] = -1;
        }
    }
    // erase previous grid state

    pathToGoal.clear();

    pair<int, int> startPos = make_pair(GetX(), GetY());

    int d = 0; // steps from start to goal

    bool stop = false;

    vector <pair<int, int>> wave;
    vector <pair<int, int>> prevWave;

    wData->grid[startPos.second][startPos.first] = d;
    prevWave.push_back(make_pair(startPos.first, startPos.second));

    while (!stop) {

        d++;

        for (int i = 0; i < prevWave.size(); i++)
        {

            if (wData->grid[prevWave[i].second][prevWave[i].first + 1] == -1) {
                wData->grid[prevWave[i].second][prevWave[i].first + 1] = d;
                wave.push_back(make_pair(prevWave[i].first + 1, prevWave[i].second));
            }
            if (wData->grid[prevWave[i].second][prevWave[i].first - 1] == -1) {
                wData->grid[prevWave[i].second][prevWave[i].first - 1] = d;
                wave.push_back(make_pair(prevWave[i].first - 1, prevWave[i].second));
            }
            if (wData->grid[prevWave[i].second + 1][prevWave[i].first] == -1) {
                wData->grid[prevWave[i].second + 1][prevWave[i].first] = d;
                wave.push_back(make_pair(prevWave[i].first, prevWave[i].second + 1));
            }
            if (wData->grid[prevWave[i].second - 1][prevWave[i].first] == -1) {
                wData->grid[prevWave[i].second - 1][prevWave[i].first] = d;
                wave.push_back(make_pair(prevWave[i].first, prevWave[i].second - 1));
            }
        }

        for (int i = 0; i < wave.size(); i++)
        {
            if (wave[i].first == targetPos.first && wave[i].second == targetPos.second) {
                stop = true;
                break;
            }
        }

        prevWave.clear();
        prevWave.swap(wave);
    }

    pair<int, int> curPos = make_pair(targetPos.first, targetPos.second);
    pathToGoal.push_back(make_pair(targetPos.first, targetPos.second));

    while (d > 0) {
        if (wData->grid[curPos.second][curPos.first + 1] == d) {
            pathToGoal.push_back(make_pair(curPos.first + 1, curPos.second));
            curPos.first++;
        }
        else if (wData->grid[curPos.second][curPos.first - 1] == d) {
            pathToGoal.push_back(make_pair(curPos.first - 1, curPos.second));
            curPos.first--;
        }
        else if (wData->grid[curPos.second + 1][curPos.first] == d) {
            pathToGoal.push_back(make_pair(curPos.first, curPos.second + 1));
            curPos.second++;
        }
        else if (wData->grid[curPos.second - 1][curPos.first] == d) {
            pathToGoal.push_back(make_pair(curPos.first, curPos.second - 1));
            curPos.second--;
        }

        d--;
    }    

    _algMove = true;
}

// -------------------- Wall ---------------

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

    else if (_type == BIG)
    {
        for (int y = 0; y < BIG_WALL_HEIGHT; y++)
        {
            for (int x = 0; x < BIG_WALL_WIDTH - 1; x++)
            {
                if (BigWallSprite[y][x] != 0 || BigWallSprite[y][x] != u' ') {
                    wallCoord.push_back(make_pair(x, y));
                }
            }
        }
    }

    else if (_type == LIL)
    {
        for (int y = 0; y < LIL_WALL_HEIGHT; y++)
        {
            for (int x = 0; x < LIL_WALL_WIDTH - 1; x++)
            {
                if (LilWallSprite[y][x] != 0 || LilWallSprite[y][x] != u' ') {
                    wallCoord.push_back(make_pair(x, y));
                }
            }
        }
    }

    else if (_type == GIANT)
    {
        for (int y = 0; y < LIL_WALL_HEIGHT; y++)
        {
            for (int x = 0; x < GIANT_WALL_WIDTH - 1; x++)
            {
                if (GiantWallSprite[y][x] != 0 || GiantWallSprite[y][x] != u' ') {
                    wallCoord.push_back(make_pair(x, y));
                }
            }
        }
    }
    
    else if (_type == THIN) {
        for (int y = 0; y < LIL_WALL_HEIGHT; y++)
        {
            for (int x = 0; x < THIN_WALL_WIDTH - 1; x++)
            {
                if (ThinWallSprite[y][x] != 0 || ThinWallSprite[y][x] != u' ') {
                    wallCoord.push_back(make_pair(x, y));
                }
            }
        }
    }

    else if (_type == REGULAR) {
        for (int y = 0; y < BIG_WALL_HEIGHT; y++)
        {
            for (int x = 0; x < REG_WALL_WIDTH - 1; x++)
            {
                if (RegWallSprite[y][x] != 0 || RegWallSprite[y][x] != u' ') {
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

    else if (_type == BIG)
    {
        for (int i = 0; i < wallCoord.size(); i++)
        {
            wData->vBuf[wallCoord[i].second + _y][wallCoord[i].first + _x] = BigWallSprite[wallCoord[i].second][wallCoord[i].first];
        }
    }

    else if (_type == LIL)
    {
        for (int i = 0; i < wallCoord.size(); i++)
        {
            wData->vBuf[wallCoord[i].second + _y][wallCoord[i].first + _x] = LilWallSprite[wallCoord[i].second][wallCoord[i].first];
        }
    }

    else if (_type == GIANT)
    {
        for (int i = 0; i < wallCoord.size(); i++)
        {
            wData->vBuf[wallCoord[i].second + _y][wallCoord[i].first + _x] = GiantWallSprite[wallCoord[i].second][wallCoord[i].first];
        }
    }

    else if (_type == THIN) {
        for (int i = 0; i < wallCoord.size(); i++)
        {
            wData->vBuf[wallCoord[i].second + _y][wallCoord[i].first + _x] = ThinWallSprite[wallCoord[i].second][wallCoord[i].first];
        }
    }

    else if (_type == REGULAR) {
        for (int i = 0; i < wallCoord.size(); i++)
        {
            wData->vBuf[wallCoord[i].second + _y][wallCoord[i].first + _x] = RegWallSprite[wallCoord[i].second][wallCoord[i].first];
        }
    }
}

// -------------------- COIN ---------------

void Coin::SetType(int type)
{
    _type = type;
}

void Coin::DrawObject()
{
    if (_type == MINI) {
        _color = Yellow;
        wData->vBuf[_y][_x] = u'.' | (_color << 8);
    }
    else if (_type == IMMORTAL) {
        _color = White;
        wData->vBuf[_y][_x] = u'O' | (_color << 8);
    }
}

// -------------------- BONUS --------------

