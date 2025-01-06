#include "option.h"
#include <SDL2/SDL_ttf.h>

void handle_input(Option *option, SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN)
    {
        if (option->type == 0)
        {
            switch (event->key.keysym.sym)
            {
            case SDLK_RIGHT:
                if (option->value < 100)
                {
                    option->value += 1;
                }
                break;
            case SDLK_LEFT:
                if (option->value > 0)
                {
                    option->value -= 1;
                }
                break;
            }
        }
    }
}

void render_option(SDL_Renderer *renderer, TTF_Font *font, Option *option, int x, int y, SDL_Color ocolor)
{
    char text[100];
    snprintf(text, sizeof(text), "%s: %d - %d", option->name, option->value, option->type);

    SDL_Color color = ocolor;
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width = surface->w;
    int text_height = surface->h;
    SDL_FreeSurface(surface);

    SDL_Rect dstrect = {x, y, text_width, text_height};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
}
