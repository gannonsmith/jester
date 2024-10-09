#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <cctype>
#include <string>
#include <vector>
#include <iostream>

#include "piece.h"
#include "move.h"


class Board {
public:
    // board dimensions
    static const int size = 8;

    // constructor to initialize board with "None"s
    Board() : squares(size * size, 0) {}

    void initialize_with_fen(const std::string& fen);

    // clears move list and generates moves
    void generate_moves(std::vector<Move>& moves);

    void generate_sliding_moves(std::vector<Move>& moves, int start_square, int piece);
    

    int& operator [](int idx) {
        return squares[idx];
    }
private:
    std::vector<int> squares; // 1D array for board representation
    std::vector<Move> moves;
    int color_to_move;
    int friendly_color;
    int opponent_color;
};

#endif