#include "rgb_interface.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

void get_pixel_color (Display *d, int x, int y, XColor *color) {
    XImage *image;
    image = XGetImage (d, RootWindow (d, DefaultScreen (d)), x, y, 1, 1, AllPlanes, XYPixmap);
    color->pixel = XGetPixel (image, 0, 0);
    XFree (image);
    XQueryColor (d, DefaultColormap(d, DefaultScreen (d)), color);
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

int get_cpu_load_percent() {
    FILE *f;
    f = fopen("/proc/loadavg", "r");

    float onemin;
    fscanf(f, "%f %*f %*f %*s %*d", &onemin);

    fclose(f);
    return onemin * 100;
}

int poll_cpu_time() {
    while (1) {
        printf("CPU Percentage = %d\n", get_cpu_load_percent());
        sleep(1);
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

            set_state(0x0F, 0, 7, 1);

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
