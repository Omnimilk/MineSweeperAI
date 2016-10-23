//
// Created by bozai on 20/10/2016.
//

#include "Strategy.hpp"
#include "mdp.hpp"

Strategy::Strategy(Solver &solver1) :solver(solver1) {}

Point Strategy::corSimRan() {
    Point choice;
    //first move
    if(!solver.isInitialized()){
        choice = solver.corFirstMove();
        return choice;
    }

    //not first move
    solver.sim();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }

    //no safe cell
    choice = solver.ranTieBreaking();
    return choice;
}

Point Strategy::corSimCSPRan() {
    Point choice;
    //first move
    if(!solver.isInitialized()){
        choice = solver.corFirstMove();
        return choice;
    }

    //not first move
    solver.sim();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }
    solver.csp();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }


    //no safe cell
    choice = solver.ranTieBreaking();
    return choice;
}

Point Strategy::corSimCSPMDPRan() {
    Point choice;
    //first move
    if(!solver.isInitialized()){
        choice = solver.corFirstMove();
        return choice;
    }

    //if condition suitable for MDP
    int numU=0;
    int numM=0;
    for(int i=0;i<solver.board.N;i++) {
        for(int j=0;j<solver.board.M;j++) {
            if(solver.board.flags[i][j]==1) numM++;
            if(solver.board.board[i][j]==-1) numU++;
        }
    }
    if (numU-numM<15 && numM<5 ) {
        mdp MDP(solver.board.board, solver.board.flags, solver.board.K);
        return MDP.go();
    }


    //not first move,then use Sim & CSP
    solver.sim();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }
    solver.csp();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }


    //no safe cell
    choice = solver.ranTieBreaking();
    return choice;
}

Point Strategy::ranSimRan() {
    Point choice;
    //first move
    if(!solver.isInitialized()){
        choice = solver.ranFirstMove();
        return choice;
    }

    //not first move
    solver.sim();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }

    //no safe cell
    choice = solver.ranTieBreaking();
    return choice;
}

Point Strategy::ranSimCSPRan() {
    Point choice;
    //first move
    if(!solver.isInitialized()){
        choice = solver.ranFirstMove();
        return choice;
    }

    //not first move
    solver.sim();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }
    solver.csp();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }


    //no safe cell
    choice = solver.ranTieBreaking();
    return choice;
}

Point Strategy::ranSimCSPMDPRan() {
    Point choice;
    //first move
    if(!solver.isInitialized()){
        choice = solver.ranFirstMove();
        return choice;
    }

    //if condition suitable for MDP
    int numU=0;
    int numM=0;
    for(int i=0;i<solver.board.N;i++) {
        for(int j=0;j<solver.board.M;j++) {
            if(solver.board.flags[i][j]==1) numM++;
            if(solver.board.board[i][j]==-1) numU++;
        }
    }
    if (numU-numM<15 && numM<5 ) {
        mdp MDP(solver.board.board, solver.board.flags, solver.board.K);
        return MDP.go();
    }


    //not first move,then use Sim & CSP
    solver.sim();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }
    solver.csp();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }


    //no safe cell
    choice = solver.ranTieBreaking();
    return choice;
}

Point Strategy::cenSimRan() {
    Point choice;
    //first move
    if(!solver.isInitialized()){
        choice = solver.cenFirstMove();
        return choice;
    }

    //not first move
    solver.sim();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }

    //no safe cell
    choice = solver.ranTieBreaking();
    return choice;
}

Point Strategy::cenSimCSPRan() {
    Point choice;
    //first move
    if(!solver.isInitialized()){
        choice = solver.cenFirstMove();
        return choice;
    }

    //not first move
    solver.sim();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }
    solver.csp();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }


    //no safe cell
    choice = solver.ranTieBreaking();
    return choice;
}

Point Strategy::cenSimCSPMDPRan() {
    Point choice;
    //first move
    if(!solver.isInitialized()){
        choice = solver.cenFirstMove();
        return choice;
    }

    //if condition suitable for MDP
    int numU=0;
    int numM=0;
    for(int i=0;i<solver.board.N;i++) {
        for(int j=0;j<solver.board.M;j++) {
            if(solver.board.flags[i][j]==1) numM++;
            if(solver.board.board[i][j]==-1) numU++;
        }
    }
    if (numU-numM<15 && numM<5 ) {
        mdp MDP(solver.board.board, solver.board.flags, solver.board.K);
        return MDP.go();
    }


    //not first move,then use Sim & CSP
    solver.sim();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }
    solver.csp();
    if(solver.safeCellExist()){
        choice = solver.chooseSafeCell();
        return choice;
    }


    //no safe cell
    choice = solver.ranTieBreaking();
    return choice;
}

