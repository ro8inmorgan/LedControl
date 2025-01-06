#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define PIXEL_SIZE 10  // Size of each "pixel" in our pixel art

// Simple pixel art monkey defined as a 2D array (1 = pixel, 0 = transparent)
const int MONKEY_WIDTH = 12;
const int MONKEY_HEIGHT = 12;
const int monkey_pixels[12][12] = {
    {0,0,1,1,1,1,1,1,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,0,0},
    {1,1,0,0,1,1,1,0,0,1,0,0},
    {1,1,0,0,1,1,1,0,0,1,0,0},
    {1,1,1,1,1,1,1,1,1,1,0,0},
    {0,1,1,1,0,0,0,1,1,0,0,0},
    {0,0,1,1,1,1,1,1,0,0,0,0},
    {0,0,0,1,1,1,1,0,0,0,0,0},
    {0,0,0,1,0,0,1,0,0,0,0,0},
    {0,0,1,1,0,0,1,1,0,0,0,0},
    {0,0,1,1,0,0,1,1,0,0,0,0}
};

void render(SDL_Renderer* renderer, int offset_x, int offset_y) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);  // Sky blue background
    SDL_RenderClear(renderer);

    // Calculate boundaries to keep monkey on screen
    int startX = (WINDOW_WIDTH - (MONKEY_WIDTH * PIXEL_SIZE)) / 2 + offset_x;
    int startY = (WINDOW_HEIGHT - (MONKEY_HEIGHT * PIXEL_SIZE)) / 2 + offset_y;

    // Enforce boundaries
    startX = SDL_max(0, SDL_min(startX, WINDOW_WIDTH - (MONKEY_WIDTH * PIXEL_SIZE)));
    startY = SDL_max(0, SDL_min(startY, WINDOW_HEIGHT - (MONKEY_HEIGHT * PIXEL_SIZE)));

    // Recalculate offset based on bounded position
    offset_x = startX - (WINDOW_WIDTH - (MONKEY_WIDTH * PIXEL_SIZE)) / 2;
    offset_y = startY - (WINDOW_HEIGHT - (MONKEY_HEIGHT * PIXEL_SIZE)) / 2;

    for (int y = 0; y < MONKEY_HEIGHT; y++) {
        for (int x = 0; x < MONKEY_WIDTH; x++) {
            if (monkey_pixels[y][x]) {
                SDL_Rect pixel = {
                    startX + (x * PIXEL_SIZE),
                    startY + (y * PIXEL_SIZE),
                    PIXEL_SIZE,
                    PIXEL_SIZE
                };
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);  // Brown color
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(16);  // Cap at roughly 60 FPS
}

void handle_input(SDL_GameController *controller, bool *running, int *offset_x, int *offset_y) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = false;
        } else if (event.type == SDL_KEYDOWN) {
            // Quit on any key press
            *running = false;
        } else if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_B || event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
                // Quit on A or B button press
                *running = false;
            }
        }
    }

    // Handle continuous movement
    if (state[SDL_SCANCODE_UP] || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
        *offset_y -= PIXEL_SIZE;
    }
    if (state[SDL_SCANCODE_DOWN] || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
        *offset_y += PIXEL_SIZE;
    }
    if (state[SDL_SCANCODE_LEFT] || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        *offset_x -= PIXEL_SIZE;
    }
    if (state[SDL_SCANCODE_RIGHT] || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        *offset_x += PIXEL_SIZE;
    }
}

int main(int argc, char* argv[]) {
    int offset_x = 0;
    int offset_y = 0;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Pixel Art Monkey",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                SDL_Log("Game Controller %s connected", SDL_GameControllerName(controller));
                break;
            }
        }
    }

    if (!controller)
    {
        SDL_Log("No game controller available");
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;

    while (running) {
        handle_input(controller, &running, &offset_x, &offset_y);
        render(renderer, offset_x, offset_y);
    }

    if (controller) {
        SDL_GameControllerClose(controller);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
