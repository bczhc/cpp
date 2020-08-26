#include "./zhc.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <termio.h>
#include <termios.h>
#include <unistd.h>
#include <utility>

using namespace bczhc;

#define MAP(x, y) (map[(x) + (y)*col])
#define RANDOM(a, b) (((double)rand() / RAND_MAX) * ((b) - (a)) + (a))
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

using namespace std;

int64_t getTimestamp() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000LL + tv.tv_usec / 1000LL;
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
    LinkedList<Point> snake{};
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
        snake.put(Point(1, 1));
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
                ::cout << ' ' << MAP(x, y);
            }
            ::cout << ::endl;
        }
    }

    public:
    SnakeGame(int col, int row, int delayMillis)
        : col(col + 2), row(row + 2), delayMillis(delayMillis) {
            init();
        };

    ~SnakeGame() { delete this->map; }

    bool getGameoverStatus() const { return gameover; }

    void move() {
        Point coor = snake.get(0);
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
        snake.putFirst(next);
        if (c != '$') {
            Point last = snake.removeLast();
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
                usleep(1000);
            }
            move();
            manualMoveTime = getTimestamp();
        }
    }

    void moveUp() {
        if (orientation == DOWN && snake.length() != 1) return;
        if (orientation == UP) return;
        manualMoveTime = getTimestamp();
        orientation = UP;
        move();
    }

    void moveDown() {
        if (orientation == UP && snake.length() != 1) return;
        if (orientation == DOWN) return;
        manualMoveTime = getTimestamp();
        orientation = DOWN;
        move();
    }

    void moveLeft() {
        if (orientation == RIGHT && snake.length() != 1) return;
        if (orientation == LEFT) return;
        manualMoveTime = getTimestamp();
        orientation = LEFT;
        move();
    }

    void moveRight() {
        if (orientation == LEFT && snake.length() != 1) return;
        if (orientation == RIGHT) return;
        manualMoveTime = getTimestamp();
        orientation = RIGHT;
        move();
    }
};

void *f(void *arg) {
    auto *game = (SnakeGame *) arg;
    game->start();
    delete game;
    exit(0);
    return nullptr;
}

char scanKeyboard() {
    char in;
    struct termios new_settings{};
    struct termios stored_settings{};
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0, &stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
    in = getchar();
    tcsetattr(0, TCSANOW, &stored_settings);
    return in;
}

inline bool cmp2(const char *str, const char *cmp, const char *orCmp) {
    return !strcmp(str, cmp) || !strcmp(str, orCmp);
}

int main(int argc, char **argv) {
    int index = 0;
    char *path = argv[0];
    for (int i = 0; path[i] != '\0'; ++i) {
        if (path[i] == '/') index = i + 1;
    }
    char *filename = path + index;
    typedef pair<char *, char *> param;
    LinkedList<param> params;
    bool help = false, invalidArguments = false;
    for (int i = 1; i < argc; ++i) {
        if (cmp2(argv[i], "--help", "-h")) {
            help = true;
            break;
        }
        if (argv[i][0] == '-') {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                param par(argv[i], argv[i + 1]);
                params.put(par);
            } else invalidArguments = true;
        }
    }
    if (help) {
        cout
            << "Usage: " << filename << " [options]" << endl
            << "Options:" << endl
            << "  -c,  --column <column>" << endl
            << "  -r,  --row <row>" << endl
            << "  -d,  --delay <delaied time (ms)>" << endl;
        return 0;
    }
    if (invalidArguments) {
        cout << "Invalid arguments." << endl;
        return 0;
    }
    int width = 10, height = 10, delay = 250;
    int len = params.length();
    for (int i = 0; i < len; ++i) {
        param par = params.get(i);
        char *first = par.first, *second = par.second;
        if (cmp2(first, "-c", "--column")) width = atoi(second);
        else if (cmp2(first, "-r", "--row")) height = atoi(second);
        else if (cmp2(first, "-d", "--delay")) delay = atoi(second);
        else {
            cout << "Unknown option: " << first << "." << endl;
            return 0;
        }
    }
    SnakeGame *game = new SnakeGame(width, height, delay);
    pthread_t t;
    pthread_create(&t, nullptr, f, (void *) game);
    char read = 0;
    while (!game->getGameoverStatus()) {
        read = scanKeyboard();
        cout << (int) read << endl;
        switch (read) {
            case 'w':
            case 'W':
                game->moveUp();
                break;
            case 's':
            case 'S':
                game->moveDown();
                break;
            case 'a':
            case 'A':
                game->moveLeft();
                break;
            case 'd':
            case 'D':
                game->moveRight();
                break;
            case 'q':
                delete game;
                exit(0);
                break;
            default:
                break;
        }
    }
    pthread_join(t, nullptr);
    delete game;
    return 0;
}
