#pragma once

#include <map>
#include <cctype>
#include <string>

#include "piece.h"
#include "move.h"


class Board {
public:
    // board dimensions
    static const int size = 8;

    // constructor to initialize board with "None"s
    Board() : squares(size * size, 0) {}

    void initialize_with_fen(const std::string& fen) {
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
                    file += int(c);
                } else {
                    int color = isupper(c) ? Piece::White : Piece::Black;
                    int type = piece_type_from_symbol[tolower(c)];
                    squares[rank * 8 + file] = type | color;
                    file++;
                }
            }
        }

        color_to_move = Piece::White;
    }

    std::vector<Move> generate_moves();

    void generate_sliding_moves(int start_square, int piece);

    int operator [](int idx) {
        return squares[idx];
    }
private:
    std::vector<int> squares; // 1D array for board representation
    std::vector<Move> moves;
    int color_to_move;
    int friendly_color;
    int opponent_color;
};