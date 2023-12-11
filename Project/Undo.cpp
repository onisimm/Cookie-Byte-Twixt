#include "Undo.h"

twixt::Undo::Undo(GameStack* gameStack, Board* gameBoard)
{
	Dot* topDot = gameStack->GetGameStack().top().first;
	

	if (Mine* ptrMine = dynamic_cast<Mine*>(topDot)) {
		m_lastDot = ptrMine;
	}
	else if (Bulldozer* ptrBulldozer = dynamic_cast<Bulldozer*>(topDot))
	{
		m_lastDot = ptrBulldozer;
	}
	else
	{
		m_lastDot = topDot;
	}
	m_type = gameStack->GetGameStack().top().second;
	if (m_type == DELETEBRIDGE)
	{
		m_deletedBridgeDot = gameStack->GetDeletedBridgesDotStack().top();
		gameStack->popDeletedBridgesStack();
	}
	board = gameBoard;
	gameStack->popGameStack();
}

void twixt::Undo::pressed()
{

	switch (m_type)
	{
	case 0:
		undoPlayers(Dot::DotStatus::Player1);
		break;
	case 1:
		undoPlayers(Dot::DotStatus::Player2);
		break;
	case 3:
		undoBulldozer();
		break;
	case 4:
		undoMines(m_lastDot);
		break;
	case 5:

		undoDeleteBridge();
		break;
	default:
		break;
	}
}

void twixt::Undo::undoPlayers(Dot::DotStatus status)
{
	if (m_lastDot->getExistingBridges().size())
	{
		board->getDot(m_lastDot->getCoordI(), m_lastDot->getCoordJ())->deleteAllBridgesForADot();
	}

	board->rebuildPossibleBridges(board->getDot(m_lastDot->getCoordI(), m_lastDot->getCoordJ()));
	board->getDot(m_lastDot->getCoordI(), m_lastDot->getCoordJ())->setStatus(Dot::DotStatus::Clear);
}

void twixt::Undo::undoBulldozer()
{
	Bulldozer* lastBulldozer = dynamic_cast<Bulldozer*>(m_lastDot);
	lastBulldozer->setToPreviousPosition(*board);
	Dot copyOfDot = lastBulldozer->getDotDestroied().top();
	board->rebuildPossibleBridges(board->getDot(copyOfDot.getCoordI(), copyOfDot.getCoordJ()));
	bool didMineExplode = false;
	board->changeDotStatus(copyOfDot.getCoordI(), copyOfDot.getCoordJ(), copyOfDot.getStatus(), didMineExplode);
}

void twixt::Undo::undoMines(Dot* mine)
{
	Mine* lastMine = dynamic_cast<Mine*>(mine);
	bool didMineExplode = false;
	
	if (lastMine->getNewPlacedMine())
	{
		board->getMatrixDot(lastMine->getNewPlacedMine()->getCoordI(), lastMine->getNewPlacedMine()->getCoordJ())->setStatus(Dot::DotStatus::Clear);

	}

	for (auto elements : dynamic_cast<Mine*>(lastMine)->getExplodedDots())
	{
		if (Mine* checkMine = dynamic_cast<Mine*>(elements))
		{
			std::cout << "DO the same\n";
			undoMines(checkMine);
		}
		else
		{
			std::cout << "REBUILT DOT " << elements->getCoordI() << " " << elements->getCoordJ() << "\n";
			board->changeDotStatus(elements->getCoordI(), elements->getCoordJ(), elements->getStatus(), didMineExplode);
		}
	}

	board->placeMine(lastMine->getCoordI(), lastMine->getCoordJ());
}

void twixt::Undo::undoDeleteBridge()
{
	board->getDot(m_lastDot->getCoordI(), m_lastDot->getCoordJ())->buildBridge(m_deletedBridgeDot);
}
