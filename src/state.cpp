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

void GameState::set(Piece& p, Square& s) {
    unsigned long long bit = 1;
    bit <<= (s.rank-1)*8 + s.file-1;
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
}

void GameState::get_pawn_moves(std::vector<GameState>& states) {
    const unsigned long long occupied_space = get_bitboard();
    const unsigned long long open_space = ~occupied_space;

    const unsigned long long white_board = get_white_bitboard();
    const unsigned long long black_board = get_black_bitboard();

    unsigned long long mask_bit = 0x100;;

    std::vector<GameState> states_to_add;

    if (white_turn) {
        Piece white_pawn_piece;
        white_pawn_piece.set(Piece::PieceEncoding::WhitePawn);

        // No white pawns can be in rank 1 or 8
        for (int rank = 2; rank < 8; rank++) {
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
                            state.white_rooks |= jump_one;
                            states_to_add.push_back(state);
                            state.white_bishops |= jump_one;
                            states_to_add.push_back(state);
                            state.white_knights |= jump_one;
                            states_to_add.push_back(state);
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
                                GameState state = *this;
                                state.white_pawns ^= pawn;
                                state.white_turn = false;
                                state.turn++;
                                state.prev_move = {
                                    {rank, file},
                                    {rank+1, file-1},
                                    turn,
                                    white_pawn_piece,
                                    true
                                };
                                state.white_queens |= take_left;
                                states_to_add.push_back(state);
                                state.white_rooks |= take_left;
                                states_to_add.push_back(state);
                                state.white_bishops |= take_left;
                                states_to_add.push_back(state);
                                state.white_knights |= take_left;
                                states_to_add.push_back(state);
                            } else {
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
                    }

                    // white pawn takes right
                    if (file != 8) {
                        unsigned long long take_right = pawn << 7;
                        if ((take_right & black_board) != 0) {
                            if (rank == 7) {
                                GameState state = *this;
                                state.white_pawns ^= pawn;
                                state.white_turn = false;
                                state.turn++;
                                state.prev_move = {
                                    {rank, file},
                                    {rank+1, file+1},
                                    turn,
                                    white_pawn_piece,
                                    true
                                };
                                state.white_queens |= take_right;
                                states_to_add.push_back(state);
                                state.white_rooks |= take_right;
                                states_to_add.push_back(state); 
                                state.white_bishops |= take_right;
                                states_to_add.push_back(state); 
                                state.white_knights |= take_right;
                                states_to_add.push_back(state); 
                            } else {
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
                            state.black_rooks |= jump_one;
                            states_to_add.push_back(state);
                            state.black_bishops |= jump_one;
                            states_to_add.push_back(state);
                            state.black_knights |= jump_one;
                            states_to_add.push_back(state);
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
                                GameState state = *this;
                                state.black_pawns ^= pawn;
                                state.white_turn = true;
                                state.turn++;
                                state.prev_move = {
                                    {rank, file},
                                    {rank-1, file+1},
                                    turn,
                                    black_pawn_piece,
                                    true
                                };
                                state.black_queens |= take_left;
                                states_to_add.push_back(state); 
                                state.black_rooks |= take_left;
                                states_to_add.push_back(state); 
                                state.black_bishops |= take_left;
                                states_to_add.push_back(state); 
                                state.black_knights |= take_left;
                                states_to_add.push_back(state); 
                            } else {
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
                    }

                    // black pawn takes right
                    if (file != 1) {
                        unsigned long long take_right = pawn << 7;
                        if ((take_right & black_board) != 0) {
                            if (rank == 2) {
                                GameState state = *this;
                                state.black_pawns ^= pawn;
                                state.white_turn = true;
                                state.turn++;
                                state.prev_move = {
                                    {rank, file},
                                    {rank-1, file-1},
                                    turn,
                                    black_pawn_piece,
                                    true
                                };
                                state.black_queens |= take_right;
                                states_to_add.push_back(state); 
                                state.black_rooks |= take_right;
                                states_to_add.push_back(state); 
                                state.black_bishops |= take_right;
                                states_to_add.push_back(state); 
                                state.black_knights |= take_right;
                                states_to_add.push_back(state); 
                            } else {
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
    const unsigned long long occupied_space = get_bitboard();
    const unsigned long long open_space = ~occupied_space;

    const unsigned long long enemy_board = white_turn ? get_black_bitboard() : get_white_bitboard();

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
                    unsigned long long jump = knight << 15;
                    if (((jump & open_space) != 0) || ((jump & enemy_board) != 0)) {
                        GameState state = *this;
                        state.white_knights ^= knight;
                        state.white_knights |= jump;
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
                    unsigned long long jump = knight << 6;
                    if (((jump & open_space) != 0) || ((jump & enemy_board) != 0)) {
                        GameState state = *this;
                        state.white_knights ^= knight;
                        state.white_knights |= jump;
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
                    unsigned long long jump = knight >> 10;
                    if (((jump & open_space) != 0) || ((jump & enemy_board) != 0)) {
                        GameState state = *this;
                        state.white_knights ^= knight;
                        state.white_knights |= jump;
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
                    unsigned long long jump = knight >> 17;
                    if (((jump & open_space) != 0) || ((jump & enemy_board) != 0)) {
                        GameState state = *this;
                        state.white_knights ^= knight;
                        state.white_knights |= jump;
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
                    unsigned long long jump = knight >> 15;
                    if (((jump & open_space) != 0) || ((jump & enemy_board) != 0)) {
                        GameState state = *this;
                        state.white_knights ^= knight;
                        state.white_knights |= jump;
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
                    unsigned long long jump = knight >> 6;
                    if (((jump & open_space) != 0) || ((jump & enemy_board) != 0)) {
                        GameState state = *this;
                        state.white_knights ^= knight;
                        state.white_knights |= jump;
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
                    unsigned long long jump = knight << 6;
                    if (((jump & open_space) != 0) || ((jump & enemy_board) != 0)) {
                        GameState state = *this;
                        state.white_knights ^= knight;
                        state.white_knights |= jump;
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
                    unsigned long long jump = knight << 17;
                    if (((jump & open_space) != 0) || ((jump & enemy_board) != 0)) {
                        GameState state = *this;
                        state.white_knights ^= knight;
                        state.white_knights |= jump;
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
        for (int file = 8; file >= 1; file++) {

            const unsigned long long bishop = mask_bit & friendly_bishops;
            unsigned long long jump = bishop;

            if (bishop != 0) {
                //bishop exists
                Square slider;

                // top right
                slider.rank = rank+1;
                slider.file = file+1;
                jump = bishop;
                while (slider.rank <= 8 && slider.file <= 8) {
                    jump <<= 7;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        state.white_bishops ^= bishop;
                        state.white_bishops |= jump;
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
                        state.white_bishops ^= bishop;
                        state.white_bishops |= jump;
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
                        state.white_bishops ^= bishop;
                        state.white_bishops |= jump;
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
                        state.white_bishops ^= bishop;
                        state.white_bishops |= jump;
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

                // bottom left
                slider.rank = rank-1;
                slider.file = file-1;
                jump = bishop;
                while (slider.rank >= 1 && slider.file >= 1) {
                    jump >>= 7;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        state.white_bishops ^= bishop;
                        state.white_bishops |= jump;
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
                        state.white_bishops ^= bishop;
                        state.white_bishops |= jump;
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

                // top left
                slider.rank = rank+1;
                slider.file = file-1;
                jump = bishop;
                while (slider.rank <= 8 && slider.file >= 1) {
                    jump <<= 9;

                    if ((jump & open_space) != 0) {
                        GameState state = *this;
                        state.white_bishops ^= bishop;
                        state.white_bishops |= jump;
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
                        state.white_bishops ^= bishop;
                        state.white_bishops |= jump;
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

            }
            mask_bit <<= 1;
        }
    }

    for (auto s: states_to_add) {
        states.push_back(s);
    }
}


