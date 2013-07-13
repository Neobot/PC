#ifndef VMATH_H
#define VMATH_H

namespace Tools
{
    #define square(x) ((x) * (x))
    #define cubic(x) ((x) * (x) * (x))

    #ifdef Q_WS_WIN
    #ifndef __MINGW32__
    #define round(x) floor(x + 0.5)
    #endif
    #endif

    const double pi = 3.141592653589793238462643383279502;
}

#endif // VMATH_H
