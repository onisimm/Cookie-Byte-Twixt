#pragma once

#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <vector>

namespace twixt {
    class Mine;
    class Bridge;
    class Dot {
    public:
        Dot();
        Dot(int i, int j);

        Dot(const Dot& newDot);
        Dot(Dot&& other) noexcept;

        Dot& operator=(Dot&& other) noexcept;

        virtual ~Dot();

        enum class DotStatus : uint8_t
        {
            Player1, // occupied by player 1
            Player2, // occupied by player 2
            Clear, // not occupied yet
            Bulldozer, //occupied by a bulldozer
            Mines //occupied by a mine
        };

        // Getters
        int getCoordI() const;
        int getCoordJ() const;
        DotStatus getStatus() const;
        const std::vector<Bridge*>& getExistingBridges() const;

        // Setters
        void setCoordI(int);
        void setCoordJ(int);
        void setStatus(const DotStatus&);
        void setExistingBridges(const std::vector<Bridge*>& existingBridges);

        // Operators overload
        Dot& operator=(const Dot& newDot); // = overload
        bool operator==(const Dot& otherDot) const; // == overload 
        friend std::ostream& operator<<(std::ostream& os, const Dot& dot); // << overload


        void addBridge(Dot* connectionDot);
        void clearExistingBridges(); // clear all the existingBridges
        const bool& checkExistingBridge(Dot* dotToCheck) const; // check if there's a bridge between this dot and dotToCheck
  

        bool isDotInPath(std::vector<std::pair<Dot*, int>> path) const;

        void deleteAllBridgesForADot();
        void removeBridgeFromExisting(Bridge* bridge);

        Dot::DotStatus returnTheOtherPlayer();

        Bridge* getBridgeFromDots(Dot* secondDot);


    private:
    
        DotStatus m_status : 3;
        int m_i, m_j; // coordinates
        std::vector<Bridge*> m_existingBridges;

        
    };
}

#include "Mine.h"
#include "Bridge.h"
