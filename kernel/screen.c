#include "screen.h"

#define VIDEO_MEMORY 0xb8000
#define WIDTH 80
#define HEIGHT 25

static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char current_color = 0x07; // Default: light grey on black

void set_color(unsigned char foreground, unsigned char background) {
    current_color = (background << 4) | (foreground & 0x0F);
}

void clear_screen() {
    unsigned char* video_memory = (unsigned char*) VIDEO_MEMORY;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            video_memory[2 * (y * WIDTH + x)] = ' ';
            video_memory[2 * (y * WIDTH + x) + 1] = 0x07;
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    current_color = 0x07;
}

void print_char(char c) {
    unsigned char* video_memory = (unsigned char*) VIDEO_MEMORY;
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = WIDTH - 1;
        }
        // Clear the character at the new position
        video_memory[2 * (cursor_y * WIDTH + cursor_x)] = ' ';
        video_memory[2 * (cursor_y * WIDTH + cursor_x) + 1] = current_color;
    } else {
        video_memory[2 * (cursor_y * WIDTH + cursor_x)] = c;
        video_memory[2 * (cursor_y * WIDTH + cursor_x) + 1] = current_color;
        cursor_x++;
        
        if (cursor_x >= WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    
    if (cursor_y >= HEIGHT) {
        // Scroll up
        for (int y = 0; y < HEIGHT - 1; y++) {
            for (int x = 0; x < WIDTH; x++) {
                video_memory[2 * (y * WIDTH + x)] = video_memory[2 * ((y + 1) * WIDTH + x)];
                video_memory[2 * (y * WIDTH + x) + 1] = video_memory[2 * ((y + 1) * WIDTH + x) + 1];
            }
        }
        // Clear last line
        for (int x = 0; x < WIDTH; x++) {
            video_memory[2 * ((HEIGHT - 1) * WIDTH + x)] = ' ';
            video_memory[2 * ((HEIGHT - 1) * WIDTH + x) + 1] = 0x07;
        }
        cursor_y = HEIGHT - 1;
    }
}

void print_string(const char* str) {
    while (*str) {
        print_char(*str++);
    }
}

void print_int(int n) {
    if (n < 0) {
        print_char('-');
        n = -n;
    }
    
    if (n >= 10) {
        print_int(n / 10);
    }
    
    print_char((n % 10) + '0');
}

void print_float(float f) {
    // Handle special cases
    if (f != f) { // NaN check
        print_string("NaN");
        return;
    }
    
    if (f < 0) {
        print_char('-');
        f = -f;
    }
    
    int int_part = (int)f;
    float frac_part = f - int_part;
    
    print_int(int_part);
    print_char('.');
    
    // In 2 chữ số phần thập phân
    frac_part *= 100;
    int frac_int = (int)frac_part;
    if (frac_int < 10) {
        print_char('0');
    }
    print_int(frac_int);
}

void set_cursor_position(int x, int y) {
    cursor_x = x;
    cursor_y = y;
}

void print_colored(const char* str, unsigned char fg, unsigned char bg) {
    unsigned char old_color = current_color;
    set_color(fg, bg);
    print_string(str);
    current_color = old_color;
}

void draw_box(int x, int y, int width, int height, unsigned char color) {
    unsigned char* video_memory = (unsigned char*) VIDEO_MEMORY;
    
    // Draw top and bottom borders
    for (int i = 0; i < width; i++) {
        video_memory[2 * (y * WIDTH + x + i)] = (i == 0 || i == width - 1) ? '+' : '-';
        video_memory[2 * (y * WIDTH + x + i) + 1] = color;
        video_memory[2 * ((y + height - 1) * WIDTH + x + i)] = (i == 0 || i == width - 1) ? '+' : '-';
        video_memory[2 * ((y + height - 1) * WIDTH + x + i) + 1] = color;
    }
    
    // Draw left and right borders
    for (int i = 1; i < height - 1; i++) {
        video_memory[2 * ((y + i) * WIDTH + x)] = '|';
        video_memory[2 * ((y + i) * WIDTH + x) + 1] = color;
        video_memory[2 * ((y + i) * WIDTH + x + width - 1)] = '|';
        video_memory[2 * ((y + i) * WIDTH + x + width - 1) + 1] = color;
    }
}

void print_centered(const char* str, int y) {
    int len = 0;
    const char* p = str;
    while (*p++) len++;
    
    int x = (WIDTH - len) / 2;
    set_cursor_position(x, y);
    print_string(str);
}