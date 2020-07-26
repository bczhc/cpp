#include "./zhc.h"
#include <bits/types/struct_timeval.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <termio.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

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
    Point() {}
  };

private:
  int col, row;
  char *map = nullptr;
  char orientation = RIGHT;
  bool gameover = 0;
  LinkedList<Point> snake{};
  int64_t manualMoveTime = 0;
  int delayMillis = 250;

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
  SnakeGame(int col, int row) : col(col), row(row) { init(); }
  SnakeGame() {
    col = 10, row = 10;
    init();
  }
  SnakeGame(int col, int row, int delayMillis)
      : col(col), row(row), delayMillis(delayMillis) {
    init();
  };

  ~SnakeGame() { delete this->map; }

  bool getGameoverStatus() { return gameover; }

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
      gameover = 1;
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
    if (orientation == UP || orientation == DOWN)
      return;
    manualMoveTime = getTimestamp();
    orientation = UP;
    move();
  }

  void moveDown() {
    if (orientation == UP || orientation == DOWN)
      return;
    manualMoveTime = getTimestamp();
    orientation = DOWN;
    move();
  }

  void moveLeft() {
    if (orientation == LEFT || orientation == RIGHT)
      return;
    manualMoveTime = getTimestamp();
    orientation = LEFT;
    move();
  }

  void moveRight() {
    if (orientation == LEFT || orientation == RIGHT)
      return;
    manualMoveTime = getTimestamp();
    orientation = RIGHT;
    move();
  }
};

void *f(void *arg) {
  SnakeGame *game = (SnakeGame *)arg;
  game->start();
  exit(0);
  return nullptr;
}

char scanKeyboard() {
  char in;
  struct termios new_settings;
  struct termios stored_settings;
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

int main(int argc, char **argv) {
  SnakeGame *game = nullptr;
  if (argc == 3) {
    game = new SnakeGame(atoi(argv[1]), atoi(argv[2]));
  } else if (argc == 4) {
    game = new SnakeGame(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
  } else
    game = new SnakeGame(10, 10);
  pthread_t t;
  pthread_create(&t, nullptr, f, (void *)game);
  ::cout << 1 << ::endl;
  char read = 0;
  while (!game->getGameoverStatus()) {
    read = scanKeyboard();
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
    default:
      break;
    }
  }
  pthread_join(t, nullptr);
  delete game;
  return 0;
}
