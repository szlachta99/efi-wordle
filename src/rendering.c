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

void fill_rect(Rect r, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color) {
  uefi_call_wrapper(gop->Blt, 10, gop, &color, EfiBltVideoFill, 0, 0, r.x, r.y,
                    r.w, r.h, 0);
}

void clear_screen(EFI_GRAPHICS_OUTPUT_BLT_PIXEL color) {
  uefi_call_wrapper(gop->Blt, 10, gop, &color, EfiBltVideoFill, 0, 0, 0, 0,
                    gop->Mode->Info->HorizontalResolution,
                    gop->Mode->Info->VerticalResolution, 0);
}

void outline_rect(Rect r, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color,
                  UINTN thickness) {
  fill_rect((Rect){r.x, r.y, thickness, r.h}, color);
  fill_rect((Rect){r.x, r.y, r.w, thickness}, color);
  fill_rect((Rect){r.x, r.y + r.h - thickness, r.w, thickness}, color);
  fill_rect((Rect){r.x + r.w - thickness, r.y, thickness, r.h}, color);
}

void draw_letter_box(char c, Vec2 pos, EFI_GRAPHICS_OUTPUT_BLT_PIXEL fg,
                     EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg) {
  int size = 50;
  Rect r = {pos.x, pos.y, size, size};
  fill_rect(r, bg);
  outline_rect(r, fg, 2);
  if (c != 0)
    draw_letter(
        c, (Vec2){pos.x + ((size - 16) / 2), pos.y + ((size - 37) / 2) + 2}, fg,
        bg, 1);
}

void draw_string(char *string, Vec2 pos, EFI_GRAPHICS_OUTPUT_BLT_PIXEL fg,
                 EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg, int scale) {
  Vec2 cursor = pos;
  while (*string != '\0') {
    if (*string != '\n') {
      draw_letter(*string, cursor, fg, bg, scale);
      cursor.x += 18 * scale;
    } else {
      cursor.x = pos.x;
      cursor.y += 41 * scale;
    }
    string++;
  }
}
void draw_letter(char c, Vec2 pos, EFI_GRAPHICS_OUTPUT_BLT_PIXEL fg,
                 EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg, UINTN scale_factor) {
  for (int row = 0; row < 37; row++) {
    int start_x = pos.x;
    int run = 0;
    int is_letter = get_pixel_value(c, 0, row);

    for (int column = 0; column < 16; column++) {
      int current_pixel = get_pixel_value(c, column, row);

      if (current_pixel == is_letter) {
        run++;
      } else {
        fill_rect((Rect){start_x, pos.y + row * scale_factor,
                         scale_factor * run, scale_factor},
                  is_letter ? fg : bg);
        start_x += scale_factor * run;
        run = 1;
        is_letter = current_pixel;
      }
    }

    if (run > 0) {
      fill_rect((Rect){start_x, pos.y + row * scale_factor, scale_factor * run,
                       scale_factor},
                is_letter ? fg : bg);
    }
  }
}
Vec2 get_screen_dim(void) {
  return (Vec2){gop->Mode->Info->HorizontalResolution,
                gop->Mode->Info->VerticalResolution};
}
