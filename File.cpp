//
// Created by zhc on 11/21/20.
//

#include "File.h"

bczhc::file::File::File(const char *path) {
    this->path = path;
}

int file::File::remove() {
    return ::remove(this->path.getCString());
}
