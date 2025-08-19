#ifndef MOVE_H
#define MOVE_H

#include <algorithm>
#include "piece.h"

class Board;

// PRECOMPUTED MOVE DATA

const int NUM_DIRECTIONS = 8;
const int NUM_SQUARES = 64;

static const int DIRECTION_OFFSETS[NUM_DIRECTIONS] = { 8, -8, -1, 1, 7, -7, 9, -9 };
extern int NUM_SQUARES_TO_EDGE[NUM_SQUARES][NUM_DIRECTIONS];

enum KnightMoves {
    LeftUp = -10,
    UpLeft = -17,
    UpRight = -15,
    RightUp = -5,
    RightDown = 10,
    DownRight = 17,
    DownLeft = 15,
    LeftDown = 6
};


static const int KNIGHT_OFFSETS[8] = { -6, -10, -17, -15, 6, 10, 15, 17 };
extern int KNIGHT_MOVES[NUM_SQUARES][NUM_SQUARES];

void precomputed_move_data();


// MOVE GENERATION

struct Move {
public:

    Move(): start_square(0), target_square(0), resulting_board() {}

    Move(int start_square, int target_square): 
        start_square(start_square), 
        target_square(target_square),
        promotion_piece(Piece::None)
    {
        //
    }

    Move(int start_square, int target_square, unsigned int promotion_piece): 
        start_square(start_square), 
        target_square(target_square),
        promotion_piece(promotion_piece)
    {
        //
    }

    const int start_square;
    const int target_square;
    const unsigned int promotion_piece = Piece::None; // for pawn promotions
    Board* resulting_board;
};


#endif