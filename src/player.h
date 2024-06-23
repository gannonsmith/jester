#include <vector>
#include "bitboard.h"
#include "helper.h"
#include "piece.h"


class Player {
    public:

    std::vector<std::vector<Piece>> get_board();
    virtual Piece get(Square& square);

    BitBoard king_board;
    BitBoard queen_board;
    BitBoard rook_board;
    BitBoard bishop_board;
    BitBoard knight_board;
    BitBoard pawn_board;
};

class WhitePlayer : public Player {
    public:
    Piece get(Square& square);

    private:
};

class BlackPlayer : public Player {
    public:
    Piece get(Square& square);


    private:
};
