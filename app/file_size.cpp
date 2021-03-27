//
// Created by bczhc on 3/27/21.
//

#include <cstdio>
#include <cstdint>
#include <iostream>
#include "../string.hpp"

using namespace std;
using namespace bczhc;

int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        cout << "Usage: Command <file-path> [-b | -k | -m | -g]" << endl;
        return 0;
    }

    String outputType = "-b";
    if (argc == 3) {
        outputType = argv[2];
    }

    FILE *fp;
    if ((fp = fopen(argv[1], "rb")) == nullptr) {
        cout << "Failed to open file." << endl;
        return 1;
    }

    if (fseeko64(fp, 0L, SEEK_END) != 0) {
        cout << "Failed to seek file." << endl;
        return 1;
    }
    int64_t size = ftello64(fp);

    cout.precision(10);
    cout << fixed;
    if (outputType.equals("-b")) {
        cout << size << endl;
    } else if (outputType.equals("-k")) {
        cout << ((double) size) / 1024.0F << endl;
    } else if (outputType.equals("-m")) {
        cout << ((double) size) / 1048576.0F << endl;
    } else if (outputType.equals("-g")) {
        cout << ((double) size) / ((double) (1024 * 1024 * 1024)) << endl;
    } else {
        String msg = "Unknown option: " + outputType;
        cout << msg.getCString() << endl;
    }
    fclose(fp);
}