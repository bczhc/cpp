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

String file::File::getExtension(const String &path) {
    int size = path.length();
    String r;
    for (int i = size - 1; i >= 0; --i) {
        char c = path.charAt(i);
        if (c != '.')
            r.insert(0, c);
        else return r;
        if (c == '/') break;
    }
    return "";
}

String file::File::getFileName(const String &path) {
    ssize_t i = path.lastIndexOf(separator);
    return path.substring(i + 1, path.length());
}

