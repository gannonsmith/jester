#include <iostream>
#include "game.h"

Game::Game() {
    std::cout << "game constructed" << std::endl;

}

void Game::run() {
    std::cout << "game running" << std::endl;
    print_board();
}

void Game::print_board() {
    std::vector<std::vector<char>>& white_board = white.get_board();
    std::vector<std::vector<char>>& black_board = black.get_board();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (white_board[i][j] != '.') {
                std::cout << white_board[i][j] << " ";
            } else if (black_board[i][j] != '.') {
                std::cout << black_board[i][j] << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}