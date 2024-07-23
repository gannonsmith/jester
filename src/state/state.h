#pragma once

#include <vector>
#include "../piece.h"
#include "../helper.h"

class GameState {
    public:

    /*
    bitboard showing which spaces are currently under "check" by the opponent
    */
    unsigned long long under_attack;

    /*
    generates all board states to the depth given
    */
    void get_states(std::vector<std::vector<GameState>>& states, int depth);

    /*
    removes piece from enemy board at position
    */
    void remove_capture(bool white_takes, unsigned long long position);

    /*
    regenerates the "under_attack" variable
    */
    unsigned long long generate_capture_spaces();

    /*
    Returns true if sqaure at position is under attack, false otherwise
    */
    bool space_check(const unsigned long long position);

    /*
    Returns true if the king is in check, false otherwise
    */
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

    const unsigned long long get_friendly_board();
    const unsigned long long get_enemy_board();

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

    public:
    unsigned long long get_white_bitboard();
    unsigned long long get_black_bitboard();
};