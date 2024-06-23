
struct Square {
    int rank;
    int file;
};

struct Move {
    Square start;
    Square end;
    int depth;
    Piece piece;
    bool capture;
};

class BitBoard {
    public:
    BitBoard(): bitboard(0) {};
    unsigned long long int get();
    void set(int rank, int file);

    unsigned long long int bitboard;
};
