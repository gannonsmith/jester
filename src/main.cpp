#include <iostream>

#include "game.h"

// default board
// const std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
// const std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq";
// const std::string starting_fen = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq";
// const std::string starting_fen = "8/8/8/8/8/8/PPPPPPPP/RNBQKBNR";
const std::string starting_fen = "4k3/8/8/8/8/8/8/R3K2R w KQ-";

//const std::string starting_fen = "8/8/4r1q1/b7/6R1/2Q5/4B3/8";

int main()
{
    Game game;
    game.init(starting_fen);
    game.render();

    std::cout << "Ending game" << std::endl;
    
    return 0;
}