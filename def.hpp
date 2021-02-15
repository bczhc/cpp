//
// Created by bczhc on 2/8/21.
//

#ifndef BCZHC_DEF_HPP
#define BCZHC_DEF_HPP

#include <cstdint>

#ifndef BCZHC_DEF_1
using uchar = uint8_t;
#endif

#ifndef BCZHC_DEF_2
#define RANDOM(a, b) (((double) rand() / RAND_MAX) * ((b) - (a)) + (a))
#endif

#endif //BCZHC_DEF_HPP
