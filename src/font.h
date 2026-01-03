#ifndef FONT_H
#define FONT_H

#include <efi.h>

extern unsigned char console_font_16x37[];
long long int character_offset(char c);
int get_pixel_value(char c, int x, int y);
#endif
