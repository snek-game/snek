#include "snek-config.h"

#include <cstdlib>

#include "ConvertEnum.h"

namespace snek
{
    enum class GridState : signed char
    {
        MOVING_WEST =           signed char(-2),
        MOVING_NORTH =          signed char(-1),
        EMPTY =                 signed char(0),
        MOVING_SOUTH =          signed char(1),
        MOVING_EAST =           signed char(2),
        ITEM, // item present
        SPECIAL // special item present
    };

    template <size_t Rows, size_t Columns>
    class Grid
    {
    public:
        using State = GridState;
    private:
        State states[Rows][Columns];
    public:
        Grid()
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

        const State& operator()(size_t R, size_t C) const
        {
            SN_ASSERT(R < Rows);
            SN_ASSERT(C < Columns);
            return states[R][C];
        }

        inline bool IsEmpty(size_t R, size_t C) const
        {
            return (*this)(R, C) == State::EMPTY;
        }

        inline bool IsItem(size_t R, size_t C) const
        {
            return (*this)(R, C) == State::ITEM;
        }

        inline bool IsSpecial(size_t R, size_t C) const
        {
            return (*this)(R, C) == State::SPECIAL;
        }

        inline bool IsSnek(size_t R, size_t C) const
        {
            auto state_val = (+(*this)(R, C));
            return (state <= signed char(2));
        }
    };
}