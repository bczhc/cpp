#include "CountCharacters.h"
#include "io.h"
#include "utf8.h"
#include <algorithm>
#include <cstdio>

using namespace bczhc;
using namespace io;

int main() {
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
    json *j = counter.getJsonData();
    printf("%s\n", j->dump().c_str());
    delete j;
    return 0;
}
