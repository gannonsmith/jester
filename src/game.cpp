#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

#include "board.h"
using namespace sf;

const int icon_size = 133;
const int padding = 14;
const int size = icon_size + padding;

const std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    


Sprite f[32]; // figures

Board board;


std::string toChessNote(Vector2f p)
{
    std::string s = "";
    s += char(p.x/size+97);
    s += char(7-p.y/size+49);
    return s;
}

Vector2f toCoord(char a, char b)
{
    int x = int(a) - 97;
    int y = 7 - int(b) + 49;
    return Vector2f(x*size,y*size);
}

void move(std::string str)
{
    Vector2f oldPos = toCoord(str[0],str[1]);
    Vector2f newPos = toCoord(str[2],str[3]);

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
            f[k].setTextureRect( IntRect(icon_size*x,icon_size*y,icon_size,icon_size) );
            f[k].setPosition(size*j,size*i);
            k++;
        }
    }
}


int main()
{
    board.initialize_with_fen(starting_fen);

    RenderWindow window(VideoMode(1168, 1166), "The Chess!");

    Texture t1,t2;
    t1.loadFromFile("images/figures.png");
    t2.loadFromFile("images/board.png");

    Sprite s(t1);
    Sprite sBoard(t2);

    for (int i = 0; i < 32; i++) {
        f[i].setTexture(t1);
    }

    loadPosition();

    bool isMove = false;
    float dx=0, dy=0;
    Vector2f oldPos,newPos;
    std::string str;
    int n=0;

    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed) {
                window.close();
            }

            // drag and drop
            if (e.type == Event::MouseButtonPressed) {
                if (e.key.code == Mouse::Left) {
                    for (int i = 0 ; i < 32; i++) {
                        if (f[i].getGlobalBounds().contains(pos.x, pos.y)) {
                            isMove = true;
                            n = i;
                            dx = pos.x - f[i].getPosition().x;
                            dy = pos.y - f[i].getPosition().y;
                            oldPos = f[i].getPosition();
                        }
                    }
                }
            }

            if (e.type == Event::MouseButtonReleased) {
                if (e.key.code == Mouse::Left) {
                    isMove = false;
                    Vector2f p = f[n].getPosition() + Vector2f(size/2,size/2);
                    Vector2f newPos = Vector2f( size*int(p.x/size), size*int(p.y/size) );
                    str = toChessNote(oldPos) + toChessNote(newPos);
                    move(str);
                    std::cout << str << std::endl;
                    f[n].setPosition(newPos);
                }
            }
        }

        if (isMove) {
            f[n].setPosition(pos.x-dx, pos.y-dy);
        }
        
        // draw
        window.clear();
        window.draw(sBoard);
        for (int i = 0; i < 32; i++) {
            window.draw(f[i]);
        }
        window.display();
    }
    
    
    return 0;
}