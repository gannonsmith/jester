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

    Board(Board& t) {
        squares = t.squares;
        color_to_move = t.color_to_move;
    }

    Board(Board& t, int start_square, int target_square) {
        squares = t.squares;
        squares[target_square] = squares[start_square];
        squares[start_square] = Piece::None;
        color_to_move = Piece::flipColor(t.color_to_move);
    }

    void initialize_with_fen(const std::string& fen);

    std::vector<Move>* get_moves() {
        return &moves;
    }

    void push_move(int start_square, int target_square);

    // clears move list and generates moves
    void generate_moves();

    void generate_white_pawn_moves(int start_square, unsigned int piece);
    void generate_black_pawn_moves(int start_square, unsigned int piece);
    void generate_sliding_moves(int start_square, unsigned int piece);
    void generate_knight_moves(int start_square, unsigned int piece);
    

    unsigned int& operator [](int idx) {
        return squares[idx];
    }
private:
    std::vector<unsigned int> squares; // 1D array for board representation
    std::vector<Move> moves;
    unsigned int color_to_move;
    //int friendly_color;
    //int opponent_color;
};

#endif