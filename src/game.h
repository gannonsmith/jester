#include <set>
#include "player.h"

class Game {
    public:

    Game();
    void run();

    void refresh_board();
    void print_board();

    void fen_setup(std::string& fen_string);

    private:
    White white;
    Black black;
    std::vector<std::vector<Piece>> display_board;
    std::set<Move> valid_moves;

    bool move(std::string& move_str, bool white_to_move);
    bool valid_piece(char piece, bool white);
    bool valid_square(char rank, char file);

    void get_moves();

    std::string get_piece_icon(Piece algebraic_piece);
};