#include "screen.h"
#include "keyboard.h"
#include "quadratic.h"

void draw_header() {
    // Draw title bar with cyan background
    set_color(COLOR_YELLOW, COLOR_CYAN);
    for (int i = 0; i < 80; i++) {
        set_cursor_position(i, 0);
        print_char(' ');
    }
    print_centered("QUADRATIC OS - Equation Solver v1.0", 0);
    
    // Reset color
    set_color(COLOR_LIGHT_GREY, COLOR_BLACK);
}

void draw_separator() {
    set_color(COLOR_CYAN, COLOR_BLACK);
    for (int i = 0; i < 78; i++) {
        print_char('=');
    }
    print_char('\n');
    set_color(COLOR_LIGHT_GREY, COLOR_BLACK);
}

void show_example() {
    set_cursor_position(0, 2);
    draw_box(2, 2, 76, 5, (COLOR_GREEN << 4) | COLOR_WHITE);
    
    set_cursor_position(4, 3);
    set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_string("Example: x^2 - 3x + 2 = 0");
    
    set_cursor_position(4, 4);
    set_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    solve_quadratic(1.0f, -3.0f, 2.0f);
}

void kernel_main() {
    clear_screen();
    draw_header();
    show_example();
    
    set_cursor_position(0, 7);
    
    // Main calculation loop
    while (1) {
        set_cursor_position(0, 8);
        draw_separator();
        
        set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
        print_string(" Enter equation coefficients (ax^2 + bx + c = 0)\n");
        set_color(COLOR_LIGHT_GREY, COLOR_BLACK);
        
        set_color(COLOR_YELLOW, COLOR_BLACK);
        float a = get_float_input(" a = ");
        float b = get_float_input(" b = ");
        float c = get_float_input(" c = ");
        set_color(COLOR_LIGHT_GREY, COLOR_BLACK);
        
        print_string("\n ");
        set_color(COLOR_LIGHT_MAGENTA, COLOR_BLACK);
        print_string("Solving: ");
        set_color(COLOR_WHITE, COLOR_BLACK);
        print_float(a);
        print_string("x^2 + ");
        print_float(b);
        print_string("x + ");
        print_float(c);
        print_string(" = 0\n");
        set_color(COLOR_LIGHT_GREY, COLOR_BLACK);
        
        print_string(" ");
        set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
        print_string("Result: ");
        set_color(COLOR_LIGHT_GREY, COLOR_BLACK);
        solve_quadratic(a, b, c);
        
        print_string("\n\n ");
        set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
        print_string("Continue? (y/n): ");
        set_color(COLOR_YELLOW, COLOR_BLACK);
        char choice = get_char_input();
        print_char(choice);
        print_char('\n');
        set_color(COLOR_LIGHT_GREY, COLOR_BLACK);
        
        if (choice != 'y' && choice != 'Y') {
            print_string("\n ");
            set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
            print_string("Thank you for using Quadratic OS!");
            set_color(COLOR_LIGHT_GREY, COLOR_BLACK);
            print_string("\n System halted.\n");
            break;
        }
        
        // Clear screen for next calculation
        clear_screen();
        draw_header();
    }
    
    // Halt system
    for(;;);
}
