#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Player.h"

using namespace std;

enum eKeyCode
{
	KEY_UP = 72,
	KEY_DOWN = 80,
	KEY_LEFT = 75,
	KEY_RIGHT = 77
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

const char BLOCK_TYPES[][4] =
{
	"  ",
	"■",
};

CPlayer player;
int* pCurBlock = nullptr;

void SetCmdCursor(int nXPos, int nYPos)
{
	COORD cursor = { (SHORT)nXPos, (SHORT)nYPos };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}

void SetCmdCursor(COORD cursor)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}

bool CompareCOORD(const COORD& coord1, const COORD& coord2)
{
	return coord1.X == coord2.X && coord1.Y == coord2.Y;
}

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

void InitGame()
{
	player.SetPosition(4, 1);
}

void DrawFrame()
{
	for (int nY = 0; nY < MAP_HEIGHT; ++nY)
	{
		for (int nX = 0; nX < MAP_WIDTH; ++nX)
		{
			printf("%s", BLOCK_TYPES[snArrMap[nY][nX]]);
		}
		printf("\n");
	}
	printf("\n");
}

void DrawPlayer()
{
	static COORD prevCursor = player.GetCursor();
	COORD playerCursor = player.GetCursor();
	int* pBlock = GetRotateBlock(player.GetBlock(), player.GetDirection());
	bool bIsCoordSame = CompareCOORD(prevCursor, playerCursor);

	// TODO 이전 기록 제거

	for (int nY = 0; nY < BLOCK_HEIGHT; ++nY)
	{
		for (int nX = 0; nX < BLOCK_WIDTH; ++nX)
		{
			if (pBlock[(nY * BLOCK_HEIGHT) + nX])
				snArrMap[playerCursor.Y + nY][playerCursor.X + nX] = pBlock[(nY * BLOCK_HEIGHT) + nX];
		}
	}
}

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
		}
	}
}

int main(void)
{
	InitGame();

	while (true)
	{
		InputKey();
		DrawPlayer();
		DrawFrame();

		Sleep(200);
		system("cls");
	}

	if (pCurBlock != nullptr)
	{
		delete[] pCurBlock;
		pCurBlock = nullptr;
	}

	return 0;
}