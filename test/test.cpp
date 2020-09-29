#include "../String.h"
#include "../third_party/practice/LinearList.hpp"

using namespace bczhc;
using namespace string;
using namespace std;
using namespace linearlist;

int main() {
    String s = "hello, ";
    String s2 = "world";
    s.append(s2);
    s.append(' ');
    s.append('h');
    s.append('i');
    s.append(' ');
    s.append('o');
    s.append('k');
    s.append(" there");
    String ss;
    ss = "aa";
    LinkedList<String> list;
    list.insert(s);
    list.insert(ss);
    LinkedList<String>::Iterator it = list.getIterator();
    if (it.moveToFirst()) do {
            cout << it.get().getCString() << endl;
        } while (it.next());
    String s3 = "test hhh";
    cout << s3.indexOf('s') << endl;
    cout << s3.indexOf("est ") << endl;
    cout << s3.indexOf("t") << endl;
    String s4 = "你好世界hhh";
    cout << s4.getCppString() << endl;
    cout << "size: " << s4.size() << endl
         << "length: " << s4.length() << endl;
    return 0;
}
