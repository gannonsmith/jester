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

    std::string get_piece_icon(char algebraic_piece);
};