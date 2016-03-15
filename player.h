#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <vector>
#include <limits>
#include <cstdio>
#include <tuple>
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    std::vector<Move*> possibleMoves(Side side, Board * board);
    Move *naiveHeuristics(std::vector<Move*> possible);
    Board *getBoard();
    Move *miniMax(std::vector<Move*> possible);
    tuple<int, Move> recursive_miniMax(Board * board, int depth, bool ismySide, Side mySide, Side opSide);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
private:
	Board *board;
	Side mySide;
	Side opSide;
};

#endif
