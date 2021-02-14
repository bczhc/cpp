//
// Created by zhc on 12/26/20.
//

#include "regex.hpp"
#include "string.hpp"

ResultList bczhc::match(const char *pattern, const char *text, int textSize) {
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

    pcre_free(pPcre);
    return list;
}

bool bczhc::test(const char *pattern, const char *text, int textSize) {
    auto pPcre = getCompiledPcre(pattern);
    int r[3] = {0};
    pcre_exec(pPcre, nullptr, text, textSize, 0, 0, r, 3);
    free(pPcre);
    return r[0] != -1;
}

pcre *bczhc::getCompiledPcre(const char *pattern) {
    const char *errorMsg = nullptr;
    int errMsgOffset = 0;
    pcre *pPcre = pcre_compile(pattern, PCRE_UTF8, &errorMsg, &errMsgOffset, nullptr);
    if (pPcre == nullptr) {
        String e = "Pattern compilation failed at offset: ";
        e.append(String::toString(errMsgOffset))
                .append(". Error message: ")
                .append(errorMsg);
        throw PCREException(e);
    }
    return pPcre;
}

ResultList bczhc::match(const char *pattern, const char *text) {
    return match(pattern, text, strlen(text));
}

bool bczhc::test(const char *pattern, const char *text) {
    return test(pattern, text, strlen(text));
}
