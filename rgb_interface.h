#include <sys/io.h>
#include <stdio.h>

typedef unsigned char uchar;

int enable_advanced_mode();
int disable_advanced_mode();
void write_byte(uchar cell, uchar byte);
int check_rgb();
void set_state(int time_interval, uchar invert_clrs, uchar fade, uchar d);
void set_solid_color(uchar r, uchar g, uchar b);
void set_color_rotation(uchar* r, uchar* g, uchar* b);