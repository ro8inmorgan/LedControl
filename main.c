#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "option.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OPTIONS 4
#define MAX_NAME_LEN 50

typedef struct
{
    char name[MAX_NAME_LEN];
    int effect;
    uint32_t color;
    int duration;
} LightSettings;

LightSettings lights[NUM_OPTIONS];

int read_settings(const char *filename, LightSettings *lights, int max_lights)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Unable to open settings file");
        return 1;
    }

    char line[256];
    int current_light = -1;
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '[')
        {
            // Section header
            char light_name[MAX_NAME_LEN];
            if (sscanf(line, "[%49[^]]]", light_name) == 1)
            {
                current_light++;
                if (current_light < max_lights)
                {
                    strncpy(lights[current_light].name, light_name, MAX_NAME_LEN - 1);
                    lights[current_light].name[MAX_NAME_LEN - 1] = '\0'; // Ensure null-termination
                }
                else
                {
                    current_light = -1; // Reset if max_lights exceeded
                }
            }
        }
        else if (current_light >= 0 && current_light < max_lights)
        {
            int temp_value;
            uint32_t temp_color;

            if (sscanf(line, "effect=%d", &temp_value) == 1)
            {
                lights[current_light].effect = temp_value;
                continue;
            }
            if (sscanf(line, "color=%x", &temp_color) == 1)
            {
                lights[current_light].color = temp_color;
                continue;
            }
            if (sscanf(line, "duration=%d", &temp_value) == 1)
            {
                lights[current_light].duration = temp_value;
                continue;
            }
        }
    }

    fclose(file);
    return 0;
}

int save_settings(const char *filename, LightSettings *lights, int max_lights)
{
    SDL_Log("saving settings");
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Unable to open settings file for writing");
        return 1;
    }

    char shmfile[256];
    snprintf(shmfile, sizeof(shmfile), "/dev/shm/%s", filename);
    FILE *shm_file = fopen(shmfile, "w");
    if (shm_file == NULL)
    {
        perror("Unable to open settings file for writing");
        return 1;
    }

    for (int i = 0; i < max_lights; ++i)
    {
        fprintf(file, "[%s]\n", lights[i].name);
        fprintf(file, "effect=%d\n", lights[i].effect);
        fprintf(file, "color=0x%06X\n", lights[i].color);
        fprintf(file, "duration=%d\n\n", lights[i].duration);

        fprintf(shm_file, "[%s]\n", lights[i].name);
        fprintf(shm_file, "effect=%d\n", lights[i].effect);
        fprintf(shm_file, "color=0x%06X\n", lights[i].color);
        fprintf(shm_file, "duration=%d\n\n", lights[i].duration);
    }

    fclose(file);
    fclose(shm_file);
    SDL_Log("saved settings to disk and shm");
    return 0;
}

void handle_light_input(LightSettings *light, SDL_Event *event, int selected_setting)
{
    const uint32_t bright_colors[] = {
        0x000080, // Navy Blue
        0x008080, // Teal
        0x0080FF, // Sky Blue
        0x00BFFF, // Deep Sky Blue
        0x00FF00, // Green
        0x00FF80, // Aqua
        0x00FFFF, // Cyan
        0x32CD32, // Lime Green
        0x40E0D0, // Turquoise
        0x483D8B, // Dark Slate Blue
        0x7B68EE, // Medium Slate Blue
        0x7FFF00, // Chartreuse
        0x80FF00, // Lime
        0x80FF80, // Light Green
        0x80FFFF, // Light Cyan
        0x8080FF, // Light Blue
        0x8A2BE2, // Blue Violet
        0x9400D3, // Dark Violet
        0xADFF2F, // Green Yellow
        0xBA55D3, // Medium Orchid
        0xC0C0C0, // Silver
        0xDA70D6, // Orchid
        0xDC143C, // Crimson
        0xDDA0DD, // Plum
        0xEE82EE, // Violet
        0xFF0000, // Red
        0xFF00FF, // Magenta
        0xFF0080, // Hot Pink
        0xFF1493, // Deep Pink
        0xFF4500, // Red Orange
        0xFF6347, // Tomato
        0xFF69B4, // Hot Pink
        0xFF80C0, // Light Magenta
        0xFF80FF, // Light Pink
        0xFF8000, // Orange Red
        0xFFA500, // Orange
        0xFFB6C1, // Pink
        0xFFD700, // Gold
        0xFFFF00, // Yellow
        0xFFFF80, // Light Yellow
        0xFFFFFF  // White
    };

    const int num_bright_colors = sizeof(bright_colors) / sizeof(bright_colors[0]);

    switch (selected_setting)
    {

    case 0: // Effect
        if (event->key.keysym.sym == SDLK_RIGHT || event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
        {
            light->effect = (light->effect % 8) + 1; // Increase effect (1 to 8)
        }
        else if (event->key.keysym.sym == SDLK_LEFT || event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
        {
            light->effect = (light->effect - 2 + 8) % 8 + 1; // Decrease effect (1 to 8)
        }
        break;
    case 1: // Color
        if (event->key.keysym.sym == SDLK_RIGHT || event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
        {
            int current_index = -1;
            for (int i = 0; i < num_bright_colors; i++)
            {
                if (bright_colors[i] == light->color)
                {
                    current_index = i;
                    break;
                }
            }
            light->color = bright_colors[(current_index + 1) % num_bright_colors];
        }
        else if (event->key.keysym.sym == SDLK_LEFT || event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
        {
            int current_index = -1;
            for (int i = 0; i < num_bright_colors; i++)
            {
                if (bright_colors[i] == light->color)
                {
                    current_index = i;
                    break;
                }
            }
            light->color = bright_colors[(current_index - 1 + num_bright_colors) % num_bright_colors];
        }
        break;
    case 2: // Duration
        if (event->key.keysym.sym == SDLK_RIGHT || event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
        {
            light->duration = (light->duration + 100) % 5000; // Increase duration
        }
        else if (event->key.keysym.sym == SDLK_LEFT || event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
        {
            light->duration = (light->duration - 100 + 5000) % 5000; // Decrease duration
        }
        break;
    }

    // Save settings after each change

    save_settings("settings.txt", lights, NUM_OPTIONS);
}
char last_button_pressed[50] = "None";

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1)
    {
        SDL_Log("Unable to initialize SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Options Example",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);

    // SDL_Window *window = SDL_CreateWindow("Options Example",
    //                                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    //                                       800, 600, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (!window)
    {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("main.ttf", 36); // Specify your font path
    if (!font)
    {
        SDL_Log("Unable to open font: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Read initial settings
    if (read_settings("settings.txt", lights, NUM_OPTIONS) != 0)
    {
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        if (SDL_IsGameController(i))
        {
            controller = SDL_GameControllerOpen(i);
            if (controller)
            {
                SDL_Log("Game Controller %s connected", SDL_GameControllerName(controller));
                break;
            }
        }
    }

    if (!controller)
    {
        SDL_Log("No game controller available");
    }

    int selected_option = 0;
    int selected_setting = 0;
    bool running = true;
    SDL_Event event;

    SDL_Color hex_to_sdl_color(uint32_t hex)
    {
        SDL_Color color;
        color.r = (hex >> 16) & 0xFF;
        color.g = (hex >> 8) & 0xFF;
        color.b = hex & 0xFF;
        color.a = 255;
        return color;
    }

    const char *effect_names[] = {
        "Linear", "Breathe", "Interval Breathe", "Static",
        "Blink 1", "Blink 2", "Blink 3", "Rainbow"};

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_DOWN:
                    selected_option = (selected_option + 1) % NUM_OPTIONS;
                    break;
                case SDLK_UP:
                    selected_option = (selected_option - 1 + NUM_OPTIONS) % NUM_OPTIONS;
                    break;
                case SDLK_TAB:
                    selected_setting = (selected_setting + 1) % 4; // Switch between settings
                    break;
                case SDLK_RIGHT:
                case SDLK_LEFT:
                    handle_light_input(&lights[selected_option], &event, selected_setting);
                    break;
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    // SDL_Log("Selected: %s -> brightness: %d, effect: %d, color: 0x%06X, duration: %d",
                    //         lights[selected_option].name,
                    //         lights[selected_option].brightness,
                    //         lights[selected_option].effect,
                    //         lights[selected_option].color,
                    //         lights[selected_option].duration);
                    break;
                }
            }
            else if (event.type == SDL_CONTROLLERBUTTONDOWN)
            {
                switch (event.cbutton.button)
                {
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    selected_setting = (selected_setting + 1) % 4; // Switch between settings
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    selected_setting = (selected_setting - 1) % 4; // Switch between settings
                    break;
                case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                    selected_option = (selected_option - 1 + NUM_OPTIONS) % NUM_OPTIONS;
                    break;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                    selected_option = (selected_option + 1) % NUM_OPTIONS;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    handle_light_input(&lights[selected_option], &event, selected_setting);
                    break;
                case SDL_CONTROLLER_BUTTON_B:
                    // strcpy(last_button_pressed, "DPAD Down");
                    // SDL_Log("Selected: %s -> brightness: %d, effect: %d, color: 0x%06X, duration: %d",
                    //         lights[selected_option].name,
                    //         lights[selected_option].brightness,
                    //         lights[selected_option].effect,
                    //         lights[selected_option].color,
                    //         lights[selected_option].duration);
                    break;
                case SDL_CONTROLLER_BUTTON_A:
                    SDL_Quit();
                    break;
                    // Add more cases for other buttons as needed
                }
            }
        }

        // Get the window size
        int window_width, window_height;
        SDL_GetWindowSize(window, &window_width, &window_height);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color color = {255, 255, 255, 255};         // Default white color
        SDL_Color highlight_color = {0, 255, 255, 255}; // Cyan color for the current setting
        SDL_Color selected_color = {255, 255, 0, 255};  // Yellow color for the selected option

        // Display light name
        char light_name_text[256];
        snprintf(light_name_text, sizeof(light_name_text), "[%s]", lights[selected_option].name);
        SDL_Surface *surface = TTF_RenderText_Solid(font, light_name_text, selected_color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        int text_width = surface->w;
        int text_height = surface->h;
        SDL_FreeSurface(surface);

        // Calculate centered position
        SDL_Rect dstrect = {(window_width - text_width) / 2, 100, text_width, text_height};
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_DestroyTexture(texture);

        // Display settings
        const char *settings_labels[3] = {"Effect", "Color", "Speed"};
        int settings_values[3] = {
            lights[selected_option].effect,
            lights[selected_option].color,
            lights[selected_option].duration};

        for (int j = 0; j < 3; ++j)
        {
            char setting_text[256];
            if (j == 0)
            { // Display effect name instead of number
                snprintf(setting_text, sizeof(setting_text), "%s: %s", settings_labels[j], effect_names[settings_values[j] - 1]);

                // Render the effect name
                SDL_Color current_color = (j == selected_setting) ? highlight_color : color;

                surface = TTF_RenderText_Solid(font, setting_text, current_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);

                text_width = surface->w;
                text_height = surface->h;
                SDL_FreeSurface(surface);

                // Calculate centered position
                dstrect = (SDL_Rect){(window_width - text_width) / 2, 150 + j * 50, text_width, text_height};
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                SDL_DestroyTexture(texture);
            }
            else if (j == 1)
            { // Display color as hex code
                snprintf(setting_text, sizeof(setting_text), "COLOR: 0x%06X", settings_values[j]);

                // Draw color cube
                SDL_Color color_cube = hex_to_sdl_color(settings_values[j]);
                SDL_Rect color_rect = {(window_width - 50) / 2 + 150, 160 + j * 50, 50, 50}; // Cube size 50x50, adjust x position as needed
                SDL_SetRenderDrawColor(renderer, color_cube.r, color_cube.g, color_cube.b, color_cube.a);
                SDL_RenderFillRect(renderer, &color_rect);

                // Render the "COLOR:" text
                SDL_Color current_color = (j == selected_setting) ? highlight_color : color; // Highlight color if selected
                surface = TTF_RenderText_Solid(font, "COLOR:", current_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);

                text_width = surface->w;
                text_height = surface->h;
                SDL_FreeSurface(surface);

                // Calculate text position
                dstrect = (SDL_Rect){(window_width - text_width) / 2, 150 + j * 50, text_width, text_height};
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                SDL_DestroyTexture(texture);
            }
            else
            {
                snprintf(setting_text, sizeof(setting_text), "%s: %d", settings_labels[j], settings_values[j]);

                SDL_Color current_color = (j == selected_setting) ? highlight_color : color;

                surface = TTF_RenderText_Solid(font, setting_text, current_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);

                text_width = surface->w;
                text_height = surface->h;
                SDL_FreeSurface(surface);

                // Calculate centered position
                dstrect = (SDL_Rect){(window_width - text_width) / 2, 150 + j * 50, text_width, text_height};
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                SDL_DestroyTexture(texture);
            }
        }

        char button_text[256];
        snprintf(button_text, sizeof(button_text), "L&R LIGHT SELECT");
        surface = TTF_RenderText_Solid(font, button_text, color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        // Calculate centered position
        dstrect = (SDL_Rect){(window_width - text_width) / 2, 400, text_width, text_height};
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_DestroyTexture(texture);

        snprintf(button_text, sizeof(button_text), "B BUTTON TO QUIT");
        surface = TTF_RenderText_Solid(font, button_text, color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        // Calculate centered position
        dstrect = (SDL_Rect){(window_width - text_width) / 2, 450, text_width, text_height};
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_DestroyTexture(texture);

        snprintf(button_text, sizeof(button_text), "By Robin Morgan :D");
        surface = TTF_RenderText_Solid(font, button_text, color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        text_width = surface->w;
        text_height = surface->h;
        SDL_FreeSurface(surface);
        dstrect = (SDL_Rect){(window_width - text_width) / 2, 530, text_width, text_height};
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_DestroyTexture(texture);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
