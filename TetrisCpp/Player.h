/**
@brief			Player class
@author			Eskeptor
@date			2023-01-17
@version		0.0.3
*/

#pragma once

class CPlayer
{
public:
	// Block direction
	enum eDirection
	{
		Dir0 = 0,
		Dir90,
		Dir180,
		Dir270,

		DirMax
	};

private:
	// Current X Position
	int m_nXPos;
	// Minimum X Position
	int m_nMinXPos;
	// Maximum X Position
	int m_nMaxXPos;
	// Current Y Position
	int m_nYPos;
	// Minimum Y Position
	int m_nMinYPos;
	// Maximum Y Position
	int m_nMaxYPos;
	// Current Block Index
	int m_nCurBlock;
	// Current Block Direction
	eDirection m_eDirection;
	// Game Score
	int m_nGameScore;
	// Game Over
	bool m_bIsGameOver;

public:
	/**
	@brief		Initialize
	*/
	CPlayer(int nXPos = 0, int nYPos = 0, int nCurBlock = 0, eDirection eDir = eDirection::Dir0)
		: m_nXPos(nXPos), m_nYPos(nYPos), m_nCurBlock(nCurBlock), m_eDirection(eDir)
		, m_nMinXPos(0), m_nMinYPos(0), m_nMaxXPos(20), m_nMaxYPos(20)
		, m_nGameScore(0), m_bIsGameOver(false)
	{}

	/**
	@brief		Set x position range
	@param		nMinXPos	Minimum x position
	@param		nMaxXPos	Maximum x position
	@return
	*/
	inline void SetXPositionRange(int nMinXPos, int nMaxXPos)
	{
		m_nMinXPos = nMinXPos;
		m_nMaxXPos = nMaxXPos;
	}

	/**
	@brief		Set y position range
	@param		nMinYPos	Minimum y position
	@param		nMaxYPos	Maximum y position
	@return
	*/
	inline void SetYPositionRange(int nMinYPos, int nMaxYPos)
	{
		m_nMinYPos = nMinYPos;
		m_nMaxYPos = nMaxYPos;
	}

	/**
	@brief		Set current position
	@param		nXPos		Current x position
	@param		nYPos		Current y position
	@return
	*/
	inline void SetPosition(int nXPos, int nYPos)
	{
		m_nXPos = nXPos;
		m_nYPos = nYPos;
	}

	/**
	@brief		Set current x position
	@param		nXPos		Current x position
	@return
	*/
	inline void SetXPosition(int nXPos)
	{
		m_nXPos = nXPos;
	}

	/**
	@brief		Set current y position
	@param		nYPos		Current y position
	@return
	*/
	inline void SetYPosition(int nYPos)
	{
		m_nYPos = nYPos;
	}

	/**
	@brief		Set current block direction
	@param		eDir		Current block direction
	@return
	*/
	inline void SetDirection(eDirection eDir)
	{
		m_eDirection = eDir;
	}

	/**
	@brief		Set current block direction to next direction
	@param
	@return
	*/
	inline void SetNextDirection()
	{
		if (m_eDirection + 1 >= eDirection::DirMax)
			m_eDirection = eDirection::Dir0;
		else
			m_eDirection = (eDirection)(m_eDirection + 1);
	}

	/**
	@brief		Set game score
	@param		nScore		Game Score
	@return
	*/
	inline void SetGameScore(int nScore)
	{
		m_nGameScore = nScore;
	}

	/**
	@brief		Add game score
	@param		nAdder		Add score
	@return
	*/
	inline void AddGameScore(int nAdder)
	{
		m_nGameScore = m_nGameScore + nAdder >= 0 ? m_nGameScore + nAdder : 0;
	}

	/**
	@brief		Set game over status
	@param		bIsGameOver		Game over status
	@return
	*/
	inline void SetGameOver(bool bIsGameOver)
	{
		m_bIsGameOver = bIsGameOver;
	}

	/**
	@brief		Set current position (Increase by adder value)
	@param		nXAdder		X Increased value
	@param		nYAdder		Y Increased value
	@return
	*/
	inline void AddPosition(int nXAdder, int nYAdder)
	{
		m_nXPos = (m_nXPos + nXAdder >= m_nMinXPos) ? (m_nXPos + nXAdder <= m_nMaxXPos ? m_nXPos + nXAdder : m_nMaxXPos) : m_nMinXPos;
		m_nYPos = (m_nYPos + nYAdder >= m_nMinYPos) ? (m_nYPos + nYAdder <= m_nMaxYPos ? m_nYPos + nYAdder : m_nMaxYPos) : m_nMinYPos;
	}

	/**
	@brief		Set current block index
	@param		nBlock		Current block index
	@return
	*/
	inline void SetBlock(int nBlock)
	{
		m_nCurBlock = nBlock;
	}

	/**
	@brief		Get current x position
	@param		
	@return		Current x position
	*/
	inline int GetXPosition() const
	{
		return m_nXPos;
	}

	/**
	@brief		Get current y position
	@param
	@return		Current y position
	*/
	inline int GetYPosition() const
	{
		return m_nYPos;
	}
	
	/**
	@brief		Get current block index
	@param
	@return		Current block index
	*/
	inline int GetBlock() const
	{
		return m_nCurBlock;
	}

	/**
	@brief		Get current block direction
	@param
	@return		Current block direction
	*/
	inline eDirection GetDirection() const
	{
		return m_eDirection;
	}

	/**
	@brief		Get next block direction
	@param
	@return		Next block direction
	*/
	inline eDirection GetNextDirection() const
	{
		return (m_eDirection + 1 >= eDirection::DirMax) ? eDirection::Dir0 : (eDirection)(m_eDirection + 1);
	}

	/**
	@brief		Get current player position (COORD)
	@param
	@return		Current player position (COORD)
	*/
	inline COORD GetCursor() const
	{
		COORD cursor{ (SHORT)m_nXPos, (SHORT)m_nYPos };
		return cursor;
	}

	/**
	@brief		Get game score
	@param
	@return		Game score
	*/
	inline int GetGameScore() const
	{
		return m_nGameScore;
	}

	/**
	@brief		Get game over status
	@param
	@return		Game over status
	*/
	inline bool GetGameOver() const
	{
		return m_bIsGameOver;
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