#include <vector>
#include "board.h"

enum Piece {
    WhiteKing = 'K',
    WhiteQueen = 'Q',
    WhiteRook = 'R',
    WhiteBishop = 'B',
    WhiteKnight = 'N',
    WhitePawn = 'P',
    BlackKing = 'k',
    BlackQueen = 'q',
    BlackRook = 'r',
    BlackBishop = 'b',
    BlackKnight = 'n',
    BlackPawn = 'p',
    Empty = '.'
};


class Player {
    public:

    std::vector<std::vector<Piece>> get_board();
    virtual Piece get(Square& square);

    KingBoard king_board;
    QueenBoard queen_board;
    RookBoard rook_board;
    BishopBoard bishop_board;
    KnightBoard knight_board;
    PawnBoard pawn_board;
};

class White : public Player {
    public:
    Piece get(Square& square);

    private:
};

class Black : public Player {
    public:
    Piece get(Square& square);


    private:
};
