#include "move.h"

int NUM_SQUARES_TO_EDGE[NUM_SQUARES][NUM_DIRECTIONS];

void precomputed_move_data () {
    for (int file = 0; file < 8; file++) {
        for (int rank = 0; rank < 8; rank++) {

            int num_north = 7 - rank;
            int num_south = rank;
            int num_west = file;
            int num_east = 7 - file;

            int square_index = rank * 8 + file;

            NUM_SQUARES_TO_EDGE[square_index][0] = num_north;
            NUM_SQUARES_TO_EDGE[square_index][1] = num_south;
            NUM_SQUARES_TO_EDGE[square_index][2] = num_west;
            NUM_SQUARES_TO_EDGE[square_index][3] = num_east;
            NUM_SQUARES_TO_EDGE[square_index][4] = std::min(num_north, num_west);
            NUM_SQUARES_TO_EDGE[square_index][5] = std::min(num_south, num_east);
            NUM_SQUARES_TO_EDGE[square_index][6] = std::min(num_north, num_east);
            NUM_SQUARES_TO_EDGE[square_index][7] = std::min(num_south, num_west);
        }
    }
}
