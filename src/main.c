#include "rendering.h"
#include <efi.h>
#include <efilib.h>

EFI_GRAPHICS_OUTPUT_BLT_PIXEL GREEN = {0, 255, 0, 0};
EFI_GRAPHICS_OUTPUT_BLT_PIXEL WHITE = {255, 255, 255, 0};
EFI_GRAPHICS_OUTPUT_BLT_PIXEL BLACK = {0, 0, 0, 0};
EFI_GRAPHICS_OUTPUT_BLT_PIXEL YELLOW = {0, 255, 255, 0};
EFI_GRAPHICS_OUTPUT_BLT_PIXEL GRAY = {128, 128, 128, 0};

UINT16 wait_for_key() {
  UINTN index;

  EFI_INPUT_KEY Key;
  uefi_call_wrapper(gBS->WaitForEvent, 3, 1, &gST->ConIn->WaitForKey, &index);

  uefi_call_wrapper(gST->ConIn->ReadKeyStroke, 2, gST->ConIn, &Key);

  return Key.UnicodeChar;
}

char utf16_to_ascii(UINT16 utf16_char) {
  // ASCII is just the first 128 values (0-127)
  if (utf16_char < 128) {
    return (char)utf16_char;
  }
  // Non-ASCII character, return placeholder
  return '?';
}

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  EFI_STATUS Status;

  InitializeLib(ImageHandle, SystemTable);

  Status = init_video();
  if (EFI_ERROR(Status)) {
    Print(L"Error occured while initializing video\n");
    return Status;
  }

  clear_screen(GRAY);

  char c = utf16_to_ascii(wait_for_key());
  int cursor = 0;
  int x_step = 55;
  int y_step = 60;
  int line_size = 10;
  UINTN x = 10, y = 10;
  while (c != 'Q') {
    draw_letter_box(c, x + (cursor % line_size) * x_step,
                    y + (cursor / line_size) * y_step, BLACK, WHITE);
    cursor++;
    c = utf16_to_ascii(wait_for_key());
  }
  return EFI_SUCCESS;
}
