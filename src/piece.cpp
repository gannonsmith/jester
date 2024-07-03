#include "piece.h"

void Piece::set(PieceEncoding p) {
    piece_encoding = p;
}

Piece::PieceEncoding Piece::get() {
    return piece_encoding;
}

Piece Piece::get(char c) {
    Piece piece;
    switch (c) {
        case 'K':
            piece.piece_encoding = PieceEncoding::WhiteKing;
            break;
        case 'Q':
            piece.piece_encoding = PieceEncoding::WhiteQueen;
            break;
        case 'R':
            piece.piece_encoding = PieceEncoding::WhiteRook;
            break;
        case 'B':
            piece.piece_encoding = PieceEncoding::WhiteBishop;
            break;
        case 'N':
            piece.piece_encoding = PieceEncoding::WhiteKnight;
            break;
        case 'P':
            piece.piece_encoding = PieceEncoding::WhitePawn;
            break;
        case 'k':
            piece.piece_encoding = PieceEncoding::BlackKing;
            break;
        case 'q':
            piece.piece_encoding = PieceEncoding::BlackQueen;
            break;
        case 'r':
            piece.piece_encoding = PieceEncoding::BlackRook;
            break;
        case 'b':
            piece.piece_encoding = PieceEncoding::BlackBishop;
            break;
        case 'n':
            piece.piece_encoding = PieceEncoding::BlackKnight;
            break;
        case 'p':
            piece.piece_encoding = PieceEncoding::BlackPawn;
            break;
        default:
            piece.piece_encoding = PieceEncoding::Empty;
            break;
        }
    return piece;
}

std::string Piece::get_display() const {
    std::string piece_icon;
    switch (piece_encoding) {
    case PieceEncoding::Empty:
        piece_icon = ".";
        break;
    case PieceEncoding::WhiteKing:
        piece_icon = "\u2654";
        break;
    case PieceEncoding::WhiteQueen:
        piece_icon = "\u2655";
        break;
    case PieceEncoding::WhiteRook:
        piece_icon = "\u2656";
        break;
    case PieceEncoding::WhiteBishop:
        piece_icon = "\u2657";
        break;
    case PieceEncoding::WhiteKnight:
        piece_icon = "\u2658";
        break;
    case PieceEncoding::WhitePawn:
        piece_icon = "\u2659";
        break;
    case PieceEncoding::BlackKing:
        piece_icon = "\u265A";
        break;
    case PieceEncoding::BlackQueen:
        piece_icon = "\u265B";
        break;
    case PieceEncoding::BlackRook:
        piece_icon = "\u265C";
        break;
    case PieceEncoding::BlackBishop:
        piece_icon = "\u265D";
        break;
    case PieceEncoding::BlackKnight:
        piece_icon = "\u265E";
        break;
    case PieceEncoding::BlackPawn:
        piece_icon = "\u265F";
        break;
    default:
        break;
    }
    return piece_icon;
}

bool Piece::empty() {
    return piece_encoding == PieceEncoding::Empty;
}

int Piece::white() {
    return piece_encoding & PieceEncoding::White;
}

int Piece::black() {
    return piece_encoding & PieceEncoding::Black;
}

int Piece::king() {
    return piece_encoding & PieceEncoding::King;
}

int Piece::queen() {
    return piece_encoding & PieceEncoding::Queen;
}

int Piece::rook() {
    return piece_encoding & PieceEncoding::Rook;
}

int Piece::bishop() {
    return piece_encoding & PieceEncoding::Bishop;
}

int Piece::knight() {
    return piece_encoding & PieceEncoding::Knight;
}

int Piece::pawn() {
    return piece_encoding & PieceEncoding::Pawn;
}
