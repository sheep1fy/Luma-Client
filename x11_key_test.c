/*
 * x11_key_test.c
 * Standalone X11 keyboard detection test.
 * Compile: gcc x11_key_test.c -o x11_key_test -lX11
 * Usage:   ./x11_key_test
 */

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

int main(void) {
    printf("=== X11 Keyboard Detection Test ===\n");
    printf("This tests whether X11 key detection works standalone.\n");
    printf("NOTE: This will NOT detect keys held in another application\n");
    printf("      on Wayland (Wayland security model). Use SDL2 hook instead.\n\n");

    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "FAILED: Could not open X11 display.\n");
        fprintf(stderr, "Is DISPLAY set? (export DISPLAY=:1)\n");
        return 1;
    }
    printf("SUCCESS: Connected to X11 display %s\n", DisplayString(display));

    KeyCode k_keycode = XKeysymToKeycode(display, XK_k);
    KeyCode c_keycode = XKeysymToKeycode(display, XK_c);
    printf("K key keycode: %d\n", k_keycode);
    printf("C key keycode: %d\n", c_keycode);
    printf("\nPoll active - press K or C (Ctrl+C to exit)\n\n");

    bool k_was_pressed = false;

    for (int i = 0; i < 600; i++) {  /* ~60 seconds */
        char keys[32];
        XQueryKeymap(display, keys);

        bool k_now = (keys[k_keycode / 8] & (1 << (k_keycode % 8))) != 0;
        bool c_now = (keys[c_keycode / 8] & (1 << (c_keycode % 8))) != 0;

        if (k_now && !k_was_pressed) {
            printf(">>> K KEY PRESSED <<<\n");
        } else if (!k_now && k_was_pressed) {
            printf(">>> K KEY RELEASED <<<\n");
        }
        k_was_pressed = k_now;

        if (c_now) {
            printf("C key held\n");
        }

        usleep(100000); /* 100ms poll */
    }

    XCloseDisplay(display);
    printf("\nTest complete.\n");
    return 0;
}
