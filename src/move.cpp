#include "move.h"

int NUM_SQUARES_TO_EDGE[NUM_SQUARES][NUM_DIRECTIONS] = { 0 };
int KNIGHT_MOVES[NUM_SQUARES][NUM_SQUARES] = { 0 };

void precomputed_move_data () {
    for (int file = 0; file < 8; file++) {
        for (int rank = 0; rank < 8; rank++) {

            // Sliding Pieces Precomputation
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


            // Knight Precomputation
            bool knight_offsets[8] = { true, true, true, true, true, true, true, true }; 

            int square_index_mod = square_index % 8;
            int square_index_div = square_index / 8;
            
            if (square_index_mod == 7) {
                knight_offsets[0] = false;
                knight_offsets[3] = false;
                knight_offsets[5] = false;
                knight_offsets[7] = false;
            }
            if (square_index_mod == 6) {
                knight_offsets[0] = false;
                knight_offsets[5] = false;
            }
            if (square_index_mod == 0) {
                // don't do 6, 15, -10, -17
                knight_offsets[0] = false;
                knight_offsets[2] = false;
                knight_offsets[4] = false;
                knight_offsets[6] = false;
            }
            if (square_index_mod == 1) {
                // don't do 6 or -10
                knight_offsets[2] = false;
                knight_offsets[4] = false;
            }
            if (square_index_div == 0) {
                // don't do -6, -10, -15, or -17
                knight_offsets[0] = false;
                knight_offsets[1] = false;
                knight_offsets[2] = false;
                knight_offsets[3] = false;
            }
            if (square_index_div == 1) {
                // don't do -15 or -17
                knight_offsets[0] = false;
                knight_offsets[1] = false;
            }
            if (square_index_div == 7) {
                // don't do 6, 10, 15, or 17
                knight_offsets[4] = false;
                knight_offsets[5] = false;
                knight_offsets[6] = false;
                knight_offsets[7] = false;
            }
            if (square_index_div == 6) {
                // don't do 15 or 17
                knight_offsets[6] = false;
                knight_offsets[7] = false;
            }
            for (int offset_index = 0; offset_index < 8; offset_index++) {
                if (knight_offsets[offset_index]) {
                    KNIGHT_MOVES[square_index][square_index + KNIGHT_OFFSETS[offset_index]] = 1;
                }
            }
        }
    }
}
