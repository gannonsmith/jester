#include <vector>
#include "board.h"

class Player {
    public:

    std::vector<std::vector<char>> get_board();
    char get(int rank, int file);

    KingBoard king_board;
    QueenBoard queen_board;
    RookBoard rook_board;
    BishopBoard bishop_board;
    KnightBoard knight_board;
    PawnBoard pawn_board;
};

class White : public Player {
    public:

    private:
};

class Black : public Player {
    public:


    private:
};
