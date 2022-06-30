#include "Game.h"

void Game::SetPos(int x, int y)
{
	sprintf_s(coord, "%s%d;%dH", CSI, y, x);
	printf(coord);
}

void Game::HotKeys(bool& pause)
{
	while (worldIsRun) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			worldIsRun = false;
		}
		else if (GetAsyncKeyState(0x50) & 0x8000) {
			pause = !pause;

			this_thread::sleep_for(milliseconds(1000));
		}
	}
}

void Game::DrawEndInfo(bool& restart)
{
	if (win) {
		SetPos(COLS + 4, 20);
		cout << "CONGRATULATION! YOU WIN!";
	}
	else {
		SetPos(COLS + 11, 20);
		cout << "GAME OVER!";
	}

	SetPos(COLS + 5, 22);
	cout << "PRESS ENTER TO RESTART";
	SetPos(COLS + 7, 23);
	cout << "PRESS ESC TO EXIT";

	bool pressed = false;
	restart = false;

	while (!pressed) {
		if (GetAsyncKeyState(VK_RETURN)) {
			restart = true;
			pressed = true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			restart = false;
			pressed = true;
		}
	}
}

void Game::DrawInfo(Player* player)
{
	SetPos(COLS + 10, 2);
	cout << score;
	SetPos(COLS + 10, 4);
	cout << player->GetLifes();
}

void Game::DrawChanges()
{
	for (int y = 0; y < ROWS; y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			if (prevBuf[y][x] == wData.vBuf[y][x]) {
				continue;
			}
			else {
				prevBuf[y][x] = wData.vBuf[y][x];

				SetPos(x, y);

				if ((prevBuf[y][x] >> 8) == Red) {
					printf(CSI "22;31m");
				}
				else if ((prevBuf[y][x] >> 8) == Green) {
					printf(CSI "22;32m");
				}
				else if ((prevBuf[y][x] >> 8) == Blue) {
					printf(CSI "22;34m");
				}
				else if ((prevBuf[y][x] >> 8) == Yellow) {
					printf(CSI "22;33m");
				}
				else if ((prevBuf[y][x] >> 8) == Purple) {
					printf(CSI "22;35m");
				}
				else if ((prevBuf[y][x] >> 8) == Cyan) {
					printf(CSI "22;36m");
				}
				else if ((prevBuf[y][x] >> 8) == White) {
					printf(CSI "22;37m");
				}
				else if ((prevBuf[y][x] >> 8) == PacMan) {
					printf(CSI "0;93m");
				}
				else printf(CSI "22; 44m");

				cout << char(prevBuf[y][x]);

				printf(CSI "1;0m");
			}
		}
	}
}

void Game::SetWall(int x, int y, int type)
{
	wall = new Wall(&wData, x, y, 0, White);
	wall->SetType(type);
	wallList.push_back(wall);
}

void Game::SetCoin(int x, int y, int type)
{
	coin = new Coin(&wData, x, y, 0, White);
	coin->SetType(type);
	coinList.push_back(coin);
	allObjectList.push_back(coin);
}

void Game::SpawnEnemies()
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		enemy = new Enemies(&wData, 54 + i * 2, 25, 1, 1 + i);
		enemyList.push_back(enemy);
		allObjectList.push_back(enemy);
	}
}

void Game::SpawnBonus(int x, int y)
{
	bonus = new FruitBonus(&wData, x, y, 3, 0);
	bonusList.push_back(bonus);
	allObjectList.push_back(bonus);
}

void Game::DrawLevel()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (wData.vBuf[i][j] == u'#' || i <= 1 || j <= 1 || j >= COLS - 1 || i >= ROWS) {
				wData.grid[i][j] = -99;
			}
			else wData.grid[i][j] = -1;
		}
	}

	SetWall(43, 21, MAIN);

	SetWall(79, 21, REGULAR);
	SetWall(33, 21, REGULAR);

	SetWall(3, 3, BIG);
	SetWall(3, 14, BIG);
	SetWall(3, 25, BIG);
	SetWall(3, 33, BIG);
	SetWall(3, 44, BIG);
	

	SetWall(89, 3, BIG);
	SetWall(89, 14, BIG);
	SetWall(89, 25, BIG);
	SetWall(89, 33, BIG);
	SetWall(89, 44, BIG);

	SetWall(33, 3, GIANT);
	SetWall(33, 12, GIANT);
	SetWall(33, 32, GIANT);
	SetWall(33, 37, GIANT);

	SetWall(33, 46, LIL);
	SetWall(69, 46, LIL);

	SetWall(53, 46, THIN);
	SetWall(61, 46, THIN);

	for (int i = 0; i < wallList.size(); i++)
	{
		wallList[i]->DrawObject();
	}

	for (int y = 2; y < ROWS; y++)
	{
		for (int x = 2; x < COLS; x += 2)
		{
			if (wData.vBuf[y][x] != u'#' && wData.vBuf[y][x] != u' ') {
				if ((y == 2 && x == 2) || (y == 2 && x == COLS - 2) || (y == ROWS - 1 && x == 2) || (y == ROWS - 1 && x == COLS - 2)) {
					SetCoin(x, y, IMMORTAL);
				}
				else SetCoin(x, y, MINI);
			}
		}
	}

}

void Game::Collision(Player* player)
{
	for (int i = 0; i < bonusList.size(); i++)
	{
		if ( ( player->GetX() + 1 == bonusList[i]->GetX() && player->GetY() == bonusList[i]->GetY() ) ||
			 ( player->GetX() - 1 == bonusList[i]->GetX() && player->GetY() == bonusList[i]->GetY() ) ||
			 ( player->GetX() == bonusList[i]->GetX() && player->GetY() + 1 == bonusList[i]->GetY() ) ||
			 ( player->GetX() == bonusList[i]->GetX() && player->GetY() - 1 == bonusList[i]->GetY() ) ||
			 ( player->GetX() == bonusList[i]->GetX() && player->GetY() == bonusList[i]->GetY() ) ) {
 
			PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);

			if (bonusList[i]->GetType() == LOW) score += 100;
			else if (bonusList[i]->GetType() == MID) score += 300;
			else if (bonusList[i]->GetType() == HIGH) score += 500;
			else if (bonusList[i]->GetType() == INSANE) score += 1000;

			wData.vBuf[bonusList[i]->GetY()][bonusList[i]->GetX()] = u' ';
			bonusList[i]->DeleteObject();
			bonusList.erase(bonusList.begin() + i);
		}
	}

	for (int i = 0; i < coinList.size(); i++)
	{
		if (player->GetX() == coinList[i]->GetX() && player->GetY() == coinList[i]->GetY()) {
			PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
			score += 10;

			if (coinList[i]->GetType() == IMMORTAL) {
				thread goImmortal([&] {
					player->Immortal(immortal);
					});
				goImmortal.detach();
			}

			coinList[i]->DeleteObject();
			coinList.erase(coinList.begin() + i);
			
			break;
		}
	}

	if (!immortal) {

		for (int i = 0; i < enemyList.size(); i++)
		{
			if (((player->GetX() + 1 == enemyList[i]->GetX()) && (player->GetY() == enemyList[i]->GetY())) ||
				((player->GetX() - 1 == enemyList[i]->GetX()) && (player->GetY() == enemyList[i]->GetY())) ||
				((player->GetX() == enemyList[i]->GetX()) && (player->GetY() - 1 == enemyList[i]->GetY())) ||
				((player->GetX() == enemyList[i]->GetX()) && (player->GetY() + 1 == enemyList[i]->GetY())) ||
				((player->GetX() == enemyList[i]->GetX()) && (player->GetY() == enemyList[i]->GetY())) ) {

				PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC);

				player->Death(worldIsRun);

				for (int j = 0; j < enemyList.size(); j++)
				{
					wData.vBuf[enemyList[j]->GetY()][enemyList[j]->GetX()] = u' ';
					enemyList[j]->DeleteObject();
				}

				Sleep(2000);
				SpawnEnemies();

				return;
			}
		}

	}
	else if (immortal) {

		for (int i = 0; i < enemyList.size(); i++)
		{
			if ( ((player->GetX() + 1 == enemyList[i]->GetX()) && (player->GetY() == enemyList[i]->GetY())) ||
				((player->GetX() - 1 == enemyList[i]->GetX()) && (player->GetY() == enemyList[i]->GetY())) ||
				((player->GetX() == enemyList[i]->GetX()) && (player->GetY() - 1 == enemyList[i]->GetY())) ||
				((player->GetX() == enemyList[i]->GetX()) && (player->GetY() + 1 == enemyList[i]->GetY())) ||
				((player->GetX() == enemyList[i]->GetX()) && (player->GetY() == enemyList[i]->GetY())) ) {
				
				PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);

				enemyList[i]->EnemyDeath();

				Sleep(100);

				score += 250;
			}
		}

	}	
}

void Game::DrawToMem()
{
	for (int i = 0; i < allObjectList.size(); i++)
	{
		if (allObjectList[i]->IsObjectDelete()) {
			allObjectList.erase(allObjectList.begin() + i);
		}
	}

	for (int i = 0; i < bonusList.size(); i++)
	{
		if (bonusList[i]->IsObjectDelete()) {
			bonusList.erase(bonusList.begin() + i);
		}
	}

	for (int i = 0; i < coinList.size(); i++)
	{
		if (coinList[i]->IsObjectDelete()) {
			coinList.erase(coinList.begin() + i);
		}
	}

	for (int i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->IsObjectDelete()) {
			enemyList.erase(enemyList.begin() + i);
		}
	}

	for (int i = 0; i < allObjectList.size(); i++)
	{
		allObjectList[i]->DrawObject();
	}
}

void Game::DrawArea()
{
	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(IDR_TEXT1), L"TEXT");

	if (hResource)
	{
		HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);

		if (hLoadedResource)
		{
			LPCSTR area = (LPCSTR)LockResource(hLoadedResource);

			if (area)
			{
				DWORD dwResourceSize = SizeofResource(hInstance, hResource);

				if (0 != dwResourceSize)
				{
					for (int i = 0; i < strnlen(area, 9000); i++) {
						cout << area[i];
					}
				}
			}
		}
	}

	setvbuf(stdout, NULL, _IONBF, 0);
}

void Game::CreateWorld() {

	term.Terminal();  // Set virtual terminal settings
	term.SetScreenSize();

	printf(CSI "?1049h"); // enable alt buffer
	printf(CSI "?25l"); // hide cursor blinking

	DrawArea();
	PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
}

void Game::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();

	bool pause = false;

	thread hotKeys([&]
		{ HotKeys(pause); }
	);

	score = 0;

	DrawLevel();

	Player* player = new Player(&wData, COLS/2, 45, 1, PacMan);
	allObjectList.push_back(player);

	SpawnEnemies();

	SpawnBonus(2, 2);
	
	DrawChanges();
	
	int tick = 0;

	while (worldIsRun) {

		if (pause) {

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "PAUSED";

			while (pause) {

			}

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "      ";

		}

		player->MoveObject();

		for (int i = 0; i < enemyList.size(); i++)
		{
			if (enemyList[i]->IsDeath() && immortal) {
				for (int step = 0; step < 3; step++)
				{
					enemyList[i]->MoveObject();
				}
			}
			else if (!enemyList[i]->IsDeath() && immortal) {
				if (tick % 3 == 0) enemyList[i]->MoveObject();
			}
			else {
				enemyList[i]->IsInVisArea(player);
				enemyList[i]->MoveObject();
			}
		}

		if (tick % 500 == 0) SpawnBonus(2, 2);

		DrawToMem();

		DrawChanges();

		Collision(player);

		DrawInfo(player);

		if (coinList.empty()) {
			worldIsRun = false;
			win = true;
		}

		Sleep(60);

		tick++;
	}

	DrawEndInfo(restart);

	hotKeys.join();

	printf(CSI "?1049l");
}
