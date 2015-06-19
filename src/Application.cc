#include <snek-pch.h>

#include <snek/Application.h>
#include <snek/Grid.h>
#include <snek/Clock.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include <iostream>

#include "Application_p.h"
#include "GameLoop_p.h"

#include "Constants_p.h"

using std::atexit;

namespace snek
{
    struct Application::Impl : public Application_LocalImpl { };

    Application::Application(int argc, char** argv)
    {
        m_Impl = new Impl();
        m_Impl->argc = argc;
        m_Impl->argv = argv;
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
        if ((m_Impl->Window = SDL_CreateWindow(
            "Snek",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            (
#if SN_PLATFORM == SN_WIN
            0
#else
            SDL_WINDOW_OPENGL
#endif
            |
            SDL_WINDOW_SHOWN))
        ) == nullptr)
        {
            return 2;
        }

        /* create a renderer for the window */
        if ((m_Impl->Renderer = SDL_CreateRenderer(m_Impl->Window, -1, (SDL_RENDERER_ACCELERATED))) == nullptr)
        {
            return 3;
        }

        /* initialize SDL_image */
        int img_flags = IMG_INIT_PNG;
        if (!(IMG_Init(img_flags) & img_flags))
        {
            return 4;
        }
        /* set exit hook for de-initializing SDL_image */
        atexit(&IMG_Quit);

        /* initialize SDL_ttf */
        if (TTF_Init() == -1)
        {
            return 5;
        }
        /* set exit hook for de-initializing SDL_ttf */
        atexit(&TTF_Quit);

        /* begin event polling loop */
        m_Impl->Running = true;
        SDL_Event poll_event;

        Clock clock;
        Duration dt = clock.Reset();
        m_Impl->SetFPS(0);

        GameBegin(*m_Impl);

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
                default:
                    break;
                }
                EventLoop(*m_Impl, poll_event);
            }

            dt = clock.Elapsed();
            /* only update logic at certain intervals */
            if (dt >= m_Impl->TickRate)
            {
                GameLoop(*m_Impl, dt);
                clock.Reset();
            }

        }

        GameEnd(*m_Impl);

        /* destroy the window and renderer */
        SDL_DestroyRenderer(m_Impl->Renderer);
        SDL_DestroyWindow(m_Impl->Window);

        return 0;
    }

}
