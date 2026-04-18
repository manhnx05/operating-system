#include "keyboard.h"
#include "screen.h"

// Keyboard Map (US Layout)
// Scancodes 0x02-0x0B are '1'-'0'
// 0x1C is Enter
// 0x0E is Backspace
// 0x34 is '.' (on keypad) or 0x33/0x34 depending on layout. Let's use standard row.
// '.' is usually 0x34 (>) or 0x33 (<) ? No, '.' is 0x34.
// '-' is 0x0C.

static char scancode_to_ascii[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // ... rest are 0 for now
};

char get_char_input() {
    unsigned char scancode = 0;
    while (1) {
        // Wait for data ready (Status Register 0x64, bit 0)
        unsigned char status;
        asm volatile("inb $0x64, %0" : "=a"(status));
        if (status & 0x01) {
            asm volatile("inb $0x60, %0" : "=a"(scancode));
            
            // Ignore break codes (key release)
            if (scancode & 0x80) {
                continue;
            }
            
            char ascii = scancode_to_ascii[scancode];
            if (ascii != 0) {
                return ascii;
            }
        }
    }
}

float get_float_input(const char* prompt) {
    print_string(prompt);
    
    char buffer[20];
    int index = 0;
    char c;
    
    while (1) {
        c = get_char_input();
        
        if (c == '\n') {
            print_char('\n');
            break;
        } else if (c == '\b') { // Backspace
            if (index > 0) {
                index--;
                // Backspace is handled in screen.c
                print_char('\b');
            }
        } else if ((c >= '0' && c <= '9') || c == '.' || c == '-') {
            if (index < 19) {
                print_char(c);
                buffer[index++] = c;
            }
        }
    }
    buffer[index] = '\0';
    
    // Chuyển chuỗi thành số float
    float result = 0.0f;
    float fraction = 0.1f;
    int is_negative = 0;
    int has_decimal = 0;
    int start_idx = 0;
    
    if (buffer[0] == '-') {
        is_negative = 1;
        start_idx = 1;
    }
    
    for (int i = start_idx; i < index; i++) {
        if (buffer[i] == '.') {
            has_decimal = 1;
        } else {
            if (has_decimal) {
                result += (buffer[i] - '0') * fraction;
                fraction *= 0.1f;
            } else {
                result = result * 10.0f + (buffer[i] - '0');
            }
        }
    }
    
    if (is_negative) {
        result = -result;
    }
    
    return result;
}