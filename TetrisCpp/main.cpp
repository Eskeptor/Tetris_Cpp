/**
@brief			Main
@author			Eskeptor
@date			2023-01-11
@version		0.0.2
*/

#include <iostream>
#include <random>
#include <Windows.h>
#include <conio.h>
#include "Player.h"

using namespace std;

// Map Width (constexpr)
constexpr int MAP_WIDTH = 12;
// Map Height (constexpr)
constexpr int MAP_HEIGHT = 22;
// Block Width (constexpr)
constexpr int BLOCK_WIDTH = 4;
// Block Height (constexpr)
constexpr int BLOCK_HEIGHT = 4;

// Key Code
enum eKeyCode
{
	KEY_UP = 72,
	KEY_DOWN = 80,
	KEY_LEFT = 75,
	KEY_RIGHT = 77,
	KEY_SPACE = 32,
};

// Rectangle Structure
struct stRect
{
	int nWidth;
	int nHeight;
};

// Console Structure
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

	// Random Seed
	random_device rdDevice;
	// Random Generation
	mt19937 rdGen;
	// Random Distribution (Block)
	uniform_int_distribution<int> rdBlockDist;
	// Random Distribution (Direction)
	uniform_int_distribution<int> rdDirDist;

	// Clock
	clock_t timeStart;

	stConsole()
		: hConsole(nullptr), hBuffer{ nullptr, }, nCurBuffer(0)
		, rdGen(rdDevice()), rdBlockDist(0, 6), rdDirDist(CPlayer::eDirection::Dir0, CPlayer::eDirection::Dir270)
		, timeStart(clock())
	{}
};

// Map Data
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

// Map Data (Backup Data)
int snArrMapBackup[MAP_HEIGHT][MAP_WIDTH] = { 0, };

// Block Data
const int BLOCKS[][BLOCK_WIDTH * BLOCK_HEIGHT] =
{
	{ 0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,2,0,0,0,2,0,0,0,2,2,0 },
	{ 0,0,0,0,0,0,2,0,0,0,2,0,0,2,2,0 },
	{ 0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0 },
	{ 0,0,0,0,0,2,0,0,2,2,2,0,0,0,0,0 },
	{ 0,0,0,0,0,0,2,0,0,2,2,0,0,2,0,0 },
	{ 0,0,0,0,0,2,0,0,0,2,2,0,0,0,2,0 },
};

// Block Type
const char BLOCK_TYPES[][4] =
{
	"  ",
	"▣",
	"□"
};

// Player Data
CPlayer g_player;
// Previous Player Data
CPlayer g_prevPlayerData;
// Block Pointer
int* g_pCurBlock = nullptr;
// Console Data
stConsole console;

/**
@brief		Function that screen clear
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
@brief		Function that Flip screen buffer
@param
@return
*/
void BufferFlip()
{
	SetConsoleActiveScreenBuffer(console.hBuffer[console.nCurBuffer]);
	console.nCurBuffer = console.nCurBuffer ? 0 : 1;
}

/**
@brief		Funtion that get rotate block data
@param		nBlockIdx		Block Index
@param		eDir			Rotate Direction
@return		Block
*/
int* GetRotateBlock(int nBlockIdx, CPlayer::eDirection eDir)
{
	if (g_pCurBlock != nullptr)
	{
		delete[] g_pCurBlock;
		g_pCurBlock = nullptr;
	}

	g_pCurBlock = new int[BLOCK_HEIGHT * BLOCK_WIDTH];
	int nMemSize = sizeof(int) * BLOCK_HEIGHT * BLOCK_WIDTH;
	memcpy_s(g_pCurBlock, nMemSize, BLOCKS[nBlockIdx], nMemSize);

	for (int nRot = 0; nRot < (int)eDir; ++nRot)
	{
		int nTemps[BLOCK_HEIGHT * BLOCK_WIDTH] = { 0, };

		for (int nY = 0; nY < BLOCK_HEIGHT; ++nY)
		{
			for (int nX = 0; nX < BLOCK_WIDTH; ++nX)
			{
				nTemps[(nX * BLOCK_WIDTH) + (BLOCK_HEIGHT - nY - 1)] = g_pCurBlock[(nY * BLOCK_HEIGHT) + nX];
			}
		}

		memcpy_s(g_pCurBlock, nMemSize, nTemps, nMemSize);
	}

	return g_pCurBlock;
}

/**
@brief		Function that get random block
@param
@return		Random block index
*/
int RandomBlock()
{
	return console.rdBlockDist(console.rdGen);
}

/**
@brief		Function that get random direction
@param
@return		Random direction index
*/
int RamdomDirection()
{
	return console.rdDirDist(console.rdGen);
}

/**
@brief		Function that initialize game data & Settings
@param
@return
*/
void InitGame()
{
	// Initialize Player Data
	{
		g_player.SetPosition(4, 1);
		g_player.SetXPositionRange(0, MAP_WIDTH);
		g_player.SetYPositionRange(0, MAP_HEIGHT);
		g_player.SetBlock(RandomBlock());
		g_player.SetDirection((CPlayer::eDirection)RamdomDirection());

		g_prevPlayerData = g_player;
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
		SetConsoleScreenBufferSize(console.hBuffer[0], consoleInfo.dwSize);
		SetConsoleWindowInfo(console.hBuffer[0], TRUE, &consoleInfo.srWindow);
		SetConsoleCursorInfo(console.hBuffer[0], &consoleCursor);

		console.hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(console.hBuffer[1], consoleInfo.dwSize);
		SetConsoleWindowInfo(console.hBuffer[1], TRUE, &consoleInfo.srWindow);
		SetConsoleCursorInfo(console.hBuffer[1], &consoleCursor);
	}

	// Map Backup
	{
		memcpy_s(snArrMapBackup, sizeof(int) * MAP_WIDTH * MAP_HEIGHT, snArrMap, sizeof(int) * MAP_WIDTH * MAP_HEIGHT);
	}
}

/**
@brief		Function to erase game data.
@param
@return
*/
void DestroyGame()
{
	if (g_pCurBlock != nullptr)
	{
		delete[] g_pCurBlock;
		g_pCurBlock = nullptr;
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
@brief		Rendering function
@param
@return
*/
void Render()
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
			WriteFile(console.hBuffer[console.nCurBuffer], BLOCK_TYPES[snArrMap[nY][nX]], sizeof(char) * 4, &dw, NULL);

			if (snArrMap[nY][nX] == 0)
			{
				SetConsoleCursorPosition(console.hBuffer[console.nCurBuffer], coord);
				WriteFile(console.hBuffer[console.nCurBuffer], BLOCK_TYPES[snArrMap[nY][nX]], sizeof(char) * 4, &dw, NULL);
			}

			// Special symbol size is 2 space.
			nXAdd += 2;
		}
	}
}

/**
@brief		Function that puts blocks into a map to match the player's position.
@param
@return
*/
void CalcPlayer()
{
	COORD playerCursor = g_player.GetCursor();

	// 이전 위치의 블록 제거
	if (g_prevPlayerData != g_player)
	{
		int* pBlock = GetRotateBlock(g_prevPlayerData.GetBlock(), g_prevPlayerData.GetDirection());
		COORD sprevCursor = g_prevPlayerData.GetCursor();

		for (int nY = 0; nY < BLOCK_HEIGHT; ++nY)
		{
			for (int nX = 0; nX < BLOCK_WIDTH; ++nX)
			{
				if (pBlock[(nY * BLOCK_HEIGHT) + nX] &&
					pBlock[(nY * BLOCK_HEIGHT) + nX] == snArrMap[sprevCursor.Y + nY][sprevCursor.X + nX])
					snArrMap[sprevCursor.Y + nY][sprevCursor.X + nX] = 0;
			}
		}

		g_prevPlayerData = g_player;
	}

	int* pBlock = GetRotateBlock(g_player.GetBlock(), g_player.GetDirection());
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
@brief		Function to determine if there is a collision
@param		pBlock		Block
@param		coordPlayer	Player coord
@return		true: Collision, false: Not collision
*/
bool IsCollision(int* pBlock, const COORD& coordPlayer)
{
	int nColision = 0;

	for (int nY = 0; nY < BLOCK_HEIGHT; ++nY)
	{
		for (int nX = 0; nX < BLOCK_WIDTH; ++nX)
		{
			nColision = pBlock[(nY * BLOCK_HEIGHT) + nX] & (snArrMapBackup[coordPlayer.Y + nY][coordPlayer.X + nX] << 1);
			nColision += pBlock[(nY * BLOCK_HEIGHT) + nX] & snArrMapBackup[coordPlayer.Y + nY][coordPlayer.X + nX];

			if (nColision > 0)
				return true;
		}
	}

	return false;
}

/**
@brief		Function to check if it can move horizontally
@param		nXAdder		X position adder
@param		nYAdder		Y position adder
@return		true: Available, false: Unavailable
*/
bool IsMoveAvailable(int nXAdder, int nYAdder)
{
	COORD coorNext = g_player.GetCursor();
	coorNext.X += nXAdder;
	coorNext.Y += nYAdder;
	int* pBlock = GetRotateBlock(g_player.GetBlock(), g_player.GetDirection());

	return !IsCollision(pBlock, coorNext);
}

/**
@brief		Function to check if it can rotate
@param
@return		true: Available, false: Unavailable
*/
bool IsRotateAvailable()
{
	int* pBlock = GetRotateBlock(g_player.GetBlock(), g_player.GetNextDirection());

	return !IsCollision(pBlock, g_player.GetCursor());
}

/**
@brief		Function to determine if the bottom has been reached
@param
@return
*/
void CheckBottom()
{
	if (IsMoveAvailable(0, 1) == false)
	{
		double dTimeDiff = clock() - console.timeStart;

		if (dTimeDiff >= 1000)
		{
			memcpy_s(snArrMapBackup, sizeof(int) * MAP_WIDTH * MAP_HEIGHT, snArrMap, sizeof(int) * MAP_WIDTH * MAP_HEIGHT);

			g_player.SetPosition(4, 1);
			g_player.SetBlock(RandomBlock());
			g_player.SetDirection((CPlayer::eDirection)RamdomDirection());

			g_prevPlayerData = g_player;
		}
	}
}

/**
@brief		Function that detects input keys.
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
				if (IsMoveAvailable(0, 1))
				{
					g_player.AddPosition(0, 1);
					console.timeStart = clock();
				}
				break;
			case eKeyCode::KEY_LEFT:
				if (IsMoveAvailable(-1, 0))
				{
					g_player.AddPosition(-1, 0);
					console.timeStart = clock();
				}
				break;
			case eKeyCode::KEY_RIGHT:
				if (IsMoveAvailable(1, 0))
				{
					g_player.AddPosition(1, 0);
					console.timeStart = clock();
				}
				break;
			case eKeyCode::KEY_SPACE:
				if (IsRotateAvailable())
				{
					g_player.SetNextDirection();
					console.timeStart = clock();
				}
				break;
		}
	}
}

/**
@brief		Function that auto down move
@param
@return
*/
void AutoDown()
{
	double dTimeDiff = clock() - console.timeStart;

	if (dTimeDiff >= 1000)
	{
		console.timeStart = clock();

		g_player.AddPosition(0, 1);
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
		CalcPlayer();
		Render();
		CheckBottom();
		AutoDown();

		ClearScreen();
		BufferFlip();
		Sleep(10);
	}

	DestroyGame();

	return 0;
}