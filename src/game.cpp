#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

#include "board.h"

const int icon_size = 133;
const int size = 100;
const int squareSize = 100;
float scaleFactor = static_cast<float>(squareSize) / 133; // 100 / 133 ≈ 0.7519
sf::Color lightColor(240, 217, 181); // Light Beige
sf::Color darkColor(181, 136, 99);   // Soft Brown
sf::Color highlightColor(255, 250, 130, 128); //reddish transparent

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

const std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    

sf::Sprite f[32]; // figures

Board board;


std::string toChessNote(sf::Vector2f p)
{
    std::string s = "";
    s += char(p.x/size+97);
    s += char(7-p.y/size+49);
    return s;
}

sf::Vector2f toCoord(char a, char b)
{
    int x = int(a) - 97;
    int y = 7 - int(b) + 49;
    return sf::Vector2f(x*size,y*size);
}

void clear_highlights() {
    for (int i = 0; i < 64; i++) {
        highlights[i] = 0;
    }
}

void highlight(sf::Vector2f pos) {
    int file = pos.x/size;
    int rank = pos.y/size;
    int idx = rank*8 + file;
    highlights[idx] = 1;
}

void move(std::string str)
{
    clear_highlights();

    sf::Vector2f oldPos = toCoord(str[0],str[1]);
    sf::Vector2f newPos = toCoord(str[2],str[3]);

    for (int i = 0; i < 32; i++) {
        if (f[i].getPosition() == newPos) {
            f[i].setPosition(-100,-100);
        }
    }

    for (int i = 0; i < 32; i++) {
        if (f[i].getPosition() == oldPos) {
            f[i].setPosition(newPos);
        }
    }

    highlight(oldPos);
    highlight(newPos);
}

void loadPosition()
{
    int k = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int n = board[i*8 + j];
            if (!n) {
                continue;
            }
            int x = (n & 7) - 1; 
            int y = (n & Piece::White) != 0 ? 1 : 0; 
            f[k].setTextureRect( sf::IntRect(icon_size*x,icon_size*y,icon_size,icon_size) );
            f[k].setPosition(size*j,size*i);
            f[k].setScale(scaleFactor, scaleFactor);
            k++;
        }
    }
}


int main()
{
    board.initialize_with_fen(starting_fen);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Board");

    sf::Texture t1,t2;
    t1.loadFromFile("images/figures.png");
    t2.loadFromFile("images/board.png");

    
    sf::Sprite sBoard(t2);

    for (int i = 0; i < 32; i++) {
        f[i].setTexture(t1);
    }

    loadPosition();

    bool isMove = false;
    float dx=0, dy=0;
    sf::Vector2f oldPos,newPos;
    std::string str;
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
                    for (int i = 0 ; i < 32; i++) {
                        if (f[i].getGlobalBounds().contains(pos.x, pos.y)) {
                            isMove = true;
                            active_figure_idx = i;
                            dx = pos.x - f[i].getPosition().x;
                            dy = pos.y - f[i].getPosition().y;
                            oldPos = f[i].getPosition();
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.key.code == sf::Mouse::Left) {
                    isMove = false;
                    sf::Vector2f p = f[active_figure_idx].getPosition() + sf::Vector2f(size/2,size/2);
                    sf::Vector2f newPos = sf::Vector2f( size*int(p.x/size), size*int(p.y/size) );
                    if (oldPos == newPos) {
                        //
                    } else {
                        str = toChessNote(oldPos) + toChessNote(newPos);
                        move(str);
                        std::cout << str << std::endl;
                        f[active_figure_idx].setPosition(newPos);
                    }
                }
            }
        }

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

            if (highlights[i]) {
                sf::RectangleShape highlight(sf::Vector2f(squareSize, squareSize));
                highlight.setFillColor(highlightColor);
                highlight.setPosition(col * squareSize, row * squareSize); 
                window.draw(highlight);
            }
        }
        
        // draw figures
        for (int i = 0; i < 32; i++) {
            window.draw(f[i]);
        }
        
        window.display();
    }
    
    
    return 0;
}