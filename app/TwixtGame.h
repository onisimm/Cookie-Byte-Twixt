#pragma once

#include "Bulldozer.h"
#include "Dot.h"
#include "GameStack.h"
#include "Minimax.h"
#include "Player.h"
#include "SaveGame.h"
#include "Undo.h"
#include <cstdint>
#include <iostream>
#include <QString>

using namespace twixt;

class TwixtGame
{
public:
	enum class GameModeType {
		Default,
		Bulldozer,
		Mines
	};

	void GameTurns(Player& player, bool& isPlaying, Board& board);
	bool IsTie(Player player1, Player player2);
	void ResetGame();

	void setGameMode(const QString& gamemode);
	void setMaxDots(const uint8_t& maxDots);
	void setMaxBridges(const uint8_t& maxBridges);
	void setGameBoardSize(const uint8_t& size);
	void setPlayer1(Player* player1);
	void setPlayer2(Player* player2);

	//getter
	GameStack getGameStack() const;

	void initializeGame();

private:
	Board* m_board;
	Player* m_player1, * m_player2;
	GameModeType m_gameMode;
	GameStack m_gameStack;
	SaveGame saveGame;
	bool blackPlayerStoleColor = false;

	uint8_t m_gameBoardSize;
	uint8_t m_maxDots;
	uint8_t m_maxBridges;
};
