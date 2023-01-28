/**
@brief			Main
@author			Eskeptor
@date			2023-01-28
@version		0.0.4
*/

#include <iostream>
#include <random>
#include <Windows.h>
#include <conio.h>
#include "Player.h"

using namespace std;

constexpr int WIN_WIDTH = 70;
constexpr int WIN_HEIGHT = 60;

// Map Width (constexpr)
constexpr int MAP_WIDTH = 12;
// Map Height (constexpr)
constexpr int MAP_HEIGHT = 22;
// Block Width (constexpr)
constexpr int BLOCK_WIDTH = 4;
// Block Height (constexpr)
constexpr int BLOCK_HEIGHT = 4;
// Player Start X Position 
constexpr int START_POS_X = 4;
// Player Start Y Position
constexpr int START_POS_Y = 1;

// Key Code
enum eKeyCode
{
	KEY_UP = 72,
	KEY_DOWN = 80,
	KEY_LEFT = 75,
	KEY_RIGHT = 77,
	KEY_SPACE = 32,
	KEY_R = 114,
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

// Origin Map
const int ORIGIN_MAP[MAP_HEIGHT][MAP_WIDTH] =
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
@brief		Function that initialize game data & Settings
@param
@return
*/
void InitGame(bool bInitConsole = true)
{
	// Initialize Player Data
	{
		g_player.SetPosition(START_POS_X, START_POS_Y);
		g_player.SetXPositionRange(-1, MAP_WIDTH);
		g_player.SetYPositionRange(0, MAP_HEIGHT);
		g_player.SetBlock(RandomBlock());
		g_player.SetDirection((CPlayer::eDirection)RamdomDirection());
		g_player.SetGameScore(0);
		g_player.SetGameOver(false);

		g_prevPlayerData = g_player;
	}

	// Initialize Console Data
	if (bInitConsole)
	{
		console.hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		console.nCurBuffer = 0;

		CONSOLE_CURSOR_INFO consoleCursor{ 1, FALSE };
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo{ 0, };
		GetConsoleScreenBufferInfo(console.hConsole, &consoleInfo);
		consoleInfo.dwSize.X = 70;
		consoleInfo.dwSize.Y = 30;
		consoleInfo.srWindow.Right = consoleInfo.dwSize.X;
		consoleInfo.srWindow.Bottom = consoleInfo.dwSize.Y;

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
		int nMapSize = sizeof(int) * MAP_WIDTH * MAP_HEIGHT;
		memcpy_s(snArrMap, nMapSize, ORIGIN_MAP, nMapSize);
		memcpy_s(snArrMapBackup, nMapSize, snArrMap, nMapSize);
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
@param		nXOffset	X Offset (Render Position)
@param		nYOffset	Y Offset (Render Position)
@return
*/
void Render(int nXOffset = 0, int nYOffset = 0)
{
	COORD coord{ 0, };
	int nXAdd = 0;
	DWORD dw = 0;
	char chBuf[256] = { 0, };

	// Map & Player(Figure) Draw
	{
		for (int nY = 0; nY < MAP_HEIGHT; ++nY)
		{
			nXAdd = 0;

			for (int nX = 0; nX < MAP_WIDTH; ++nX)
			{				
				coord.X = nXAdd + nXOffset;
				coord.Y = nY + nYOffset;

				SetConsoleCursorPosition(console.hBuffer[console.nCurBuffer], coord);
				WriteFile(console.hBuffer[console.nCurBuffer], BLOCK_TYPES[snArrMap[nY][nX]], sizeof(BLOCK_TYPES[snArrMap[nY][nX]]), &dw, NULL);

				if (snArrMap[nY][nX] == 0)
				{
					SetConsoleCursorPosition(console.hBuffer[console.nCurBuffer], coord);
					WriteFile(console.hBuffer[console.nCurBuffer], BLOCK_TYPES[snArrMap[nY][nX]], sizeof(BLOCK_TYPES[snArrMap[nY][nX]]), &dw, NULL);
					//nXAdd += 1;
				}

				// Special symbol size is 2 space.
				nXAdd += 2;
			}

			int nSize = WIN_WIDTH - coord.X;
			if (nSize > 0)
			{
				for (int i = 0; i < nSize - 1; i++)
				{
					coord.X += (i + 1);
					SetConsoleCursorPosition(console.hBuffer[console.nCurBuffer], coord);
					WriteFile(console.hBuffer[console.nCurBuffer], BLOCK_TYPES[0], sizeof(BLOCK_TYPES[0]), &dw, NULL);
				}
			}
		}
	}

	// Game Over
	if (g_player.GetGameOver())
	{
		coord.X = 5 + nXOffset;
		coord.Y = 10 + nYOffset;
		memset(chBuf, 0, sizeof(chBuf));
		int nLen = sprintf_s(chBuf, sizeof(chBuf), "Game Over");
		SetConsoleCursorPosition(console.hBuffer[console.nCurBuffer], coord);
		WriteFile(console.hBuffer[console.nCurBuffer], chBuf, nLen, &dw, NULL);
	}

	// Player Real Position
	{
		//char chBuf[256] = { 0, };
		//coord.X = 26;
		//coord.Y = 4;
		//int nLen = sprintf_s(chBuf, sizeof(chBuf), "X: %02d, Y: %02d", g_player.GetXPosition(), g_player.GetYPosition());
		//SetConsoleCursorPosition(console.hBuffer[console.nCurBuffer], coord);
		//WriteFile(console.hBuffer[console.nCurBuffer], chBuf, nLen, &dw, NULL);
	}

	// Score
	{
		coord.X = 27 + nXOffset;
		coord.Y = 0 + nYOffset;
		memset(chBuf, 0, sizeof(chBuf));
		int nLen = sprintf_s(chBuf, sizeof(chBuf), "Score: %6d", g_player.GetGameScore());
		SetConsoleCursorPosition(console.hBuffer[console.nCurBuffer], coord);
		WriteFile(console.hBuffer[console.nCurBuffer], chBuf, nLen, &dw, NULL);
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

	// 새로운 블럭이 놓아졌을 때 더 이상 움직일 수 없는지 확인 후 게임오버 정함
	if (g_player.GetXPosition() == START_POS_X &&
		g_player.GetYPosition() == START_POS_Y)
		g_player.SetGameOver(!IsMoveAvailable(0, 1));
}

/**
@brief		Check Fill Line
@param
@return		true: Fill and Line Clear, false: Not fill
*/
bool CheckFillLine()
{
	COORD curPos = g_player.GetCursor();
	bool bFill = true;
	int nSize = 0;
	bool bLineCleared = false;

	for (int nY = curPos.Y; nY < curPos.Y + 4; ++nY)
	{
		bFill = true;

		for (int nX = 1; nX < MAP_WIDTH; ++nX)
		{
			if (snArrMapBackup[nY][nX] == 0)
			{
				bFill = false;
				break;
			}
		}

		if (bFill &&
			nY < MAP_HEIGHT - 1)
		{
			nSize = sizeof(int) * MAP_WIDTH * (nY - 1);
			memcpy_s(snArrMapBackup[2], nSize, snArrMapBackup[1], nSize);
			bLineCleared = true;
		}
	}

	return bLineCleared;
}

/**
@brief		Function to determine if the bottom has been reached
@param
@return
*/
void CheckBottom()
{
	double dTimeDiff = clock() - console.timeStart;

	if (dTimeDiff < 1000)
		return;	

	if (g_player.GetGameOver())
		return;
	
	console.timeStart = clock();

	if (IsMoveAvailable(0, 1))
	{
		// Y Move
		g_player.AddPosition(0, 1);
		return;
	}

	memcpy_s(snArrMapBackup, sizeof(int) * MAP_WIDTH * MAP_HEIGHT, snArrMap, sizeof(int) * MAP_WIDTH * MAP_HEIGHT);

	// Check Fill Line
	if (CheckFillLine())
	{
		g_player.AddGameScore(1);
		memcpy_s(snArrMap, sizeof(int) * MAP_WIDTH * MAP_HEIGHT, snArrMapBackup, sizeof(int) * MAP_WIDTH * MAP_HEIGHT);
	}

	g_player.SetPosition(START_POS_X, START_POS_Y);
	g_player.SetBlock(RandomBlock());
	g_player.SetDirection((CPlayer::eDirection)RamdomDirection());

	g_prevPlayerData = g_player;
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
			{
				if (g_player.GetGameOver())
					return;

				while (IsMoveAvailable(0, 1))
				{
					g_player.AddPosition(0, 1);
					Sleep(0);
				}
				console.timeStart = clock() - 1000;
				break;
			}
			case eKeyCode::KEY_DOWN:
			{
				if (g_player.GetGameOver())
					return;

				if (IsMoveAvailable(0, 1))
				{
					g_player.AddPosition(0, 1);
					console.timeStart = clock();
				}
				break;
			}
			case eKeyCode::KEY_LEFT:
			{
				if (g_player.GetGameOver())
					return;

				if (IsMoveAvailable(-1, 0))
				{
					g_player.AddPosition(-1, 0);
				}
				break;
			}
			case eKeyCode::KEY_RIGHT:
			{
				if (g_player.GetGameOver())
					return;

				if (IsMoveAvailable(1, 0))
				{
					g_player.AddPosition(1, 0);
				}
				break;
			}
			case eKeyCode::KEY_SPACE:
			{
				if (g_player.GetGameOver())
					return;

				if (IsRotateAvailable())
				{
					g_player.SetNextDirection();
				}
				break;
			}
			case eKeyCode::KEY_R:
			{
				InitGame(false);

				break;
			}
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
		CalcPlayer();

		CheckBottom();
		Render(3, 1);

		ClearScreen();
		BufferFlip();
		Sleep(1);
	}

	DestroyGame();

	return 0;
}