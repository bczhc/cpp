#include "./sapi.h"
#include <iostream>
#include "../third_party/practice/LinearList.hpp"
#include "../utf8.h"
#include "../Concurrent.h"

using namespace bczhc;
using namespace utf8;
using namespace linearlist;

using namespace std;

wchar_t *utf8ToWChar(const char *s) {
    SequentialList<wchar_t> list{};
    SolvedUTF8Properties solved{};
    int offset = 0;
    while (s[offset] != '\0') {
        solveUTF8Bytes(solved, s + offset);
        offset += solved.bytesLength;
        if (solved.codepoint < 0xFFFF) {
            list.insert((wchar_t) solved.codepoint);
        } else {
            int t = solved.codepoint - 0x10000;
            list.insert((wchar_t)((t >> 10) & 0b1111111111) + 0xD800);
            list.insert((wchar_t)((t & 0b1111111111) + 0xDC00));
        }
    }
    int len = list.length();
    auto *r = new wchar_t[len + 1];
    r[len] = '\0';
    for (int i = 0; i < len; ++i) {
        r[i] = list.get(i);
    }
    return r;
}


int say(const char *s) {
    wchar_t *wc = utf8ToWChar(s);
    ISpVoice *pVoice = nullptr;

    if (FAILED(::CoInitialize(nullptr)))
        return 1;

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **) &pVoice);
    if (SUCCEEDED(hr)) {
        pVoice->lpVtbl->SetPriority(pVoice, static_cast<SPVPRIORITY>(2));
        pVoice->lpVtbl->SetRate(pVoice, 100);
        pVoice->lpVtbl->Speak(pVoice, wc, SPEAKFLAGS::SPF_DEFAULT, nullptr);
        pVoice->lpVtbl->Release(pVoice);
        delete wc;
        pVoice = nullptr;
    }

    ::CoUninitialize();
    return 0;
}

using namespace concurrent;

class R : public Runnable {
private:
    const char *s;
public:
    explicit R(const char *s) : s(s) {}
    void run() override {
        say(this->s);
        delete this;
    }
};

void asyncSay(const char *s) {
    R *r = new R(s);
    Thread t2(r);
}

int main(int argc, char **argv) {
    return 0;
}