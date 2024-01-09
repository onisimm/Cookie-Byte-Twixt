#pragma once

#include "Dot.h"
#include "Player.h"
#include "Bulldozer.h"
#include "GameStack.h"
#include "Undo.h"
#include "SaveGame.h"
#include "Minimax.h"
#include <iostream>
#include <QString>
#include <cstdint>

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

	//getter
	GameStack getGameStack() const;

private:
	GameModeType m_gameMode;
	GameStack m_gameStack;
	SaveGame saveGame;
	bool blackPlayerStoleColor = false;

	uint8_t m_gameBoardSize;
	uint8_t m_maxDots;
	uint8_t m_maxBridges;
};
