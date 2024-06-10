#include "board.h"

Board::Board(unsigned long long int bitboard_in): bitboard(bitboard_in) {}

unsigned long long int Board::get() {
    return bitboard;
}

