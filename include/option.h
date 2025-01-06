#ifndef OPTION_H
#define OPTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    const char *name;
    int value;
    int type;
} Option;

void handle_input(Option *option, SDL_Event *event);
void render_option(SDL_Renderer *renderer, TTF_Font *font, Option *option, int x, int y, SDL_Color ocolor);

#endif
