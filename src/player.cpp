#include <iostream>
#include "player.h"

char Player::get(int rank, int file) {
    int shift_num = ((rank - 8) * 8) + file + 1;
    unsigned long long int bit = 1;
    unsigned long long int mask_bit = bit << shift_num;

    char piece;
    if ((king_board.bitboard & mask_bit) != 0) {
        piece = 'K';
    } else if ((queen_board.bitboard & mask_bit) != 0) {
        piece = 'Q';
    } else if ((rook_board.bitboard & mask_bit) != 0) {
        piece = 'R';
    } else if ((bishop_board.bitboard & mask_bit) != 0) {
        piece = 'B';
    } else if ((knight_board.bitboard & mask_bit) != 0) {
        piece = 'N';
    } else if ((pawn_board.bitboard & mask_bit) != 0) {
        piece = 'P';
    } else {
       piece = '.';
    }
    return piece;
}

std::vector<std::vector<char>> Player::get_board() {
    std::vector<std::vector<char>> board(8, std::vector<char>(8));
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            board[rank][file] = get(rank, file);
        }
    }
    return board;
}

