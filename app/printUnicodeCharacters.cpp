#include "../third_party/json-single-header/single_include/nlohmann/json.hpp"
#include <cstdio>
#include "../String.h"
#include <cstdlib>
#include <iostream>

using json = nlohmann::json;
using namespace bczhc;
using namespace string::utf8;
using namespace std;

int main(int argc, char **argv) {
    int begin = 0, end = 128;
    if (argc == 2) end = atoi(argv[1]);
    else if (argc == 3)
        begin = atoi(argv[1]), end = atoi(argv[2]);
    FILE *f = nullptr;
    if ((f = fopen("./o", "wb")) == nullptr) {
        fprintf(stderr, "Open file error.");
        return 0;
    }
    char u8Char[5], size;
    json j;
    int index = 0;
    for (int i = begin; i < end; ++i) {
        size = getUTF8Size(i);
        unicode2UTF8(u8Char, i), u8Char[size] = '\0';
        j[index] = u8Char;
        ++index;
    }
    cout << j.dump() << endl;
    fclose(f);
    return 0;
}
