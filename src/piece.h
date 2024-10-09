#ifndef PIECE_H
#define PIECE_H

class Piece {
    public:
    static bool isColor(int piece, int color) {
        int piece_color = piece & Color;
        return piece_color == color;
    }

    static bool isSlidingPiece (int piece) {
        //std::cout << "in sliding" << std::endl;
        unsigned int type = piece & Type;
        return (type == Queen) || (type == Rook) || (type == Bishop);
    }

    static bool isType (int piece, int t) {
        unsigned int type = piece & Type;
        return type == t;
    }

    static const unsigned int None = 0;
    static const unsigned int King = 1;
    static const unsigned int Queen = 2;
    static const unsigned int Knight = 3;
    static const unsigned int Bishop = 4;
    static const unsigned int Rook = 5;
    static const unsigned int Pawn = 6;

    static const unsigned int Type = 7;

    static const unsigned int White = 8;
    static const unsigned int Black = 16;

    static const unsigned int Color = 24;
};

#endif