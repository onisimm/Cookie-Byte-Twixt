#include "Undo.h"

twixt::Undo::Undo(std::reference_wrapper<GameStack> gameStack, Board* gameBoard):board{*gameBoard}
{
	Observer_ptr<Dot> topDot = gameStack.get().getGameStack().top().first;

	//Check if it is mine
	Mine* ptrMine = topDot.To<Mine>();
	if (ptrMine)
	{
		m_lastDot.reset(new Mine(*ptrMine));
	}
	else
	{
		//Check if it is bulldozer
		Bulldozer* ptrBulldozer = topDot.To<Bulldozer>();
		if (ptrBulldozer)
		{
			m_lastDot.reset(new Bulldozer(*ptrBulldozer));
		}
		else
		{
			//then it is peg
			Peg* ptrDot = topDot.To<Peg>();
			m_lastDot.reset(new Peg(*ptrDot));
		}
	}
	m_type = gameStack.get().getGameStack().top().second;
	if (m_type == DELETEBRIDGE)
	{
		m_deletedBridgeDot = gameStack.get().getDeletedBridgesDotStack().top();
		gameStack.get().popDeletedBridgesStack();
	}
	if (m_type == ADDBRIDGE)
	{
		m_addedBridgeDot = gameStack.get().getAddedBridgesDotStack().top();
		gameStack.get().popAddedBridgesStack();
	}
	gameStack.get().popGameStack();
}

void twixt::Undo::pressed()
{
	//mtype for undo:
	// case 0 = player1
	// case 1 = player2
	// case 3 = bulldozer
	// case 4 = mine
	// case 5 = deleteBridge
	// case 6 = addbridge
	switch (m_type)
	{
	case 0:
		undoPlayers(Dot::Status::Player1);
		break;
	case 1:
		undoPlayers(Dot::Status::Player2);
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
	case 6:
		undoAddBridge();
		break;
	default:
		break;
	}
}

void twixt::Undo::undoPlayers(Dot::Status status)
{
	if (dynamic_cast<Peg*>(m_lastDot.get())->getExistingBridges().size())
	{
		board.get().deleteAllBridgesForAPegInBoard(Observer_ptr<Peg>(dynamic_cast<Peg*>(board.get().getDot(m_lastDot->getCoordI(), m_lastDot->getCoordJ()).get())));
	}
	size_t lastDotI = m_lastDot->getCoordI();
	size_t lastDotJ = m_lastDot->getCoordJ();
	board.get().getDot(m_lastDot->getCoordI(), m_lastDot->getCoordJ()).reset(new Dot(m_lastDot->getCoordI(), m_lastDot->getCoordJ()));
}

void twixt::Undo::undoBulldozer()
{
	Observer_ptr<Bulldozer> lastBulldozer(dynamic_cast<Bulldozer*>(m_lastDot.get()));
	size_t coordILastBulldozer = lastBulldozer.GetPointer()->getCoordI();
	size_t coordJLastBulldozer = lastBulldozer.GetPointer()->getCoordJ();
	//set lastBulldozer to previous position
	lastBulldozer.GetPointer()->setToPreviousPosition(board);

	if (lastBulldozer.GetPointer()->getPegDestroyed().empty())
		return;
	//make a copy of the last DestroyedDot
	Peg copyOfDot = lastBulldozer.GetPointer()->getPegDestroyed().top();

	//delete the last dot that is on the i and j of the destroyedDot
	board.get().getDot(coordILastBulldozer, coordJLastBulldozer).reset(new Peg(copyOfDot));
	Observer_ptr<Dot>(board.get().getDot(coordILastBulldozer, coordJLastBulldozer).get()).To<Peg>()->setExistingBridges({});


	//build bridges for both copyOfDot and the other dot in bridge
	for (size_t index=0; index< copyOfDot.getExistingBridges().size(); index++)
	{
		board.get().addBridgeInBoard(Observer_ptr<Dot>(board.get().getDot(coordILastBulldozer, coordJLastBulldozer).get()).To<Peg>(),
			Observer_ptr<Dot>(board.get().getDot(lastBulldozer.GetPointer()->getPegBridgeDestroyed().top().getCoordI(), lastBulldozer.GetPointer()->getPegBridgeDestroyed().top().getCoordJ()).get()).To<Peg>());
		lastBulldozer.GetPointer()->getPegBridgeDestroyed().pop();
		board.get().m_bulldozer.GetPointer()->getPegBridgeDestroyed().pop();
	}

	board.get().m_bulldozer.GetPointer()->getPegDestroyed().pop();

}

void twixt::Undo::undoMines(std::unique_ptr<Dot>& mine)
{
	Observer_ptr<Dot> lastDot(mine.get());
	Observer_ptr<Mine> lastMine = lastDot.To<Mine>();
	bool didMineExplode = false;

	std::array<std::pair<int, int>, 8> positions{ { {-1, -1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1} } };
	for (auto& [coordRow,coordColumn] : positions)
	{
		if (coordRow + lastMine.GetPointer()->getCoordI() >= 0 && coordRow + lastMine.GetPointer()->getCoordI() < board.get().getSize() && coordColumn + lastMine.GetPointer()->getCoordJ() >= 0 && coordColumn + lastMine.GetPointer()->getCoordJ() < board.get().getSize()) // check boundaries
			if(board.get().getDot(coordRow + lastMine.GetPointer()->getCoordI(), coordColumn + lastMine.GetPointer()->getCoordJ()).get()->getStatus() == Dot::Status::Exploded)
				board.get().getDot(coordRow + lastMine.GetPointer()->getCoordI(),coordColumn + lastMine.GetPointer()->getCoordJ()).get()->setStatus(Dot::Status::Empty);
	}
	

	if (lastMine.GetPointer()->getNewPlacedMine().GetPointer())
	{
		board.get().getMatrixDot(lastMine.GetPointer()->getNewPlacedMine().GetPointer()->getCoordI(), lastMine.GetPointer()->getNewPlacedMine().GetPointer()->getCoordJ()).reset(new Dot(lastMine.GetPointer()->getNewPlacedMine().GetPointer()->getCoordI(), lastMine.GetPointer()->getNewPlacedMine().GetPointer()->getCoordJ()));
	}

	for (auto& elements : lastMine.GetPointer()->getExplodedDots())
	{
		std::unique_ptr<Dot> checkMine;
		if (dynamic_cast<Mine*>(elements.get()))
		{
			checkMine.reset(new Mine(*dynamic_cast<Mine*>(elements.get())));
			std::cout << "Do the same\n";
			undoMines(checkMine);
		}
		else
		{
			board.get().placePeg(elements->getCoordI(), elements->getCoordJ(), elements->getStatus());
		}
	}
	size_t index = 0;
	for (auto& elements : lastMine.GetPointer()->getExplodedDots())
	{

		if (dynamic_cast<Peg*>(elements.get()))
		{
			for (auto bridges : dynamic_cast<Peg*>(elements.get())->getExistingBridges())
			{
				bool existsBridge = false;
				for(auto& bridgesInBoard : board.get().getBridges())
				{
					if (bridgesInBoard.get()->isPillarInBridge((Observer_ptr<Dot>(board.get().getDot(elements->getCoordI(), elements->getCoordJ()).get())).To<Peg>()) && bridgesInBoard.get()->isPillarInBridge((Observer_ptr<Dot>(board.get().getDot(lastMine.GetPointer()->getExplodedBridgesTheOtherDot()[index].get()->getCoordI(), lastMine.GetPointer()->getExplodedBridgesTheOtherDot()[index].get()->getCoordJ()).get())).To<Peg>()))
						existsBridge = true;
				}
				if(!existsBridge)
					board.get().addBridgeInBoard((Observer_ptr<Dot>(board.get().getDot(elements->getCoordI(), elements->getCoordJ()).get())).To<Peg>(), (Observer_ptr<Dot>(board.get().getDot(lastMine.GetPointer()->getExplodedBridgesTheOtherDot()[index].get()->getCoordI(), lastMine.GetPointer()->getExplodedBridgesTheOtherDot()[index].get()->getCoordJ()).get())).To<Peg>());
				index++;
			}
		}
	}

	board.get().placeMine(lastMine.GetPointer()->getCoordI(), lastMine.GetPointer()->getCoordJ());
}

void twixt::Undo::undoDeleteBridge()
{
	Observer_ptr<Dot> deletedDot(board.get().getDot(m_deletedBridgeDot.getCoordI(), m_deletedBridgeDot.getCoordJ()).get());
	board.get().addBridgeInBoard(Observer_ptr<Peg>(dynamic_cast<Peg*>(board.get().getDot(m_lastDot->getCoordI(), m_lastDot->getCoordJ()).get())), deletedDot.To<Peg>());
}

void twixt::Undo::undoAddBridge()
{
	Observer_ptr<Dot> addedDot(board.get().getDot(m_addedBridgeDot.getCoordI(), m_addedBridgeDot.getCoordJ()).get());
	board.get().deleteBridgeInBoard(Observer_ptr<Peg>(dynamic_cast<Peg*>(board.get().getDot(m_lastDot->getCoordI(), m_lastDot->getCoordJ()).get())), addedDot.To<Peg>());
}
