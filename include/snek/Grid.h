#pragma once

#include "snek-config.h"

#include <cstdlib>

#include "ConvertEnum.h"
#include "Point.h"

namespace snek
{
    enum class GridState : signed char
    {
        WEST  =          signed char( -2 ),
        NORTH =          signed char( -1 ),
        EMPTY =          signed char(  0 ),
        SOUTH =          signed char(  1 ),
        EAST  =          signed char(  2 ),
        FOOD
    };

    template <size_t Rows, size_t Columns>
    class Grid
    {
    public:
        typedef GridState State;
        static const size_t RowsCount;
        static const size_t ColumnsCount;
    private:
        State states[Rows][Columns];
    public:
        Grid()
        {
        }

        void Reset()
        {
            for (auto r = size_t(0); r < Rows; ++r)
            {
                for (auto c = size_t(0); c < Columns; ++c)
                {
                    states[r][c] = State::EMPTY;
                }
            }
        }

        State& operator()(size_t R, size_t C)
        {
            SN_ASSERT(R < Rows);
            SN_ASSERT(C < Columns);
            return states[R][C];
        }

        State& operator()(const Point& point)
        {
            SN_ASSERT(point.x < Rows && point.x >= 0);
            SN_ASSERT(point.y < Columns && point.y >= 0);
            return states[static_cast<size_t>(point.x)][static_cast<size_t>(point.y)];
        }

        const State& operator()(size_t R, size_t C) const
        {
            SN_ASSERT(R < Rows);
            SN_ASSERT(C < Columns);
            return states[R][C];
        }

        const State& operator()(const Point& point) const
        {
            SN_ASSERT(point.x < Rows && point.x >= 0);
            SN_ASSERT(point.y < Columns && point.y >= 0);
            return states[static_cast<size_t>(point.x)][static_cast<size_t>(point.y)];
        }

        inline bool IsEmpty(size_t R, size_t C) const
        {
            return (*this)(R, C) == State::EMPTY;
        }

        inline bool IsEmpty(const Point& point) const
        {
            return (*this)(point) == State::EMPTY;
        }

        inline bool IsFood(size_t R, size_t C) const
        {
            return (*this)(R, C) == State::FOOD;
        }

        inline bool IsFood(const Point& point) const
        {
            return (*this)(point) == State::FOOD;
        }

        inline bool IsDirection(size_t R, size_t C) const
        {
            auto state_val = (+(*this)(R, C));
            return (state_val && state_val <= signed char(2));
        }

        inline bool IsDirection(const Point& point) const
        {
            auto state_val = (+(*this)(point));
            return (state_val && state_val <= signed char(2));
        }
    };

    template <size_t Rows, size_t Columns>
    const size_t Grid<Rows, Columns>::RowsCount = Rows;

    template <size_t Rows, size_t Columns>
    const size_t Grid<Rows, Columns>::ColumnsCount = Columns;
}