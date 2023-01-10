#pragma once

class CPlayer
{
public:
	enum eDirection
	{
		Dir0 = 0,
		Dir90,
		Dir180,
		Dir270,

		DirMax
	};

private:
	int m_nXPos;
	int m_nYPos;
	int m_nCurBlock;
	int m_nMinXPos;
	int m_nMinYPos;
	int m_nMaxXPos;
	int m_nMaxYPos;
	eDirection m_eDirection;

public:
	CPlayer(int nXPos = 0, int nYPos = 0, int nCurBlock = 0, eDirection eDir = eDirection::Dir0)
		: m_nXPos(nXPos), m_nYPos(nYPos), m_nCurBlock(nCurBlock), m_eDirection(eDir)
		, m_nMinXPos(0), m_nMinYPos(0), m_nMaxXPos(20), m_nMaxYPos(20)
	{}

	inline void SetXPositionRange(int nMinXPos, int nMaxXPos)
	{
		m_nMinXPos = nMinXPos;
		m_nMaxXPos = nMaxXPos;
	}

	inline void SetYPositionRange(int nMinYPos, int nMaxYPos)
	{
		m_nMinYPos = nMinYPos;
		m_nMaxYPos = nMaxYPos;
	}

	inline void SetPosition(int nXPos, int nYPos)
	{
		m_nXPos = nXPos;
		m_nYPos = nYPos;
	}

	inline void SetXPosition(int nXPos)
	{
		m_nXPos = nXPos;
	}

	inline void SetYPosition(int nYPos)
	{
		m_nYPos = nYPos;
	}

	inline void SetDirection(eDirection eDir)
	{
		m_eDirection = eDir;
	}

	inline void SetNextDirection()
	{
		if (m_eDirection + 1 >= eDirection::DirMax)
			m_eDirection = eDirection::Dir0;
		else
			m_eDirection = (eDirection)(m_eDirection + 1);
	}

	inline void AddPosition(int nXAdder, int nYAdder)
	{
		m_nXPos = (m_nXPos + nXAdder >= m_nMinXPos) ? (m_nXPos + nXAdder <= m_nMaxXPos ? m_nXPos + nXAdder : m_nMaxXPos) : m_nMinXPos;
		m_nYPos = (m_nYPos + nYAdder >= m_nMinYPos) ? (m_nYPos + nYAdder <= m_nMaxYPos ? m_nYPos + nYAdder : m_nMaxYPos) : m_nMinYPos;
	}

	inline void SetBlock(int nBlock)
	{
		m_nCurBlock = nBlock;
	}

	inline int GetXPosition() const
	{
		return m_nXPos;
	}

	inline int GetYPosition() const
	{
		return m_nYPos;
	}

	inline int GetBlock() const
	{
		return m_nCurBlock;
	}

	inline eDirection GetDirection() const
	{
		return m_eDirection;
	}

	inline COORD GetCursor()
	{
		COORD cursor{ (SHORT)m_nXPos, (SHORT)m_nYPos };
		return cursor;
	}

	CPlayer& operator=(CPlayer& player)
	{
		m_nXPos = player.m_nXPos;
		m_nYPos = player.m_nYPos;
		m_nCurBlock = player.m_nCurBlock;
		m_eDirection = player.m_eDirection;
		m_nMinXPos = player.m_nMinXPos;
		m_nMinYPos = player.m_nMinYPos;
		m_nMaxXPos = player.m_nMaxXPos;
		m_nMaxYPos = player.m_nMaxYPos;

		return *this;
	}

	friend bool operator==(const CPlayer& player1, const CPlayer& player2)
	{
		return (player1.m_nXPos == player2.m_nXPos) &&
			(player1.m_nYPos == player2.m_nYPos) &&
			(player1.m_nCurBlock == player2.m_nCurBlock) &&
			(player1.m_eDirection == player2.m_eDirection) &&
			(player1.m_nMinXPos == player2.m_nMinXPos) &&
			(player1.m_nMinYPos == player2.m_nMinYPos) && 
			(player1.m_nMaxXPos == player2.m_nMaxXPos) &&
			(player1.m_nMaxYPos == player2.m_nMaxYPos);
	}

	friend bool operator!=(const CPlayer& player1, const CPlayer& player2)
	{
		return !(player1 == player2);
	}
};