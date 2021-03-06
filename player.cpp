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
    delete board;
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
    // First do the opponent's move
    board->doMove(opponentsMove, opSide);

    // Find all possible moves
    std::vector<Move*> possible = possibleMoves(mySide, board);
    // If no possible moves, must pass.
    /*if (possible.size() == 0)
    {
        return NULL;
    }*/
    Move * best_move = miniMax(possible);
    // Find best move via minimax
    /*Move * best_move = new Move(-1, -1);
    std::cerr << "HI" << std::endl;
    *best_move = std::get<1>(recursive_miniMax(board, 3, true, mySide, opSide));
    std::cerr << best_move->getX() << std::endl;*/
    // Update internal board
    board->doMove(best_move, mySide);
    return best_move;
}

/*
 * Finds and returns all the possible moves, given a side and a board
 * to search.
 */
std::vector<Move*> Player::possibleMoves(Side side, Board * board)
{
    std::vector<Move*> possible;
    // Loop through all positions of the board
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            // Create a new move to check
            Move * move = new Move(x, y);
            // Check if that move is legal; if so, add to vector
            if (board->checkMove(move, side))
            {
                possible.push_back(move);
            }
        }
    }
    return possible;
}

/*
 * Implementation of naive heuristics approach - determines the best
 * move by maximizing current score, and returns that move.
 */
Move *Player::naiveHeuristics(std::vector<Move*> possible)
{
    // Initialize best_score to be the smallest int
    int best_score = std::numeric_limits<int>::min();
    Move * best_move;
    // Loop through all possible moves
    for (unsigned int i = 0; i < possible.size(); i++)
    {
        // Try each move on a new copy of the board
        Board * copy = board->copy();
        copy->doMove(possible[i], mySide);
        // Calculate score of that move
        int score = copy->heuristics(mySide, opSide);
        if (score > best_score)
        {
            best_score = score;
            best_move = possible[i];
        }
        // Delete the dynamically allocated copy.
        delete copy;
    }
    return best_move;
}

/*
 * Returns the private field board
 */
Board *Player::getBoard()
{
    return board;
}

/*
 * Implementation of 2-ply minimax approach - aims to minimize the
 * opponent's gain while maximizing own gain. Returns the move that
 * does that.
 */
Move *Player::miniMax(std::vector<Move*> possible)
{

    Move * best_move = NULL;
    // Initialize best_score to be the lowest int possible
    int best_score = std::numeric_limits<int>::min();

    // Loop through all possible moves
    for (unsigned int i = 0; i < possible.size(); i++)
    {
        // Initialize worst_score, which keeps track of the worst-case
        // scenario for a particular move.
        int worst_score = std::numeric_limits<int>::max();

        Board * copy = board->copy();
        copy->doMove(possible[i], mySide);

        // Find all moves possible for the opponent as a result of this
        // move.
        std::vector<Move*> opMoves = possibleMoves(opSide, copy);

        // Loop through all of the opponent's moves & find the worst-
        // case scenario.
        for (unsigned int j = 0; j < opMoves.size(); j++)
        {
            Board * copy2 = copy->copy();
            copy2->doMove(opMoves[j], opSide);

            int score = copy2->heuristics(mySide, opSide);
            if (score < worst_score)
            {
                worst_score = score;
            }
            delete copy2;
        }
        for (unsigned int k = 0; k < opMoves.size(); k++)
        {
            delete opMoves[k];
        }

        // Find the least-worst case scenario. That is the best move.
        if (worst_score > best_score)
        {
            if (!(best_move == NULL))
            {
                Move * temp = best_move;
                best_score = worst_score;
                best_move = possible[i];
                delete temp;
            }
            else
            {
                best_score = worst_score;
                best_move = possible[i];
            }
        }
        else
        {
            delete possible[i];
        }
        delete copy;
    }
    return best_move;
}

std::tuple<int, Move> Player::recursive_miniMax(Board * board, int depth, bool ismySide, Side mySide, Side opSide)
{
    if (depth == 0 || board->isDone())
    {
        return std::make_tuple(board->heuristics(mySide, opSide), Move(-1, -1));
    }
    if (ismySide)
    {
        int bestValue = std::numeric_limits<int>::min();
        int val;
        std::vector<Move*> possible = possibleMoves(mySide, board);
        Board * copy = board->copy();
        Move * best_move = possible[0];
        for (unsigned int i = 1; i < possible.size(); i++)
        {
            copy->doMove(possible[i], mySide);
            val = std::get<0>(recursive_miniMax(copy, depth - 1, false, mySide, opSide));
            if (val > bestValue)
            {
                bestValue = val;
                best_move = possible[i];
            }
        }
        return std::make_tuple(bestValue, *best_move);
    }
    else
    {
        int bestValue = std::numeric_limits<int>::max();
        int val;
        std::vector<Move*> possible = possibleMoves(opSide, board);
        Board * copy = board->copy();
        Move * best_move = possible[0];
        for (unsigned int i = 1; i < possible.size(); i++)
        {
            copy->doMove(possible[i], opSide);
            val = std::get<0>(recursive_miniMax(copy, depth - 1, true, mySide, opSide));
            if (val < bestValue)
            {
                bestValue = val;
                best_move = possible[i];
            }
        }
        return std::make_tuple(bestValue, *best_move);
    }
}
