#include <iostream>
#include "game.h"

Game::Game() {

    std::cout << "Enter starting FEN string" << std::endl;
    std::string fen;
    std::cin >> fen;
    fen_setup(fen);
    print_board();
}

void Game::fen_setup(std::string& fen_string) {
    int rank = 0;
    int file = 0;
    for (char c: fen_string) {
        if (c == '/') {
            rank++;
            file = 0;
            continue;
        }
        if ('0' <= c && c <= '9') {
            file += c - '0';
            continue;
        }

        switch (c)
        {
        case 'K':
            white.king_board.set(rank, file);
            break;
        case 'Q':
            white.queen_board.set(rank, file);
            break;
        case 'R':
            white.rook_board.set(rank, file);
            break;
        case 'B':
            white.bishop_board.set(rank, file);
            break;
        case 'N':
            white.knight_board.set(rank, file);
            break;
        case 'P':
            white.pawn_board.set(rank, file);
            break;
        case 'k':
            black.king_board.set(rank, file);
            break;
        case 'q':
            black.queen_board.set(rank, file);
            break;
        case 'r':
            black.rook_board.set(rank, file);
            break;
        case 'b':
            black.bishop_board.set(rank, file);
            break;
        case 'n':
            black.knight_board.set(rank, file);
            break;
        case 'p':
            black.pawn_board.set(rank, file);
            break;
        default:
            std::cout << "fen string probably invalid" << std::endl;
            break;
        }
        file++;
    }
}

void Game::run() {
    std::cout << "game running" << std::endl;
    print_board();
}

void Game::print_board() {
    std::vector<std::vector<char>> white_board = white.get_board();
    std::vector<std::vector<char>> black_board = black.get_board();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (white_board[i][j] != '.') {
                std::cout << white_board[i][j] << " ";
            } else if (black_board[i][j] != '.') {
                std::cout << black_board[i][j] << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}