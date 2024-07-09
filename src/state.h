#pragma once

#include <vector>
#include "piece.h"
#include "helper.h"

class GameState {
    public:

    unsigned long long under_attack;

    void get_states(std::vector<std::vector<GameState>>& states);
    static unsigned long long create_state(unsigned long long old_board, unsigned long long moving_piece, unsigned long long new_move);
    void remove_capture(bool white_takes, unsigned long long position);

    unsigned long long generate_capture_spaces();
    /*
    Returns true if position is under attack
    */
    bool space_check(const unsigned long long position);
    bool king_check();

    void get_pawn_moves(std::vector<GameState>& states);
    void get_knight_moves(std::vector<GameState>& states);
    void get_bishop_moves(std::vector<GameState>& states);
    void get_rook_moves(std::vector<GameState>& states);
    void get_queen_moves(std::vector<GameState>& states);
    void get_king_moves(std::vector<GameState>& states);
    void get_castle_moves(std::vector<GameState>& states);
    
    void set(Piece& p, Square& s);
    void set_empty();
    unsigned long long get_bitboard();

    void test_switch_turn();

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
    bool white_castle_right;
    bool white_castle_left;
    bool black_castle_right;
    bool black_castle_left;

    unsigned long long get_white_bitboard();
    unsigned long long get_black_bitboard();
};