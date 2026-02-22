#include "vga.h"
#include "io.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

static const unsigned SCREEN_WIDTH = 80;
static const unsigned SCREEN_HEIGHT = 25;
static const uint8_t DEFAULT_COLOR = 0x07;

static uint8_t* screenBuffer = (uint8_t*)0xB8000;
static int screenX = 0, screenY = 0;

void putchr(int x, int y, char c)
{
    screenBuffer[2 * (y * SCREEN_WIDTH + x)] = c;
}

void putcolor(int x, int y, uint8_t color)
{
    screenBuffer[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}

char getchr(int x, int y)
{
    return screenBuffer[2 * (y * SCREEN_WIDTH + x)];
}

uint8_t getcolor(int x, int y)
{
    return screenBuffer[2 * (y * SCREEN_WIDTH + x) + 1];
}

void setcursor(int x, int y)
{
    int pos = y * SCREEN_WIDTH + x;

   i686_outb(0x3D4, 0x0F);
   i686_outb(0x3D5, (uint8_t)(pos & 0xFF));
   i686_outb(0x3D4, 0x0E);
   i686_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

VGAPos VGA_GetCursorPos()
{
    VGAPos pos = { screenX, screenY };
    return pos;
}

void VGA_clrscr()
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putchr(x, y, '\0');
            putcolor(x, y, DEFAULT_COLOR);
        }

    screenX = 0;
    screenY = 0;
    setcursor(screenX, screenY);
}

void scrollback(int lines)
{
    for (int y = lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putchr(x, y - lines, getchr(x, y));
            putcolor(x, y - lines, getcolor(x, y));
        }

    for (int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putchr(x, y, '\0');
            putcolor(x, y, DEFAULT_COLOR);
        }

    screenY -= lines;
}

void VGA_putc(char c)
{
    switch (c)
    {
    case '\n':
        screenX = 0;
        screenY++;
        break;
    case '\t':
        for (int i = 0; i < 4; i++)
            VGA_putc(' ');
        break;
    case '\r':
        screenX = 0;
        break;
    default:
        putchr(screenX, screenY, c);
        putcolor(screenX, screenY, DEFAULT_COLOR);
        screenX++;
        break;
    }

    if (screenX >= SCREEN_WIDTH)
    {
        screenY++;
        screenX = 0;
    }
    if (screenY >= SCREEN_HEIGHT)
        scrollback(1);

    setcursor(screenX, screenY);
}