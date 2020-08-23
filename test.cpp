#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "./zhc.h"

using namespace std;

typedef int(*F)(int, int);
typedef int(*F2)(int, int(*)(int ,int));

int main() {
	 F2 f2 = [](int a, F f2) -> int {
		return a * f2(2, 3);
	};
	int r = f2(2, [](int a, int b) {
		return a + b;
	});
	::cout << r << ::endl;
	return 0;
}
