#include <vector>
#include "board.h"

class Player {
    public:

    Player();
    virtual std::vector<std::vector<char>>& get_board();

    std::vector<std::vector<char>> display_board;

    KingBoard king_board;
    QueenBoard queen_board;
    RookBoard rook_board;
    BishopBoard bishop_board;
    KnightBoard knight_board;
    PawnBoard pawn_board;
};

class White : public Player {
    public:
    White();
    
    std::vector<std::vector<char>>& get_board();

    private:
};

class Black : public Player {
    public:
    Black();
    std::vector<std::vector<char>>& get_board();


    private:
};
