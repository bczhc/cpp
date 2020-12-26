//
// Created by zhc on 12/26/20.
//

#include "RegExp.h"

using namespace regex;

ResultList regex::match(const char *pattern, const char *text, int textSize) {
    LinkedList<String> list;
    auto *pPcre = getCompiledPcre(pattern);

    int r[3] = {0};
    while (r[0] != -1) {
        pcre_exec(pPcre, nullptr, text, textSize, r[1], 0, r, 3);
        String s;
        for (int i = r[0]; i < r[1]; ++i) {
            s.append(text[i]);
        }
        list.insert(s);
    }

    free(pPcre);
    return list;
}

bool regex::test(const char *pattern, const char *text, int textSize) {
    auto pPcre = getCompiledPcre(pattern);
    int r[3] = {0};
    pcre_exec(pPcre, nullptr, text, textSize, 0, 0, r, 3);
    free(pPcre);
    return r[0] != -1;
}

pcre *regex::getCompiledPcre(const char *pattern) {
    const char *errorMsg = nullptr;
    int errMsgOffset = 0;
    pcre *pPcre = pcre_compile(pattern, 0, &errorMsg, &errMsgOffset, nullptr);
    if (pPcre == nullptr) {
        String e = "Pattern compilation failed at offset: ";
        e.append(String::toString(errMsgOffset))
                .append(". Error message: ")
                .append(errorMsg);
        throw e;
    }
    return pPcre;
}

ResultList regex::match(const char *pattern, const char *text) {
    return match(pattern, text, strlen(text));
}

bool regex::test(const char *pattern, const char *text) {
    return test(pattern, text, strlen(text));
}
