#include "bitboard.h"

unsigned long long int BitBoard::get() {
    return bitboard;
}

void BitBoard::set(Square& square) { 
    int shift_num = ((square.rank-8) * 8) + square.file-9;
    unsigned long long int bit = 1;
    bitboard |= bit << shift_num;
}

