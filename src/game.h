#include "player.h"

class Game {
    public:
    Game();
    void run();

    void print_board();

    void fen_setup(std::string& fen_string);

    private:
    White white;
    Black black;
};