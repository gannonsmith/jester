#include "state.h"

void GameState::get_castle_moves(std::vector<GameState>& states) {
    const unsigned long long occupied_space = get_bitboard();
    //const unsigned long long open_space = ~occupied_space;

    //const unsigned long long enemy_board = white_turn ? get_black_bitboard() : get_white_bitboard();
    //const unsigned long long friendly_board = white_turn ? get_white_bitboard() : get_black_bitboard();

    std::vector<GameState> states_to_add;

    Piece king_piece;
    if (white_turn) {
        king_piece.set(Piece::PieceEncoding::WhiteKing);
    } else {
        king_piece.set(Piece::PieceEncoding::BlackKing);
    }
    const unsigned long long a1 = 0x80;
    const unsigned long long e1 = 0x8;
    const unsigned long long h1 = 0x1;
    
    const unsigned long long a8 = 0x8000000000000000;
    const unsigned long long e8 = 0x800000000000000;
    const unsigned long long h8 = 0x100000000000000;

    if (white_turn && (e1 & white_kings) != 0) {
        if (white_castle_right && (h1 & white_rooks) != 0) {
            const unsigned long long between = 0x6;
            const unsigned long long f1 = 0x4;
            const unsigned long long g1 = 0x2;
            if ((between & occupied_space) == 0 && !space_check(e1) && !space_check(f1) && !space_check(g1)) {
                //can castle right
                GameState state = *this;
                state.white_kings ^= e1;
                state.white_kings |= g1;
                state.white_rooks ^= h1;
                state.white_rooks |= e1;
                
                state.white_turn = !white_turn;
                state.turn++;
                state.prev_move = {
                    {1, 5},
                    {1, 7},
                    turn,
                    king_piece,
                    false
                };
                state.white_castle_right = false;
                state.white_castle_left = false;
                state.under_attack = state.generate_capture_spaces();
                states_to_add.push_back(state); 
            }
        }
        if (white_castle_left && (a1 & white_rooks) != 0) {
            const unsigned long long between = 0x70;
            const unsigned long long b1 = 0x40;
            const unsigned long long c1 = 0x20;
            const unsigned long long d1 = 0x10;
            if ((between & occupied_space) == 0 && !space_check(b1) && !space_check(c1) && !space_check(d1) && !space_check(e1)) {
                //can castle right
                GameState state = *this;
                state.white_kings ^= e1;
                state.white_kings |= c1;
                state.white_rooks ^= a1;
                state.white_rooks |= d1;
                
                state.white_turn = !white_turn;
                state.turn++;
                state.prev_move = {
                    {1, 5},
                    {1, 3},
                    turn,
                    king_piece,
                    false
                };
                state.white_castle_right = false;
                state.white_castle_left = false;
                state.under_attack = state.generate_capture_spaces();
                states_to_add.push_back(state); 
            }
        }
    } else if (!white_turn && (e8 & black_kings) != 0) {
        if (black_castle_right && (h8 & black_rooks) != 0) {
            const unsigned long long between = 0x600000000000000;
            const unsigned long long f8 = 0x400000000000000;
            const unsigned long long g8 = 0x200000000000000;
            if ((between & occupied_space) == 0 && !space_check(e8) && !space_check(f8) && !space_check(g8)) {
                //can castle right
                GameState state = *this;
                state.black_kings ^= e8;
                state.black_kings |= g8;
                state.black_rooks ^= h8;
                state.black_rooks |= e8;
                
                state.white_turn = !white_turn;
                state.turn++;
                state.prev_move = {
                    {8, 5},
                    {8, 7},
                    turn,
                    king_piece,
                    false
                };
                state.black_castle_right = false;
                state.black_castle_left = false;
                state.under_attack = state.generate_capture_spaces();
                states_to_add.push_back(state); 
            }
        }
        if (black_castle_left && (a8 & black_rooks) != 0) {
            const unsigned long long between = 0x7000000000000000;
            const unsigned long long b8 = 0x4000000000000000;
            const unsigned long long c8 = 0x2000000000000000;
            const unsigned long long d8 = 0x1000000000000000;
            if ((between & occupied_space) == 0 && !space_check(b8) && !space_check(c8) && !space_check(d8) && !space_check(e8)) {
                //can castle right
                GameState state = *this;
                state.black_kings ^= e8;
                state.black_kings |= c8;
                state.black_rooks ^= a8;
                state.black_rooks |= d8;
                
                state.white_turn = !white_turn;
                state.turn++;
                state.prev_move = {
                    {8, 5},
                    {8, 3},
                    turn,
                    king_piece,
                    false
                };
                state.black_castle_right = false;
                state.black_castle_left = false;
                state.under_attack = state.generate_capture_spaces();
                states_to_add.push_back(state); 
            }
        }
    }

    for (auto s: states_to_add) {
        states.push_back(s);
    }
}
