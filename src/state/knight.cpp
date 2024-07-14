#include "state.h"



void GameState::get_knight_moves(std::vector<GameState>& states) {
    //const unsigned long long occupied_space = get_bitboard();
    //const unsigned long long open_space = ~occupied_space;

    const unsigned long long enemy_board = white_turn ? get_black_bitboard() : get_white_bitboard();
    const unsigned long long friendly_board = white_turn ? get_white_bitboard() : get_black_bitboard();

    unsigned long long mask_bit = 0x1;

    std::vector<GameState> states_to_add;

    Piece knight_piece;
    unsigned long long friendly_knights;
    if (white_turn) {
        knight_piece.set(Piece::PieceEncoding::WhiteKnight);
        friendly_knights = white_knights;
    } else {
        knight_piece.set(Piece::PieceEncoding::BlackKnight);
        friendly_knights = black_knights;
    }

    for (int rank = 1; rank <= 8; rank++) {
        for (int file = 8; file >= 1; file--) {

            const unsigned long long knight = mask_bit & friendly_knights;
            if (knight != 0) {
                // 8 different moves

                // up right
                if (rank <= 6 && file <= 7) {
                    const unsigned long long jump = knight << 15;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_knights ^= knight;
                            check_state.white_knights |= jump;
                        } else {
                            check_state.black_knights ^= knight;
                            check_state.black_knights |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_knights ^= knight;
                                state.white_knights |= jump;
                            } else {
                                state.black_knights ^= knight;
                                state.black_knights |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+2, file+1},
                                turn,
                                knight_piece,
                                ((jump & enemy_board) != 0)
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }

                    }
                }

                // right up
                if (rank <= 7 && file <= 6) {
                    const unsigned long long jump = knight << 6;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_knights ^= knight;
                            check_state.white_knights |= jump;
                        } else {
                            check_state.black_knights ^= knight;
                            check_state.black_knights |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_knights ^= knight;
                                state.white_knights |= jump;
                            } else {
                                state.black_knights ^= knight;
                                state.black_knights |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+1, file+2},
                                turn,
                                knight_piece,
                                ((jump & enemy_board) != 0)
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // right down
                if (rank >= 2 && file <= 6) {
                    const unsigned long long jump = knight >> 10;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_knights ^= knight;
                            check_state.white_knights |= jump;
                        } else {
                            check_state.black_knights ^= knight;
                            check_state.black_knights |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_knights ^= knight;
                                state.white_knights |= jump;
                            } else {
                                state.black_knights ^= knight;
                                state.black_knights |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-1, file+2},
                                turn,
                                knight_piece,
                                ((jump & enemy_board) != 0)
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // down right
                if (rank >= 3 && file <= 7) {
                    const unsigned long long jump = knight >> 17;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_knights ^= knight;
                            check_state.white_knights |= jump;
                        } else {
                            check_state.black_knights ^= knight;
                            check_state.black_knights |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_knights ^= knight;
                                state.white_knights |= jump;
                            } else {
                                state.black_knights ^= knight;
                                state.black_knights |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-2, file+1},
                                turn,
                                knight_piece,
                                ((jump & enemy_board) != 0)
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // down left
                if (rank >= 3 && file >= 2) {
                    const unsigned long long jump = knight >> 15;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_knights ^= knight;
                            check_state.white_knights |= jump;
                        } else {
                            check_state.black_knights ^= knight;
                            check_state.black_knights |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_knights ^= knight;
                                state.white_knights |= jump;
                            } else {
                                state.black_knights ^= knight;
                                state.black_knights |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-2, file-1},
                                turn,
                                knight_piece,
                                ((jump & enemy_board) != 0)
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // left down
                if (rank >= 2 && file >= 3) {
                    const unsigned long long jump = knight >> 6;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_knights ^= knight;
                            check_state.white_knights |= jump;
                        } else {
                            check_state.black_knights ^= knight;
                            check_state.black_knights |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_knights ^= knight;
                                state.white_knights |= jump;
                            } else {
                                state.black_knights ^= knight;
                                state.black_knights |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-1, file-2},
                                turn,
                                knight_piece,
                                ((jump & enemy_board) != 0)
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // left up
                if (rank <= 7 && file >= 3) {
                    const unsigned long long jump = knight << 10;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_knights ^= knight;
                            check_state.white_knights |= jump;
                        } else {
                            check_state.black_knights ^= knight;
                            check_state.black_knights |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_knights ^= knight;
                                state.white_knights |= jump;
                            } else {
                                state.black_knights ^= knight;
                                state.black_knights |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+1, file-2},
                                turn,
                                knight_piece,
                                ((jump & enemy_board) != 0)
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    }
                }

                // up left
                if (rank <= 6 && file >= 2) {
                    const unsigned long long jump = knight << 17;
                    if ((jump & friendly_board) == 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_knights ^= knight;
                            check_state.white_knights |= jump;
                        } else {
                            check_state.black_knights ^= knight;
                            check_state.black_knights |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_knights ^= knight;
                                state.white_knights |= jump;
                            } else {
                                state.black_knights ^= knight;
                                state.black_knights |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+2, file-1},
                                turn,
                                knight_piece,
                                ((jump & enemy_board) != 0)
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