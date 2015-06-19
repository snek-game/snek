#include <snek-pch.h>

#include <snek/Texture.h>

#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

namespace snek
{
    Texture::Texture()
    {
        tex = NULL;
        width = 0;
        height = 0;
    }

    Texture::~Texture()
    {
        Release();
    }

    bool Texture::LoadText(const std::string& text, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color)
    {
        /* get rid of preexisting texture */
        Release();

        /* render text surface */
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
        if (surface == NULL)
        {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        }
        else
        {
            /* create texture from surface pixels */
            tex = SDL_CreateTextureFromSurface(renderer, surface);
            if (tex == NULL)
            {
                ::printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            }
            else
            {
                /* get image dimensions */
                width = surface->w;
                height = surface->h;
            }

            /* get rid of old surface */
            SDL_FreeSurface(surface);
        }

        /* Return success */
        return IsValid();
    }

    bool Texture::LoadTextureFromPath(const std::string& path, SDL_Renderer* renderer)
    {
        /* get rid of preexisting texture */
        Release();

        /* render text surface */
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (surface == NULL)
        {
            printf("Unable to render texture surface! SDL_image Error: %s\n", IMG_GetError());
        }
        else
        {
            /* create texture from surface pixels */
            tex = SDL_CreateTextureFromSurface(renderer, surface);
            if (tex == NULL)
            {
                ::printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
            }
            else
            {
                /* get image dimensions */
                width = surface->w;
                height = surface->h;
            }

            /* get rid of old surface */
            SDL_FreeSurface(surface);
        }

        /* Return success */
        return IsValid();
    }

    bool Texture::IsValid() const
    {
        return tex != NULL;
    }

    void Texture::Release()
    {
        /* Free texture if it exists */
        if (tex != NULL)
        {
            SDL_DestroyTexture(tex);
            tex = NULL;
            width = 0;
            height = 0;
        }
    }

    void Texture::Render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) const
    {
        /* Set rendering space and render to screen */
        SDL_Rect renderQuad = { x, y, width, height };

        /* Set clip rendering dimensions */
        if (clip != NULL)
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        /* Render to screen */
        SDL_RenderCopyEx(renderer, tex, clip, &renderQuad, angle, center, flip);
    }

    int Texture::GetWidth() const
    {
        return width;
    }

    int Texture::GetHeight() const
    {
        return height;
    }
}