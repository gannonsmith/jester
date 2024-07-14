#include "state.h"


void GameState::get_bishop_moves(std::vector<GameState>& states) {
    const unsigned long long occupied_space = get_bitboard();
    const unsigned long long open_space = ~occupied_space;

    const unsigned long long friendly_board = white_turn ? get_white_bitboard() : get_black_bitboard();
    const unsigned long long enemy_board = white_turn ? get_black_bitboard() : get_white_bitboard();

    unsigned long long mask_bit = 0x1;

    std::vector<GameState> states_to_add;

    Piece bishop_piece;
    unsigned long long friendly_bishops;
    if (white_turn) {
        bishop_piece.set(Piece::PieceEncoding::WhiteKnight);
        friendly_bishops = white_bishops;
    } else {
        bishop_piece.set(Piece::PieceEncoding::BlackKnight);
        friendly_bishops = black_bishops;
    }

    for (int rank = 1; rank <= 8; rank++) {
        for (int file = 8; file >= 1; file--) {

            const unsigned long long bishop = mask_bit & friendly_bishops;

            if (bishop != 0) {
                //bishop exists
                unsigned long long jump;
                Square slider;

                // top right
                slider.rank = rank+1;
                slider.file = file+1;
                jump = bishop;
                while (slider.rank <= 8 && slider.file <= 8) {
                    jump <<= 7;

                    if ((jump & open_space) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_bishops ^= bishop;
                            check_state.white_bishops |= jump;
                        } else {
                            check_state.black_bishops ^= bishop;
                            check_state.black_bishops |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_bishops ^= bishop;
                                state.white_bishops |= jump;
                            } else {
                                state.black_bishops ^= bishop;
                                state.black_bishops |= jump;
                            }
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                bishop_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    } else if ((jump & enemy_board) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_bishops ^= bishop;
                            check_state.white_bishops |= jump;
                        } else {
                            check_state.black_bishops ^= bishop;
                            check_state.black_bishops |= jump;
                        }
                        check_state.remove_capture(white_turn, jump);  
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_bishops ^= bishop;
                                state.white_bishops |= jump;
                            } else {
                                state.black_bishops ^= bishop;
                                state.black_bishops |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                bishop_piece,
                                true
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                        break;
                    } else if ((jump & friendly_board) != 0) {
                        break;
                    }
                    slider.rank++;
                    slider.file++;
                }

                // bottom right
                slider.rank = rank-1;
                slider.file = file+1;
                jump = bishop;
                while (slider.rank >= 1 && slider.file <= 8) {
                    jump >>= 9;

                    if ((jump & open_space) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_bishops ^= bishop;
                            check_state.white_bishops |= jump;
                        } else {
                            check_state.black_bishops ^= bishop;
                            check_state.black_bishops |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_bishops ^= bishop;
                                state.white_bishops |= jump;
                            } else {
                                state.black_bishops ^= bishop;
                                state.black_bishops |= jump;
                            }
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                bishop_piece,
                                false
                            };
                            states_to_add.push_back(state); 
                        }
                    } else if ((jump & enemy_board) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_bishops ^= bishop;
                            check_state.white_bishops |= jump;
                        } else {
                            check_state.black_bishops ^= bishop;
                            check_state.black_bishops |= jump;
                        }
                        check_state.remove_capture(white_turn, jump);
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_bishops ^= bishop;
                                state.white_bishops |= jump;
                            } else {
                                state.black_bishops ^= bishop;
                                state.black_bishops |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                bishop_piece,
                                true
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                        break;
                    } else if ((jump & friendly_board) != 0) {
                        break;
                    }
                    slider.rank--;
                    slider.file++;
                }

                // bottom left
                slider.rank = rank-1;
                slider.file = file-1;
                jump = bishop;
                while (slider.rank >= 1 && slider.file >= 1) {
                    jump >>= 7;

                    if ((jump & open_space) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_bishops ^= bishop;
                            check_state.white_bishops |= jump;
                        } else {
                            check_state.black_bishops ^= bishop;
                            check_state.black_bishops |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_bishops ^= bishop;
                                state.white_bishops |= jump;
                            } else {
                                state.black_bishops ^= bishop;
                                state.black_bishops |= jump;
                            }
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                bishop_piece,
                                false
                            };
                            states_to_add.push_back(state); 
                        }
                    } else if ((jump & enemy_board) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_bishops ^= bishop;
                            check_state.white_bishops |= jump;
                        } else {
                            check_state.black_bishops ^= bishop;
                            check_state.black_bishops |= jump;
                        }
                        check_state.remove_capture(white_turn, jump);
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_bishops ^= bishop;
                                state.white_bishops |= jump;
                            } else {
                                state.black_bishops ^= bishop;
                                state.black_bishops |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                bishop_piece,
                                true
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                        break;
                    } else if ((jump & friendly_board) != 0) {
                        break;
                    }
                    slider.rank--;
                    slider.file--;
                }

                // top left
                slider.rank = rank+1;
                slider.file = file-1;
                jump = bishop;
                while (slider.rank <= 8 && slider.file >= 1) {
                    jump <<= 9;

                    if ((jump & open_space) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_bishops ^= bishop;
                            check_state.white_bishops |= jump;
                        } else {
                            check_state.black_bishops ^= bishop;
                            check_state.black_bishops |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_bishops ^= bishop;
                                state.white_bishops |= jump;
                            } else {
                                state.black_bishops ^= bishop;
                                state.black_bishops |= jump;
                            }
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                bishop_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    } else if ((jump & enemy_board) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_bishops ^= bishop;
                            check_state.white_bishops |= jump;
                        } else {
                            check_state.black_bishops ^= bishop;
                            check_state.black_bishops |= jump;
                        }
                        check_state.remove_capture(white_turn, jump);
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_bishops ^= bishop;
                                state.white_bishops |= jump;
                            } else {
                                state.black_bishops ^= bishop;
                                state.black_bishops |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                bishop_piece,
                                true
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                        break;
                    } else if ((jump & friendly_board) != 0) {
                        break;
                    }
                    slider.rank++;
                    slider.file--;
                }

            }
            mask_bit <<= 1;
        }
    }

    for (auto s: states_to_add) {
        states.push_back(s);
    }
}