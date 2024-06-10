
class Board {
    public:
    Board(unsigned long long int bitboard_in);
    unsigned long long int get();

    unsigned long long int bitboard;
};

class KingBoard : public Board {
    KingBoard(unsigned long long int bitboard_in): bitboard(bitboard_in);
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