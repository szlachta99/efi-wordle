#include "colors.h"
#include "dictionary.h"
#include "efidef.h"
#include "game.h"
#include "rendering.h"
#include <efi.h>
#include <efilib.h>

void init_game(Game *game) {
  game->cursor = 0;
  game->guesses = 0;
  int random;
  EFI_TIME time;
  uefi_call_wrapper(gRT->GetTime, 2, &time, NULL);
  random = (time.Year * 12 + time.Month * 31 + time.Day) % word_count;
  for (int i = 0; i < 5; i++) {
    game->word[i] = dictionary[random][i];
  }
  for (int i = 0; i < sizeof(game->words); i++) {
    game->words[i] = 0;
  }
  game->status = Ongoing;
}

EFI_GRAPHICS_OUTPUT_BLT_PIXEL get_background_color(char c, int pos,
                                                   char word[5]) {
  for (int i = 0; i < 5; i++) {
    if (c == '\0')
      return LIGHT_GRAY;
    if (word[i] == c) {
      if (i == pos) {
        return GREEN;
      } else {
        return YELLOW;
      }
    }
  }
  return GRAY;
}

static int is_valid(char *word) {
  for (int i = 0; i < word_count; i++) {
    for (int c = 0; c < 5; c++) {
      if (word[c] != dictionary[i][c])
        goto next;
    }
    return 1;
  next:
    continue;
  }
  return 0;
}

void draw_guesses(Game *game) {
  Vec2 screen_dim = get_screen_dim();
  Rect bounds;
  bounds.w = 55 * 5 + 5; // padding 5 pixels and 5 pixel gap between tiles
  bounds.h = 55 * 6 + 5;
  bounds.x = (screen_dim.x - bounds.w) / 2;
  bounds.y = (screen_dim.y - bounds.h) / 2;
  fill_rect(bounds, WHITE);
  Vec2 pos;
  for (int i = 0; i < 30; i++) {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg;
    int row = i / 5;
    int col = i % 5;

    if (row < game->guesses)
      bg = get_background_color(game->words[i], i % 5, game->word);
    else if (row != game->guesses || col != game->cursor)
      bg = LIGHT_GRAY;
    else
      bg = DARK_GRAY;

    pos.x = bounds.x + 5 + 55 * col;
    pos.y = bounds.y + 5 + 55 * row;
    draw_letter_box(game->words[i], pos, BLACK, bg);
  }
}

void on_keypress(Game *game, char key) {
  if (key == 8) { // backspace
    if (game->cursor == 0) {
      return;
    }
    game->cursor--;
    game->words[game->cursor + 5 * game->guesses] = 0;
  } else if (key == 0) {
    game->status = Exited;
    return;
  } else if (key == 13) { // enter
    if (game->cursor != 5)
      return;
    if (!is_valid(game->words + 5 * game->guesses))
      return;
    game->status = Won;
    for (int i = 0; i < 5; i++) {
      if (game->words[i + 5 * game->guesses] != game->word[i])
        game->status = Ongoing;
    }
    game->guesses++;
    if (game->status != Won) {
      game->cursor = 0;
      if (game->guesses == 6) {
        game->status = Lost;
      }
    }
  } else if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z')) {
    if (game->cursor >= 5)
      return;
    key = key < 'a' ? key - 'A' + 'a' : key;
    game->words[game->cursor + 5 * game->guesses] = key;
    game->cursor++;
  }
  draw_guesses(game);
}
