#include <snek-pch.h>

#include "Application_p.h"

#include <boost/limits.hpp>

void Application_LocalImpl::SetFPS(int fps)
{
    if (fps <= 0) { TickRate = snek::Seconds(std::numeric_limits<float>::infinity()); }
    else { TickRate = snek::Seconds(1.f / float(fps)); }
}