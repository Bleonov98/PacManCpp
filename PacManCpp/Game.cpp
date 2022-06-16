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
	if (level == 5) {
		SetPos(COLS + 3, 17);
		cout << "CONGRATULATIONS! YOU WIN";
	}
	else {
		SetPos(31, 17);
		cout << "GAME OVER!";
		SetPos(31, 20);
		cout << "LEVEL: " << level + 1 << "/5";
	}
	SetPos(32, 19);
	cout << "SCORE: " << score;
	SetPos(25, 22);
	cout << "PRESS ENTER TO RESTART";
	SetPos(27, 23);
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

void Game::DrawInfo()
{
	SetPos(COLS + 10, 2);
	cout << score;
	SetPos(COLS + 10, 4);
	cout << level + 1;
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
					printf(CSI "41;31m");
				}
				else if ((prevBuf[y][x] >> 8) == Green) {
					printf(CSI "42;32m");
				}
				else if ((prevBuf[y][x] >> 8) == Blue) {
					printf(CSI "44;34m");
				}
				else if ((prevBuf[y][x] >> 8) == Yellow) {
					printf(CSI "43;33m");
				}
				else if ((prevBuf[y][x] >> 8) == Purple) {
					printf(CSI "45;35m");
				}
				else if ((prevBuf[y][x] >> 8) == Cyan) {
					printf(CSI "46;36m");
				}
				else if ((prevBuf[y][x] >> 8) == White) {
					printf(CSI "47;37m");
				}
				else printf(CSI "0;0m");

				cout << char(prevBuf[y][x]);

				printf(CSI "1;0m");
			}
		}
	}
}

void Game::DrawToMem()
{
	/*for (int i = 0; i < allGameObjects.size(); i++)
	{
		if (allGameObjects[i]->deleteShape) {
			allGameObjects.erase(allGameObjects.begin() + i);
		}
	}

	for (int i = 0; i < allGameObjects.size(); i++)
	{
		allGameObjects[i]->DrawObject();
	}*/
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
					for (int i = 0; i < strnlen(area, 2192); i++) {
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
}

void Game::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();

	bool pause = false;

	thread hotKeys([&]
		{ HotKeys(pause); }
	);

	level = 0;
	score = 0;

	while (worldIsRun) {
		if (pause) {

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "PAUSED";

			while (pause) {

			}

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "      ";

		}

		DrawToMem();

		DrawChanges();

		DrawInfo();

		Sleep(25);
	}

	DrawEndInfo(restart);

	hotKeys.join();

	printf(CSI "?1049l");
}