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
#include <sys/sysinfo.h>
#include <fcntl.h>

using namespace std;
using namespace bczhc;

int16_t a = 0x1234;

static Endianness endianness = *(int8_t *) &a == 0x12 ? Endianness::BIG : Endianness::LITTLE;
static bool b = endianness == Endianness::LITTLE;

int main(int argc, char **argv) {
    return 0;
}