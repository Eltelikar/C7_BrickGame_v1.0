#include "backend.h"

GameInfo_t *getGameInfo_t() {
  static GameInfo_t *game_state = NULL;
  if (game_state == NULL) {
    game_state = malloc(sizeof(GameInfo_t));
    if (game_state != NULL) {
      game_state->field = malloc(HEIGH * sizeof(int *));
      if (!game_state->field) errorQuit("couldn't manage to allocate memory");
      if (game_state->field != NULL) {
        int *data = calloc(HEIGH * WIDTH, sizeof(int));
        if (!data) errorQuit("couldn't manage to allocate memory");
        for (int i = 0; i < HEIGH; i++) {
          game_state->field[i] = data + i * WIDTH;
        }
      } else {
        errorQuit("couldn't manage to allocate memory");
      }

      game_state->current = malloc(BLOCKS * sizeof(T_obj));
      game_state->next = malloc(BLOCKS * sizeof(T_obj));
      if (game_state->next == NULL || game_state->current == NULL) {
        errorQuit("couldn't manage to allocate memory");
      }

      game_state->score = 0;
      game_state->high_score = 0;
      game_state->level = 1;
      game_state->speed = 1;
      game_state->pause = 0;
      game_state->started = 0;
      game_state->stoped = 0;
    } else {
      errorQuit("couldn't allocate memory");
    }
  }

  return game_state;
}

void freeField() {
  GameInfo_t *game_state = getGameInfo_t();
  free(game_state->field[0]);
  free(game_state->field);
  free(game_state->current);
}

void startGame() {
  GameInfo_t *game_state = getGameInfo_t();
  FILE *saveFile = fopen("Tetris.save", "r+");
  int high_scor = 0;

  if (saveFile != NULL) {
    fscanf(saveFile, "%d", &high_scor);
    fclose(saveFile);
  }

  game_state->high_score = high_scor;
  game_state->started = 1;

  srand((unsigned)time(NULL));
  game_state->next[0].type = rand() % TETRAMINOS;

  MakeFigures();
}

void playAgain() {
  GameInfo_t *game_state = getGameInfo_t();
  game_state->score = 0;
  game_state->level = 1;
  MakeFigures();
  game_state->stoped = 0;
}

void pauseGame() {
  GameInfo_t *stats = getGameInfo_t();
  if (stats->pause == 1) {
    stats->pause = 0;
  } else {
    stats->pause = 1;
  }
}

void quitGame() {
  GameInfo_t *game_state = getGameInfo_t();
  FILE *file = fopen("Tetris.save", "w");
  fprintf(file, "%d", game_state->high_score);
  fclose(file);
  quitNcurses();
  freeField();
  exit(EXIT_SUCCESS);
}

void quitNcurses() {
  delwin(stdscr);
  curs_set(0);
  endwin();
  refresh();
}

void errorQuit(char *msg) {
  quitNcurses();
  perror(msg);
  exit(EXIT_FAILURE);
}

void MakeFigures() {
  GameInfo_t *game_state = getGameInfo_t();
  game_state->current[0].type = game_state->next[0].type;
  game_state->next[0].type = rand() % TETRAMINOS;
  int type_current = 0;
  int type_next = 0;
  type_current = game_state->current[0].type;
  type_next = game_state->next[0].type;

  for (int i = 0; i < BLOCKS; i++) {
    game_state->current[i].rotation = 0;
    game_state->current[i].type = game_state->current[0].type;

    T_location cell_c = getTetramino(type_current, 0, i);
    game_state->current[i].location.row = cell_c.row;
    game_state->current[i].location.column = cell_c.column;

    game_state->next[i].rotation = 0;
    game_state->next[i].type = game_state->next[0].type;
    T_location cell_n = getTetramino(type_next, 0, i);
    game_state->next[i].location.row = cell_n.row;
    game_state->next[i].location.column = cell_n.column;
  }
  putFigOnField();
}

void putFigOnField() {
  GameInfo_t *game_state = getGameInfo_t();
  T_obj *temp = game_state->current;
  for (int i = 0; i < BLOCKS; i++) {
    if (game_state->started && game_state->stoped == 0) {
      if (game_state->field[temp[i].location.row][temp[i].location.column] ==
          1) {
        TerminateGame();
      } else {
        game_state->field[temp[i].location.row][temp[i].location.column] = 1;
      }
    }
  }
}

void removeTetramino() {
  GameInfo_t *game_state = getGameInfo_t();
  T_obj *curr = game_state->current;
  for (int i = 0; i < BLOCKS; i++) {
    int y = curr[i].location.column;
    int x = curr[i].location.row;
    game_state->field[x][y] = 0;
  }
}

void moveDown() {
  GameInfo_t *game_state = getGameInfo_t();
  int x, y;

  if (canItGoDown()) {
    removeTetramino();
    for (int i = 0; i < BLOCKS; i++) {
      if (game_state->current[i].location.row < HEIGH - 1) {
        x = game_state->current[i].location.column;
        y = game_state->current[i].location.row;
        game_state->current[i].location.row = y + 1;
        game_state->field[y + 1][x] = 1;
      }
    }
  } else {
    MakeFigures();
  }
}

GameInfo_t updateCurrentState() {
  GameInfo_t *game_state = getGameInfo_t();
  return *game_state;
}

int getGameSpeed(int index) {
  const int speed[11] = {0, 17, 14, 12, 10, 9, 6, 5, 4, 3, 2};

  return speed[index];
}

void TerminateGame() {
  GameInfo_t *game_state = getGameInfo_t();
  game_state->stoped = 1;

  for (int j = 0; j < BLOCKS; j++) {
    game_state->next[j].location.row = 0;
    game_state->next[j].location.column = 0;
    game_state->next[j].type = rand() % 7;

    game_state->current[j].location.row = 0;
    game_state->current[j].location.column = 0;
  }
  for (int i = 0; i < HEIGH; i++) {
    for (int j = 0; j < WIDTH; j++) {
      game_state->field[i][j] = 0;
    }
  }
}

void userInput(UserAction_t action, bool hold) {
  if (!hold && action == Start) {
    startGame();
  }
  if (hold) {
    switch (action) {
      case Start:
        break;

      case Pause:
        pauseGame();
        break;

      case Terminate:
        TerminateGame();
        break;

      case Left:
        moveFigure(action);
        break;

      case Right:
        moveFigure(action);
        break;

      case Down:
        moveFigure(action);
        break;

      case Action:
        rotateFigure();
        break;

      default:
        break;
    }
  }
}
