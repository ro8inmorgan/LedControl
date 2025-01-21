#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to print a character in the given RGB color
void print_color_char(char r, char g, char b, char c)
{
    printf("\033[38;2;%d;%d;%dm%c\033[0m", r, g, b, c);
}

int main()
{
    while (1)
    {
        FILE *file = fopen("colors.txt", "r");
        if (file == NULL)
        {
            perror("Failed to open file");
            return 1;
        }

        char hex[7]; // Buffer to store hex color (6 characters + null terminator)
        while (fscanf(file, "%6s", hex) == 1)
        {
            // Convert hex to RGB
            int r, g, b;
            sscanf(hex, "%02x%02x%02x", &r, &g, &b);

            // Print a character ('#') in the corresponding color
            print_color_char(r, g, b, '#');
        }
        printf("\n");
        fclose(file);
    }
    return 0;
}
