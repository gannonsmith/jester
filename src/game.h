#include "player.h"

class Game {
    public:

    struct Move {
        Square start;
        Square end;
        Piece piece;
        bool capture;
    };

    Game();
    void run();

    void get_board(std::vector<std::vector<Piece>>& board);
    void print_board(std::vector<std::vector<Piece>>& board);

    void fen_setup(std::string& fen_string);

    private:
    White white;
    Black black;

    bool move(std::string& move_str, bool white_to_move);
    bool valid_piece(char piece, bool white);
    bool valid_square(char rank, char file);
    bool is_valid_move(Move& move);

    std::string get_piece_icon(Piece algebraic_piece);
};