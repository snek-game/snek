#include <snek-pch.h>

#include <cstdlib>

#include <snek/snek-config.h>
#include <snek/snek-dbg-console.h>
#include <snek/linkSDL.h>
#include <snek/linkBoost.h>

#include <SDL/SDL.h>
#include <SDL/SDL_main.h>

using std::atexit;

#if defined(SN_DEBUG)
/* console io */
#   include <iostream>
using std::clog;
using std::endl;
#endif

/* Pseudo entry-point of the program */
int /*SDL_*/main(int argc, char* argv[])
{
    /* create debug console if necessary */
    SN_ALLOC_DEBUG_CONSOLE();

    /* initialize SDL and check if it caused an error */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        return 1;
    }
    /* set exit hook for de-initializing SDL */
    atexit(&SDL_Quit);

    SDL_Window* window;
    SDL_Renderer* renderer;

    /* create SDL window and check for failure */
    if ((window = SDL_CreateWindow("Snek", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, (SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN))) == nullptr)
    {
        return 2;
    }
    
    /* create a renderer for the window */
    if ((renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) == nullptr)
    {
        return 3;
    }

    /* begin event polling loop */
    auto running = true;
    SDL_Event poll_event;

    while (running)
    {
        /* poll all events in queue */
        while (SDL_PollEvent(&poll_event))
        {
            switch (poll_event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                /* quit on escape */
                if (poll_event.key.keysym.sym == SDLK_ESCAPE) { running = false; }
            default:
                break;
            }
        }

        /* clear the screen and draw */
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        /* simulate 60 FPS (use only when VSync isn't on) */
        // SDL_Delay(16);
    }

    /* destroy the window and renderer */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}