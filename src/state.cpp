#include "state.h"

unsigned long long GameState::get_bitboard() {
    unsigned long long bitboard = get_white_bitboard() | get_black_bitboard();
    return bitboard;
}


unsigned long long GameState::get_white_bitboard() {
    unsigned long long bitboard = 0;
    bitboard |= white_kings | white_queens | white_rooks |
    white_bishops | white_knights | white_pawns;
    
    return bitboard;
}

unsigned long long GameState::get_black_bitboard() {
    unsigned long long bitboard = 0;
    bitboard |= black_kings | black_queens | black_rooks |
    black_bishops | black_knights | black_pawns;

    return bitboard;
}

void GameState::get_pawn_moves(std::vector<GameState>& states) {
    const unsigned long long occupied_space = get_bitboard();
    const unsigned long long open_space = ~occupied_space;

    const unsigned long long white_board = get_white_bitboard();
    const unsigned long long black_board = get_black_bitboard();

    unsigned long long mask_bit;

    std::vector<GameState> states_to_add;

    if (white_turn) {
        Piece white_pawn_piece;
        white_pawn_piece.set(Piece::PieceEncoding::WhitePawn);

        // No white pawns can be in rank 1 or 8
        mask_bit = 0x100;
        for (int rank = 2; rank < 8; rank++) {
            for (int file = 1; file <= 8; file++) {
                unsigned long long pawn = mask_bit & white_pawns;

                // there is a white pawn at bit "pawn"
                if (pawn != 0) {
                    // white pawn move one forward
                    unsigned long long jump_one = pawn << 8;
                    if ((jump_one & open_space) != 0) {
                        GameState state = *this;
                        state.white_pawns ^= pawn;
                        state.white_pawns |= jump_one;
                        state.white_turn = false;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {rank+1, file},
                            turn,
                            white_pawn_piece,
                            false
                        };
                        states_to_add.push_back(state);
                    }

                    // white pawn move two forward
                    if (rank == 2) {
                        unsigned long long jump_two = pawn << 16;
                        if (((jump_two | jump_one) & occupied_space) == 0) {
                            GameState state = *this;
                            state.white_pawns ^= pawn;
                            state.white_pawns |= jump_two;
                            state.white_turn = false;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+2, file},
                                turn,
                                white_pawn_piece,
                                false
                            };
                            states_to_add.push_back(state); 
                        }
                    }

                    // white pawn takes left
                    if (file != 1) {
                        unsigned long long take_left = pawn << 9;
                        if ((take_left & black_board) != 0) {
                            GameState state = *this;
                            state.white_pawns ^= pawn;
                            state.white_pawns |= take_left;
                            state.white_turn = false;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+1, file-1},
                                turn,
                                white_pawn_piece,
                                true
                            };
                            states_to_add.push_back(state); 
                        }
                    }

                    // white pawn takes right
                    if (file != 8) {
                        unsigned long long take_right = pawn << 7;
                        if ((take_right & black_board) != 0) {
                            GameState state = *this;
                            state.white_pawns ^= pawn;
                            state.white_pawns |= take_right;
                            state.white_turn = false;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+1, file+1},
                                turn,
                                white_pawn_piece,
                                true
                            };
                            states_to_add.push_back(state); 
                        }
                    }

                }

                mask_bit <<= 1;
            }
        }
    } else {
        Piece black_pawn_piece;
        black_pawn_piece.set(Piece::PieceEncoding::BlackPawn);

        // No black pawns can be in rank 1 or 8
        mask_bit = 0x100;
        for (int rank = 2; rank < 8; rank++) {
            for (int file = 1; file <= 8; file++) {
                unsigned long long pawn = mask_bit & black_pawns;

                // there is a black pawn at bit "pawn"
                if (pawn != 0) {
                    // black pawn move one forward
                    unsigned long long jump_one = pawn >> 8;
                    if ((jump_one & open_space) != 0) {
                        GameState state = *this;
                        state.black_pawns ^= pawn;
                        state.black_pawns |= jump_one;
                        state.white_turn = true;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {rank-1, file},
                            turn,
                            black_pawn_piece,
                            false
                        };
                        states_to_add.push_back(state);
                    }

                    // black pawn move two forward
                    if (rank == 7) {
                        unsigned long long jump_two = pawn >> 16;
                        if (((jump_two | jump_one) & occupied_space) == 0) {
                            GameState state = *this;
                            state.black_pawns ^= pawn;
                            state.black_pawns |= jump_two;
                            state.white_turn = true;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-2, file},
                                turn,
                                black_pawn_piece,
                                false
                            };
                            states_to_add.push_back(state); 
                        }
                    }

                    // black pawn takes left
                    if (file != 8) {
                        unsigned long long take_left = pawn >> 9;
                        if ((take_left & white_board) != 0) {
                            GameState state = *this;
                            state.black_pawns ^= pawn;
                            state.black_pawns |= take_left;
                            state.white_turn = true;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-1, file+1},
                                turn,
                                black_pawn_piece,
                                true
                            };
                            states_to_add.push_back(state); 
                        }
                    }

                    // black pawn takes right
                    if (file != 1) {
                        unsigned long long take_right = pawn << 7;
                        if ((take_right & black_board) != 0) {
                            GameState state = *this;
                            state.black_pawns ^= pawn;
                            state.black_pawns |= take_right;
                            state.white_turn = true;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-1, file-1},
                                turn,
                                black_pawn_piece,
                                true
                            };
                            states_to_add.push_back(state); 
                        }
                    }

                }

                mask_bit <<= 1;
            }
        }
        
    }
    for (auto s: states_to_add) {
        states.push_back(s);
    }
}



