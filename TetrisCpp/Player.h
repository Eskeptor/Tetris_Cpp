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
	eDirection m_eDirection;

public:
	CPlayer(int nXPos = 0, int nYPos = 0, int nCurBlock = 0, eDirection eDir = eDirection::Dir0)
		: m_nXPos(nXPos), m_nYPos(nYPos), m_nCurBlock(nCurBlock), m_eDirection(eDir)
	{}

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
		m_nXPos = m_nXPos + nXAdder > 0 ? m_nXPos + nXAdder : 0;
		m_nYPos = m_nYPos + nYAdder > 0 ? m_nYPos + nYAdder : 0;
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
};