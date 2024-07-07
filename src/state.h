#pragma once

#include <vector>
#include "piece.h"
#include "helper.h"

class GameState {
    public:

    void get_states(std::vector<std::vector<GameState>>& states);
    static unsigned long long create_state(unsigned long long old_board, unsigned long long moving_piece, unsigned long long new_move);
    void remove_capture(bool white_takes, unsigned long long position);

    void get_pawn_moves(std::vector<GameState>& states);
    void get_knight_moves(std::vector<GameState>& states);
    void get_bishop_moves(std::vector<GameState>& states);
    void get_rook_moves(std::vector<GameState>& states);
    void get_queen_moves(std::vector<GameState>& states);
    
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