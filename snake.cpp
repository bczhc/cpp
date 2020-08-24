#include "./zhc.h"
#include <cstdio>
#include <cstring>
#include <iostream>

#define RANDOM(a, b) (((double)rand() / RAND_MAX) * (b - a) + a)

class SnakeGame {
    class Point {
    public:
        int x = 0;
        int y = 0;
    };

private:
    int x = 10, y = 10;
    LinkedList <Point> snake;
    int &gameover;
    char *map = nullptr;
    Point food{};

    Point generateRandomFood() {
        Point p{.x = 1, .y = 2};
        return p;
    }

    void move(int offX, int offY) {
        Point next;
        Point prev = snake.get(0);
        next.x = prev.x + offX;
        next.y = prev.y + offY;
        char nextChar = getMapPoint(next.x, next.y);
        if (nextChar != '$')
            snake.removeLast();
        snake.putFirst(next);
        if (nextChar == '$') placeFood();
        if (nextChar == '*' || nextChar == 'O' || nextChar == 'X') gameover = 1;
        drawMap();
    }

    inline char getMapPoint(int x_, int y_) { return map[x_ + y_ * x]; }

    inline void setMapPoint(int x_, int y_, char c) {
        map[x_ + y_ * x] = c;
    }

    inline void placeFood() {
        char p;
        do {
            food.x = RANDOM(1, x - 2), food.y = RANDOM(1, y - 2);
            p = getMapPoint(food.x, food.y);
        } while (p == 'O' || p == 'X');
        setMapPoint(food.x, food.y, '$');
    }

    inline void initMap() {
        size_t mapSize = (x + 2) * (y + 2);
        map = (char *) malloc(mapSize);
        memset(map, ' ', mapSize);
        for (int i = 0; i < y; ++i) {
            if (i == 0 || i == y - 1)
                for (int j = 0; j < x; ++j) {
                    setMapPoint(j, i, '*');
                }
            else
                setMapPoint(0, i, '*'), setMapPoint(x - 1, i, '*');
        }
    }

    void drawMap() {
        for (int i = 1; i < y - 1; ++i) {
            for (int j = 1; j < x - 1; ++j) {
                setMapPoint(j, i, ' ');
            }
        }
        int snakeLen = snake.length();
        for (int i = 0; i < snakeLen; ++i) {
            Point node = snake.get(i);
            setMapPoint(node.x, node.y, 'O');
        }
        Point head = snake.get(0);
        setMapPoint(head.x, head.y, 'X');
        setMapPoint(food.x, food.y, '$');
    }

public:
    SnakeGame(int x, int y, int &gameoverMark) : x(x), y(y), gameover(gameoverMark) {
        initMap();
        Point firstPoint{.x = 3, .y = 3};
        snake.put(firstPoint);
        drawMap();
        placeFood();
    }

    void moveR() { move(1, 0); }

    void moveL() { move(-1, 0); }

    void moveU() { move(0, -1); }

    void moveD() { move(0, 1); }

    void print() {
        for (int i = 0; i < y; ++i) {
            for (int j = 0; j < x; ++j) {
                cout << getMapPoint(j, i) << ' ';
            }
            cout << endl;
        }
    }
};

int main(int argc, char **argv) {
    int gameover = 0;
    SnakeGame game(10, 10, gameover);
    while (!gameover) {
        char c;
        scanf("%c", &c);
        switch (c) {
            case 'w':
            case 'W':
                game.moveU();
                break;
            case 'a':
            case 'A':
                game.moveL();
                break;
            case 's':
            case 'S':
                game.moveD();
                break;
            case 'd':
            case 'D':
                game.moveR();
                break;
        }
        game.print();
    }
    return 0;
}
