#include "../concurrent.h"
#include "../file.h"
#include "../exception.hpp"
#include <iostream>
#include "../utils.hpp"
#include "../io.h"

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace file;
using namespace utils;
using namespace io;

int main() {
    cout << "enter..." << endl;
    InputStream is(stdin);
    LineReader reader(is);
    for (;;) {
        const String line = reader.readLine();
        if (line.isNull()) break;
        try {
            int64_t r;
            if (line.indexOf(',') != -1) {
                auto split = line.split(",");
                r = Long::parseLong(split[0], Integer::parseInt(split[1]));
            } else {
                r = Long::parseLong(line);
            }
            cout << r << endl;
        } catch (const NumberFormatException &e) {
            cout << e.what() << endl;
        }
    }
    return 1;
}