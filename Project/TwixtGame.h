#pragma once
#include"Dot.h"
#include"Player.h"
#include "Bulldozer.h"
#include "GameStack.h"
#include "Undo.h"
#include "SaveGame.h"
#include "Minimax.h"
#include<iostream>
#include <cstdint>

using namespace twixt;

class TwixtGame
{
public:
	enum class GameMode {
		Default,
		Bulldozer,
		Mines
	};

	void ReadPlayers(Player& player1, Player& player2);
	void GameTurns(Player& player, bool& isPlaying, Board& board);
	bool IsTie(Player player1, Player player2);
	void GameLoop(Board& board, Player player1, Player player2, Bulldozer bulldozer = Bulldozer());
	void ResetGame();
	void Run();

	//getter
	GameStack getGameStack() const;
private:
	GameMode m_gameMode;
	GameStack m_gameStack;
	SaveGame saveGame;
};