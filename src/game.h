#ifndef GAME_H
#define GAME_H

typedef struct {
  char words[6 * 5];
  int guesses;
  int cursor;
  char word[5];
  enum { Ongoing, Won, Lost, Exited } status;
} Game;

void on_keypress(Game *game, char key);
void draw_guesses(Game *game);
void init_game(Game *game);

#endif
