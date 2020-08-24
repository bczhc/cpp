#include "zhc.h"
#include "third_party/json/single_include/nlohmann/json.hpp"

using json = nlohmann::json;

using namespace bczhc;
using namespace std;

int main() {
    json j;
    char s[] = {-62, -96, 0};
    j[0] = s;
    cout << j.dump(4) << endl;
    return 0;
}
