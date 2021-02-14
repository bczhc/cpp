//
// Created by zhc on 11/21/20.
//

#include "file.hpp"
#include "string.hpp"

File::File(const char *path) {
    this->path = path;
}

int File::remove() {
    return ::remove(this->path.getCString());
}

int File::move(const char *fileName) {
    return ::remove(fileName);
}

String File::getExtension(const String &path) {
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

String File::getFileName(const String &path) {
    ssize_t i = path.lastIndexOf(separator);
    return path.substring(i + 1, path.length());
}

int64_t File::getFileSize() {
    FILE *fp = fopen(path.getCString(), "rb");
    int64_t sz = File::getFileSize(fp);
    fclose(fp);
    return sz;
}

int64_t File::getFileSize(FILE *fp) {
    int64_t sz;
    fseek(fp, 0L, SEEK_END);
    sz = (int64_t) ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return sz;
}

