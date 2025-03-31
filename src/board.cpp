#include <vector>
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

    int file=0, rank=7;
    for (char c: fen) {
        if (c == '/') {
            file = 0;
            rank--;
        } else {
            if (isdigit(c)) {
                file += c - '0';
            } else {
                int color = isupper(c) ? Piece::White : Piece::Black;
                int type = piece_type_from_symbol[tolower(c)];
                squares[rank * 8 + file] = type | color;
                file++;
            }
        }
    }

    color_to_move = Piece::White;
    //friendly_color = Piece::White;
    //opponent_color = Piece::Black;
}


void Board::push_move(int start_square, int target_square) {
    moves.emplace_back(start_square, target_square);
    //Board* new_board = moves.back().resulting_board;
    //new_board = new Board(*this, start_square, target_square);
    moves.back().resulting_board = new Board(*this, start_square, target_square);
}


void Board::generate_moves()
{
    moves.clear();
    for (int start_square = 0; start_square < 64; start_square++) {
        unsigned int piece = squares[start_square];
        if (Piece::isType(piece, Piece::Pawn)) {
            generate_pawn_moves(start_square, piece);
        }
        if (Piece::isSlidingPiece(piece)) {
            generate_sliding_moves(start_square, piece);
        }
        if (Piece::isType(piece, Piece::Knight)) {
            generate_knight_moves(start_square, piece);
        }
    }
}

void Board::generate_pawn_moves(int start_square, unsigned int piece)
{

    //if (Piece::isColor(piece, Piece::White)) {
        // shouldn't ever happen
        if ((start_square / 8) == 0) {
            return;
        }

        // TODO: pawn promotion
        if ((start_square / 8) == 1) {
            return;
        }

        // diagonal right
        int target_square = start_square - 7;
        unsigned int piece_on_target_square = squares[target_square];
        if (Piece::isColor(piece_on_target_square, Piece::Black)) {
            push_move(start_square, target_square);
        }

        // diagonal left
        target_square = start_square - 9;
        piece_on_target_square = squares[target_square];
        if (Piece::isColor(piece_on_target_square, Piece::Black)) {
            push_move(start_square, target_square);
        }

        // front
        target_square = start_square - 8;
        piece_on_target_square = squares[target_square];

        // blocked by piece
        if (Piece::isPiece(piece_on_target_square)) {
            return;
        }

        push_move(start_square, target_square);

        // starting pawn positions
        if ((start_square / 8) == 6) {
            target_square = start_square - 16;
            piece_on_target_square = squares[target_square];

            // blocked by piece
            if (Piece::isPiece(piece_on_target_square)) {
                return;
            }

            push_move(start_square, target_square);
        }

    // } else if (Piece::isColor(piece, Piece::Black)) {
    //     return;
    // }
    
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
            push_move(start_square, target_square);

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

            push_move(start_square, target_square);
        }
    }
}
