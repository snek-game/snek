#pragma once

#include "Point.h"

namespace snek
{
    class Snake
    {
    public:
        Point Head;
        Point Tail;

        Snake() { Reset(); }
        inline void Reset() { Head = Point(0, 0); Tail = Point(0, 0); }
    };
}