#include <iostream>
#include "game.h"

Game::Game() {

    max_depth = 5;

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
    // TESTING STUFF
    Piece p;
    p.set(Piece::PieceEncoding::WhitePawn);
    std::cout << p << std::endl;
    get_pawn_moves(true, 1);
    get_pawn_moves(false, 1);
    std::cout << "Total moves: " << valid_moves.size() << std::endl << std::endl;
    print_moves();
    return;

    std::cout << "game running" << std::endl;

    std::vector<std::vector<Piece>> board(8, std::vector<Piece>(8));
    std::string move;
    bool valid_move;

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

void Game::get_pawn_moves(bool white_move, int depth) {
    const unsigned long long occupied_space = get_bitboard();
    const unsigned long long open_space = ~occupied_space;

    const unsigned long long white_board = get_white_bitboard();
    const unsigned long long black_board = get_black_bitboard();

    const unsigned long long third_rank = 16711680;
    const unsigned long long sixth_rank = 280375465082880;

    unsigned long long mask_bit;

    if (white_move) {
        Piece white_pawn;
        white_pawn.set(Piece::PieceEncoding::WhitePawn);

        // WHITE PAWN MOVE: ONE FORWARD
        unsigned long long jump_one = white.pawn_board.bitboard << 8;
        std::cout << white.pawn_board.bitboard << std::endl;
        jump_one &= open_space;
        mask_bit = 65536;
        for (int rank = 3; rank <= 8; rank++) {
            for (int file = 1; file <= 8; file++) {
                if ((mask_bit & jump_one) != 0) {
                    Move m{
                        Square{rank-1, file},
                        Square{rank, file},
                        depth,
                        white_pawn,
                        false
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
        }

        // WHITE PAWN MOVE: TWO FORWARD
        unsigned long long jump_two = (third_rank & jump_one) << 8;
        jump_two &= open_space;
        mask_bit = 16777216;
        for (int file = 1; file <= 8; file++) {
            if ((mask_bit & jump_two) != 0) {
                Move m{
                    Square{2, file},
                    Square{4, file},
                    depth,
                    white_pawn,
                    false
                };
                valid_moves.push_back(m);
            }
            mask_bit <<= 1;
        }
        return;

        // WHITE PAWN MOVE: TAKE TOP LEFT
        unsigned long long take_left = white.pawn_board.bitboard << 9;
        take_left &= black_board;
        mask_bit = 65536;
        for (int rank = 3; rank <= 8; rank++) {
            mask_bit <<= 1;
            for (int file = 1; file <= 7; file++) {
                if ((mask_bit & take_left) != 0) {
                    Move m{
                        Square{rank-1, file+1},
                        Square{rank, file},
                        depth,
                        white_pawn,
                        true
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
        }

        // WHITE PAWN MOVE: TAKE TOP RIGHT
        unsigned long long take_right = white.pawn_board.bitboard << 7;
        take_right &= black_board;
        mask_bit = 65536;
        for (int rank = 3; rank <= 8; rank++) {
            for (int file = 2; file <= 8; file++) {
                if ((mask_bit & take_left) != 0) {
                    Move m{
                        Square{rank-1, file-1},
                        Square{rank, file},
                        depth,
                        white_pawn,
                        true
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
            mask_bit <<= 1;
        }
    } else {
        Piece black_pawn;
        black_pawn.set(Piece::PieceEncoding::BlackPawn);

        // BLACK PAWN MOVE: ONE FORWARD
        unsigned long long jump_one = black.pawn_board.bitboard >> 8;
        jump_one &= open_space;
        mask_bit = 1;
        for (int rank = 1; rank <= 6; rank++) {
            for (int file = 1; file <= 8; file++) {
                if ((mask_bit & jump_one) != 0) {
                    Move m{
                        Square{rank+1, file},
                        Square{rank, file},
                        depth,
                        black_pawn,
                        false
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
        }

        // BLACK PAWN MOVE: TWO FORWARD
        unsigned long long jump_two = (sixth_rank & jump_one) >> 8;
        jump_two &= open_space;
        mask_bit = 4294967296;
        for (int file = 1; file <= 8; file++) {
            if ((mask_bit & jump_two) != 0) {
                Move m{
                    Square{7, file},
                    Square{5, file},
                    depth,
                    black_pawn,
                    false
                };
                valid_moves.push_back(m);
            }
            mask_bit <<= 1;
        }

        // BLACK PAWN MOVE: TAKE BOTTOM LEFT
        unsigned long long take_left = black.pawn_board.bitboard >> 7;
        take_left &= white_board;
        mask_bit = 1;
        for (int rank = 1; rank <= 6; rank++) {
            mask_bit <<= 1;
            for (int file = 1; file <= 7; file++) {
                if ((mask_bit & take_left) != 0) {
                    Move m{
                        Square{rank+1, file+1},
                        Square{rank, file},
                        depth,
                        black_pawn,
                        true
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
        }

        // BLACK PAWN MOVE: TAKE BOTTOM RIGHT
        unsigned long long take_right = white.pawn_board.bitboard >> 9;
        take_right &= white_board;
        mask_bit = 65536;
        for (int rank = 1; rank <= 6; rank++) {
            for (int file = 2; file <= 8; file++) {
                if ((mask_bit & take_left) != 0) {
                    Move m{
                        Square{rank+1, file-1},
                        Square{rank, file},
                        depth,
                        black_pawn,
                        true
                    };
                    valid_moves.push_back(m);
                }
                mask_bit <<= 1;
            }
            mask_bit <<= 1;
        }
    }
}

void Game::print_moves() {
    for (auto move: valid_moves) {
        std::cout << move << std::endl;
    }
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


unsigned long long Game::get_bitboard() {
    unsigned long long bitboard = get_white_bitboard() | get_black_bitboard();
    return bitboard;
}


unsigned long long Game::get_white_bitboard() {
    unsigned long long bitboard = 0;
    bitboard |= white.king_board.bitboard | 
    white.queen_board.bitboard |
    white.rook_board.bitboard |
    white.bishop_board.bitboard |
    white.knight_board.bitboard |
    white.pawn_board.bitboard;

    return bitboard;
}

unsigned long long Game::get_black_bitboard() {
    unsigned long long bitboard = 0;
    bitboard |= black.king_board.bitboard |
    black.queen_board.bitboard |
    black.rook_board.bitboard |
    black.bishop_board.bitboard |
    black.knight_board.bitboard |
    black.pawn_board.bitboard;

    return bitboard;
}


void Game::print_bitboards() {
    unsigned long long int bitboard = get_bitboard();
    
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