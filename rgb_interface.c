#include "rgb_interface.h"

unsigned int base_port = 0x4E;
unsigned int rgb_bank = 0x12;
unsigned int red = 0xf0;
unsigned int green = 0xf4;
unsigned int blue = 0xf8;

int enable_advanced_mode() {
    outb(0x87, base_port);
    outb(0x87, base_port);

    return 0;
}

int disable_advanced_mode() {
    outb(0xAA, base_port);
}

void write_byte(uchar cell, uchar byte) {
    outb(cell, base_port);
    outb(byte, base_port + 1);
}

int check_rgb() {
    outb(0x07, base_port);
    outb(rgb_bank, base_port + 1);

    outb(0xe0, base_port);
    uchar rgb_enable = inb(base_port + 1);

    if ((rgb_enable & 0xe0) != 0xe0) {
        return 1;
    } else {
        return 0;
    }
}

void set_state(int time_interval, uchar invert_clrs, uchar fade, uchar d) {
    uchar time_byte = time_interval & 255;
    uchar other_byte = ((fade & 7) << 5) | (invert_clrs << 2) | (d << 1);
    
    printf("Setting rgb state 0x%02x %02x\n", time_byte, other_byte);

    write_byte(0xfe, time_byte);
    write_byte(0xff, other_byte);
}

void set_solid_color(uchar r, uchar g, uchar b) {
    r = r | r << 4;
    r = ~r;
    g = g | g << 4;
    g = ~g;
    b = b | b << 4;
    b = ~b;
    
    write_byte(red, r);
    write_byte(red + 1, r);
    write_byte(red + 2, r);
    write_byte(red + 3, r);

    write_byte(green, g);
    write_byte(green + 1, g);
    write_byte(green + 2, g);
    write_byte(green + 3, g);

    write_byte(blue, b);
    write_byte(blue + 1, b);
    write_byte(blue + 2, b);
    write_byte(blue + 3, b);
}

void set_color_rotation(uchar* r, uchar* g, uchar* b) {
    write_byte(red, ~r[0]);
    write_byte(red + 1, ~r[1]);
    write_byte(red + 2, ~r[2]);
    write_byte(red + 3, ~r[3]);

    write_byte(green, ~g[0]);
    write_byte(green + 1, ~g[1]);
    write_byte(green + 2, ~g[2]);
    write_byte(green + 3, ~g[3]);

    write_byte(blue, ~b[0]);
    write_byte(blue + 1, ~b[1]);
    write_byte(blue + 2, ~b[2]);
    write_byte(blue + 3, ~b[3]);
}

void set_blinking(int on) {
    write_byte(0xE4, on == 1 ? 0x8 : 0x0);
}