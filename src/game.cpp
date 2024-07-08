#include <iostream>
#include "game.h"

Game::Game() {

    max_depth = 5;
    current_state.set_empty();

    std::cout << "Enter starting FEN string:" << std::endl;
    std::string fen = "1r4K1/1r6/6N1/8/8/8/8/8";
    //fen = "8/8/8/8/8/8/8/8";
    //std::cin >> fen;
    fen_setup(fen);
    current_state.under_attack = current_state.generate_capture_spaces();

    std::cout << "Initial Setup:" << std::endl;
    print_board();

    print_bitboards(current_state.get_bitboard());
    //std::cout << std::endl << "breakpoint" << std::endl;

    std::cout << "Moves should be printed in long algebraic notation." << std::endl;
}

void Game::run() {
    // TESTING STUFF
    //current_state.test_switch_turn();
    current_state.get_states(game_states);
    std::cout << "Total moves: " << game_states[0].size() << std::endl << std::endl;
    //print_bitboards(current_state.generate_capture_spaces());
    //print_bitboards(game_states[0][0].get_bitboard());
    //print_bitboards(game_states[0][0].under_attack);
    //print_moves();
    return;
    // END TESTING STUFF

    std::cout << "game running" << std::endl;

    std::vector<std::vector<Piece>> board(8, std::vector<Piece>(8));
    std::string move;
    //bool valid_move;

    int count = 0;
    while (count == 0) {
        
        Move white_move;
        bool end = get_move(white_move, true);
        if (end) {
            std::cout << "Ending game" << std::endl;
            return;
        }

        Move black_move;
        end = get_move(black_move, false);
        if (end) {
            std::cout << "Ending game" << std::endl;
            return;
        }

        count++;
    }
}

bool Game::get_move(Move& move, bool white_to_move) {
    std::string move_str;

    bool valid_move = false;
    while (!valid_move) {
        if (white_to_move) {
            std::cout << "White to move" << std::endl << "Answer 'q' to exit" << std::endl;
        } else {
            std::cout << "Black to move" << std::endl << "Answer 'q' to exit" << std::endl;
        }
        print_board();

        std::cin >> move_str;
        if (move_str == "q") {
            return false;
        }
        valid_move = str_to_move(move, move_str, white_to_move);
    }

    if (white_to_move) {
        std::cout << "White does move: " << move_str << std::endl;
    } else {
        std::cout << "Black does move: " << move_str << std::endl;
    }

    return true;
}


bool Game::str_to_move(Move& move, std::string& move_str, bool white_to_move) {
    if (move_str.size() != 6) {
        std::cout << "move string is invalid length" << std::endl;
        return false;
    }

    Piece piece;
    if (!valid_piece(piece, move_str[0], white_to_move)) {
        std::cout << "piece character is invalid" << std::endl;
        return false;
    }

    Square start_square;
    Square end_square;
    if (!valid_square(start_square, move_str[2], move_str[1]) ||
        !valid_square(end_square, move_str[5], move_str[4])) 
    {
        std::cout << "invalid square" << std::endl;
        return false;
    }

    bool capture;
    if (move_str[3] != '-' && move_str[3] != 'x') {
        std::cout << "capture not specified" << std::endl;
        return false;
    }
    capture = move_str[3] == 'x';

    move.start = start_square;
    move.end = end_square;
    move.depth = 1;
    move.piece = piece;
    move.capture = capture;

    // TODO
    if (false) {
        std::cout << "cannot move there" << std::endl;
        return false;
    }

    return true;
}


bool Game::valid_piece(Piece& piece, char piece_c, bool white) {
    if (white && 
        (piece_c == 'K' || piece_c == 'Q' || piece_c == 'R' || piece_c == 'B' || 
        piece_c == 'N' || piece_c == 'P')) 
    {
        piece = Piece::get(piece_c);
        return true;
    } else if (!white && 
        (piece_c == 'k' || piece_c == 'q' || piece_c == 'r' || piece_c == 'b' || 
        piece_c == 'n' || piece_c == 'p'))
    {
        piece = Piece::get(piece_c);
        return true;
    } else {
        return false;
    }
}

bool Game::valid_square(Square& square, char rank, char file) {
    if (rank < '1' || rank > '8' || file < 'a' || 'h' < file) {
        return false;
    }
    square.rank = rank - '1' + 1;
    square.file = file - 'a' + 1;
    return true;
}

void Game::get_moves(bool white_to_move, int depth) {
    return;
    
}

void Game::fen_setup(std::string& fen_string) {
    int rank = 8;
    int file = 1;
    for (char c: fen_string) {
        if (c == '/') {
            rank--;
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
    for (int i = 7; i >= 0; i--) {
        std::cout << i+1 << "  |";
        for (int j = 7; j >= 0; j--) {
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
    board[(square.rank-1)*8 + 8-square.file] = piece;
    current_state.set(piece, square);
}


void Game::print_bitboards(const unsigned long long bitboard) {
    unsigned long long bit = 1;
    bit <<= 63;
    for (int rank = 8; rank >= 1; rank--) {
        for (int file = 1; file <= 8; file++) {
            if ((bit & bitboard) == 0) {
                std::cout << 0;
            } else {
                std::cout << 1;
            }
            bit >>= 1;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}