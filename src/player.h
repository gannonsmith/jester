#include "board.h"

class Player {
    public:

    virtual void print_board();

    private:
    KingBoard king_board;
    QueenBoard queen_board;
    RookBoard rook_board;
    BishopBoard bishop_board;
    KnightBoard knight_board;
    PawnBoard pawn_board;
};

class Black : public Player {
    public:
    void print_board();

    private:
};

class White : public Player {
    public:
    void print_board();

    private:

};
