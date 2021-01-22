//
// Created by zhc on 11/21/20.
//

#include "file.h"

bczhc::file::File::File(const char *path) {
    this->path = path;
}

int file::File::remove() {
    return ::remove(this->path.getCString());
}

int file::File::move(const char *fileName) {
    return ::remove(fileName);
}
