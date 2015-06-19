#pragma once

#include <snek/Grid.h>
#include <snek/Duration.h>

#include <SDL/SDL.h>

struct Application_LocalImpl
{
    int argc;
    char** argv;

    snek::Grid<64, 48> Grid;
    SDL_Window* Window;
    SDL_Renderer* Renderer;
    bool Running;
    snek::Duration TickRate = snek::Seconds(0);

    void SetFPS(int fps);
};