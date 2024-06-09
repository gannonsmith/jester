#include <iostream>
#include "game.h"

int main(int argc, char** argv) {
    std::cout << "Hello world" << std::endl;

    Game g = Game();
    g.run();

    return 0;
}