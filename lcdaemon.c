#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#define MAX_LIGHTS 10
#define MAX_NAME_LEN 50

typedef struct {
    char name[MAX_NAME_LEN];
    int brightness;
    int effect;
    int duration;
    uint32_t color;
    bool updated;
} LightSettings;

bool first_run = true;

volatile sig_atomic_t running = 1;

void handle_sigterm(int sig) {
    running = 0;
}

void handle_sigcont(int sig) { 
    first_run = true; 
}

int read_settings(const char *filename, LightSettings *lights, int max_lights) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open settings file");
        return 1;
    }

    char line[256];
    int current_light = -1;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '[') {
            // Section header
            char light_name[MAX_NAME_LEN];
            if (sscanf(line, "[%49[^]]]", light_name) == 1) {
                current_light++;
                if (current_light < max_lights) {
                    strncpy(lights[current_light].name, light_name, MAX_NAME_LEN - 1);
                    lights[current_light].name[MAX_NAME_LEN - 1] = '\0'; // Ensure null-termination
                    lights[current_light].updated = false; // Initialize updated flag
                } else {
                    current_light = -1; // Reset if max_lights exceeded
                }
            }
        } else if (current_light >= 0 && current_light < max_lights) {
            int temp_value;
            int temp_duration;
            uint32_t temp_color;
            if (sscanf(line, "brightness=%d", &temp_value) == 1) {
                if (lights[current_light].brightness != temp_value) {
                    lights[current_light].brightness = temp_value;
                    lights[current_light].updated = true;
                }
                continue;
            }
            if (sscanf(line, "effect=%d", &temp_value) == 1) {
                if (lights[current_light].effect != temp_value) {
                    lights[current_light].effect = temp_value;
                    lights[current_light].updated = true;
                }
                continue;
            }
            if (sscanf(line, "color=%x", &temp_color) == 1) {
                if (lights[current_light].color != temp_color) {
                    lights[current_light].color = temp_color;
                    lights[current_light].updated = true;
                }
                continue;
            }
            if (sscanf(line, "duration=%d", &temp_duration) == 1) {
                if (lights[current_light].duration != temp_duration) {
                    lights[current_light].duration = temp_duration;
                    lights[current_light].updated = true;
                }
                continue;
            }
        }
    }

    fclose(file);
    return 0;
}

void HSVtoRGB(float h, float s, float v, int *r, int *g, int *b) {
    int i = floor(h / 60);
    float f = h / 60 - i;
    float p = v * (1 - s);
    float q = v * (1 - s * f);
    float t = v * (1 - s * (1 - f));
    switch (i) {
        case 0: *r = v * 255; *g = t * 255; *b = p * 255; break;
        case 1: *r = q * 255; *g = v * 255; *b = p * 255; break;
        case 2: *r = p * 255; *g = v * 255; *b = t * 255; break;
        case 3: *r = p * 255; *g = q * 255; *b = v * 255; break;
        case 4: *r = t * 255; *g = p * 255; *b = v * 255; break;
        default: *r = v * 255; *g = p * 255; *b = q * 255; break;
    }
}

void update_light_settings(LightSettings *light, const char *dir, int *value) {
    char filepath[256];
    FILE *file;

    // Update brightness based on specific light settings
    if (strcmp(light->name, "m") == 0) {
        snprintf(filepath, sizeof(filepath), "%s/max_scale", dir);
    } else if (strcmp(light->name, "l") == 0 || strcmp(light->name, "r") == 0 || strcmp(light->name, "lr") == 0) {
        snprintf(filepath, sizeof(filepath), "%s/max_scale_lr", dir);
    } else if (strcmp(light->name, "f1") == 0 || strcmp(light->name, "f2") == 0) {
        snprintf(filepath, sizeof(filepath), "%s/max_scale_f1f2", dir);
    }
    file = fopen(filepath, "w");
    if (file != NULL) {
        fprintf(file, "%d\n", light->brightness);
        fclose(file);
    }

    // Update effect and other settings
    snprintf(filepath, sizeof(filepath), "%s/effect_rgb_hex_%s", dir, light->name);
    file = fopen(filepath, "w");
    if (file != NULL) {
        int r, g, b;
        if (light->effect == 8) {
            HSVtoRGB((float)(*value % 360), 1.0, 1.0, &r, &g, &b);
            fprintf(file, "%02X%02X%02X\n", r, g, b);
        } else {
            fprintf(file, "%06X\n", light->color);
        }
        fclose(file);
    }

    snprintf(filepath, sizeof(filepath), "%s/effect_cycles_%s", dir, light->name);
    file = fopen(filepath, "w");
    if (file != NULL) {
        fprintf(file, "%d\n", -1);
        fclose(file);
    }

    snprintf(filepath, sizeof(filepath), "%s/effect_duration_%s", dir, light->name);
    file = fopen(filepath, "w");
    if (file != NULL) {
        fprintf(file, "%d\n", light->duration);
        fclose(file);
    }

    snprintf(filepath, sizeof(filepath), "%s/effect_%s", dir, light->name);
    file = fopen(filepath, "w");
    if (file != NULL) {
        fprintf(file, "%d\n", light->effect == 8 ? 4 : light->effect);
        fclose(file);
    }
}

bool checkIfEffectChanged(LightSettings *light) {
    char filepath[256];
    FILE *file;
    snprintf(filepath, sizeof(filepath), "/sys/class/led_anim/effect_%s", light->name);
    file = fopen(filepath, "r");
    if (file != NULL) {
        char current_value[20];
        if (fgets(current_value, sizeof(current_value), file)) {
            int current_effect;
            sscanf(current_value, "%d", &current_effect);
            fclose(file);
            return (light->effect != current_effect);
        }
    }
    return false;
}

int main() {
    LightSettings lights[MAX_LIGHTS] = {0};

    int value = 0; // Initialize value for color cycling
    signal(SIGTERM, handle_sigterm);
    signal(SIGCONT, handle_sigcont);

    while (running) {
        if (read_settings("settings.txt", lights, MAX_LIGHTS) != 0) {
            return 1;
        }

        for (int i = 0; i < MAX_LIGHTS; i++) {
            // Check current effect before updating
            if (checkIfEffectChanged(&lights[i])) {
                lights[i].updated = true;
            }

            if (lights[i].updated || first_run || lights[i].effect == 8) {
                update_light_settings(&lights[i], "/sys/class/led_anim", &value);
                lights[i].updated = false;
            }
        }

        value++;
        first_run = false; // Set to false after the first iteration
        usleep(100000);
    }

    printf("Received SIGTERM, exiting color app...\n");

    return 0;
}
