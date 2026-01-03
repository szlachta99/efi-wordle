#include "font.h"
#include "rendering.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

EFI_STATUS init_video(void) {
  EFI_STATUS Status;
  UINTN HandleCount;
  EFI_HANDLE *Handles;

  Status =
      uefi_call_wrapper(gBS->LocateProtocol, 3, &gEfiGraphicsOutputProtocolGuid,
                        NULL, (void **)&gop);
  if (EFI_ERROR(Status)) {
    Print(L"Failed to locate Graphics Output Protocol: %r\n", Status);
    return Status;
  }

  return EFI_SUCCESS;
}

static void putpixel(UINTN x, UINTN y, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color) {
  uefi_call_wrapper(gop->Blt, 10, gop, &color, EfiBltVideoFill, 0, 0, x, y, 1,
                    1, 0);
}

void fill_rect(UINTN x, UINTN y, UINTN w, UINTN h,
               EFI_GRAPHICS_OUTPUT_BLT_PIXEL color) {
  uefi_call_wrapper(gop->Blt, 10, gop, &color, EfiBltVideoFill, 0, 0, x, y, w,
                    h, 0);
}

void clear_screen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL color) {
  uefi_call_wrapper(gop->Blt, 10, gop, &color, EfiBltVideoFill, 0, 0, 0, 0,
                    gop->Mode->Info->HorizontalResolution,
                    gop->Mode->Info->VerticalResolution, 0);
}

void outline_rect(UINTN x, UINTN y, UINTN w, UINTN h,
                  EFI_GRAPHICS_OUTPUT_BLT_PIXEL color, UINTN thickness) {
  fill_rect(x, y, thickness, h, color);
  fill_rect(x, y, w, thickness, color);
  fill_rect(x, y + h - thickness, w, thickness, color);
  fill_rect(x + w - thickness, y, thickness, h, color);
}

void draw_letter_box(char c, UINTN x, UINTN y, EFI_GRAPHICS_OUTPUT_BLT_PIXEL fg,
                     EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg) {
  int size = 50;
  fill_rect(x, y, size, size, bg);
  outline_rect(x, y, size, size, fg, 2);
  draw_letter(c, x + ((size - 16) / 2), y + ((size - 37) / 2) + 2, fg, bg, 1);
}

void draw_letter(char c, UINTN x, UINTN y, EFI_GRAPHICS_OUTPUT_BLT_PIXEL fg,
                 EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg, UINTN scale_factor) {
  for (int row = 0; row < 37; row++) {
    int start_x = x;
    int run = 0;
    int is_letter = get_pixel_value(c, 0, row);

    for (int column = 0; column < 16; column++) {
      int current_pixel = get_pixel_value(c, column, row);

      if (current_pixel == is_letter) {
        run++;
      } else {
        fill_rect(start_x, y + row * scale_factor, scale_factor * run,
                  scale_factor, is_letter ? fg : bg);
        start_x += scale_factor * run;
        run = 1;
        is_letter = current_pixel;
      }
    }

    if (run > 0) {
      fill_rect(start_x, y + row * scale_factor, scale_factor * run,
                scale_factor, is_letter ? fg : bg);
    }
  }
}
