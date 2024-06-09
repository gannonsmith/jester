
class Board {
    public:
    Board();
    unsigned long long int get();

    private:
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