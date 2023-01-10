#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Player.h"

// TODO Hit 판정 만들기

using namespace std;

enum eKeyCode
{
	KEY_UP = 72,
	KEY_DOWN = 80,
	KEY_LEFT = 75,
	KEY_RIGHT = 77,
	KEY_SPACE = 32,
};

struct stRect
{
	int nWidth;
	int nHeight;
};

struct stConsole
{
	// Console Handler
	HANDLE hConsole;
	// Console Rect Data
	stRect rtConsole;
	// Console Buffer Handler
	HANDLE hBuffer[2];
	// Current Console Buffer Index
	int nCurBuffer;
};

constexpr int MAP_WIDTH = 12;
constexpr int MAP_HEIGHT = 22;
constexpr int BLOCK_WIDTH = 4;
constexpr int BLOCK_HEIGHT = 4;

int snArrMap[MAP_HEIGHT][MAP_WIDTH] = 
{
	{1,1,1,1,1,1,1,1,1,1,1,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,0,0,0,0,0,0,0,0,0,0,1,},
	{1,1,1,1,1,1,1,1,1,1,1,1,},
};

const int BLOCKS[][BLOCK_WIDTH * BLOCK_HEIGHT] =
{
	{ 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0 },
	{ 0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,0 },
	{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0 },
};

const char BLOCK_TYPES[][3] =
{
	"  ",
	"■",
};

CPlayer player;
int* pCurBlock = nullptr;
stConsole console{ 0, };


/**
@brief		Clear Console Screen
@param
@return
*/
void ClearScreen()
{
	COORD pos{ 0, };
	DWORD dwWritten = 0;
	unsigned size = console.rtConsole.nWidth * console.rtConsole.nHeight;

	FillConsoleOutputCharacter(console.hConsole, ' ', size, pos, &dwWritten);
	SetConsoleCursorPosition(console.hConsole, pos);
}

/**
@brief		Get Rotate Block Data
@param		nBlockIdx		Block Index
@param		eDir			Rotate Direction
@return		Block
*/
int* GetRotateBlock(int nBlockIdx, CPlayer::eDirection eDir)
{
	if (pCurBlock != nullptr)
	{
		delete[] pCurBlock;
		pCurBlock = nullptr;
	}

	pCurBlock = new int[BLOCK_HEIGHT * BLOCK_WIDTH];
	int nMemSize = sizeof(int) * BLOCK_HEIGHT * BLOCK_WIDTH;
	memcpy_s(pCurBlock, nMemSize, BLOCKS[nBlockIdx], nMemSize);

	for (int nRot = 0; nRot < (int)eDir; ++nRot)
	{
		int nTemps[BLOCK_HEIGHT * BLOCK_WIDTH] = { 0, };

		for (int nY = 0; nY < BLOCK_HEIGHT; ++nY)
		{
			for (int nX = 0; nX < BLOCK_WIDTH; ++nX)
			{
				nTemps[(nX * BLOCK_WIDTH) + (BLOCK_HEIGHT - nY - 1)] = pCurBlock[(nY * BLOCK_HEIGHT) + nX];
			}
		}

		memcpy_s(pCurBlock, nMemSize, nTemps, nMemSize);
	}

	return pCurBlock;
}

/**
@brief		Initialize Game Data & Settings Function
@param
@return
*/
void InitGame()
{
	// Initialize Player Data
	{
		player.SetPosition(4, 1);
		player.SetXPositionRange(0, MAP_WIDTH);
		player.SetYPositionRange(0, MAP_HEIGHT);
		player.SetBlock(2);
	}

	// Initialize Console Data
	{
		console.hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		console.nCurBuffer = 0;

		CONSOLE_CURSOR_INFO consoleCursor{ 1, FALSE };
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo{ 0, };
		GetConsoleScreenBufferInfo(console.hConsole, &consoleInfo);
		console.rtConsole.nWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left;
		console.rtConsole.nHeight = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top;

		console.hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		//SetConsoleScreenBufferSize(console.hBuffer[0], consoleInfo.dwSize);
		//SetConsoleWindowInfo(console.hBuffer[0], TRUE, &consoleInfo.srWindow);
		SetConsoleCursorInfo(console.hBuffer[0], &consoleCursor);

		console.hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		//SetConsoleScreenBufferSize(console.hBuffer[1], consoleInfo.dwSize);
		//SetConsoleWindowInfo(console.hBuffer[1], TRUE, &consoleInfo.srWindow);
		SetConsoleCursorInfo(console.hBuffer[1], &consoleCursor);
	}
}

void BufferFlip()
{
	SetConsoleActiveScreenBuffer(console.hBuffer[console.nCurBuffer]);
	console.nCurBuffer = console.nCurBuffer ? 0 : 1;
}

/**
@brief		Destroy Game Data
@param
@return
*/
void DestroyGame()
{
	if (pCurBlock != nullptr)
	{
		delete[] pCurBlock;
		pCurBlock = nullptr;
	}

	if (console.hBuffer[0] != nullptr)
	{
		CloseHandle(console.hBuffer[0]);
	}

	if (console.hBuffer[1] != nullptr)
	{
		CloseHandle(console.hBuffer[1]);
	}
}

/**
@brief		Draw Frame Function
@param
@return
*/
void DrawFrame()
{
	COORD coord{ 0, };
	int nXAdd = 0;

	for (int nY = 0; nY < MAP_HEIGHT; ++nY)
	{
		nXAdd = 0;

		for (int nX = 0; nX < MAP_WIDTH; ++nX)
		{
			DWORD dw;
			coord.X = nXAdd;
			coord.Y = nY;

			SetConsoleCursorPosition(console.hBuffer[console.nCurBuffer], coord);
			WriteFile(console.hBuffer[console.nCurBuffer], BLOCK_TYPES[snArrMap[nY][nX]], sizeof(char) * 3, &dw, NULL);

			if (snArrMap[nY][nX] == 0)
			{
				coord.X = ++nXAdd;
				SetConsoleCursorPosition(console.hBuffer[console.nCurBuffer], coord);
				WriteFile(console.hBuffer[console.nCurBuffer], BLOCK_TYPES[snArrMap[nY][nX]], sizeof(char) * 3, &dw, NULL);
			}

			++nXAdd;
			//printf("%s", BLOCK_TYPES[snArrMap[nY][nX]]);
		}
		//printf("\n");
	}
	//printf("\n");
}

/**
@brief		Draw Player Function
@param
@return
*/
void DrawPlayer()
{
	static CPlayer sprevPlayerData = player;
	COORD playerCursor = player.GetCursor();

	// 이전 위치의 블록 제거
	if (sprevPlayerData != player)
	{
		int* pBlock = GetRotateBlock(sprevPlayerData.GetBlock(), sprevPlayerData.GetDirection());
		COORD sprevCursor = sprevPlayerData.GetCursor();

		for (int nY = 0; nY < BLOCK_HEIGHT; ++nY)
		{
			for (int nX = 0; nX < BLOCK_WIDTH; ++nX)
			{
				if (pBlock[(nY * BLOCK_HEIGHT) + nX] &&
					pBlock[(nY * BLOCK_HEIGHT) + nX] == snArrMap[sprevCursor.Y + nY][sprevCursor.X + nX])
					snArrMap[sprevCursor.Y + nY][sprevCursor.X + nX] = 0;
			}
		}

		sprevPlayerData = player;
	}

	int* pBlock = GetRotateBlock(player.GetBlock(), player.GetDirection());
	for (int nY = 0; nY < BLOCK_HEIGHT; ++nY)
	{
		for (int nX = 0; nX < BLOCK_WIDTH; ++nX)
		{
			if (pBlock[(nY * BLOCK_HEIGHT) + nX])
				snArrMap[playerCursor.Y + nY][playerCursor.X + nX] = pBlock[(nY * BLOCK_HEIGHT) + nX];
		}
	}
}

/**
@brief		Input Key Function
@param
@return
*/
void InputKey()
{
	int nKey = 0;

	if (_kbhit() > 0)
	{
		nKey = _getch();

		switch (nKey)
		{
			case eKeyCode::KEY_UP:
				break;
			case eKeyCode::KEY_DOWN:
				player.AddPosition(0, 1);
				break;
			case eKeyCode::KEY_LEFT:
				player.AddPosition(-1, 0);
				break;
			case eKeyCode::KEY_RIGHT:
				player.AddPosition(1, 0);
				break;
			case eKeyCode::KEY_SPACE:
				player.SetNextDirection();
				break;
		}
	}
}

/**
@brief		Main Function
@param
@return
*/
int main(void)
{
	InitGame();

	while (true)
	{
		InputKey();
		DrawPlayer();
		DrawFrame();

		Sleep(10);
		ClearScreen();
		BufferFlip();
	}

	DestroyGame();

	return 0;
}