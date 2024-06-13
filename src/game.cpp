#include <iostream>
#include "game.h"

Game::Game() {

    std::cout << "Enter starting FEN string:" << std::endl;
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    //std::cin >> fen;
    fen_setup(fen);
    std::cout << "Initial Setup:" << std::endl;
    std::vector<std::vector<char>> board(8, std::vector<char>(8));
    get_board(board, false);
    print_board(board);
}

void Game::run() {
    std::cout << "game running" << std::endl;
    int count = 0;
    std::string move;
    while (count < 2) {
        std::vector<std::vector<char>> board(8, std::vector<char>(8));

        std::cout << "White to move" << std::endl;
        get_board(board, true);
        print_board(board);

        std::cin >> move;
        std::cout << "White does move: " << move << std::endl;

        std::cout << "Black to move" << std::endl;
        get_board(board, false);
        print_board(board);

        std::cin >> move;
        std::cout << "Black does move: " << move << std::endl;

        count++;
    }
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

        switch (c) {
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

void Game::get_board(std::vector<std::vector<char>>& board, bool is_white) {
    std::vector<std::vector<char>> white_board = white.get_board();
    std::vector<std::vector<char>> black_board = black.get_board();

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (is_white) {
                if (white_board[rank][file] != '.') {
                    board[rank][file] = white_board[rank][file];
                } else if (black_board[rank][file] != '.') {
                    board[rank][file] = char(black_board[rank][file] + 32);
                } else {
                    board[rank][file] = '.';
                }
            } else {
                if (white_board[rank][file] != '.') {
                    board[7 - rank][7 - file] = white_board[rank][file];
                } else if (black_board[rank][file] != '.') {
                    board[7 - rank][7 - file] = char(black_board[rank][file] + 32);
                } else {
                    board[7 - rank][7 - file] = '.';
                }
            }
        }
    }
}

void Game::print_board(std::vector<std::vector<char>>& board) {
    std::cout << std::endl;
    int h_length = 8;

    for (int i = 0; i < h_length; i++) {
        std::cout << " \u23AF\u23AF\u23AF";
    } 
    std::cout << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << '|';
        for (int j = 0; j < 8; j++) {
            std::string piece = get_piece_icon(board[i][j]);
            if (piece == ".") {
                if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                    std::cout << " \u25A0 ";
                } else {
                    std::cout << " \u25A1 ";
                }
                std::cout << "\uFFE8";
            } else {
                std::cout << " " << piece << " \uFFE8";
            }
        }
        std::cout << std::endl;
        for (int i = 0; i < h_length; i++) {
            std::cout << " \u23AF\u23AF\u23AF";
        } 
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::string Game::get_piece_icon(char algebraic_piece) {
    std::string piece_icon;
    switch (algebraic_piece) {
    case '.':
        piece_icon = ".";
        break;
    case 'K':
        piece_icon = "\u2654";
        break;
    case 'Q':
        piece_icon = "\u2655";
        break;
    case 'R':
        piece_icon = "\u2656";
        break;
    case 'B':
        piece_icon = "\u2657";
        break;
    case 'N':
        piece_icon = "\u2658";
        break;
    case 'P':
        piece_icon = "\u2659";
        break;
    case 'k':
        piece_icon = "\u265A";
        break;
    case 'q':
        piece_icon = "\u265B";
        break;
    case 'r':
        piece_icon = "\u265C";
        break;
    case 'b':
        piece_icon = "\u265D";
        break;
    case 'n':
        piece_icon = "\u265E";
        break;
    case 'p':
        piece_icon = "\u265F";
        break;
    
    default:
        break;
    }
    return piece_icon;
}