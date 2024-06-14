#include <iostream>
#include "game.h"

Game::Game() {

    std::cout << "Enter starting FEN string:" << std::endl;
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    //std::cin >> fen;
    fen_setup(fen);
    std::cout << "Initial Setup:" << std::endl;
    std::vector<std::vector<char>> board(8, std::vector<char>(8));
    get_board(board);
    print_board(board);

    std::cout << "Moves should be printed in long algebraic notation." << std::endl;
}

void Game::run() {
    std::cout << "game running" << std::endl;

    std::vector<std::vector<char>> board(8, std::vector<char>(8));
    std::string move;
    bool valid_move;

    int count = 0;
    while (count < 2) {
        valid_move = false;
        while (!valid_move) {
            std::cout << "White to move" << std::endl;
            get_board(board);
            print_board(board);

            std::cin >> move;
            valid_move = this->move(move, true);
        }
        std::cout << "White does move: " << move << std::endl;

        valid_move = false;
        while (!valid_move) {
            std::cout << "Black to move" << std::endl;
            get_board(board);
            print_board(board);

            std::cin >> move;
            valid_move = this->move(move, false);
        }
        std::cout << "Black does move: " << move << std::endl;

        count++;
    }
}

bool Game::move(std::string& move, bool white_to_move) {
    if (move.size() != 6) {
        std::cout << "move string is invalid length" << std::endl;
        return false;
    }
    // Ne2-e4
    if (!valid_piece(move[0], white_to_move)) {
        std::cout << "piece character is invalid" << std::endl;
        return false;
    }

    if (!valid_square(move[1], move[2]) || !valid_square(move[4], move[5])) {
        std::cout << "invalid squares" << std::endl;
        return false;
    }
    


}

bool Game::valid_piece(char piece, bool white) {
    if (white && 
        (
            piece == 'K' || piece == 'Q' || piece == 'R' || piece == 'B' || 
            piece == 'N' || piece == 'P'
        )
    ) {
        return true;
    } else if (!white && 
        (
            piece == 'k' || piece == 'q' || piece == 'r' || piece == 'b' || 
            piece == 'n' || piece == 'p'
        )
    ) {
        return true;
    } else {
        return false;
    }
}

bool Game::valid_square(char file, char rank) {
    if (rank < '1' || rank > '8' || file < 'a' || 'h' < file) {
        return false;
    } else {
        return true;
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

void Game::get_board(std::vector<std::vector<char>>& board) {
    std::vector<std::vector<char>> white_board = white.get_board();
    std::vector<std::vector<char>> black_board = black.get_board();

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (white_board[rank][file] != '.') {
                board[rank][file] = white_board[rank][file];
            } else if (black_board[rank][file] != '.') {
                board[rank][file] = char(black_board[rank][file] + 32);
            } else {
                board[rank][file] = '.';
            }
        }
    }
}

void Game::print_board(std::vector<std::vector<char>>& board) {
    std::cout << std::endl;
    int h_length = 8;

    std::cout << "   ";
    for (int i = 0; i < h_length; i++) {
        std::cout << " \u23AF\u23AF\u23AF";
    } 
    std::cout << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << 8 - i << "  |";
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
        std::cout << "   ";
        for (int i = 0; i < h_length; i++) {
            std::cout << " \u23AF\u23AF\u23AF";
        } 
        std::cout << std::endl;
    }
    std::cout << "  ";
    for (char c = 'a'; c < 'i'; c++) {
        std::cout << "   " << c;
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