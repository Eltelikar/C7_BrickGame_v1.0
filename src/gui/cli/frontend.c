#include "frontend.h"

int InitGame() {
  GameInfo_t *game_state = getGameInfo_t();
  while (1) {
    bool hold = game_state->started;

    int key = getch();
    UserAction_t Action;
    parseKey(key, &Action);
    userInput(Action, hold);
  }

  return EXIT_SUCCESS;
}

void parseKey(int key, UserAction_t *action) {
  GameInfo_t *game_state = getGameInfo_t();
  switch (key) {
    case 's':
    case 'S':
      if (game_state->started) {
        *action = Pause;
      } else
        *action = Start;
      break;
    case 'r':
    case 'R':
      *action = Action;
      break;
    case KEY_DOWN:
      *action = Down;
      break;
    case KEY_LEFT:
      *action = Left;
      break;
    case KEY_RIGHT:
      *action = Right;
      break;
    case 'Q':
    case 'q':
      *action = Terminate;
      break;
    case 'N':
    case 'n':
      if (game_state->stoped == 1) {
        quitGame();
      }
      break;
    case 'Y':
    case 'y':
      if (game_state->stoped == 1) {
        playAgain();
      }
      break;
    default:
      *action = Up;
  }
}

void drawStartMessage() {
  MVPRINTW(HEIGH / 2, (WIDTH - 21) / 2 + 6, "Press S to start!");
}

void printRectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void drawRecs() {
  GameInfo_t game_state = updateCurrentState();
  printRectangle(0, HEIGH + 1, 0, HEIGH + 1);
  printRectangle(0, HEIGH + 1, HEIGH + 2, HEIGH + UI_WIDTH + 3);

  printRectangle(1, 8, HEIGH + 3, HEIGH + UI_WIDTH + 2);
  printRectangle(9, 11, HEIGH + 3, HEIGH + UI_WIDTH + 2);
  printRectangle(12, 15, HEIGH + 3, HEIGH + UI_WIDTH + 2);
  printRectangle(16, 19, HEIGH + 3, HEIGH + UI_WIDTH + 2);

  MVPRINTW(2, HEIGH + 5, " NEXT");
  MVPRINTW(10, HEIGH + 5, " LEVEL %d", game_state.level);
  MVPRINTW(13, HEIGH + 5, " SCORE");
  MVPRINTW(14, HEIGH + 5, " %d", game_state.score);
  MVPRINTW(17, HEIGH + 5, "HI_SCORE");
  MVPRINTW(18, HEIGH + 5, " %d", game_state.high_score);
}

void updateField() {
  GameInfo_t game_state = updateCurrentState();
  if (game_state.started && !game_state.stoped) {
    clear();
    drawRecs();
    for (int i = 0; i < HEIGH; i++) {
      for (int j = 0; j < WIDTH; j++) {
        if (game_state.field[i][j] == 1) {
          MVADDCH(i + 1, j * 2 + 1, ' ' | A_REVERSE | COLOR_PAIR(1));
          MVADDCH(i + 1, j * 2 + 2, ' ' | A_REVERSE | COLOR_PAIR(1));
        } else {
          MVADDCH(i + 1, j * 2 + 1, ' ');
          MVADDCH(i + 1, j * 2 + 2, ' ');
        }
      }
    }
    DrawNext();
    refresh();

  } else if (game_state.started == 0) {
    if (initscr() == NULL) {
      perror("error initialising ncurses");
      exit(EXIT_FAILURE);
    }
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(0);

    drawStartMessage();
    start_color();

    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
  } else {
    DrawAgain();
  }
}

void DrawNext() {
  GameInfo_t game_state = updateCurrentState();
  int start_y = 4;
  int start_x = HEIGH + 4;
  for (int i = 0; i < BLOCKS; i++) {
    for (int j = 0; j < BLOCKS + 1; j++) {
      MVADDCH(i + start_y, j * 2 + start_x, ' ');
      MVADDCH(i + start_y, j * 2 + start_x + 1, ' ');
    }
  }
  for (int i = 0; i < BLOCKS; i++) {
    MVADDCH(game_state.next[i].location.row + start_y + 1,
            game_state.next[i].location.column * 2 + 1 + start_x,
            ' ' | A_REVERSE | COLOR_PAIR(1));
    MVADDCH(game_state.next[i].location.row + start_y + 1,
            game_state.next[i].location.column * 2 + 2 + start_x,
            ' ' | A_REVERSE | COLOR_PAIR(1));
  }
}

void DrawAgain() {
  GameInfo_t game_state = updateCurrentState();
  clear();
  MVPRINTW(HEIGH / 2 - 3, (WIDTH - (int)strlen("GAME OVER")) / 2 + 5,
           "GAME OVER");
  MVPRINTW(HEIGH / 2 - 2, (WIDTH - (int)strlen("Your score is %d")) / 2 + 5,
           "Your score is %d", game_state.score);
  MVPRINTW(HEIGH / 2 - 1, (WIDTH - (int)strlen("Highscore is %d")) / 2 + 5,
           "Highscore is %d", game_state.high_score);
  MVPRINTW(HEIGH / 2, (WIDTH - (int)strlen("Play again? [y/n]")) / 2 + 5,
           "Play again? [y/n]");
}