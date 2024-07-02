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
                        states_to_add.push_back(state);
                    }

                    // white pawn move two forward
                    if (rank == 2) {
                        unsigned long long jump_two = jump_one << 8;
                        if (((jump_two | jump_one) & occupied_space) == 0) {
                            GameState state = *this;
                            state.white_pawns ^= pawn;
                            state.white_pawns |= jump_two;
                            states_to_add.push_back(state); 
                        }
                    }

                    if (file != 1) {

                    }

                    if (file != 8) {

                    }

                }

                mask_bit <<= 1;
            }
        }




        Piece white_pawn_piece;
        white_pawn_piece.set(Piece::PieceEncoding::WhitePawn);

        // WHITE PAWN MOVE: ONE FORWARD
        unsigned long long jump_one = white_pawn << 8;
        jump_one &= open_space;
        mask_bit = 65536;
        for (int rank = 3; rank <= 8; rank++) {
            for (int file = 1; file <= 8; file++) {
                if ((mask_bit & jump_one) != 0) {
                    Move m{
                        Square{rank-1, file},
                        Square{rank, file},
                        depth,
                        white_pawn_piece,
                        false
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
        }

        // WHITE PAWN MOVE: TWO FORWARD
        unsigned long long jump_two = (third_rank & jump_one) << 8;
        jump_two &= open_space;
        mask_bit = 16777216;
        for (int file = 1; file <= 8; file++) {
            if ((mask_bit & jump_two) != 0) {
                Move m{
                    Square{2, file},
                    Square{4, file},
                    depth,
                    white_pawn_piece,
                    false
                };
                valid_moves.push_back(m);
            }
            mask_bit <<= 1;
        }
        return;

        // WHITE PAWN MOVE: TAKE TOP LEFT
        unsigned long long take_left = white.pawn_board.bitboard << 9;
        take_left &= black_board;
        mask_bit = 65536;
        for (int rank = 3; rank <= 8; rank++) {
            mask_bit <<= 1;
            for (int file = 1; file <= 7; file++) {
                if ((mask_bit & take_left) != 0) {
                    Move m{
                        Square{rank-1, file+1},
                        Square{rank, file},
                        depth,
                        white_pawn,
                        true
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
        }

        // WHITE PAWN MOVE: TAKE TOP RIGHT
        unsigned long long take_right = white.pawn_board.bitboard << 7;
        take_right &= black_board;
        mask_bit = 65536;
        for (int rank = 3; rank <= 8; rank++) {
            for (int file = 2; file <= 8; file++) {
                if ((mask_bit & take_left) != 0) {
                    Move m{
                        Square{rank-1, file-1},
                        Square{rank, file},
                        depth,
                        white_pawn,
                        true
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
            mask_bit <<= 1;
        }
    } else {
        Piece black_pawn;
        black_pawn.set(Piece::PieceEncoding::BlackPawn);

        // BLACK PAWN MOVE: ONE FORWARD
        unsigned long long jump_one = black.pawn_board.bitboard >> 8;
        jump_one &= open_space;
        mask_bit = 1;
        for (int rank = 1; rank <= 6; rank++) {
            for (int file = 1; file <= 8; file++) {
                if ((mask_bit & jump_one) != 0) {
                    Move m{
                        Square{rank+1, file},
                        Square{rank, file},
                        depth,
                        black_pawn,
                        false
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
        }

        // BLACK PAWN MOVE: TWO FORWARD
        unsigned long long jump_two = (sixth_rank & jump_one) >> 8;
        jump_two &= open_space;
        mask_bit = 4294967296;
        for (int file = 1; file <= 8; file++) {
            if ((mask_bit & jump_two) != 0) {
                Move m{
                    Square{7, file},
                    Square{5, file},
                    depth,
                    black_pawn,
                    false
                };
                valid_moves.push_back(m);
            }
            mask_bit <<= 1;
        }

        // BLACK PAWN MOVE: TAKE BOTTOM LEFT
        unsigned long long take_left = black.pawn_board.bitboard >> 7;
        take_left &= white_board;
        mask_bit = 1;
        for (int rank = 1; rank <= 6; rank++) {
            mask_bit <<= 1;
            for (int file = 1; file <= 7; file++) {
                if ((mask_bit & take_left) != 0) {
                    Move m{
                        Square{rank+1, file+1},
                        Square{rank, file},
                        depth,
                        black_pawn,
                        true
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
        }

        // BLACK PAWN MOVE: TAKE BOTTOM RIGHT
        unsigned long long take_right = white.pawn_board.bitboard >> 9;
        take_right &= white_board;
        mask_bit = 65536;
        for (int rank = 1; rank <= 6; rank++) {
            for (int file = 2; file <= 8; file++) {
                if ((mask_bit & take_left) != 0) {
                    Move m{
                        Square{rank+1, file-1},
                        Square{rank, file},
                        depth,
                        black_pawn,
                        true
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
            mask_bit <<= 1;
        }
    }
}



