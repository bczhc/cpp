#include "./zhc.h"

void pi(int64_t n) {
  long a[2] = {956, 80}, b[2] = {57121, 25}, i = 0, j, k, p, q, r, s = 2, t, u,
       v, N, M = 10000;
  N = n;
  N = N / 4 + 3;
  long *pi = new long[N], *e = new long[N];
  while (i < N)
    pi[i++] = 0;
  while (--s + 1) {
    for (*e = a[k = s], i = N; --i;)
      e[i] = 0;
    for (q = 1; j = i - 1, i < N; e[i] ? 0 : ++i, q += 2, k = !k)
      for (r = v = 0; ++j < N; pi[j] += k ? u : -u)
        u = (t = v * M + (e[j] = (p = r * M + e[j]) / b[s])) / q, r = p % b[s],
        v = t % q;
  }
  while (--i)
    (pi[i] = (t = pi[i] + s) % M) < 0 ? pi[i] += M, s = t / M - 1 : s = t / M;
  for (cout << "3."; ++i < N - 2;)
    cout << pi[i];
  delete[] pi, delete[] e;
  cout << endl;
}

int main(int argc, char **argv) {
  LinkedList<int> list;
  for (int32_t i = 0; i < 10; ++i) {
    list.put(i, i);
  }
  for (int32_t i = 0; i < 10; ++i) {
    cout << list.length() - 1 << " " << list.get(i) << endl;
  }
  return 0;
}
