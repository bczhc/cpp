#include "../concurrent.hpp"
#include "../file.hpp"
#include "../exception.hpp"
#include <iostream>
#include "../utils.hpp"
#include "../io.hpp"
#include "../array.hpp"
#include "../def.hpp"
#include <cassert>
#include "../app/base128/Base128Lib.h"
#include "../sqlite3.hpp"
#include <memory>
#include "../array_list.hpp"
#include "../linked_list.hpp"
#include "../stack.hpp"
#include "../queue.hpp"
#include "../iterator.hpp"
#include "../symbol_table.hpp"
#include "../shared_pointer.hpp"
#include <unordered_map>
#include "../regex.hpp"
#include "../io.hpp"
#include <random>
#include <termios.h>
#include <cstdarg>

using namespace std;
using namespace bczhc;

char scanKeyboard() {
    char in;
    int stdinFD = fileno(stdin);
    struct termios new_settings{};
    struct termios stored_settings{};
    tcgetattr(stdinFD, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= ~(tcflag_t) ICANON;
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(stdinFD, TCSANOW, &new_settings);
    in = (char) getchar();
    tcsetattr(stdinFD, TCSANOW, &stored_settings);
    return in;
}

ArrayList<String> f() {
    ArrayList<String> a;
    a.add("asd");
    return a;
}

String f2() {
    String r = "asd";
    return r;
}

int main() {
    String lineStr;
    lineStr = String();
    lineStr.append('a');
    return 0;
}