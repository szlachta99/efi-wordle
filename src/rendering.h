#ifndef RENDERING_H
#define RENDERING_H

#include <efi.h>
#include <efilib.h>

extern EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

typedef struct {
  UINTN x;
  UINTN y;
} Vec2;

typedef struct {
  UINTN x, y, w, h;
} Rect;

EFI_STATUS init_video(void);

void clear_screen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);

void draw_letter(char c, Vec2 pos, EFI_GRAPHICS_OUTPUT_BLT_PIXEL fg,
                 EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg, UINTN scale_factor);
void draw_letter_box(char c, Vec2 pos, EFI_GRAPHICS_OUTPUT_BLT_PIXEL fg,
                     EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg);
void draw_string(char *string, Vec2 pos, EFI_GRAPHICS_OUTPUT_BLT_PIXEL fg,
                 EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg, int scale);

void fill_rect(Rect r, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);
void outline_rect(Rect r, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color, UINTN thickness);

Vec2 get_screen_dim(void);

#endif
