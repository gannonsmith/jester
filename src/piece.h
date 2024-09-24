
class Piece {
    public:
    static bool isColor(int piece, int color) {
        if (piece | None == 0) {
            return false;
        } else {
            if (color == White) {
                return piece & White;
            } else {
                return piece & Black;
            }
        }
    }

    static bool isSlidingPiece (int piece) {
        return (piece & Queen) | (piece & Rook) | (piece & Bishop);
    }

    static bool isType (int piece, int t) {
        return piece & t;
    }

    static const int None = 0;
    static const int King = 1;
    static const int Queen = 2;
    static const int Knight = 3;
    static const int Bishop = 4;
    static const int Rook = 5;
    static const int Pawn = 6;

    static const int White = 8;
    static const int Black = 16;
};