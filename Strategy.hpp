//
// Created by bozai on 20/10/2016.
//

#ifndef NEWMINESWEEPERAI_STRATEGY_H
#define NEWMINESWEEPERAI_STRATEGY_H

#include "Solver.hpp"

class Strategy {
Solver solver;
public:
    Strategy(Solver &solver1);
    Point corSimRan();
    Point corSimCSPRan();
    Point corSimCSPMDPRan();
    Point ranSimRan();
    Point ranSimCSPRan();
    Point ranSimCSPMDPRan();
    Point cenSimRan();
    Point cenSimCSPRan();
    Point cenSimCSPMDPRan();

};


#endif //NEWMINESWEEPERAI_STRATEGY_H
