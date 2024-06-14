#include <iostream>
#include "player.h"

Piece Player::get(Square& square) {
    return Piece::Empty;
}

Piece White::get(Square& square) {
    int shift_num = ((square.rank - 8) * 8) + square.file + 1;
    unsigned long long int bit = 1;
    unsigned long long int mask_bit = bit << shift_num;

    if ((king_board.bitboard & mask_bit) != 0) {
        return Piece::WhiteKing;
    } else if ((queen_board.bitboard & mask_bit) != 0) {
        return Piece::WhiteQueen;
    } else if ((rook_board.bitboard & mask_bit) != 0) {
        return Piece::WhiteRook;
    } else if ((bishop_board.bitboard & mask_bit) != 0) {
        return Piece::WhiteBishop;
    } else if ((knight_board.bitboard & mask_bit) != 0) {
        return Piece::WhiteKnight;
    } else if ((pawn_board.bitboard & mask_bit) != 0) {
        return Piece::WhitePawn;
    } else {
        return Piece::Empty;
    }
}

Piece Black::get(Square& square) {
    int shift_num = ((square.rank - 8) * 8) + square.file + 1;
    unsigned long long int bit = 1;
    unsigned long long int mask_bit = bit << shift_num;

    if ((king_board.bitboard & mask_bit) != 0) {
        return Piece::BlackKing;
    } else if ((queen_board.bitboard & mask_bit) != 0) {
        return Piece::BlackQueen;
    } else if ((rook_board.bitboard & mask_bit) != 0) {
        return Piece::BlackRook;
    } else if ((bishop_board.bitboard & mask_bit) != 0) {
        return Piece::BlackBishop;
    } else if ((knight_board.bitboard & mask_bit) != 0) {
        return Piece::BlackKnight;
    } else if ((pawn_board.bitboard & mask_bit) != 0) {
        return Piece::BlackPawn;
    } else {
        return Piece::Empty;
    }
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

