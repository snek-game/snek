#include <snek-pch.h>

#include <cstdlib>

#include <snek/snek-config.h>
#include <snek/snek-dbg-console.h>

#include <snek/Application.h>

#include <snek/linkSDL.h>
#include <snek/linkBoost.h>

#include <SDL/SDL_main.h>

/* Pseudo entry-point of the program */
int /*SDL_*/main(int argc, char* argv[])
{
    /* create debug console if necessary */
    SN_ALLOC_DEBUG_CONSOLE();
    snek::Application app;
    return app.Run();
}