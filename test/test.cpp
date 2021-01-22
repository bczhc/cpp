#include "../concurrent.h"
#include "../file.h"
#include "../io.h"
#include "../regex.h"
#include "../sqlite3.h"
#include "../string.h"
#include "../utils.hpp"
#include "../math/fourier_series.h"
#include "../third_party/practice/LinearList.hpp"
#include "thread"
#include <arpa/inet.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <ifaddrs.h>
#include <iostream>
#include <netinet/in.h>
#include <new>
#include <pcre.h>
#include <pthread.h>
#include <string>

using namespace std;
using namespace bczhc;
using namespace linearlist;
using namespace bczhc::string;
using namespace concurrent;
using namespace utils;
using namespace file;
using namespace io;
using namespace regex;

MutexLock lock;         // NOLINT(cert-err58-cpp)
CountDownLatch latch(2);// NOLINT(cert-err58-cpp)

String getIpAddress() {
    String r = nullptr;
    struct ifaddrs *ifAddrStruct = NULL;
    struct ifaddrs *ifa = NULL;
    void *tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) {// check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            if (String::equal(ifa->ifa_name, "wlan0")) r = addressBuffer;
        } else if (ifa->ifa_addr->sa_family == AF_INET6) {// check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr = &((struct sockaddr_in6 *) ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            //   printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
        }
    }
    if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
    return r;
}

int main() {
    String ipAddress = getIpAddress();
    cout << ipAddress.getCString() << endl;
    return 0;
    auto separated = ipAddress.split(".");
    int len = separated.length();
    for (int i = 0; i < len; ++i) {
        cout << separated.get(i).getCString() << endl;
    }
    return 0;
}
