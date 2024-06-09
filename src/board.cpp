#include "board.h"

Board::Board() {
    bitboard = 0;
}

unsigned long long int Board::get() {
    return bitboard;
}

