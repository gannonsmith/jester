#include <set>
#include "player.h"
#include "piece.h"

class Game {
    public:

    Game();
    void run();

    void print_board();
    void set_square(Piece piece, Square& square);

    void fen_setup(std::string& fen_string);

    private:
    White white;
    Black black;
    Piece board[64];
    std::set<Move> valid_moves;

    bool move(std::string& move_str, bool white_to_move);
    bool valid_piece(char piece, bool white);
    bool valid_square(char rank, char file);

    void get_moves();

    // Piece helpers

};