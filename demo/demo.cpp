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

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: Command" << endl;
        return 0;
    }
    FILE *fp;
    if ((fp = fopen(argv[1], "rb")) == nullptr) {
        return 1;
    }

    int i1 = fseeko64(fp, 0L, SEEK_END);
    cout << i1 << endl;
    long i = ftell(fp);
    cout << i << endl;

    fclose(fp);
    return 0;
}