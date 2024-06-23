#include <iostream>
#include "player.h"

Piece Player::get(Square& square) {
    return Piece();
}

Piece WhitePlayer::get(Square& square) {
    int shift_num = ((square.rank - 8) * 8) + square.file + 1;
    unsigned long long int bit = 1;
    unsigned long long int mask_bit = bit << shift_num;

    return Piece();
}

Piece BlackPlayer::get(Square& square) {
    int shift_num = ((square.rank - 8) * 8) + square.file + 1;
    unsigned long long int bit = 1;
    unsigned long long int mask_bit = bit << shift_num;

    return Piece();
}

std::vector<std::vector<Piece>> Player::get_board() {
    std::vector<std::vector<Piece>> board(8, std::vector<Piece>(8));
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Square square {rank, file};
            board[rank][file] = get(square);
        }
    }
    return board;
}

