#include <iostream>
#include "player.h"

std::vector<std::vector<char>>* Player::get_board() {
    std::cout << "Non-specific player called get-board" << std::endl;
}

std::vector<std::vector<char>>* White::get_board() {
    
}

std::vector<std::vector<char>>* Black::get_board() {

}
