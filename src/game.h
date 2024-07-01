#include <set>
#include "player.h"
#include "helper.h"
#include "piece.h"

struct Move {
    Square start;
    Square end;
    int depth;
    Piece piece;
    bool capture;
};

class Game {
    public:

    Game();
    void run();

    void print_board();

    /*
    rank, file 1-8
    */
    void set_square(Piece piece, Square& square);

    void fen_setup(std::string& fen_string);

    private:
    Player white;
    Player black;
    Piece board[64];
    std::set<Move> valid_moves;

    void get_moves(bool white_to_move);
    bool get_move(Move& move, bool white_to_move);

    void get_pawn_moves(bool white_move, int index);

    bool str_to_move(Move& move, std::string& move_str, bool white_to_move);
    bool valid_piece(Piece& piece, char piece_c, bool white);
    bool valid_square(Square& square, char rank, char file);


    unsigned long long get_bitboard();
    unsigned long long get_white_bitboard();
    unsigned long long get_black_bitboard();
    void print_bitboards();
};