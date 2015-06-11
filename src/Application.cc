#include <snek-pch.h>

#include <snek/Application.h>
#include <snek/Grid.h>
#include <snek/Clock.h>

#include <SDL/SDL.h>

#include <iostream>

#define WINDOW_WIDTH    (640)
#define WINDOW_HEIGHT   (480)
#define TARGET_FPS      (60)

using std::atexit;

#if defined(SN_DEBUG)
/* console io */
#   include <iostream>
using std::clog;
using std::endl;
#endif

struct LocalImpl
{
    snek::Grid<64, 48> Grid;
    SDL_Window* Window;
    SDL_Renderer* Renderer;
    bool Running;
    float TickRate;
};

namespace snek
{
    struct Application::Impl : public LocalImpl { };

    Application::Application()
    {
        m_Impl = new Impl();
    }

    Application::~Application()
    {
        delete m_Impl;
    }

    int Application::Run()
    {
        /* initialize SDL and check if it caused an error */
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            return 1;
        }
        /* set exit hook for de-initializing SDL */
        atexit(&SDL_Quit);

        /* create SDL window and check for failure */
        if ((m_Impl->Window = SDL_CreateWindow("Snek", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, (SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN))) == nullptr)
        {
            return 2;
        }

        /* create a renderer for the window */
        if ((m_Impl->Renderer = SDL_CreateRenderer(m_Impl->Window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) == nullptr)
        {
            return 3;
        }

        /* begin event polling loop */
        m_Impl->Running = true;
        SDL_Event poll_event;

        auto* window = m_Impl->Window;
        auto* renderer = m_Impl->Renderer;

        Clock clock;
        auto dt = clock.Reset();
        const auto target = snek::Seconds(1.f / TARGET_FPS);

        while (m_Impl->Running)
        {
            /* poll all events in queue */
            while (SDL_PollEvent(&poll_event))
            {
                switch (poll_event.type)
                {
                case SDL_QUIT:
                    m_Impl->Running = false;
                    break;
                case SDL_KEYDOWN:
                    /* quit on escape */
                    if (poll_event.key.keysym.sym == SDLK_ESCAPE) { m_Impl->Running = false; }
                default:
                    break;
                }
            }

            dt = clock.Elapsed();
            /* only update logic at certain intervals */
            if (dt >= target)
            {

                // clog << clock.Elapsed().Ms() << endl;
                clock.Reset();
            }

            /* clear the screen and draw */
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);

        }

        /* destroy the window and renderer */
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        return 0;
    }

}