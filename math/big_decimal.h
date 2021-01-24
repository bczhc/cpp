#ifndef BCZHC_BIG_DECIMAL_H
#define BCZHC_BIG_DECIMAL_H

#include "../string.hpp"

using namespace bczhc::string;

namespace bczhc {
    namespace bigdecimal {
        class BigDecimal {
            public:
            BigDecimal(String num);

            BigDecimal add(BigDecimal a);

            BigDecimal multiply(BigDecimal a);

            BigDecimal devide(BigDecimal a);

            BigDecimal subtract(BigDecimal a);
        };
    }// namespace bigdecimal
}// namespace bczhc

#endif//BCZHC_BIG_DECIMAL_H
