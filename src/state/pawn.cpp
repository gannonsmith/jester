#include "state.h"

void GameState::get_pawn_moves(std::vector<GameState>& states) {
    const unsigned long long occupied_space = get_bitboard();
    const unsigned long long open_space = ~occupied_space;

    const unsigned long long white_board = get_white_bitboard();
    const unsigned long long black_board = get_black_bitboard();

    unsigned long long mask_bit = 0x1;

    std::vector<GameState> states_to_add;

    if (white_turn) {
        std::cout << "here" << std::endl;
        
        Piece white_pawn_piece;
        white_pawn_piece.set(Piece::PieceEncoding::WhitePawn);

        for (int rank = 1; rank <= 8; rank++) {
            for (int file = 8; file >= 1; file--) {
                unsigned long long pawn = mask_bit & white_pawns;

                // there is a white pawn at bit "pawn"
                if (pawn != 0) {
                    // white pawn move one forward
                    unsigned long long jump_one = pawn << 8;
                    if ((jump_one & open_space) != 0) {
                        GameState check_state = *this;
                        check_state.white_pawns ^= pawn;
                        check_state.white_pawns |= jump_one;
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            // handle upgrading pawn
                            if (rank == 7) {
                                GameState state = *this;
                                state.white_pawns ^= pawn;
                                state.white_turn = !white_turn;
                                state.turn++;
                                state.prev_move = {
                                    {rank, file},
                                    {rank+1, file},
                                    turn,
                                    white_pawn_piece,
                                    false
                                };
                            
                                state.white_queens |= jump_one;
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state);
                                state.white_queens ^= jump_one;

                                state.white_rooks |= jump_one;
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state);
                                state.white_rooks ^= jump_one;

                                state.white_bishops |= jump_one;
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state);
                                state.white_rooks ^= jump_one;

                                state.white_bishops |= jump_one;
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state);
                                state.white_knights ^= jump_one;
                            } else {
                                GameState state = *this;
                                state.white_pawns ^= pawn;
                                state.white_pawns |= jump_one;
                                state.white_turn = !white_turn;
                                state.turn++;
                                state.prev_move = {
                                    {rank, file},
                                    {rank+1, file},
                                    turn,
                                    white_pawn_piece,
                                    false
                                };
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state);
                            }
                        }
                    }

                    // white pawn move two forward
                    if (rank == 2) {
                        unsigned long long jump_two = pawn << 16;
                        if (((jump_two | jump_one) & occupied_space) == 0) {
                            GameState check_state = *this;
                            check_state.white_pawns ^= pawn;
                            check_state.white_pawns |= jump_two;
                            check_state.under_attack = check_state.generate_capture_spaces();
                            if (!check_state.king_check()) {
                                GameState state = *this;
                                state.white_pawns ^= pawn;
                                state.white_pawns |= jump_two;
                                state.white_turn = !white_turn;
                                state.turn++;
                                state.prev_move = {
                                    {rank, file},
                                    {rank+2, file},
                                    turn,
                                    white_pawn_piece,
                                    false
                                };
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state); 
                            }
                        }
                    }

                    // white pawn takes left
                    if (file != 1) {
                        unsigned long long take_left = pawn << 9;
                        if ((take_left & black_board) != 0) {
                            GameState check_state = *this;
                            check_state.white_pawns ^= pawn;
                            check_state.white_pawns |= take_left;
                            check_state.under_attack = check_state.generate_capture_spaces();
                            if (!check_state.king_check()) {
                                if (rank == 7) {
                                    GameState state_template = *this;
                                    state_template.white_pawns ^= pawn;
                                    state_template.white_turn = !white_turn;
                                    state_template.turn++;
                                    state_template.prev_move = {
                                        {rank, file},
                                        {rank+1, file-1},
                                        turn,
                                        white_pawn_piece,
                                        true
                                    };

                                    GameState state = state_template;
                                    state.white_queens |= take_left;
                                    state.remove_capture(true, take_left);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.white_rooks |= take_left;
                                    state.remove_capture(true, take_left);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.white_bishops |= take_left;
                                    state.remove_capture(true, take_left);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.white_bishops |= take_left;
                                    state.remove_capture(true, take_left);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);
                                } else {
                                    GameState state = *this;
                                    state.white_pawns ^= pawn;
                                    state.white_pawns |= take_left;
                                    state.remove_capture(true, take_left);
                                    state.white_turn = !white_turn;
                                    state.turn++;
                                    state.prev_move = {
                                        {rank, file},
                                        {rank+1, file-1},
                                        turn,
                                        white_pawn_piece,
                                        true
                                    };
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);
                                } 
                            }
                        }
                    }

                    // white pawn takes right
                    if (file != 8) {
                        unsigned long long take_right = pawn << 7;
                        if ((take_right & black_board) != 0) {
                            GameState check_state = *this;
                            check_state.white_pawns ^= pawn;
                            check_state.white_pawns |= take_right;
                            check_state.under_attack = check_state.generate_capture_spaces();
                            if (!check_state.king_check()) {
                                if (rank == 7) {
                                    GameState state_template = *this;
                                    state_template.white_pawns ^= pawn;
                                    state_template.white_turn = !white_turn;
                                    state_template.turn++;
                                    state_template.prev_move = {
                                        {rank, file},
                                        {rank+1, file+1},
                                        turn,
                                        white_pawn_piece,
                                        true
                                    };
                                    GameState state = state_template;
                                    state.white_queens |= take_right;
                                    state.remove_capture(true, take_right);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.white_rooks |= take_right;
                                    state.remove_capture(true, take_right);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.white_bishops |= take_right;
                                    state.remove_capture(true, take_right);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.white_bishops |= take_right;
                                    state.remove_capture(true, take_right);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);
                                } else {
                                    GameState state = *this;
                                    state.white_pawns ^= pawn;
                                    state.white_pawns |= take_right;
                                    state.remove_capture(true, take_right);
                                    state.white_turn = !white_turn;
                                    state.turn++;
                                    state.prev_move = {
                                        {rank, file},
                                        {rank+1, file+1},
                                        turn,
                                        white_pawn_piece,
                                        true
                                    };
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state); 
                                }
                            }
                        }
                    }
                }
                mask_bit <<= 1;
            }
        }
    } else {
        //TESTING 
        std::cout << "black pawn move" << std::endl;

        //
        Piece black_pawn_piece;
        black_pawn_piece.set(Piece::PieceEncoding::BlackPawn);

        mask_bit = 0x1;
        for (int rank = 1; rank <= 8; rank++) {
            for (int file = 8; file >= 1; file--) {
                const unsigned long long pawn = mask_bit & black_pawns;

                // there is a black pawn at bit "pawn"
                if (pawn != 0) {
                    // black pawn move one forward
                    unsigned long long jump_one = pawn >> 8;
                    if ((jump_one & open_space) != 0) {
                        GameState check_state = *this;
                        check_state.black_pawns ^= pawn;
                        check_state.black_pawns |= jump_one;
                        check_state.under_attack = check_state.generate_capture_spaces();
                        if (!check_state.king_check()) {
                            if (rank == 2) {
                                GameState state = *this;
                                state.black_pawns ^= pawn;
                                state.white_turn = !white_turn;
                                state.turn++;
                                state.prev_move = {
                                    {rank, file},
                                    {rank-1, file},
                                    turn,
                                    black_pawn_piece,
                                    false
                                };
                                state.black_queens |= jump_one;
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state);
                                state.black_queens ^= jump_one;

                                state.black_rooks |= jump_one;
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state);
                                state.black_rooks ^= jump_one;

                                state.black_bishops |= jump_one;
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state);
                                state.black_bishops ^= jump_one;

                                state.black_knights |= jump_one;
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state);
                                state.black_knights ^= jump_one;
                            } else {
                                GameState state = *this;
                                state.black_pawns ^= pawn;
                                state.black_pawns |= jump_one;
                                state.white_turn = !white_turn;
                                state.turn++;
                                state.prev_move = {
                                    {rank, file},
                                    {rank-1, file},
                                    turn,
                                    black_pawn_piece,
                                    false
                                };
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state);
                            }
                        }
                    }

                    // black pawn move two forward
                    if (rank == 7) {
                        unsigned long long jump_two = pawn >> 16;
                        if (((jump_two | jump_one) & occupied_space) == 0) {
                            GameState check_state = *this;
                            check_state.black_pawns ^= pawn;
                            check_state.black_pawns |= jump_two;
                            check_state.under_attack = check_state.generate_capture_spaces();
                            if (!check_state.king_check()) {
                                GameState state = *this;
                                state.black_pawns ^= pawn;
                                state.black_pawns |= jump_two;
                                state.white_turn = !white_turn;
                                state.turn++;
                                state.prev_move = {
                                    {rank, file},
                                    {rank-2, file},
                                    turn,
                                    black_pawn_piece,
                                    false
                                };
                                state.under_attack = state.generate_capture_spaces();
                                states_to_add.push_back(state); 
                            }
                        }
                    }

                    // black pawn takes left
                    if (file != 8) {
                        unsigned long long take_left = pawn >> 9;
                        if ((take_left & white_board) != 0) {
                            GameState check_state = *this;
                            check_state.black_pawns ^= pawn;
                            check_state.black_pawns |= take_left;
                            check_state.under_attack = check_state.generate_capture_spaces();
                            if (!check_state.king_check()) {
                                if (rank == 2) {
                                    GameState state_template = *this;
                                    state_template.black_pawns ^= pawn;
                                    state_template.white_turn = !white_turn;
                                    state_template.turn++;
                                    state_template.prev_move = {
                                        {rank, file},
                                        {rank-1, file+1},
                                        turn,
                                        black_pawn_piece,
                                        true
                                    };

                                    GameState state = state_template;
                                    state.black_queens |= take_left;
                                    state.remove_capture(false, take_left);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.black_rooks |= take_left;
                                    state.remove_capture(false, take_left);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.black_bishops |= take_left;
                                    state.remove_capture(false, take_left);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.black_knights |= take_left;
                                    state.remove_capture(false, take_left);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);
                                } else {
                                    GameState state = *this;
                                    state.black_pawns ^= pawn;
                                    state.black_pawns |= take_left;
                                    state.remove_capture(false, take_left);
                                    state.white_turn = !white_turn;
                                    state.turn++;
                                    state.prev_move = {
                                        {rank, file},
                                        {rank-1, file+1},
                                        turn,
                                        black_pawn_piece,
                                        true
                                    };
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state); 
                                }
                            }
                        }
                    }

                    // black pawn takes right
                    if (file != 1) {
                        unsigned long long take_right = pawn >> 7;
                        if ((take_right & black_board) != 0) {
                            GameState check_state = *this;
                            check_state.black_pawns ^= pawn;
                            check_state.black_pawns |= take_right;
                            check_state.under_attack = check_state.generate_capture_spaces();
                            if (!check_state.king_check()) {
                                if (rank == 2) {
                                    GameState state_template = *this;
                                    state_template.black_pawns ^= pawn;
                                    state_template.white_turn = !white_turn;
                                    state_template.turn++;
                                    state_template.prev_move = {
                                        {rank, file},
                                        {rank-1, file-1},
                                        turn,
                                        black_pawn_piece,
                                        true
                                    };
                                    GameState state = state_template;
                                    state.black_queens |= take_right;
                                    state.remove_capture(false, take_right);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.black_rooks |= take_right;
                                    state.remove_capture(false, take_right);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.black_bishops |= take_right;
                                    state.remove_capture(false, take_right);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);

                                    state = state_template;
                                    state.black_knights |= take_right;
                                    state.remove_capture(false, take_right);
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state);
                                } else {
                                    GameState state = *this;
                                    state.black_pawns ^= pawn;
                                    state.black_pawns |= take_right;
                                    state.remove_capture(false, take_right);
                                    state.white_turn = !white_turn;
                                    state.turn++;
                                    state.prev_move = {
                                        {rank, file},
                                        {rank-1, file-1},
                                        turn,
                                        black_pawn_piece,
                                        true
                                    };
                                    state.under_attack = state.generate_capture_spaces();
                                    states_to_add.push_back(state); 
                                }
                            }
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