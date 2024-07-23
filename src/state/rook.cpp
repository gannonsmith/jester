#include "state.h"


void GameState::get_rook_moves(std::vector<GameState>& states) {
    const unsigned long long occupied_space = get_bitboard();
    const unsigned long long open_space = ~occupied_space;

    const unsigned long long enemy_board = get_enemy_board();
    const unsigned long long friendly_board = get_friendly_board();
    
    unsigned long long mask_bit = 0x1;

    std::vector<GameState> states_to_add;

    Piece rook_piece;
    unsigned long long friendly_rooks;
    if (white_turn) {
        rook_piece.set(Piece::PieceEncoding::WhiteRook);
        friendly_rooks = white_rooks;
    } else {
        rook_piece.set(Piece::PieceEncoding::BlackRook);
        friendly_rooks = black_rooks;
    }

    for (int rank = 1; rank <= 8; rank++) {
        for (int file = 8; file >= 1; file--) {

            const unsigned long long rook = mask_bit & friendly_rooks;

            if (rook != 0) {
                //rook exists
                unsigned long long jump;
                Square slider;

                // up
                slider.rank = rank+1;
                slider.file = file;
                jump = rook;
                while (slider.rank <= 8) {
                    jump <<= 8;

                    if ((jump & open_space) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_rooks ^= rook;
                            check_state.white_rooks |= jump;
                        } else {
                            check_state.black_rooks ^= rook;
                            check_state.black_rooks |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_rooks ^= rook;
                                state.white_rooks |= jump;
                            } else {
                                state.black_rooks ^= rook;
                                state.black_rooks |= jump;
                            }
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                rook_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    } else if ((jump & enemy_board) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_rooks ^= rook;
                            check_state.white_rooks |= jump;
                        } else {
                            check_state.black_rooks ^= rook;
                            check_state.black_rooks |= jump;
                        }
                        check_state.remove_capture(white_turn, jump);
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_rooks ^= rook;
                                state.white_rooks |= jump;
                            } else {
                                state.black_rooks ^= rook;
                                state.black_rooks |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                rook_piece,
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
                }

                // right
                slider.rank = rank;
                slider.file = file+1;
                jump = rook;
                while (slider.file <= 8) {
                    jump >>= 1;

                    if ((jump & open_space) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_rooks ^= rook;
                            check_state.white_rooks |= jump;
                        } else {
                            check_state.black_rooks ^= rook;
                            check_state.black_rooks |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) { 
                            GameState state = *this;
                            if (white_turn) {
                                state.white_rooks ^= rook;
                                state.white_rooks |= jump;
                            } else {
                                state.black_rooks ^= rook;
                                state.black_rooks |= jump;
                            }
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                rook_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    } else if ((jump & enemy_board) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_rooks ^= rook;
                            check_state.white_rooks |= jump;
                        } else {
                            check_state.black_rooks ^= rook;
                            check_state.black_rooks |= jump;
                        }
                        check_state.remove_capture(white_turn, jump);
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {   
                            GameState state = *this;
                            if (white_turn) {
                                state.white_rooks ^= rook;
                                state.white_rooks |= jump;
                            } else {
                                state.black_rooks ^= rook;
                                state.black_rooks |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                rook_piece,
                                true
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                        break;
                    } else if ((jump & friendly_board) != 0) {
                        break;
                    }
                    slider.file++;
                }

                // down
                slider.rank = rank-1;
                slider.file = file;
                jump = rook;
                while (slider.rank >= 1) {
                    jump >>= 8;

                    if ((jump & open_space) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_rooks ^= rook;
                            check_state.white_rooks |= jump;
                        } else {
                            check_state.black_rooks ^= rook;
                            check_state.black_rooks |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {  
                            GameState state = *this;
                            if (white_turn) {
                                state.white_rooks ^= rook;
                                state.white_rooks |= jump;
                            } else {
                                state.black_rooks ^= rook;
                                state.black_rooks |= jump;
                            }
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                rook_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    } else if ((jump & enemy_board) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_rooks ^= rook;
                            check_state.white_rooks |= jump;
                        } else {
                            check_state.black_rooks ^= rook;
                            check_state.black_rooks |= jump;
                        }
                        check_state.remove_capture(white_turn, jump);
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_rooks ^= rook;
                                state.white_rooks |= jump;
                            } else {
                                state.black_rooks ^= rook;
                                state.black_rooks |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                rook_piece,
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
                }

                // left
                slider.rank = rank;
                slider.file = file-1;
                jump = rook;
                while (slider.file >= 1) {
                    jump <<= 1;

                    if ((jump & open_space) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_rooks ^= rook;
                            check_state.white_rooks |= jump;
                        } else {
                            check_state.black_rooks ^= rook;
                            check_state.black_rooks |= jump;
                        }
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) { 
                            GameState state = *this;
                            if (white_turn) {
                                state.white_rooks ^= rook;
                                state.white_rooks |= jump;
                            } else {
                                state.black_rooks ^= rook;
                                state.black_rooks |= jump;
                            }
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                rook_piece,
                                false
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                    } else if ((jump & enemy_board) != 0) {
                        GameState check_state = *this;
                        if (white_turn) {
                            check_state.white_rooks ^= rook;
                            check_state.white_rooks |= jump;
                        } else {
                            check_state.black_rooks ^= rook;
                            check_state.black_rooks |= jump;
                        }
                        check_state.remove_capture(white_turn, jump);
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            GameState state = *this;
                            if (white_turn) {
                                state.white_rooks ^= rook;
                                state.white_rooks |= jump;
                            } else {
                                state.black_rooks ^= rook;
                                state.black_rooks |= jump;
                            }
                            state.remove_capture(white_turn, jump);
                            state.white_turn = !white_turn;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {slider.rank, slider.file},
                                turn,
                                rook_piece,
                                true
                            };
                            state.under_attack = state.generate_capture_spaces();
                            states_to_add.push_back(state); 
                        }
                        break;
                    } else if ((jump & friendly_board) != 0) {
                        break;
                    }
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
