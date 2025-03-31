#include "game.h"

const int icon_size = 133;
const int size = 100;
const int squareSize = 100;
float scaleFactor = static_cast<float>(squareSize) / 133; // 100 / 133 ≈ 0.7519
sf::Color lightColor(240, 217, 181); // Light Beige
sf::Color darkColor(181, 136, 99);   // Soft Brown
sf::Color highlightColor(255, 165, 0, 128); //orangish transparent
sf::Color selectedColor(255, 0, 0, 128); // reddish transparent

std::map<int, int> piece_to_figure_idx = {
    { Piece::Black | Piece::King,   0 },
    { Piece::Black | Piece::Queen,  1 },
    { Piece::Black | Piece::Bishop, 2 },
    { Piece::Black | Piece::Knight, 3 },
    { Piece::Black | Piece::Rook,   4 },
    { Piece::Black | Piece::Pawn,   5 },
    { Piece::White | Piece::King,   6 },
    { Piece::White | Piece::Queen,  7 },
    { Piece::White | Piece::Bishop, 8 },
    { Piece::White | Piece::Knight, 9 },
    { Piece::White | Piece::Rook,  10 },
    { Piece::White | Piece::Pawn,  11 }
};


// highlighted squares
int highlights[64] = 
    {0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0};

int selected_square = -1;

sf::Sprite f_template[12]; // figure template
std::vector<sf::Sprite> f; // figures

std::string toChessNote(sf::Vector2f p)
{
    std::string s = "";
    s += char(p.x/size+97);
    s += char(7-p.y/size+49);
    return s;
}

int to_index(sf::Vector2f pos) {
    int file = pos.x/size;
    int rank = pos.y/size;
    int idx = rank*8 + file;
    return idx;
}

sf::Vector2f toCoord(char a, char b)
{
    int x = int(a) - 97;
    int y = 7 - int(b) + 49;
    return sf::Vector2f(x*size,y*size);
}

/*
    Clears highlight array.
*/
void clear_highlights() {
    for (int i = 0; i < 64; i++) {
        highlights[i] = 0;
    }
}

/*
    Sets highlight array to 1 for the given square index.
*/
void highlight(int square_idx) {
    highlights[square_idx] = 1;
}

void Game::highlight_moves(int start_square) {
    for (Move move: *moves) {
        if (move.start_square == start_square) {
            highlight(move.start_square);
            highlight(move.target_square);
        }
    }
}

void Game::generate() {
    board->generate_moves();
    moves = board->get_moves();
    std::cout << moves->size() << std::endl;
}

void Game::move(sf::Vector2f oldPos, sf::Vector2f newPos)
{
    clear_highlights();
    
    int old_idx = to_index(oldPos);
    int new_idx = to_index(newPos);

    if (old_idx == new_idx) {
        return;
    }

    std::string str = toChessNote(oldPos) + toChessNote(newPos);
    std::cout << str << std::endl;
    if (moves == nullptr) {
        return;
    }
    
    for (Move move: *moves) {
        if (move.start_square == old_idx && move.target_square == new_idx) {
            // TODO: handle pawn promotions*
            board = move.resulting_board;
            break;
        }
    }
    
    load_position();

    highlight(to_index(oldPos));
    highlight(to_index(newPos));
}

void Game::load_position()
{
    f.clear();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int n = (*board)[i*8 + j];
            if (n == Piece::None) {
                continue;
            }
            int figure_idx = piece_to_figure_idx[n];
            f.emplace_back(f_template[figure_idx]);
            f.back().setPosition(size*j,size*i);
        }
    }
    generate();
}

void Game::preload_figure_template(sf::Texture& t)
{
    t.loadFromFile("images/figures.png");

    int x, y;
    for (int i = 0; i < 12; i++) {
        x = i % 6;
        y = i / 6;
        f_template[i].setTexture(t);
        f_template[i].setTextureRect( sf::IntRect(icon_size*x,icon_size*y,icon_size,icon_size) );
        f_template[i].setScale(scaleFactor, scaleFactor);
    }
}

void Game::render()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Board");

    sf::Texture t;
    preload_figure_template(t);

    load_position();

    bool isMove = false;
    float dx=0, dy=0;
    sf::Vector2f oldPos,newPos;
    int active_figure_idx = 0;
    

    while (window.isOpen())
    {
        sf::Vector2i pos = sf::Mouse::getPosition(window);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            window.clear(sf::Color::White);

            // drag and drop
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.key.code == sf::Mouse::Left) {
                    for (int i = 0 ; i < f.size(); i++) {
                        if (f[i].getGlobalBounds().contains(pos.x, pos.y)) {
                            isMove = true;
                            active_figure_idx = i;
                            dx = pos.x - f[i].getPosition().x;
                            dy = pos.y - f[i].getPosition().y;
                            oldPos = f[i].getPosition();

                            clear_highlights();

                            int old_idx = to_index(oldPos);
                            selected_square = old_idx;
                            //tmp
                            {
                                std::cout << selected_square << std::endl;
                            }
                            highlight_moves(old_idx);
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.key.code == sf::Mouse::Left && isMove) {
                    isMove = false;
                    sf::Vector2f p = f[active_figure_idx].getPosition() + sf::Vector2f(size/2,size/2);
                    sf::Vector2f newPos = sf::Vector2f( size*int(p.x/size), size*int(p.y/size) );

                    // TODO: determine it is an actual move
                    f[active_figure_idx].setPosition(newPos);
                    selected_square = -1;

                    move(oldPos, newPos);
                }
            }
        }

        // for click and dragging pieces
        if (isMove) {
            f[active_figure_idx].setPosition(pos.x-dx, pos.y-dy);
        }

        // draw chessboard
        for (int i = 0; i < 64; i++)
        {
            int row = i / 8;
            int col = i % 8;
            sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
            square.setPosition(col * squareSize, row * squareSize);

            if ((row + col) % 2 == 0) {
                square.setFillColor(lightColor); 
            } else {
                square.setFillColor(darkColor); 
            }

            // draw squares
            window.draw(square);

            if (i == selected_square) {
                sf::RectangleShape highlight(sf::Vector2f(squareSize, squareSize));
                highlight.setFillColor(selectedColor);
                highlight.setPosition(col * squareSize, row * squareSize); 
                window.draw(highlight);
            } else if (highlights[i]) {
                sf::RectangleShape highlight(sf::Vector2f(squareSize, squareSize));
                highlight.setFillColor(highlightColor);
                highlight.setPosition(col * squareSize, row * squareSize); 
                window.draw(highlight);
            }
        }
        
        // draw figures
        for (int i = 0; i < f.size(); i++) {
            window.draw(f[i]);
        }
        
        window.display();
    }
}