#include <vector>
#include "board.h"

std::vector<Move> Board::generate_moves() {
    std::vector<Move> moves;

    for (int start_square = 0; start_square < 64; start_square++) {
        int piece = squares[start_square];
        if (Piece::isColor(piece, color_to_move)) {
            if (Piece::isSlidingPiece(piece)) {
                generate_sliding_moves(start_square, piece);
            }
        }
    }

    return moves;
}

void Board::generate_sliding_moves(int start_square, int piece) {
    int start_dir_index = (Piece::isType(piece, Piece::Bishop)) ? 4 : 0;
    int end_dir_index = (Piece::isType(piece, Piece::Rook)) ? 4 : 8;

    for (int direction_index = start_dir_index; direction_index < end_dir_index; direction_index++) {
        for (int n = 0; n < NUM_SQUARES_TO_EDGE[start_square][direction_index]; n++) {

            int target_square = start_square + DIRECTION_OFFSETS[direction_index] * (n+1);
            int piece_on_target_square = squares[target_square];

            // blocked by friendly piece
            if (Piece::isColor(piece_on_target_square, friendly_color)) {
                break;
            }

            moves.push_back({start_square, target_square});

            // blocked by enemy piece
            if (Piece::isColor(piece_on_target_square, opponent_color)) {
                break;
            }
        }
    }
}
