struct Square {
    int rank;
    int file;
};

class Board {
    public:
    Board(): bitboard(0) {};
    unsigned long long int get();
    void set(int rank, int file);

    unsigned long long int bitboard;
};

class KingBoard : public Board {
    
};

class QueenBoard : public Board {

};

class RookBoard : public Board {

};

class BishopBoard : public Board {

};

class KnightBoard : public Board {

};

class PawnBoard : public Board {

};