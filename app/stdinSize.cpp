//
// Created by zhc on 11/21/20.
//

#include "../io.hpp"
#include <cstdint>
#include <iostream>

using namespace bczhc;

int main() {
    InputStream in(stdin);
    int64_t size = 0;
    int read;
    char buf[10240];
    while ((read = in.read(buf, 10240)) > 0) {
        size += read;
    }
    std::cout << size << ::std::endl;
    return 0;
}
