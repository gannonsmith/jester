#ifndef MOVE_H
#define MOVE_H

#include <algorithm>

// PRECOMPUTED MOVE DATA

const int NUM_DIRECTIONS = 8;
const int NUM_SQUARES = 64;

static const int DIRECTION_OFFSETS[NUM_DIRECTIONS] = { 8, -8, -1, 1, 7, -7, 9, -9 };
extern int NUM_SQUARES_TO_EDGE[NUM_SQUARES][NUM_DIRECTIONS];

void precomputed_move_data();


// MOVE GENERATION

struct Move {
public:
    const int start_square;
    const int target_square;
};


#endif