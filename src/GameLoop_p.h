#pragma once

#include <SDL/SDL.h>

#include "Application_p.h"
#include <snek/Duration.h>

namespace snek
{
    void GameBegin(Application_LocalImpl& impl);
    void GameLoop(Application_LocalImpl& impl, const Duration& dt);
    void EventLoop(Application_LocalImpl& impl, SDL_Event& event);
    void GameEnd(Application_LocalImpl& impl);
}