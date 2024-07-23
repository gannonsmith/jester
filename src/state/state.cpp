#include "state.h"

void GameState::test_switch_turn() {
    white_turn = !white_turn;
    under_attack = generate_capture_spaces();
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
    under_attack = 0;
    white_castle_left = true;
    white_castle_right = true;
    black_castle_left = true;
    black_castle_right = true;
}

void GameState::get_states(std::vector<std::vector<GameState>>& states, int depth) {
    for (int i = 0; i < depth; i++) {
        states.push_back({});
        for (auto s: states[i]) {
            s.get_pawn_moves(states[i+1]);
            s.get_knight_moves(states[i+1]);
            s.get_bishop_moves(states[i+1]);
            s.get_rook_moves(states[i+1]);
            s.get_queen_moves(states[i+1]);
            s.get_king_moves(states[i+1]);
            s.get_castle_moves(states[i+1]);
        }
    }
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
                    captures |= mask_bit >> 8;
                }
                if (rank <= 7) {
                    if (file >= 2) {
                        captures |= mask_bit << 9;
                    }
                    if (file <= 7) {
                        captures |= mask_bit << 7;
                    }
                    captures |= mask_bit << 8;
                }
                if (file >= 2) {
                    captures |= mask_bit << 1;
                }
                if (file <= 7) {
                    captures |= mask_bit >> 1;
                }
            }
            mask_bit <<= 1;
        }
    }
    return captures;
}

bool GameState::space_check(const unsigned long long position) {
    return (position & under_attack) != 0;
}

bool GameState::king_check() {
    if (white_turn) {
        return (white_kings & under_attack) != 0;
    } else {
        return (black_kings & under_attack) != 0;
    }
}

const unsigned long long GameState::get_friendly_board() {
    return white_turn ? get_white_bitboard() : get_black_bitboard();
}

const unsigned long long GameState::get_enemy_board() {
    return white_turn ? get_black_bitboard() : get_white_bitboard();
}