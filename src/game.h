#include <iostream>
#include "player.h"
#include "helper.h"
#include "piece.h"

struct Move {
    Square start;
    Square end;
    int depth;
    Piece piece;
    bool capture;

    friend std::ostream& operator<<(std::ostream& os, Move const & m) {
        return os << m.piece << ": " << m.start << " -> " << m.end << std::endl;
    };
};

class Game {
    public:

    Game();
    void run();

    void print_board();
    void print_moves();

    /*
    rank, file 1-8
    */
    void set_square(Piece piece, Square& square);

    void fen_setup(std::string& fen_string);

    private:
    Player white;
    Player black;
    Piece board[64];
    std::vector<Move> valid_moves;
    int max_depth;

    void get_moves(bool white_to_move, int depth);
    bool get_move(Move& move, bool white_to_move);

    void get_pawn_moves(bool white_move, int depth);

    bool str_to_move(Move& move, std::string& move_str, bool white_to_move);
    bool valid_piece(Piece& piece, char piece_c, bool white);
    bool valid_square(Square& square, char rank, char file);


    unsigned long long get_bitboard();
    unsigned long long get_white_bitboard();
    unsigned long long get_black_bitboard();
    void print_bitboards();
};