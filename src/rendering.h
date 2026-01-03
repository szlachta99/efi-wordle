#ifndef RENDERING_H
#define RENDERING_H

#include <efi.h>
#include <efilib.h>

extern EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

EFI_STATUS init_video(void);

void draw_letter(char c, UINTN x, UINTN y, EFI_GRAPHICS_OUTPUT_BLT_PIXEL fg,
                 EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg, UINTN scale_factor);
void clear_screen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);

void fill_rect(UINTN x, UINTN y, UINTN w, UINTN h,
               EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);
void outline_rect(UINTN x, UINTN y, UINTN w, UINTN h,
                  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color, UINTN thickness);
void draw_letter_box(char c, UINTN x, UINTN y, EFI_GRAPHICS_OUTPUT_BLT_PIXEL fg,
                     EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg);
#endif
