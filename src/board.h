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

    // helper for castling structure
    struct Castle {
        bool white_king = false;
        bool white_queen = false;
        bool black_king = false;
        bool black_queen = false;
    };

    // constructor to initialize board with "None"s
    Board() : squares(size * size, 0) {}

    Board(Board& t) {
        squares = t.squares;
        color_to_move = t.color_to_move;
        castle = t.castle;
    }

    Board(Board& t, int start_square, int target_square) {
        squares = t.squares;
        squares[target_square] = squares[start_square];
        squares[start_square] = Piece::None;
        color_to_move = Piece::flipColor(t.color_to_move);
        castle = t.castle;
    }

    // constructor for move with promotion
    Board(Board& t, int start_square, int target_square, unsigned int piece) {
        squares = t.squares;
        squares[target_square] = piece;
        squares[start_square] = Piece::None;
        color_to_move = Piece::flipColor(t.color_to_move);
    }

    ~Board() {
        std::cout << "destructing board" << std::endl;
        for (auto move: moves) {
            delete move.resulting_board;
        }
    }

    void initialize_with_fen(const std::string& fen);

    std::vector<Move>* get_moves() {
        return &moves;
    }

    void push_move(int start_square, int target_square);

    // push move with promotion (pawns only)
    void push_move(int start_square, int target_square, unsigned int piece);

    // clears move list and generates moves
    void generate_moves();

    void generate_white_pawn_moves(int start_square, unsigned int piece);
    void generate_black_pawn_moves(int start_square, unsigned int piece);
    void generate_pawn_promotion(int start_square, int target_square, unsigned int piece);
    void generate_sliding_moves(int start_square, unsigned int piece);
    void generate_knight_moves(int start_square, unsigned int piece);
    void generate_castle_moves();
    

    unsigned int& operator [](int idx) {
        return squares[idx];
    }
private:
    std::vector<unsigned int> squares; // 1D array for board representation
    std::vector<unsigned int> check_squares; // squares that are "under attack"
    std::vector<Move> moves;
    unsigned int color_to_move;
    Castle castle;
};

#endif