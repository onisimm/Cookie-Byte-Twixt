//dot.cpp
#include "Dot.h"
#include <iostream>

namespace twixt {

    // Constructors
    Dot::Dot() : m_status{ DotStatus::Clear }, m_i{ 0 }, m_j{ 0 } {}
    Dot::Dot(const Dot& newDot) : m_status{ newDot.m_status }, m_i{ newDot.m_i }, m_j{ newDot.m_j }, m_existingBridges{ newDot.m_existingBridges } {}

    // Destructor
    Dot::~Dot() {}

    // Getters
    int Dot::getCoordI() const
    {
        return m_i;
    }

    int Dot::getCoordJ() const
    {
        return m_j;
    }

    twixt::Dot::DotStatus Dot::getStatus() const
    {
        return m_status;
    }

    // Setters
    void Dot::setCoordI(int x)
    {
        m_i = x;
    }

    void Dot::setCoordJ(int y)
    {
        m_j = y;
    }

    void Dot::setStatus(DotStatus status)
    {
        m_status = status;
    }

    // Operatorul egal
    Dot& Dot::operator=(const Dot& newDot)
    {
        this->m_status = newDot.m_status;
        this->m_i = newDot.m_i;
        this->m_j = newDot.m_j;
        this->m_existingBridges = newDot.m_existingBridges;
        return *this;
    }

    bool Dot::operator==(const Dot& otherDot) const
    {
        return this->m_i == otherDot.m_i && this->m_j == otherDot.m_j;
    }

    void Dot::addPossibleBridge(Dot* possibleBridgeDot)
    {
        if (std::find(m_possibleBridges.begin(), m_possibleBridges.end(), possibleBridgeDot) == m_possibleBridges.end())
        {
            m_possibleBridges.push_back(possibleBridgeDot);
        }
    }

    const std::vector<Dot*>& Dot::getPossibleBridges() const
    {
        return m_possibleBridges;
    }

    void Dot::clearPossibleBridges()
    {
        m_possibleBridges.clear();
    }

    void Dot::buildBridge(Dot* connectionDot)
    {
        m_existingBridges.push_back(connectionDot);
        connectionDot->m_existingBridges.push_back(this);

        std::cout << "BUILT BRIDGE between " << this->getCoordI() << " " << this->getCoordJ() <<
            " AND " << connectionDot->getCoordI() << " " << connectionDot->getCoordJ() << std::endl;
    }

    const std::vector<Dot*>& Dot::getExistingBridges() const
    {
        return m_existingBridges;
    }

    const bool& Dot::checkExistingBridge(Dot* dotToCheck) const
    {
        return std::find(m_existingBridges.begin(), m_existingBridges.end(), dotToCheck) != m_existingBridges.end();
    }

    bool Dot::isDotInPath(std::vector<std::pair<Dot, int>> path) const
    {
        for (auto i : path)
            if (i.first == *this)
                return true;
        return false;
    }

    void Dot::clearExistingBridges()
    {
        m_existingBridges.clear();
    }

    std::ostream& operator<<(std::ostream& os, const twixt::Dot& dot) {
        // Output the relevant information about the Dot object
        os << "Dot Position: (" << dot.getCoordI() << ", " << dot.getCoordJ() << ") "
            << "Status: ";

        /*switch (dot.getStatus()) {
        case twixt::Dot::DotStatus::Player1:
            os << "Player1";
            break;
        case twixt::Dot::DotStatus::Player2:
            os << "Player2";
            break;
        case twixt::Dot::DotStatus::Clear:
            os << "Clear";
            break;
        }*/

        //os << ", Possible Bridges: " << dot.getPossibleBridges().size()
        //    << ", Existing Bridges: " << dot.getExistingBridges().size();

        os << std::endl;

        return os;
    }

}
