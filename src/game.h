#include "player.h"

class Game {
    public:
    Game();
    void run();

    void get_board(std::vector<std::vector<char>>& board);
    void print_board(std::vector<std::vector<char>>& board);

    void fen_setup(std::string& fen_string);

    private:
    White white;
    Black black;

    bool move(std::string& move, bool white_to_move);
    bool valid_piece(char piece, bool white);
    bool valid_square(char rank, char file);

    std::string get_piece_icon(char algebraic_piece);
};