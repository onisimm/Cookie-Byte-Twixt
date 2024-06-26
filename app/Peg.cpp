#include "Peg.h"
#include "Peg.h"

twixt::Peg::Peg() : Dot()
{}

twixt::Peg::Peg(const Peg& newPeg) : Dot(newPeg), m_existingBridges{ newPeg.m_existingBridges }
{}

twixt::Peg::~Peg()
{}

twixt::Peg& twixt::Peg::operator=(const Peg& newPeg)
{
    this->m_status = newPeg.m_status;
    this->m_i = newPeg.m_i;
    this->m_j = newPeg.m_j;
    m_existingBridges = newPeg.m_existingBridges;
    return *this;
}

bool twixt::Peg::operator==(const Peg& newPeg)
{
    return (this->m_status == newPeg.m_status && this->m_i == newPeg.m_i && this->m_j == newPeg.m_j);
}

 std::vector<twixt::Observer_ptr<twixt::Bridge>> twixt::Peg::getExistingBridges()
{
    return m_existingBridges;
}

void twixt::Peg::setExistingBridges(const std::vector<Observer_ptr<Bridge>>& existingBridges)
{
    m_existingBridges = existingBridges;
}

void twixt::Peg::addBridge(Observer_ptr<Bridge> myBridge)
{
    m_existingBridges.push_back(myBridge);
}


bool twixt::Peg::isPegInPath(std::vector<std::pair<Observer_ptr<Peg>, size_t>> path) const
{
    for (const auto& [peg, position] : path)
    {
        if (peg.GetPointer()->getCoordI() == this->getCoordI() && peg.GetPointer()->getCoordJ() == this->getCoordJ())
        {
            return true;
        }
    }
    return false;
}

void twixt::Peg::removeBridgeFromExisting(Observer_ptr<Bridge> bridge)
{
    m_existingBridges.erase(find(m_existingBridges.begin(), m_existingBridges.end(), bridge));
}

twixt::Dot::Status twixt::Peg::returnTheOtherPlayer()
{
    if (m_status == Dot::Status::Player1)
        return Dot::Status::Player2;
    if (m_status == Dot::Status::Player2)
        return Dot::Status::Player1;
    return Dot::Status::Empty;
}

twixt::Observer_ptr<twixt::Bridge> twixt::Peg::getBridgeFromPegs(Observer_ptr<Peg> secondDot)
{
    auto it = std::find_if(m_existingBridges.begin(), m_existingBridges.end(), [&secondDot](Observer_ptr<Bridge> bridge) {
        return bridge.GetPointer()->isPillarInBridge(secondDot);
        });

    if (it != m_existingBridges.end()) {
        return *it; // Return a pointer to the found bridge
    }

    return nullptr; // Return nullptr if no matching bridge is found

}
