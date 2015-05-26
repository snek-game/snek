#include <SDL/SDL.h>
#include <SDL/SDL_main.h>

#include <snek/linkSDL.h>

#include <cstdlib>

using std::atexit;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    atexit(&SDL_Quit);

    return 0;
}