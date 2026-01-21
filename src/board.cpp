#include <vector>
#include <cassert>
#include "board.h"

void Board::initialize_with_fen(const std::string& fen)
{
    std::map<char, int> piece_type_from_symbol = {
        {'k', Piece::King},
        {'p', Piece::Pawn},
        {'n', Piece::Knight},
        {'b', Piece::Bishop},
        {'r', Piece::Rook},
        {'q', Piece::Queen}
    };

    int file=0, rank=0, end_of_fen=0;
    for (int i = 0; i < fen.size(); i++) {
        char c = fen[i];
        if (c == '/') {
            file = 0;
            rank++;
        } else {
            if (isdigit(c)) {
                file += c - '0';
            } else if (c == ' ') {
                end_of_fen = i;
                break;
            } else {
                int color = isupper(c) ? Piece::White : Piece::Black;
                int type = piece_type_from_symbol[tolower(c)];
                squares[rank * 8 + file] = type | color;
                file++;
            }
        }
    }


    if (fen[end_of_fen+1] == 'w') {
        color_to_move = Piece::White;
    } else if (fen[end_of_fen+1] == 'b') {
        color_to_move = Piece::Black;
    } else {
        std::cerr << "Invalid FEN: " << fen << std::endl;
        return;
    }

    int castle_idx = end_of_fen + 3;
    while (fen[castle_idx] != ' ') {
        char c = fen[castle_idx];
        if (c == '-') {
            break;
        } else if (c == 'K') {
            castle.white_king = true;
        } else if (c == 'Q') {
            castle.white_queen = true;
        } else if (c == 'k') {
            castle.black_king = true;
        } else if (c == 'q') {
            castle.black_queen = true;
        }
        castle_idx++;
    }
}


void Board::push_move(int start_square, int target_square, bool is_castle) {
    if (is_castle) {
        std::cout << "pushing castle move" << std::endl;
    }
    moves.emplace_back(start_square, target_square, is_castle);
    moves.back().resulting_board = new Board(*this, start_square, target_square, is_castle);
}

void Board::push_move(int start_square, int target_square, unsigned int piece) {
    std::cout << "Pushing move with promotion: " << start_square << " to " << target_square << " with piece " << piece << std::endl;
    moves.emplace_back(start_square, target_square, piece);
    moves.back().resulting_board = new Board(*this, start_square, target_square, piece);
}

bool Board::under_check(int check_square) {
    // TODO, this really should be calculated once, kind of a running board of "under attack"
    // // TODO, if piece (king) is blocking what would be a check anyways (think |.|K|Q|)
                
    int rank = check_square / 8;
    int file = check_square % 8;

    unsigned int opponent_color = Piece::flipColor(color_to_move);

    // rook-type moves
    for (int direction_index = 0; direction_index < 4; direction_index++) {
        for (int n = 0; n < NUM_SQUARES_TO_EDGE[check_square][direction_index]; n++) {

            int target_square = check_square + DIRECTION_OFFSETS[direction_index] * (n+1);
            unsigned int piece_on_target_square = squares[target_square];

            if (Piece::isPiece(piece_on_target_square)) {
                if (Piece::isColor(piece_on_target_square, opponent_color)) {
                    if (n == 0 && Piece::isType(piece_on_target_square, Piece::King)) {
                        return true;
                    }

                    if (Piece::isType(piece_on_target_square, Piece::Rook) || Piece::isType(piece_on_target_square, Piece::Queen)) {
                        return true;
                    }
                }
                break;
            }
        }
    }

    // diagonal-type moves
    for (int direction_index = 4; direction_index < 8; direction_index++) {
        for (int n = 0; n < NUM_SQUARES_TO_EDGE[check_square][direction_index]; n++) {

            int target_square = check_square + DIRECTION_OFFSETS[direction_index] * (n+1);
            unsigned int piece_on_target_square = squares[target_square];

            if (Piece::isPiece(piece_on_target_square)) {
                if (Piece::isColor(piece_on_target_square, opponent_color)) {
                    if (n == 0 && Piece::isType(piece_on_target_square, Piece::King)) {
                        return true;
                    }

                    if (Piece::isType(piece_on_target_square, Piece::Bishop) || Piece::isType(piece_on_target_square, Piece::Queen)) {
                        return true;
                    }
                }
                break;
            }
        }
    }

    // knight-type moves
    for (int target_square = 0; target_square < 64; target_square++) {
        if (KNIGHT_MOVES[check_square][target_square]) {
            unsigned int piece_on_target_square = squares[target_square];

            if (Piece::isPiece(piece_on_target_square)) {
                if (Piece::isColor(piece_on_target_square, opponent_color)) {
                    if (Piece::isType(piece_on_target_square, Piece::Knight)) {
                        return true;
                    }
                }
            }
        }
    }

    // pawn moves
    if (color_to_move == Piece::White) {
        if (rank > 0 && file > 0) {
            unsigned int piece = squares[check_square - 9];
            if (Piece::isColor(piece, opponent_color) && Piece::isType(piece, Piece::Pawn)) {
                return true;
            }
        }
        if (rank > 0 && file < 8) {
            unsigned int piece = squares[check_square - 7];
            if (Piece::isColor(piece, opponent_color) && Piece::isType(piece, Piece::Pawn)) {
                return true;
            }
        }
    } else {
        if (rank < 8 && file > 0) {
            unsigned int piece = squares[check_square + 7];
            if (Piece::isColor(piece, opponent_color) && Piece::isType(piece, Piece::Pawn)) {
                return true;
            }
        }
        if (rank < 8 && file < 8) {
            unsigned int piece = squares[check_square + 9];
            if (Piece::isColor(piece, opponent_color) && Piece::isType(piece, Piece::Pawn)) {
                return true;
            }
        }
    }

    return false;
}

void Board::generate_moves()
{
    moves.clear();
    for (int start_square = 0; start_square < 64; start_square++) {
        unsigned int piece = squares[start_square];
        if (!Piece::isColor(piece, color_to_move)) {
            continue; 
        }
        if (Piece::isType(piece, Piece::Pawn)) {
            if (Piece::isColor(piece, Piece::White)) {
                generate_white_pawn_moves(start_square, piece);
            } else if (Piece::isColor(piece, Piece::Black)) {
                generate_black_pawn_moves(start_square, piece);
            }
        }
        if (Piece::isSlidingPiece(piece)) {
            generate_sliding_moves(start_square, piece);
        }
        if (Piece::isType(piece, Piece::Knight)) {
            generate_knight_moves(start_square, piece);
        }
        if (Piece::isType(piece, Piece::King)) {
            generate_king_moves(start_square, piece);
        }
    }
    generate_castle_moves();
}

void Board::generate_white_pawn_moves(int start_square, unsigned int piece)
{
    assert(Piece::isColor(piece, Piece::White));
    assert(Piece::isType(piece, Piece::Pawn));
    assert(start_square >= 0 && start_square < 64);
    assert(squares[start_square] == piece);
    assert((start_square / 8) >= 1 && (start_square / 8) <= 6);

    bool promotion_move = (start_square / 8) == 1;

    // diagonal right
    int target_square = start_square - 7;
    unsigned int piece_on_target_square = squares[target_square];
    const bool is_right_edge = (start_square % 8 == 7);
    if (!is_right_edge && Piece::isColor(piece_on_target_square, Piece::Black)) {
        if (promotion_move) {
            generate_pawn_promotion(start_square, target_square, piece);
        } else {
            push_move(start_square, target_square, false);
        }
    }

    // diagonal left
    target_square = start_square - 9;
    piece_on_target_square = squares[target_square];
    const bool is_left_edge = (start_square % 8 == 0);
    if (!is_left_edge && Piece::isColor(piece_on_target_square, Piece::Black)) {
        if (promotion_move) {
            generate_pawn_promotion(start_square, target_square, piece);
        } else {
            push_move(start_square, target_square, false);
        }
    }

    // front
    target_square = start_square - 8;
    piece_on_target_square = squares[target_square];

    // blocked by piece
    if (Piece::isPiece(piece_on_target_square)) {
        return;
    }

    if (promotion_move) {
        generate_pawn_promotion(start_square, target_square, piece);
    } else {
        push_move(start_square, target_square, false);
    }

    // starting pawn positions
    if ((start_square / 8) == 6) {
        target_square = start_square - 16;
        piece_on_target_square = squares[target_square];

        // blocked by piece
        if (Piece::isPiece(piece_on_target_square)) {
            return;
        }

        push_move(start_square, target_square, false);
    }
}

void Board::generate_black_pawn_moves(int start_square, unsigned int piece)
{
    assert(Piece::isColor(piece, Piece::Black));
    assert(Piece::isType(piece, Piece::Pawn));
    assert(start_square >= 0 && start_square < 64);
    assert(squares[start_square] == piece);
    assert((start_square / 8) >= 1 && (start_square / 8) <= 6);

    bool promotion_move = (start_square / 8) == 6;

    // diagonal down right
    int target_square = start_square + 9;
    unsigned int piece_on_target_square = squares[target_square];
    const bool is_right_edge = (start_square % 8 == 7);
    if (!is_right_edge && Piece::isColor(piece_on_target_square, Piece::White)) {
        if (promotion_move) {
            generate_pawn_promotion(start_square, target_square, piece);
        } else {
            push_move(start_square, target_square, false);
        }
    }

    // diagonal down left
    target_square = start_square + 7;
    piece_on_target_square = squares[target_square];
    const bool is_left_edge = (start_square % 8 == 0);
    if (!is_left_edge && Piece::isColor(piece_on_target_square, Piece::White)) {
        if (promotion_move) {
            generate_pawn_promotion(start_square, target_square, piece);
        } else {
            push_move(start_square, target_square, false);
        }
    }

    // front
    target_square = start_square + 8;
    piece_on_target_square = squares[target_square];

    // blocked by piece
    if (Piece::isPiece(piece_on_target_square)) {
        return;
    }

    if (promotion_move) {
        generate_pawn_promotion(start_square, target_square, piece);
    } else {
        push_move(start_square, target_square, false);
    }

    // starting pawn positions
    if ((start_square / 8) == 1) {
        target_square = start_square + 16;
        piece_on_target_square = squares[target_square];

        // blocked by piece
        if (Piece::isPiece(piece_on_target_square)) {
            return;
        }

        push_move(start_square, target_square, false);
    }
}

void Board::generate_pawn_promotion(int start_square, int target_square, unsigned int piece)
{
    assert(Piece::isType(piece, Piece::Pawn));
    assert(start_square >= 0 && start_square < 64);
    assert(target_square >= 0 && target_square < 64);
    assert(squares[start_square] == piece);
    assert((target_square / 8) == 0 || (target_square / 8) == 7);

    std::cout << "Generating pawn promotion from " << start_square << " to " << target_square << std::endl;

    if (Piece::isColor(piece, Piece::White)) {
        push_move(start_square, target_square, Piece::Queen | Piece::White);
        push_move(start_square, target_square, Piece::Rook | Piece::White);
        push_move(start_square, target_square, Piece::Bishop | Piece::White);
        push_move(start_square, target_square, Piece::Knight | Piece::White);
    } else if (Piece::isColor(piece, Piece::Black)) {
        push_move(start_square, target_square, Piece::Queen | Piece::Black);
        push_move(start_square, target_square, Piece::Rook | Piece::Black);
        push_move(start_square, target_square, Piece::Bishop | Piece::Black);
        push_move(start_square, target_square, Piece::Knight | Piece::Black);
    }
}

void Board::generate_sliding_moves(int start_square, unsigned int piece)
{
    int start_dir_index = (Piece::isType(piece, Piece::Bishop)) ? 4 : 0;
    int end_dir_index = (Piece::isType(piece, Piece::Rook)) ? 4 : 8;
    unsigned int piece_color = piece & Piece::Color;
    unsigned int opponent_color = Piece::flipColor(piece_color);

    for (int direction_index = start_dir_index; direction_index < end_dir_index; direction_index++) {
        for (int n = 0; n < NUM_SQUARES_TO_EDGE[start_square][direction_index]; n++) {

            int target_square = start_square + DIRECTION_OFFSETS[direction_index] * (n+1);
            unsigned int piece_on_target_square = squares[target_square];

            // blocked by friendly piece
            if (Piece::isColor(piece_on_target_square, piece_color)) {
                break;
            }

            //std::cout << "generated move" << std::endl;
            push_move(start_square, target_square, false);

            // blocked by enemy piece
            if (Piece::isColor(piece_on_target_square, opponent_color)) {
                break;
            }
        }
    }
}

void Board::generate_knight_moves(int start_square, unsigned int piece)
{
    unsigned int piece_color = piece & Piece::Color;

    // TODO make this more efficient
    for (int target_square = 0; target_square < 64; target_square++) {
        if (KNIGHT_MOVES[start_square][target_square]) {

            // blocked by friendly piece
            if (Piece::isColor(squares[target_square], piece_color)) {
                continue;
            }

            push_move(start_square, target_square, false);
        }
    }
}

void Board::generate_king_moves(int start_square, unsigned int piece) {
    unsigned int piece_color = piece & Piece::Color;
    unsigned int opponent_color = Piece::flipColor(piece_color);
    for (int direction_index = 0; direction_index < 8; direction_index++) {
        if (NUM_SQUARES_TO_EDGE[start_square][direction_index] > 0) {

            int target_square = start_square + DIRECTION_OFFSETS[direction_index];
            unsigned int piece_on_target_square = squares[target_square];

            // blocked by friendly piece
            if (Piece::isColor(piece_on_target_square, piece_color)) {
                continue;
            }

            if (under_check(target_square)) {
                continue;
            }

            //std::cout << "generated move" << std::endl;
            push_move(start_square, target_square, false);

            // blocked by enemy piece
            if (Piece::isColor(piece_on_target_square, opponent_color)) {
                continue;
            }
        }
    }
}

void Board::generate_castle_moves()
{

    if (color_to_move == Piece::White) {
        int start_square = 60;
        // King side
        if (castle.white_king) {
            std::cout << "white has castle available" << std::endl;
            if (!squares[61] && !squares[62]) {
            // spaces are clear
                if (!under_check(60) && !under_check(61) && !under_check(62) && !under_check(63)) {
                    // spaces aren't under attack
                    push_move(start_square, 62, true);
                }
            }
        }

        // Queen side
        if (castle.white_queen) {
            if (!squares[59] && !squares[58] && !squares[57]) {
                if (!under_check(60) && !under_check(59) && !under_check(58) && !under_check(57) && !under_check(56)) {
                    push_move(start_square, 58, true);
                }
            }
        }

    } else if (color_to_move == Piece::Black) {
        int start_square = 4;
        // King side
        if (castle.black_king) {
            if (!squares[5] && !squares[6]) {
            // spaces are clear
                if (!under_check(4) && !under_check(5) && !under_check(6) && !under_check(7)) {
                    // spaces aren't under attack
                    push_move(start_square, 6, true);
                }
            }
        }

        // Queen side
        if (castle.black_queen) {
            if (!squares[1] && !squares[2] && !squares[3]) {
                if (!under_check(0) && !under_check(1) && !under_check(2) && !under_check(3) && !under_check(4)) {
                    push_move(start_square, 2, true);
                }
            }
        }
    }
}