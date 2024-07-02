#pragma once

#include <iostream>

struct Square {
    int rank;
    int file;

    friend std::ostream& operator<<(std::ostream& os, Square const & s) {
        return os << "(" << char(s.file + 'a' - 1) << s.rank << ")";
    };
};
