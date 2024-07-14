#include "state.h"


void GameState::get_king_moves(std::vector<GameState>& states) {
    //const unsigned long long occupied_space = get_bitboard();
    //const unsigned long long open_space = ~occupied_space;

    //const unsigned long long enemy_board = white_turn ? get_black_bitboard() : get_white_bitboard();
    const unsigned long long friendly_board = white_turn ? get_white_bitboard() : get_black_bitboard();

    unsigned long long mask_bit = 0x1;

    std::vector<GameState> states_to_add;

    Piece king_piece;
    unsigned long long friendly_kings;
    if (white_turn) {
        king_piece.set(Piece::PieceEncoding::WhiteKing);
        friendly_kings = white_kings;
    } else {
        king_piece.set(Piece::PieceEncoding::BlackKing);
        friendly_kings = black_kings;
    }

    for (int rank = 1; rank <= 8; rank++) {
        for (int file = 8; file >= 1; file--) {

            const unsigned long long king = mask_bit & friendly_kings;

            if (king != 0) {
                // king exists

                // up
                if (rank != 8) {
                    unsigned long long jump = king << 8;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_kings ^= king;
                            check_state.white_kings |= jump;
                        } else {
                            check_state.black_kings ^= king;
                            check_state.black_kings |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_kings ^= king;
                                state.white_kings |= jump;
                            } else {
                                state.black_kings ^= king;
                                state.black_kings |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+1, file},
                                turn,
                                king_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state);
                        } 
                    }
                }

                // up right
                if (rank != 8 && file != 8) {
                    unsigned long long jump = king << 7;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_kings ^= king;
                            check_state.white_kings |= jump;
                        } else {
                            check_state.black_kings ^= king;
                            check_state.black_kings |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_kings ^= king;
                                state.white_kings |= jump;
                            } else {
                                state.black_kings ^= king;
                                state.black_kings |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+1, file+1},
                                turn,
                                king_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // right
                if (file != 8) {
                    unsigned long long jump = king >> 1;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_kings ^= king;
                            check_state.white_kings |= jump;
                        } else {
                            check_state.black_kings ^= king;
                            check_state.black_kings |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_kings ^= king;
                                state.white_kings |= jump;
                            } else {
                                state.black_kings ^= king;
                                state.black_kings |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank, file+1},
                                turn,
                                king_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // down right
                if (rank != 1 && file != 8) {
                    unsigned long long jump = king >> 9;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_kings ^= king;
                            check_state.white_kings |= jump;
                        } else {
                            check_state.black_kings ^= king;
                            check_state.black_kings |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_kings ^= king;
                                state.white_kings |= jump;
                            } else {
                                state.black_kings ^= king;
                                state.black_kings |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-1, file+1},
                                turn,
                                king_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // down
                if (rank != 1) {
                    unsigned long long jump = king >> 8;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_kings ^= king;
                            check_state.white_kings |= jump;
                        } else {
                            check_state.black_kings ^= king;
                            check_state.black_kings |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_kings ^= king;
                                state.white_kings |= jump;
                            } else {
                                state.black_kings ^= king;
                                state.black_kings |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-1, file},
                                turn,
                                king_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // down left
                if (rank != 1 && file != 1) {
                    unsigned long long jump = king >> 7;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_kings ^= king;
                            check_state.white_kings |= jump;
                        } else {
                            check_state.black_kings ^= king;
                            check_state.black_kings |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_kings ^= king;
                                state.white_kings |= jump;
                            } else {
                                state.black_kings ^= king;
                                state.black_kings |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-1, file-1},
                                turn,
                                king_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // left
                if (file != 1) {
                    unsigned long long jump = king << 1;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_kings ^= king;
                            check_state.white_kings |= jump;
                        } else {
                            check_state.black_kings ^= king;
                            check_state.black_kings |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_kings ^= king;
                                state.white_kings |= jump;
                            } else {
                                state.black_kings ^= king;
                                state.black_kings |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank, file-1},
                                turn,
                                king_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // up left
                if (rank != 8 && file != 1) {
                    unsigned long long jump = king << 9;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_kings ^= king;
                            check_state.white_kings |= jump;
                        } else {
                            check_state.black_kings ^= king;
                            check_state.black_kings |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_kings ^= king;
                                state.white_kings |= jump;
                            } else {
                                state.black_kings ^= king;
                                state.black_kings |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+1, file-1},
                                turn,
                                king_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }
            }
            mask_bit <<= 1;
        }
    }

    for (auto s: states_to_add) {
        states.push_back(s);
    }
}


