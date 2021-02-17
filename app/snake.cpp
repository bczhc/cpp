#include <iostream>

#ifndef __WIN32

#include "../linked_list.hpp"
#include "../doubly_linked_list.hpp"
#include <cstdint>
#include <cstdio>
#include <termios.h>
#include "../utils.hpp"
#include "../concurrent.hpp"

#define MAP(x, y) (map[(x) + (y) *col])
#ifndef RANDOM
#define RANDOM(a, b) (((double) rand() / RAND_MAX) * ((b) - (a)) + (a))
#endif
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

using namespace bczhc;

int64_t getTimestamp() {
    return getCurrentTimeMillis();
}

class SnakeGame {
    class Point {
    public:
        int x = 0;
        int y = 0;

        Point(int x, int y) : x(x), y(y) {}

        Point() = default;
    };

private:
    int col, row;
    char *map = nullptr;
    char orientation = RIGHT;
    bool gameover = false;
    bczhc::DoublyLinkedList<Point> snake{};
    int64_t manualMoveTime = 0;
    int delayMillis;

    void init() {
        int size = (col + 2) * (row + 2);
        map = new char[size];
        memset(map, ' ', size);
        // fill the wall
        for (int i = 0; i < col; ++i) {
            MAP(i, 0) = '*';
            MAP(i, row - 1) = '*';
        }
        for (int i = 0; i < row; ++i) {
            MAP(0, i) = '*';
            MAP(col - 1, i) = '*';
        }
        // the snake's initial head
        snake.insert(Point(1, 1));
        generateFood();
    }

    void drawToMap() {
        int length = snake.length();
        Point head = snake.get(0);
        MAP(head.x, head.y) = 'O';
        for (int i = 1; i < length; ++i) {
            Point p = snake.get(i);
            MAP(p.x, p.y) = 'X';
        }
    }

    void generateFood() {
        int ranX, ranY;
        do {
            ranX = RANDOM(1, col - 2), ranY = RANDOM(1, row - 2);
        } while (MAP(ranX, ranY) != ' ');
        MAP(ranX, ranY) = '$';
    }

    void print() {
        for (int y = 0; y < row; ++y) {
            for (int x = 0; x < col; ++x) {
                printf(" %c", MAP(x, y));
            }
            printf("\n");
        }
    }

public:
    SnakeGame(int col, int row, int delayMillis)
            : col(col + 2), row(row + 2), delayMillis(delayMillis) {
        init();
    };

    ~SnakeGame() { delete[] this->map; }

    bool getGameoverStatus() const { return gameover; }

    void move() {
        Point coor = snake.getFirst();
        Point next = coor;
        switch (orientation) {
            case UP:
                --next.y;
                break;
            case DOWN:
                ++next.y;
                break;
            case LEFT:
                --next.x;
                break;
            case RIGHT:
                ++next.x;
                break;
            default:
                break;
        }
        char c = MAP(next.x, next.y);
        snake.insert(0, next);
        if (c != '$') {
            Point last = snake.remove(snake.length() - 1);
            MAP(last.x, last.y) = ' ';
        }
        c = MAP(next.x, next.y);
        if (c == '*' || c == 'X') {
            gameover = true;
            goto end;
        }
        if (c == '$')
            generateFood();
        drawToMap();
        print();
        end:;
    }

    void start() {
        manualMoveTime = getTimestamp();
        while (!gameover) {
            for (;;) {
                if (getTimestamp() - manualMoveTime >= delayMillis)
                    break;
                Thread::sleep(1);
            }
            move();
            manualMoveTime = getTimestamp();
        }
    }

    void moveUp() {
        if (orientation == DOWN && snake.length() != 1)
            return;
        if (orientation == UP)
            return;
        manualMoveTime = getTimestamp();
        orientation = UP;
        move();
    }

    void moveDown() {
        if (orientation == UP && snake.length() != 1)
            return;
        if (orientation == DOWN)
            return;
        manualMoveTime = getTimestamp();
        orientation = DOWN;
        move();
    }

    void moveLeft() {
        if (orientation == RIGHT && snake.length() != 1)
            return;
        if (orientation == LEFT)
            return;
        manualMoveTime = getTimestamp();
        orientation = LEFT;
        move();
    }

    void moveRight() {
        if (orientation == LEFT && snake.length() != 1)
            return;
        if (orientation == RIGHT)
            return;
        manualMoveTime = getTimestamp();
        orientation = RIGHT;
        move();
    }
};

class R : public Runnable {
private:
    SnakeGame *game;
public:
    explicit R(SnakeGame &game) : game(&game) {}

    void run() override {
        game->start();
        delete this;
        exit(0);
    }
};

char scanKeyboard() {
    char in;
    int stdinFD = fileno(stdin);
    struct termios new_settings{};
    struct termios stored_settings{};
    tcgetattr(stdinFD, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= ~((tcflag_t) ICANON | (tcflag_t) ECHO);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(stdinFD, TCSANOW, &new_settings);
    in = (char) getchar();
    tcsetattr(stdinFD, TCSANOW, &stored_settings);
    return in;
}

inline bool cmp2(const char *str, const char *cmp, const char *orCmp) {
    return !strcmp(str, cmp) || !strcmp(str, orCmp);
}

struct Param {
    char *a, *b;
};

int main(int argc, char **argv) {
    srand((unsigned int) time(nullptr));
    int index = 0;
    char *path = argv[0];
    for (int i = 0; path[i] != '\0'; ++i) {
        if (path[i] == '/')
            index = i + 1;
    }
    char *filename = path + index;
    bczhc::LinkedList<Param> params;
    bool help = false, invalidArguments = false;
    for (int i = 1; i < argc; ++i) {
        if (cmp2(argv[i], "--help", "-h")) {
            help = true;
            break;
        }
        if (argv[i][0] == '-') {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                Param par = {argv[i], argv[i + 1]};
                params.insert(par);
            } else
                invalidArguments = true;
        }
    }
    if (help) {
        std::cout << "Usage: " << filename << " [options]" << std::endl
                  << "Options:" << std::endl
                  << "  -c,  --column <column>" << std::endl
                  << "  -r,  --row <row>" << std::endl
                  << "  -d,  --delay <delaied time (ms)>" << std::endl;
        return 0;
    }
    if (invalidArguments) {
        std::cout << "Invalid arguments." << std::endl;
        return 0;
    }
    int width = 10, height = 10, delay = 250;
    int len = params.length();
    for (int i = 0; i < len; ++i) {
        Param par = params.get(i);
        char *first = par.a, *second = par.b;
        if (cmp2(first, "-c", "--column"))
            width = Integer::parseInt(second);
        else if (cmp2(first, "-r", "--row"))
            height = Integer::parseInt(second);
        else if (cmp2(first, "-d", "--delay"))
            delay = Integer::parseInt(second);
        else {
            std::cout << "Unknown option: " << first << "." << std::endl;
            return 0;
        }
    }
    SnakeGame game(width, height, delay);
    Thread t(new R(game));
    char read = 0;
    while (!game.getGameoverStatus()) {
        read = scanKeyboard();
        switch (read) {
            case 'w':
            case 'W':
                game.moveUp();
                break;
            case 's':
            case 'S':
                game.moveDown();
                break;
            case 'a':
            case 'A':
                game.moveLeft();
                break;
            case 'd':
            case 'D':
                game.moveRight();
                break;
            case 'q':
                exit(0);
                break;
            default:
                break;
        }
    }
    t.join();
    return 0;
}

#else
int main() {
    cout << "not supported for Windows" << endl;
    return 0;
}
#endif
