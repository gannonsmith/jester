#include "player.h"

class Game {
    public:
    Game();
    void run();

    void print_board();

    private:
    White white;
    Black black;
};