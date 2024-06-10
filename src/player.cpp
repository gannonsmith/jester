#include <iostream>
#include "player.h"

std::vector<std::vector<char>>& White::get_board() {
    return display_board;
}

std::vector<std::vector<char>>& Black::get_board() {
    return display_board;
}

White::White() {
    pawn_board = PawnBoard(65280);
}

Black::Black() {

}
