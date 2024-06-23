#include "helper.h"

class BitBoard {
    public:
    BitBoard(): bitboard(0) {};
    unsigned long long int get();

    /*
    rank, file 1-8
    */
    void set(Square& square);

    unsigned long long int bitboard;
};
