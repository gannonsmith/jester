#pragma once

#include <iostream>

struct Square {
    int rank;
    int file;

    friend std::ostream& operator<<(std::ostream& os, Square const & s) {
        return os << "(" << char(s.file + 'a' - 1) << s.rank << ")";
    };
};

struct Move {
    Square start;
    Square end;
    int depth;
    Piece piece;
    bool capture;

    friend std::ostream& operator<<(std::ostream& os, Move const & m) {
        return os << m.piece << ": " << m.start << " -> " << m.end << std::endl;
    };
};