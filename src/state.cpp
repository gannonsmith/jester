#include "state.h"

void GameState::test_switch_turn() {
    white_turn = !white_turn;
}

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

void GameState::set(Piece& p, Square& s) {
    unsigned long long bit = 1;
    bit <<= (s.rank-1)*8 + 8-s.file;
    unsigned long long* ptr;
    switch (p.get()) {
    case Piece::PieceEncoding::WhiteKing:
        ptr = &white_kings;
        break;
    case Piece::PieceEncoding::WhiteQueen:
        ptr = &white_queens;
        break;
    case Piece::PieceEncoding::WhiteRook:
        ptr = &white_rooks;
        break;
    case Piece::PieceEncoding::WhiteBishop:
        ptr = &white_bishops;
        break;
    case Piece::PieceEncoding::WhiteKnight:
        ptr = &white_knights;
        break;
    case Piece::PieceEncoding::WhitePawn:
        ptr = &white_pawns;
        break;
    case Piece::PieceEncoding::BlackKing:
        ptr = &black_kings;
        break;
    case Piece::PieceEncoding::BlackQueen:
        ptr = &black_queens;
        break;
    case Piece::PieceEncoding::BlackRook:
        ptr = &black_rooks;
        break;
    case Piece::PieceEncoding::BlackBishop:
        ptr = &black_bishops;
        break;
    case Piece::PieceEncoding::BlackKnight:
        ptr = &black_knights;
        break;
    case Piece::PieceEncoding::BlackPawn:
        ptr = &black_pawns;
        break;
    default:
        std::cout << "something broke" << std::endl;
        return;
    }
    *ptr |= bit;
}

void GameState::set_empty() {
    white_turn = true;
    turn = 0;
    white_kings = 0;
    white_queens = 0;
    white_rooks = 0;
    white_bishops = 0;
    white_knights = 0;
    white_pawns = 0;
    black_kings = 0;
    black_queens = 0;
    black_rooks = 0;
    black_bishops = 0;
    black_knights = 0;
    black_pawns = 0;
}

void GameState::get_states(std::vector<std::vector<GameState>>& states) {
    states.push_back({});
    get_pawn_moves(states[0]);
    get_knight_moves(states[0]);
    get_bishop_moves(states[0]);
    get_rook_moves(states[0]);
    get_queen_moves(states[0]);
    get_king_moves(states[0]);
}

unsigned long long GameState::create_state(unsigned long long board, unsigned long long moving_piece, unsigned long long new_move) {
    board ^= moving_piece;
    board |= new_move;
    return board;
}

void GameState::remove_capture(bool white_takes, unsigned long long position) {
    if (white_takes) {
        black_kings ^= position;
        black_queens ^= position;
        black_rooks ^= position;
        black_bishops ^= position;
        black_knights ^= position;
        black_pawns ^= position;
    } else {
        white_kings ^= position;
        white_queens ^= position;
        white_rooks ^= position;
        white_bishops ^= position;
        white_knights ^= position;
        white_pawns ^= position;
    }
}

unsigned long long GameState::generate_capture_spaces() {
    unsigned long long captures = 0;

    unsigned long long mask_bit = 1;

    const unsigned long long occupied_board = get_bitboard();

    const unsigned long long enemy_pawns = white_turn ? black_pawns : white_pawns;
    const unsigned long long enemy_knights = white_turn ? black_knights : white_knights;
    const unsigned long long enemy_bishops = white_turn ? black_bishops : white_bishops;
    const unsigned long long enemy_rooks = white_turn ? black_rooks : white_rooks;
    const unsigned long long enemy_queens = white_turn ? black_queens : white_queens;
    const unsigned long long enemy_kings = white_turn ? black_kings : white_kings;

    for (int rank = 1; rank <= 8; rank++) {
        for (int file = 8; file >= 1; file--) {
            if ((enemy_pawns & mask_bit) != 0) {
                if (white_turn) {
                    if (file >= 2) {
                        captures |= mask_bit >> 7;
                    }
                    if (file <= 7) {
                        captures |= mask_bit >> 9;
                    }
                } else {
                    if (file >= 2) {
                        captures |= mask_bit << 9;
                    }
                    if (file <= 7) {
                        captures |= mask_bit << 7;
                    }
                    captures |= mask_bit << 7;
                }
            } else if ((enemy_knights & mask_bit) != 0) {
                if (rank <= 6) {
                    if (rank <= 7) {
                        // left up
                        if (file >= 3) {
                            captures |= mask_bit << 10;
                        }
                        // right up
                        if (file <= 6) {
                            captures |= mask_bit << 6;
                        }
                    }
                    // up left
                    if (file >= 2) {
                        captures |= mask_bit << 17;
                    }
                    // up right
                    if (file <= 7) {
                        captures |= mask_bit << 15;
                    }
                }
                if (rank >= 3) {
                    if (rank >= 2) {
                        // left down
                        if (file >= 3) {
                            captures |= mask_bit >> 6;
                        }
                        // right down
                        if (file <= 6) {
                            captures |= mask_bit >> 10;
                        }
                    }
                    // down left
                    if (file >= 2) {
                        captures |= mask_bit >> 15;
                    }
                    // down right
                    if (file <= 7) {
                        captures |= mask_bit >> 17;
                    }
                }
            } else if ((enemy_bishops & mask_bit) != 0) {
                Square slider;
                unsigned long long bit;
                
                // up right
                slider.rank = rank + 1;
                slider.file = file + 1;
                bit = mask_bit << 7;
                while (slider.rank <= 8 && slider.file <= 8) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit <<= 7;
                    slider.rank++;
                    slider.file++;
                }

                // down right
                slider.rank = rank - 1;
                slider.file = file + 1;
                bit = mask_bit >> 9;
                while (slider.rank >= 1 && slider.file <= 8) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit >>= 9;
                    slider.rank--;
                    slider.file++;
                }

                // down left
                slider.rank = rank - 1;
                slider.file = file - 1;
                bit = mask_bit >> 7;
                while (slider.rank >= 1 && slider.file >= 1) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit >>= 7;
                    slider.rank--;
                    slider.file--;
                }

                // up left
                slider.rank = rank + 1;
                slider.file = file - 1;
                bit = mask_bit << 9;
                while (slider.rank <= 8 && slider.file >= 1) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit <<= 9;
                    slider.rank++;
                    slider.file--;
                }
            } else if ((enemy_rooks & mask_bit) != 0) {
                Square slider;
                unsigned long long bit;
                
                // up
                slider.rank = rank + 1;
                slider.file = file;
                bit = mask_bit << 8;
                while (slider.rank <= 8) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit <<= 8;
                    slider.rank++;
                }

                // right
                slider.rank = rank;
                slider.file = file + 1;
                bit = mask_bit >> 1;
                while (slider.file <= 8) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit >>= 1;
                    slider.file++;
                }

                // down
                slider.rank = rank - 1;
                slider.file = file;
                bit = mask_bit >> 8;
                while (slider.rank >= 1) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit >>= 8;
                    slider.rank--;
                }

                // left
                slider.rank = rank;
                slider.file = file - 1;
                bit = mask_bit << 1;
                while (slider.file >= 1) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit <<= 1;
                    slider.file--;
                }
            } else if ((enemy_queens & mask_bit) != 0) {
                Square slider;
                unsigned long long bit;
                
                // up right
                slider.rank = rank + 1;
                slider.file = file + 1;
                bit = mask_bit << 7;
                while (slider.rank <= 8 && slider.file <= 8) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit <<= 7;
                    slider.rank++;
                    slider.file++;
                }

                // down right
                slider.rank = rank - 1;
                slider.file = file + 1;
                bit = mask_bit >> 9;
                while (slider.rank >= 1 && slider.file <= 8) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit >>= 9;
                    slider.rank--;
                    slider.file++;
                }

                // down left
                slider.rank = rank - 1;
                slider.file = file - 1;
                bit = mask_bit >> 7;
                while (slider.rank >= 1 && slider.file >= 1) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit >>= 7;
                    slider.rank--;
                    slider.file--;
                }

                // up left
                slider.rank = rank + 1;
                slider.file = file - 1;
                bit = mask_bit << 9;
                while (slider.rank <= 8 && slider.file >= 1) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit <<= 9;
                    slider.rank++;
                    slider.file--;
                }
                // up
                slider.rank = rank + 1;
                slider.file = file;
                bit = mask_bit << 8;
                while (slider.rank <= 8) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit <<= 8;
                    slider.rank++;
                }

                // right
                slider.rank = rank;
                slider.file = file + 1;
                bit = mask_bit >> 1;
                while (slider.file <= 8) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit >>= 1;
                    slider.file++;
                }

                // down
                slider.rank = rank - 1;
                slider.file = file;
                bit = mask_bit >> 8;
                while (slider.rank >= 1) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit >>= 8;
                    slider.rank--;
                }

                // left
                slider.rank = rank;
                slider.file = file - 1;
                bit = mask_bit << 1;
                while (slider.file >= 1) {
                    captures |= bit;
                    if ((bit & occupied_board) != 0) {
                        break;
                    }
                    bit <<= 1;
                    slider.file--;
                }
            } else if ((enemy_kings & mask_bit) != 0) {
                if (rank >= 2) {
                    if (file >= 2) {
                        captures |= mask_bit >> 7;
                    }
                    if (file <= 7) {
                        captures |= mask_bit >> 9;
                    }
                    captures |= mask_bit << 1;
                }
                if (rank <= 7) {
                    if (file >= 2) {
                        captures |= mask_bit << 9;
                    }
                    if (file <= 7) {
                        captures |= mask_bit << 7;
                    }
                    captures |= mask_bit >> 1;
                }
                if (file >= 2) {
                    captures |= mask_bit >> 8;
                }
                if (file <= 7) {
                    captures |= mask_bit << 8;
                }
            }
            mask_bit <<= 1;
        }
    }
    return captures;
}

bool GameState::space_check(const unsigned long long position) {
    //TODO 
    return position & get_white_bitboard();

}

void GameState::get_pawn_moves(std::vector<GameState>& states) {
    const unsigned long long occupied_space = get_bitboard();
    const unsigned long long open_space = ~occupied_space;

    const unsigned long long white_board = get_white_bitboard();
    const unsigned long long black_board = get_black_bitboard();

    unsigned long long mask_bit = 0x1;

    std::vector<GameState> states_to_add;

    if (white_turn) {
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
                        // handle upgrading pawn
                        if (rank == 7) {
                            GameState state = *this;
                            state.white_pawns ^= pawn;
                            state.white_turn = false;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank+1, file},
                                turn,
                                white_pawn_piece,
                                false
                            };
                            state.white_queens |= jump_one;
                            states_to_add.push_back(state);
                            state.white_queens ^= jump_one;

                            state.white_rooks |= jump_one;
                            states_to_add.push_back(state);
                            state.white_rooks ^= jump_one;

                            state.white_bishops |= jump_one;
                            states_to_add.push_back(state);
                            state.white_rooks ^= jump_one;

                            state.white_bishops |= jump_one;
                            states_to_add.push_back(state);
                            state.white_knights ^= jump_one;
                        } else {
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
                            if (rank == 7) {
                                GameState state_template = *this;
                                state_template.white_pawns ^= pawn;
                                state_template.white_turn = false;
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
                                states_to_add.push_back(state);

                                state = state_template;
                                state.white_rooks |= take_left;
                                state.remove_capture(true, take_left);
                                states_to_add.push_back(state);

                                state = state_template;
                                state.white_bishops |= take_left;
                                state.remove_capture(true, take_left);
                                states_to_add.push_back(state);

                                state = state_template;
                                state.white_bishops |= take_left;
                                state.remove_capture(true, take_left);
                                states_to_add.push_back(state);
                            } else {
                                GameState state = *this;
                                state.white_pawns ^= pawn;
                                state.white_pawns |= take_left;
                                state.remove_capture(true, take_left);
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
                    }

                    // white pawn takes right
                    if (file != 8) {
                        unsigned long long take_right = pawn << 7;
                        if ((take_right & black_board) != 0) {
                            if (rank == 7) {
                                GameState state_template = *this;
                                state_template.white_pawns ^= pawn;
                                state_template.white_turn = false;
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
                                states_to_add.push_back(state);

                                state = state_template;
                                state.white_rooks |= take_right;
                                state.remove_capture(true, take_right);
                                states_to_add.push_back(state);

                                state = state_template;
                                state.white_bishops |= take_right;
                                state.remove_capture(true, take_right);
                                states_to_add.push_back(state);

                                state = state_template;
                                state.white_bishops |= take_right;
                                state.remove_capture(true, take_right);
                                states_to_add.push_back(state);
                            } else {
                                GameState state = *this;
                                state.white_pawns ^= pawn;
                                state.white_pawns |= take_right;
                                state.remove_capture(true, take_right);
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
                }
                mask_bit <<= 1;
            }
        }
    } else {
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
                        if (rank == 2) {
                            GameState state = *this;
                            state.black_pawns ^= pawn;
                            state.white_turn = true;
                            state.turn++;
                            state.prev_move = {
                                {rank, file},
                                {rank-1, file},
                                turn,
                                black_pawn_piece,
                                false
                            };
                            state.black_queens |= jump_one;
                            states_to_add.push_back(state);
                            state.black_queens ^= jump_one;

                            state.black_rooks |= jump_one;
                            states_to_add.push_back(state);
                            state.black_rooks ^= jump_one;

                            state.black_bishops |= jump_one;
                            states_to_add.push_back(state);
                            state.black_bishops ^= jump_one;

                            state.black_knights |= jump_one;
                            states_to_add.push_back(state);
                            state.black_knights ^= jump_one;
                        } else {
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
                            if (rank == 2) {
                                GameState state_template = *this;
                                state_template.black_pawns ^= pawn;
                                state_template.white_turn = true;
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
                                states_to_add.push_back(state);

                                state = state_template;
                                state.black_rooks |= take_left;
                                state.remove_capture(false, take_left);
                                states_to_add.push_back(state);

                                state = state_template;
                                state.black_bishops |= take_left;
                                state.remove_capture(false, take_left);
                                states_to_add.push_back(state);

                                state = state_template;
                                state.black_knights |= take_left;
                                state.remove_capture(false, take_left);
                                states_to_add.push_back(state);
                            } else {
                                GameState state = *this;
                                state.black_pawns ^= pawn;
                                state.black_pawns |= take_left;
                                state.remove_capture(false, take_left);
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
                    }

                    // black pawn takes right
                    if (file != 1) {
                        unsigned long long take_right = pawn >> 7;
                        if ((take_right & black_board) != 0) {
                            if (rank == 2) {
                                GameState state_template = *this;
                                state_template.black_pawns ^= pawn;
                                state_template.white_turn = true;
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
                                states_to_add.push_back(state);

                                state = state_template;
                                state.black_rooks |= take_right;
                                state.remove_capture(false, take_right);
                                states_to_add.push_back(state);

                                state = state_template;
                                state.black_bishops |= take_right;
                                state.remove_capture(false, take_right);
                                states_to_add.push_back(state);

                                state = state_template;
                                state.black_knights |= take_right;
                                state.remove_capture(false, take_right);
                                states_to_add.push_back(state);
                            } else {
                                GameState state = *this;
                                state.black_pawns ^= pawn;
                                state.black_pawns |= take_right;
                                state.remove_capture(false, take_right);
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
                }
                mask_bit <<= 1;
            }
        }
    }
    for (auto s: states_to_add) {
        states.push_back(s);
    }
}

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
                        states_to_add.push_back(state); 
                    }
                }

                // right up
                if (rank <= 7 && file <= 6) {
                    const unsigned long long jump = knight << 6;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // right down
                if (rank >= 2 && file <= 6) {
                    const unsigned long long jump = knight >> 10;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // down right
                if (rank >= 3 && file <= 7) {
                    const unsigned long long jump = knight >> 17;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // down left
                if (rank >= 3 && file >= 2) {
                    const unsigned long long jump = knight >> 15;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // left down
                if (rank >= 2 && file >= 3) {
                    const unsigned long long jump = knight >> 6;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // left up
                if (rank <= 7 && file >= 3) {
                    const unsigned long long jump = knight << 10;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // up left
                if (rank <= 6 && file >= 2) {
                    const unsigned long long jump = knight << 17;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
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
                    } else if ((jump & enemy_board) != 0) {
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
                        states_to_add.push_back(state); 
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
                    } else if ((jump & enemy_board) != 0) {
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
                        states_to_add.push_back(state); 
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
                    } else if ((jump & enemy_board) != 0) {
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
                        states_to_add.push_back(state); 
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
                    } else if ((jump & enemy_board) != 0) {
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
                        states_to_add.push_back(state); 
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


void GameState::get_rook_moves(std::vector<GameState>& states) {
    const unsigned long long occupied_space = get_bitboard();
    const unsigned long long open_space = ~occupied_space;

    const unsigned long long friendly_board = white_turn ? get_white_bitboard() : get_black_bitboard();
    const unsigned long long enemy_board = white_turn ? get_black_bitboard() : get_white_bitboard();

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
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
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
                        states_to_add.push_back(state); 
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
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
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
                        states_to_add.push_back(state); 
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
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
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
                        states_to_add.push_back(state); 
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
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
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
                        states_to_add.push_back(state); 
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

void GameState::get_queen_moves(std::vector<GameState>& states) {
    const unsigned long long occupied_space = get_bitboard();
    const unsigned long long open_space = ~occupied_space;

    const unsigned long long friendly_board = white_turn ? get_white_bitboard() : get_black_bitboard();
    const unsigned long long enemy_board = white_turn ? get_black_bitboard() : get_white_bitboard();

    unsigned long long mask_bit = 0x1;

    std::vector<GameState> states_to_add;

    Piece queen_piece;
    unsigned long long friendly_queens;
    if (white_turn) {
        queen_piece.set(Piece::PieceEncoding::WhiteQueen);
        friendly_queens = white_queens;
    } else {
        queen_piece.set(Piece::PieceEncoding::BlackQueen);
        friendly_queens = black_queens;
    }

    for (int rank = 1; rank <= 8; rank++) {
        for (int file = 8; file >= 1; file--) {

            const unsigned long long queen = mask_bit & friendly_queens;

            if (queen != 0) {
                //queen exists
                unsigned long long jump;
                Square slider;

                // up
                slider.rank = rank+1;
                slider.file = file;
                jump = queen;
                while (slider.rank <= 8) {
                    jump <<= 8;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            false
                        };
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.remove_capture(white_turn, jump);
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            true
                        };
                        states_to_add.push_back(state); 
                        break;
                    } else if ((jump & friendly_board) != 0) {
                        break;
                    }
                    slider.rank++;
                }

                // right
                slider.rank = rank;
                slider.file = file+1;
                jump = queen;
                while (slider.file <= 8) {
                    jump >>= 1;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            false
                        };
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.remove_capture(white_turn, jump);
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            true
                        };
                        states_to_add.push_back(state); 
                        break;
                    } else if ((jump & friendly_board) != 0) {
                        break;
                    }
                    slider.file++;
                }

                // down
                slider.rank = rank-1;
                slider.file = file;
                jump = queen;
                while (slider.rank >= 1) {
                    jump >>= 8;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            false
                        };
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.remove_capture(white_turn, jump);
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            true
                        };
                        states_to_add.push_back(state); 
                        break;
                    } else if ((jump & friendly_board) != 0) {
                        break;
                    }
                    slider.rank--;
                }

                // left
                slider.rank = rank;
                slider.file = file-1;
                jump = queen;
                while (slider.file >= 1) {
                    jump <<= 1;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            false
                        };
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.remove_capture(white_turn, jump);
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            true
                        };
                        states_to_add.push_back(state); 
                        break;
                    } else if ((jump & friendly_board) != 0) {
                        break;
                    }
                    slider.file--;  
                }

                // top right
                slider.rank = rank+1;
                slider.file = file+1;
                jump = queen;
                while (slider.rank <= 8 && slider.file <= 8) {
                    jump <<= 7;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            false
                        };
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.remove_capture(white_turn, jump);
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            true
                        };
                        states_to_add.push_back(state); 
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
                jump = queen;
                while (slider.rank >= 1 && slider.file <= 8) {
                    jump >>= 9;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            false
                        };
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.remove_capture(white_turn, jump);
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            true
                        };
                        states_to_add.push_back(state); 
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
                jump = queen;
                while (slider.rank >= 1 && slider.file >= 1) {
                    jump >>= 7;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            false
                        };
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.remove_capture(white_turn, jump);
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            true
                        };
                        states_to_add.push_back(state); 
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
                jump = queen;
                while (slider.rank <= 8 && slider.file >= 1) {
                    jump <<= 9;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            false
                        };
                        states_to_add.push_back(state); 
                    } else if ((jump & enemy_board) != 0) {
                        GameState state = *this;
                        if (white_turn) {
                            state.white_queens ^= queen;
                            state.white_queens |= jump;
                        } else {
                            state.black_queens ^= queen;
                            state.black_queens |= jump;
                        }
                        state.remove_capture(white_turn, jump);
                        state.white_turn = !white_turn;
                        state.turn++;
                        state.prev_move = {
                            {rank, file},
                            {slider.rank, slider.file},
                            turn,
                            queen_piece,
                            true
                        };
                        states_to_add.push_back(state); 
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
                        states_to_add.push_back(state); 
                    }
                }

                // up right
                if (rank != 8 && file != 8) {
                    unsigned long long jump = king << 7;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // right
                if (file != 8) {
                    unsigned long long jump = king >> 1;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // down right
                if (rank != 1 && file != 8) {
                    unsigned long long jump = king >> 9;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // down
                if (rank != 1) {
                    unsigned long long jump = king >> 8;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // down left
                if (rank != 1 && file != 1) {
                    unsigned long long jump = king >> 7;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // left
                if (file != 1) {
                    unsigned long long jump = king << 1;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
                    }
                }

                // up left
                if (rank != 8 && file != 1) {
                    unsigned long long jump = king << 9;
                    if ((jump & friendly_board) == 0) {
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
                        states_to_add.push_back(state); 
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

