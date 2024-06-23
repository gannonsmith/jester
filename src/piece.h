#pragma once

#include <string>

class Piece {

    enum PieceEncoding {
        King = 0b00100000,
        Queen = 0b00010000,
        Rook = 0b00001000,
        Bishop = 0b00000100,
        Knight = 0b00000010,
        Pawn = 0b00000001,
        White = 0b10000000,
        WhiteKing = 0b10100000,
        WhiteQueen = 0b10010000,
        WhiteRook = 0b10001000,
        WhiteBishop = 0b10000100,
        WhiteKnight = 0b10000010,
        WhitePawn = 0b10000001,
        Black = 0b01000000,
        BlackKing = 0b01100000,
        BlackQueen = 0b01010000,
        BlackRook = 0b01001000,
        BlackBishop = 0b01000100,
        BlackKnight = 0b01000010,
        BlackPawn = 0b01000001,
        Empty = 0b00000000
    };

    public:

    static Piece get(char c);
    std::string get_display();

    bool empty();

    // returns 0 if false, if true then some number
    int white();
    int black();
    int king();
    int queen();
    int rook();
    int bishop();
    int knight();
    int pawn();

    private:
    PieceEncoding piece_encoding;
};