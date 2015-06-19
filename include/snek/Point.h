#pragma once

#include <SDL/SDL.h>

namespace snek
{
    struct Point : public SDL_Point
    {
        Point() = default;
        Point(int x, int y) { this->x = x; this->y = y; }
        Point(const Point& other) { this->x = other.x; this->y = other.y; }

        inline bool operator ==(const SDL_Point& other)
        {
            return (x == other.x) && (y == other.y);
        }
    };
}