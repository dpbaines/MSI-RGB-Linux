#include "rgb_interface.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <time.h>

void get_pixel_color (Display *d, int x, int y, XColor *color) {
    XImage *image;
    image = XGetImage (d, RootWindow (d, DefaultScreen (d)), x, y, 1, 1, AllPlanes, XYPixmap);
    color->pixel = XGetPixel (image, 0, 0);
    XFree (image);
    XQueryColor (d, DefaultColormap(d, DefaultScreen (d)), color);
}

int is_night() {
    time_t seconds;
    struct tm* local;
    seconds = time(NULL);
    local = localtime(&seconds);

    if (local->tm_hour < 7)
        return 1;
    
    return 0;
}

void loop_screen_check() {
    while(1) {
        XColor c;
        Display *d = XOpenDisplay(NULL);
        get_pixel_color(d, 1920, 1080, &c);

        set_solid_color(c.red >> 12, c.green >> 12, c.blue >> 12);
        printf("Setting color to %hu %hu %hu\n", c.red, c.green, c.blue);
        sleep(1);
    }
}

colour linear_colour_scale(colour start, colour finish, float percent) {
    // Use linear interpolation because I can't find an easier algorithm

    float r_m = finish.red - start.red;
    float g_m = finish.green - start.green;
    float b_m = finish.blue - start.blue;

    colour new_col;
    new_col.red = percent * r_m + start.red;
    new_col.green = percent * g_m + start.green;
    new_col.blue = percent * b_m + start.blue;

    return new_col;
}

int get_cpu_load_percent() {
    FILE *f;
    f = fopen("/proc/loadavg", "r");

    float onemin;
    fscanf(f, "%f %*f %*f %*s %*d", &onemin);

    fclose(f);
    return onemin * 100;
}

int poll_cpu_time() {

    colour start_colour;
    colour finish_colour;

    colour start_colour_high;
    colour finish_colour_high;

    start_colour.red = 0;
    start_colour.green = 15;
    start_colour.blue = 0;

    finish_colour.red = 15;
    finish_colour.green = 0;
    finish_colour.blue = 0;

    start_colour_high.red = 15;
    start_colour_high.green = 0;
    start_colour_high.blue = 0;

    finish_colour_high.red = 0;
    finish_colour_high.green = 0;
    finish_colour_high.blue = 15;

    while (1) {
        int cpu_load_percentage = get_cpu_load_percent();
        printf("CPU Percentage = %d\n", cpu_load_percentage);
        
        colour current_load;

        if (is_night() == 1) {
            printf("Is night time\n");
            if (cpu_load_percentage > 300) {
                set_blinking(1);
            } else {
                set_blinking(0);
            }

            set_solid_color(15, 0, 0);
        } else {
            if (cpu_load_percentage < 300) {
                current_load = linear_colour_scale(start_colour, finish_colour, ((float) cpu_load_percentage / 300.0));
            } else {
                current_load = linear_colour_scale(start_colour_high, finish_colour_high, ((float) (cpu_load_percentage - 300) / 900.0));
            }

            printf("Setting colour to %d %d %d\n", current_load.red, current_load.green, current_load.blue);

            set_solid_color(current_load.red, current_load.green, current_load.blue);
        }

        sleep(5);
    }
}

int main(int argc, char** argv) {
    int perms = ioperm(0x4E, 5, 1);

    if (perms == -1) {
        printf("No permissions dafuq\n");
    }

    enable_advanced_mode();

    if (!perms) {
        if(check_rgb() == 0) {
            printf("RGB detected\n");

            set_state(0xFF, 0, 0, 1);

            // uchar reddd[4] = {0xFF, 0xFF, 0x00, 0x00};
            // uchar greennn[4] = {0x00, 0xFF, 0xFF, 0x00};
            // uchar blueeee[4] = {0x00, 0x00, 0x00, 0xFF};

            set_solid_color(0xff, 0x00, 0x00);
            poll_cpu_time();
        } else {
            printf("No rgb L\n");
        }
    }

    disable_advanced_mode();

    return 0;
}
