#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    board = new Board();
    this->mySide = side;
    if (side == BLACK)
    {
        this->opSide = WHITE;
    }
    else
    {
        this->opSide = BLACK;
    }
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    board->doMove(opponentsMove, opSide);
    std::vector<Move*> possible = possibleMoves(mySide, board);
    if (possible.size() == 0)
    {
        return NULL;
    }
    Move * best_move = miniMax(possible);
    board->doMove(best_move, mySide);
    return best_move;
}

std::vector<Move*> Player::possibleMoves(Side side, Board * board)
{
    std::vector<Move*> possible;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            Move * move = new Move(x, y);
            if (board->checkMove(move, side))
            {
                possible.push_back(move);
            }
        }
    }
    return possible;
}

Move *Player::simpleHeuristics(std::vector<Move*> possible)
{
    int best_score = std::numeric_limits<int>::min();
    Move * best_move;
    for (unsigned int i = 0; i < possible.size(); i++)
    {
        Board * copy = board->copy();
        copy->doMove(possible[i], mySide);
        int score = copy->count(mySide) - copy->count(opSide);
        if (score > best_score)
        {
            best_score = score;
            best_move = possible[i];
        }
    }
    return best_move;
}

Board *Player::getBoard()
{
    return board;
}

Move *Player::miniMax(std::vector<Move*> possible)
{

    Move * best_move;
    int best_score = std::numeric_limits<int>::min();


    for (unsigned int i = 0; i < possible.size(); i++)
    {
        int worst_score = std::numeric_limits<int>::max();

        Board * copy = board->copy();
        copy->doMove(possible[i], mySide);

        std::vector<Move*> opMoves = possibleMoves(opSide, copy);

        for (unsigned int j = 0; j < opMoves.size(); j++)
        {
            Board * copy2 = copy->copy();
            copy2->doMove(opMoves[j], opSide);

            int score = copy2->count(mySide) - copy2->count(opSide);
            if (score < worst_score)
            {
                worst_score = score;
            }
        }
        if (worst_score > best_score)
        {
            best_score = worst_score;
            best_move = possible[i];
        }
    }
    return best_move;
}