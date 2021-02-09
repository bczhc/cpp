#include "CountCharacters.h"
#include "../../io.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

using namespace bczhc;
using namespace std;

inline bool range(int codepoint) {
    if (codepoint >= 0x4e00 && codepoint <= 0x9fff)
        return true;
    if (codepoint >= 0x3400 && codepoint <= 0x4dbf)
        return true;
    if (codepoint >= 0x20000 && codepoint <= 0x2a6df)
        return true;
    if (codepoint >= 0x2a700 && codepoint <= 0x2b73f)
        return true;
    if (codepoint >= 0x2b740 && codepoint <= 0x2b81f)
        return true;
    if (codepoint >= 0x2b820 && codepoint <= 0x2ceaf)
        return true;
    if (codepoint >= 0xf900 && codepoint <= 0xfaff)
        return true;
    if (codepoint >= 0x2f800 && codepoint <= 0x2fa1f)
        return true;
    return false;
}

int main(int argc, char **argv) {
    int han = 0;
    if (argc == 2 && !strcmp(argv[1], "--han"))
        han = 1;
    CharacterCounter counter;
    class C : public U8StringCallback {
    private:
        CharacterCounter &m_counter;

    public:
        void callback(char *s, int size) override {
            m_counter.countCharacters(s, size);
        }

        C(CharacterCounter &counter) : m_counter(counter) {}
    } c(counter);
    solveU8FromStream(stdin, c);
    if (han) {
        int cc = 0;
        using vec = vector<pair<int, int64_t>>;
        vec *v = counter.getSortVector();
        int len = v->size();
        char u8Char[5];
        for (int i = len - 1; i >= 0; --i) {
            int codepoint = (*v)[i].first;
            if (!range(codepoint))
                continue;
            unicode2UTF8(u8Char, codepoint);
            u8Char[getUTF8Size(codepoint)] = '\0';
            cout << u8Char;
            ++cc;
            if (cc % 25 == 0)
                cout << endl;
        }
        cout << endl
             << "Total: " << cc << endl;
    } else {
        json *j = counter.getJsonData();
        cout << j->dump() << endl;
        delete j;
    }
    return 0;
}
