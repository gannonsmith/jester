#include <iostream>
#include "game.h"

Game::Game() {

    std::cout << "Enter starting FEN string:" << std::endl;
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    //std::cin >> fen;
    fen_setup(fen);

    std::cout << "Initial Setup:" << std::endl;
    print_board();

    print_bitboards();

    std::cout << "Moves should be printed in long algebraic notation." << std::endl;
}

void Game::run() {
    std::cout << "game running" << std::endl;

    std::vector<std::vector<Piece>> board(8, std::vector<Piece>(8));
    std::string move;
    bool valid_move;

    int count = 0;
    while (count == 0) {
        valid_move = false;
        while (!valid_move) {
            std::cout << "White to move" << std::endl << "Answer 'q' to exit" << std::endl;
            //refresh_board();
            print_board();

            std::cin >> move;
            if (move == "q") {
                return;
            }
            valid_move = this->move(move, true);
        }
        std::cout << "White does move: " << move << std::endl;

        valid_move = false;
        while (!valid_move) {
            std::cout << "Black to move" << std::endl << "Answer 'q' to exit" << std::endl;
            //refresh_board();
            print_board();

            std::cin >> move;
            if (move == "q") {
                return;
            }
            valid_move = this->move(move, false);
        }
        std::cout << "Black does move: " << move << std::endl;

        count++;
    }
}

bool Game::move(std::string& move_str, bool white_to_move) {
    if (move_str.size() != 6) {
        std::cout << "move string is invalid length" << std::endl;
        return false;
    }
    if (!valid_piece(move_str[0], white_to_move)) {
        std::cout << "piece character is invalid" << std::endl;
        return false;
    }
    if (!valid_square(move_str[2], move_str[1]) ||
        !valid_square(move_str[5], move_str[4])) 
    {
        std::cout << "invalid square" << std::endl;
        return false;
    }
    if (move_str[3] != '-' && move_str[3] != 'x') {
        std::cout << "capture not specified" << std::endl;
        return false;
    }

    Square start_square {int(move_str[2]), int(move_str[1])};
    
    if (true)
    {
        std::cout << "piece doesn't match starting square" << std::endl;
        return false;
    }
    /*
    Move move {
        {int(move_str[2]), int(move_str[1])},
        {int(move_str[5]), int(move_str[4])},
        {1},
        {Piece(move_str[0])},
        move_str[3] == 'x'
    };*/

    if (true) {
        std::cout << "cannot move there" << std::endl;
        return false;
    }

    //make move
    return true;
}

bool Game::valid_piece(char piece, bool white) {
    if (white && 
        (piece == 'K' || piece == 'Q' || piece == 'R' || piece == 'B' || 
        piece == 'N' || piece == 'P')) 
    {
        return true;
    } else if (!white && 
        (piece == 'k' || piece == 'q' || piece == 'r' || piece == 'b' || 
        piece == 'n' || piece == 'p'))
    {
        return true;
    } else {
        return false;
    }
}

bool Game::valid_square(char rank, char file) {
    if (rank < '1' || rank > '8' || file < 'a' || 'h' < file) {
        return false;
    } else {
        return true;
    }
}

void Game::get_moves() {
    //refresh_board();
    // white pawns
    unsigned long long one_forward = white.pawn_board.bitboard << 8;
    unsigned long long two_forward = (65280 & white.pawn_board.bitboard) << 16;
    unsigned long long one_forward_left = white.pawn_board.bitboard << 7;
    unsigned long long one_forward_right = white.pawn_board.bitboard << 9;

    unsigned long long knight_up_left = white.knight_board.bitboard << 15;
    unsigned long long knight_up_right = white.knight_board.bitboard << 17;
    unsigned long long knight_left_up = white.knight_board.bitboard << 6;
    unsigned long long knight_right_up = white.knight_board.bitboard << 10;
    unsigned long long knight_down_left = white.knight_board.bitboard >> 15;
    unsigned long long knight_down_right = white.knight_board.bitboard >> 17;
    unsigned long long knight_left_down = white.knight_board.bitboard >> 6;
    unsigned long long knight_right_down = white.knight_board.bitboard >> 10;
}

void Game::fen_setup(std::string& fen_string) {
    int rank = 1;
    int file = 1;
    for (char c: fen_string) {
        if (c == '/') {
            rank++;
            file = 1;
            continue;
        }
        if ('0' <= c && c <= '9') {
            file += c - '0';
            continue;
        }
        Piece piece = Piece::get(c);
        Square square{rank, file};
        set_square(piece, square);
        
        file++;
    }
}


void Game::print_board() {
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
            std::string piece_str = board[(i)*8 + j].get_display();
            if (piece_str == ".") {
                if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                    std::cout << " \u25A0 ";
                } else {
                    std::cout << " \u25A1 ";
                }
                std::cout << "\uFFE8";
            } else {
                std::cout << " " << piece_str << " \uFFE8";
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


void Game::set_square(Piece piece, Square& square) {
    board[(square.rank-1)*8 + square.file-1] = piece;

    Player* p;
    if (piece.white()) {
        p = &white;
    } else if (piece.black()) {
        p = &black;
    } else {
        std::cout << "something is wrong" << std::endl;
        return;
    }

    if (piece.king()) {
        p->king_board.set(square);
    } else if (piece.queen()) {
        p->queen_board.set(square);
    } else if (piece.rook()) {
        p->rook_board.set(square);
    } else if (piece.bishop()) {
        p->bishop_board.set(square);
    } else if (piece.knight()) {
        p->knight_board.set(square);
    } else if (piece.pawn()) {
        p->pawn_board.set(square);
    } else {
        std::cout << "something is wrong" << std::endl;
        return;
    }
}


void Game::print_bitboards() {
    unsigned long long int bitboard = 0;
    bitboard |= white.king_board.bitboard | 
    white.queen_board.bitboard |
    white.rook_board.bitboard |
    white.bishop_board.bitboard |
    white.knight_board.bitboard |
    white.pawn_board.bitboard |
    black.king_board.bitboard |
    black.queen_board.bitboard |
    black.rook_board.bitboard |
    black.bishop_board.bitboard |
    black.knight_board.bitboard |
    black.pawn_board.bitboard;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            unsigned long long int tmp = bitboard >> ((rank*8) + file);
            unsigned long long int one = 1;
            auto t = tmp & one;
            std::cout << t;
        }
        std::cout << std::endl;
    }
}