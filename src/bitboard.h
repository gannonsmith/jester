

class BitBoard {
    public:
    BitBoard(): bitboard(0) {};
    unsigned long long int get();
    void set(int rank, int file);

    unsigned long long int bitboard;
};
