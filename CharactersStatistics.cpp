#include "CountCharacters.h"
#include <cstdio>
#include <iostream>

using namespace std;
#define BUFFER_SIZE 8192
using namespace bczhc;

int main() {
    CharacterCounter counter;
    FILE *in = stdin;
    char buf[BUFFER_SIZE];
    int readLen = 0;
    while ((readLen = fread(buf, 1, BUFFER_SIZE, in)) > 0) {
        counter.countCharacters(buf, readLen);
    }
    json *j = counter.getJsonData();
    cout << j->dump() << endl;
    delete j;
    return 0;
}
