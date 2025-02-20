#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>

#include "board.h"
#include "move.h"

class Game {
public:

    Game() {
        precomputed_move_data();
    }

    ~Game() {
        // deconstruct board
        // moves is already deconstructed
    }

    void init(const std::string& fen) {
        board.initialize_with_fen(fen);
    }

    void preload_figure_template(sf::Texture& t);
    void render();

    void load_position();

    void move(sf::Vector2f oldPos, sf::Vector2f newPos);

    void highlight_moves(int start_square);
    void generate();


private:
    Board board;
    std::vector<Move>* moves;
};

#endif