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
    friendly_color = Piece::White;
    opponent_color = Piece::Black;
}

void Board::generate_moves(std::vector<Move>& moves)
{
    moves.clear();
    for (int start_square = 0; start_square < 64; start_square++) {
        int piece = squares[start_square];
        if (Piece::isColor(piece, color_to_move)) {
            if (Piece::isSlidingPiece(piece)) {
                generate_sliding_moves(moves, start_square, piece);
            }
        }
    }
}

void Board::generate_sliding_moves(std::vector<Move>& moves, int start_square, int piece)
{
    int start_dir_index = (Piece::isType(piece, Piece::Bishop)) ? 4 : 0;
    int end_dir_index = (Piece::isType(piece, Piece::Rook)) ? 4 : 8;

    /*
    for (int i = 0; i < 64; i++) {
        std::cout << "{";
        for (int j = 0; j < 8; j++) {
            std::cout << NUM_SQUARES_TO_EDGE[i][j] << ",";
        }
        std::cout << "}" << std::endl;
    }
    */

    for (int direction_index = start_dir_index; direction_index < end_dir_index; direction_index++) {
        for (int n = 0; n < NUM_SQUARES_TO_EDGE[start_square][direction_index]; n++) {

            int target_square = start_square + DIRECTION_OFFSETS[direction_index] * (n+1);
            int piece_on_target_square = squares[target_square];

            // blocked by friendly piece
            if (Piece::isColor(piece_on_target_square, friendly_color)) {
                break;
            }

            //std::cout << "generated move" << std::endl;
            moves.push_back({start_square, target_square});

            // blocked by enemy piece
            if (Piece::isColor(piece_on_target_square, opponent_color)) {
                break;
            }
        }
    }
}
