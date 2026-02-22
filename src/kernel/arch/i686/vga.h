#pragma once

typedef struct
{
    int x;
    int y;
} VGAPos;

void VGA_SetCursor(int x, int y);
VGAPos VGA_GetCursorPos();
void VGA_clrscr();
void VGA_putc(char c);