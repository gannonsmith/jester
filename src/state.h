#pragma once

#include <vector>
#include "piece.h"
#include "helper.h"

class GameState {
    public:

    void get_states(std::vector<std::vector<GameState>>& states);

    void get_pawn_moves(std::vector<GameState>& states);
    void set(Piece& p, Square& s);
    void set_empty();
    unsigned long long get_bitboard();

    private:

    bool white_turn;
    int turn;
    unsigned long long white_kings;
    unsigned long long white_queens;
    unsigned long long white_rooks;
    unsigned long long white_bishops;
    unsigned long long white_knights;
    unsigned long long white_pawns;
    unsigned long long black_kings;
    unsigned long long black_queens;
    unsigned long long black_rooks;
    unsigned long long black_bishops;
    unsigned long long black_knights;
    unsigned long long black_pawns;
    Move prev_move;

    unsigned long long get_white_bitboard();
    unsigned long long get_black_bitboard();
};