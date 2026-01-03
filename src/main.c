#include "colors.h"
#include "game.h"
#include "rendering.h"
#include <efi.h>
#include <efilib.h>

UINT16 wait_for_key() {
  UINTN index;

  EFI_INPUT_KEY Key;
  uefi_call_wrapper(gBS->WaitForEvent, 3, 1, &gST->ConIn->WaitForKey, &index);

  uefi_call_wrapper(gST->ConIn->ReadKeyStroke, 2, gST->ConIn, &Key);

  return Key.UnicodeChar;
}

char utf16_to_ascii(UINT16 utf16_char) {
  if (utf16_char < 128) {
    return (char)utf16_char;
  }
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
  Game game;
  init_game(&game);
  clear_screen(GRAY);

  draw_string("WORDLE", (Vec2){10, 10}, GREEN, GRAY, 2);
  draw_guesses(&game);
  while (game.status == Ongoing) {
    char c = utf16_to_ascii(wait_for_key());
    on_keypress(&game, c);
    if (game.status == Exited) {
      draw_string("Are you sure to leave? (Y/N)", (Vec2){50, 100}, RED, GRAY,
                  1);
      while (c != 'y' && c != 'n') {
        c = utf16_to_ascii(wait_for_key());
      }
      if (c == 'n') {
        game.status = Ongoing;
        clear_screen(GRAY);
        draw_string("WORDLE", (Vec2){10, 10}, GREEN, GRAY, 2);
        draw_guesses(&game);
      }
    }
  }

  if (game.status == Won) {
    draw_string("YOU WON!", (Vec2){50, 100}, GREEN, GRAY, 1);
  } else if (game.status == Lost) {
    draw_string("YOU LOST!", (Vec2){50, 100}, RED, GRAY, 1);
  }
  if (game.status != Exited)
    wait_for_key();
  clear_screen(BLACK);
  return EFI_SUCCESS;
}
