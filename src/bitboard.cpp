#include "bitboard.h"

unsigned long long int BitBoard::get() {
    return bitboard;
}

void BitBoard::set(int rank, int file) { 
    // 0 <= rank < 8
    // 0 <= file < 8
    int shift_num = ((rank - 8) * 8) + file + 1;
    unsigned long long int bit = 1;
    bitboard |= bit << shift_num;
}

