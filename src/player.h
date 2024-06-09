#include "board.h"

class Player {
    public:

    private:
    KingBoard king_board;
    QueenBoard queen_board;
    RookBoard rook_board;
    BishopBoard bishop_board;
    KnightBoard knight_board;
    PawnBoard pawn_board;
};

class White : public Player {

};

class Black : public Player {

};
