#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "button.h"
#include "gpio.h"
#include "lcd.h"
#include "lcd_cgram.h"

#define VERSION_NUMBER 1.00

static int set_colour(const char *s);
static int set_cursor(const char *s);
static void print_help();

int main(int argc, char *const *argv)
{
    int r = 0;
    int s = 0;
    int opts_index = 0;

    int lcd_off = 0;
    int verbose = 0;
    char *colour_string = 0;
    char *cursor_string = 0;

    char *short_opts = "hc:u:v";
    struct option long_opts[] = {
        {"off", no_argument, &lcd_off, 1},
        {"verbose", no_argument, &verbose, 1},
        {"help", no_argument, NULL, 'h'},
        {"colour", required_argument, NULL, 'c'},
        {"cursor", required_argument, NULL, 'u'},
        {0, 0, 0, 0}
    };

    GPIO_open();
    LCD_init(0);

    while ((s =
            getopt_long(argc, argv, short_opts, long_opts,
                        &opts_index)) != -1) {
        switch (s) {
        case 'h':
            print_help();
            break;
        case 'c':
            r += set_colour(optarg);
            colour_string = optarg;
            break;
        case 'u':
            cursor_string = optarg;
            r += set_cursor(optarg);
            break;
        case 'v':
            verbose = 1;
        case '?':
            break;
        }
    };

    /* printing off the remaining arguments */
    int optind_old = optind;
    int n = 0;
    if (optind < argc) {
        while (optind < argc) {
            n += LCD_wrap_printf("%s ", argv[optind++]);
        }
        LCD_cursor_move(-1);
        optind = optind_old;
    } else if (!lcd_off) {
        int c;
        int n = 0;
        while ((c = getchar()) != EOF) {
            if (n == LCD_LENGTH) {
                LCD_putchar('\n');
                n = 0;
            }
            LCD_putchar(c);
            n++;
        }
    } else {
        LCD_off();
    }

    /* verbose flag print off more messages */
    if (verbose) {
        if (colour_string) {
            printf("Setting LCD colour to: %s\n", colour_string);
        }
        if (cursor_string) {
            printf("Setting cursor to: %s\n", cursor_string);
        }

        printf("Printed %d characters: \n", n);
        while (optind < argc) {
            printf("%s ", argv[optind++]);
        }
        printf("\n");

        if (lcd_off) {
            printf("Turning off LCD\n");
        }
    }
    return r;
}

/**
 * @brief set LCD colour from a string.
 */
static int set_colour(const char *carg)
{
    if (!strcasecmp(carg, "Black")) {
        return LCD_colour(Black);
    } else if (!strcasecmp(carg, "Red")) {
        return LCD_colour(Red);
    } else if (!strcasecmp(carg, "Yellow")) {
        return LCD_colour(Yellow);
    } else if (!strcasecmp(carg, "Green")) {
        return LCD_colour(Green);
    } else if (!strcasecmp(carg, "Cyan")) {
        return LCD_colour(Cyan);
    } else if (!strcasecmp(carg, "Blue")) {
        return LCD_colour(Blue);
    } else if (!strcasecmp(carg, "Magenta")) {
        return LCD_colour(Magenta);
    } else if (!strcasecmp(carg, "White")) {
        return LCD_colour(White);
    } else {
        fprintf(stderr, "set_colour error: Invalid colour.\n");
        LCD_colour(White);
    }
    return 2;
}

/**
 * @brief Set cursor using a string.
 */
static int set_cursor(const char *carg)
{
    if (!strcasecmp(carg, "Blink")) {
        return LCD_cmd(DISPLAY_SET | DISPLAY_ON | CURSOR_BLINK_ON);
    } else if (!strcasecmp(carg, "On")) {
        return LCD_cmd(DISPLAY_SET | DISPLAY_ON | CURSOR_ON);
    } else if (!strcasecmp(carg, "Off")) {
        return 0;
    } else {
        fprintf(stderr, "set_cursor error: Invalid option.\n");
    }
    return 3;
}

static void print_help()
{
    printf("Adafruit-RPi-LCD v%.2f, \
Adafruit Raspberry Pi LCD Plate Controller\n\
Usage: adafruit-rpi-lcd [OPTION]... [MESSAGE]...\n\
This program also accepts input from the standard input.\n\n\
\
  -c, --colour\
\t\t\tSet LCD colour, possible colours include:\n\
\t\t\t\tBlack, Red, Yellow, Green, Cyan, Blue,\n\
\t\t\t\tMagenta, White.\n\
\t\t\t\tColour names are case insensitive.\n\
  -u, --cursor\
\t\t\tSet LCD cursor, possible options include:\n\
\t\t\t\tOn, Off, Blink\n\
\t\t\t\tCursor options are case insensitive.\n\n\
  -v, --verbose\
\t\t\tTurn on the verbose mode\n\
  -h, --help\
\t\t\tPrint this help message\n\
      --off\
\t\t\tTurn off the LCD.\n\n\
Report bugs and make suggestions at:\n\
https://github.com/fangfufu/Adafruit-RPi-LCD/issues\n", VERSION_NUMBER);
    LCD_off();
    exit(1);
}
