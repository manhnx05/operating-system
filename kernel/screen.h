#ifndef SCREEN_H
#define SCREEN_H

// VGA Color codes
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GREY 7
#define COLOR_DARK_GREY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

void clear_screen();
void print_char(char c);
void print_string(const char* str);
void print_float(float f);
void print_int(int n);
void set_cursor_position(int x, int y);
void set_color(unsigned char foreground, unsigned char background);
void print_colored(const char* str, unsigned char fg, unsigned char bg);
void draw_box(int x, int y, int width, int height, unsigned char color);
void print_centered(const char* str, int y);

#endif