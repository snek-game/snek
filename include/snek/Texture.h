#pragma once

#include <SDL/SDL.h>
#include <string>

#include <SDL/SDL_ttf.h>

namespace snek
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        /* Loads image at specified path */
        bool LoadTextureFromPath(const std::string& path, SDL_Renderer* renderer);

        /* Creates image from font string */
        bool LoadText(const std::string& text, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color);

        /* Checks if valid */
        bool IsValid() const;

        /* Deallocates texture */
        void Release();

        /* Set color modulation */
        void SetColor(Uint8 red, Uint8 green, Uint8 blue);

        /* Set blending */
        void SetBlendMode(SDL_BlendMode blending);

        /* Set alpha modulation */
        void SetAlpha(Uint8 alpha);

        /* Renders texture */
        void Render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) const;

        /* Gets image dimensions */
        int GetWidth() const;
        int GetHeight() const;

    private:
        /* The hardware texture */
        SDL_Texture* tex;

        /* Image dimensions */
        int width;
        int height;
    };
}