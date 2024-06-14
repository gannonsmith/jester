#include <iostream>
#include "game.h"

Game::Game() {

    std::cout << "Enter starting FEN string:" << std::endl;
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    //std::cin >> fen;
    fen_setup(fen);
    std::cout << "Initial Setup:" << std::endl;
    std::vector<std::vector<Piece>> board(8, std::vector<Piece>(8));
    get_board(board);
    print_board(board);

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
            get_board(board);
            print_board(board);

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
            get_board(board);
            print_board(board);

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
    
    if ((white_to_move && move_str[0] != white.get(start_square)) ||
        (!white_to_move && move_str[0] != black.get(start_square)))
    {
        std::cout << "piece doesn't match starting square" << std::endl;
        return false;
    }

    Move move {
        {int(move_str[2]), int(move_str[1])},
        {int(move_str[5]), int(move_str[4])},
        {Piece(move_str[0])},
        move_str[3] == 'x'
    };

    if (!is_valid_move(move)) {
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

bool Game::is_valid_move(Move& move) {
    //char piece;
    return true;
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

void Game::get_board(std::vector<std::vector<Piece>>& board) {
    std::vector<std::vector<Piece>> white_board = white.get_board();
    std::vector<std::vector<Piece>> black_board = black.get_board();

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (white_board[rank][file] != '.') {
                board[rank][file] = white_board[rank][file];
            } else if (black_board[rank][file] != '.') {
                board[rank][file] = black_board[rank][file];
            } else {
                board[rank][file] = Piece::Empty;
            }
        }
    }
}

void Game::print_board(std::vector<std::vector<Piece>>& board) {
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
            std::string piece_str = get_piece_icon(board[i][j]);
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

std::string Game::get_piece_icon(Piece algebraic_piece) {
    std::string piece_icon;
    switch (algebraic_piece) {
    case Piece::Empty:
        piece_icon = ".";
        break;
    case Piece::WhiteKing:
        piece_icon = "\u2654";
        break;
    case Piece::WhiteQueen:
        piece_icon = "\u2655";
        break;
    case Piece::WhiteRook:
        piece_icon = "\u2656";
        break;
    case Piece::WhiteBishop:
        piece_icon = "\u2657";
        break;
    case Piece::WhiteKnight:
        piece_icon = "\u2658";
        break;
    case Piece::WhitePawn:
        piece_icon = "\u2659";
        break;
    case Piece::BlackKing:
        piece_icon = "\u265A";
        break;
    case Piece::BlackQueen:
        piece_icon = "\u265B";
        break;
    case Piece::BlackRook:
        piece_icon = "\u265C";
        break;
    case Piece::BlackBishop:
        piece_icon = "\u265D";
        break;
    case Piece::BlackKnight:
        piece_icon = "\u265E";
        break;
    case Piece::BlackPawn:
        piece_icon = "\u265F";
        break;
    default:
        break;
    }
    return piece_icon;
}