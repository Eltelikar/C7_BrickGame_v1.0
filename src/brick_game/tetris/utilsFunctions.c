#include "backend.h"

int canItGoDown() {
  int res = 1;
  GameInfo_t *game_state = getGameInfo_t();
  T_obj *temp = game_state->current;
  for (int j = 0; j < BLOCKS; j++) {
    int x = temp[j].location.column;
    int y = temp[j].location.row;

    if (temp[j].location.row == HEIGH - 1) {
      res = 0;
    } else if (game_state->field[y + 1][x] == 1) {
      int temp_res = 0;
      for (int i = 0; i < BLOCKS; i++) {
        if (game_state->field[y + 1][x] == 1 && temp[i].location.row == y + 1 &&
            temp[i].location.column == x) {
          temp_res++;
        }
      }
      if (!temp_res) {
        res = 0;
      }
    }
  }
  return res;
}

int canItRotate() {
  int res = 1;
  GameInfo_t *game_state = getGameInfo_t();
  T_obj *temp = game_state->current;
  int orintation = (temp[0].rotation + 1) % 4;

  T_location cell_old = getTetramino(temp[0].type, temp[0].rotation, 0);
  int diff_y = temp[0].location.row - cell_old.row;
  int diff_x = temp[0].location.column - cell_old.column;
  for (int i = 0; i < BLOCKS; i++) {
    T_location cell_new = getTetramino(temp[i].type, orintation, i);
    if (cell_new.row + diff_y == HEIGH || cell_new.column + diff_x == WIDTH ||
        cell_new.column + diff_x < 0) {
      res = 0;
    } else if (game_state->field[cell_new.row + diff_y]
                                [cell_new.column + diff_x] == 1) {
      int temp_res = 0;
      for (int j = 0; j < BLOCKS; j++) {
        if (temp[j].location.column == cell_new.column + diff_x &&
            temp[j].location.row == cell_new.row + diff_y) {
          temp_res++;
        }
      }
      if (!temp_res) {
        res = 0;
      }
    }
  }
  return res;
}

int canItGoLeft() {
  int res = 1;
  GameInfo_t *game_state = getGameInfo_t();
  T_obj *temp = game_state->current;
  for (int j = 0; j < BLOCKS; j++) {
    int x = temp[j].location.column;
    int y = temp[j].location.row;

    if (temp[j].location.column == 0) {
      res = 0;
    } else if (game_state->field[y][x - 1] == 1) {
      int temp_res = 0;
      for (int i = 0; i < BLOCKS; i++) {
        if (game_state->field[y][x - 1] == 1 &&
            temp[i].location.column == x - 1) {
          temp_res++;
        }
      }
      if (!temp_res) {
        res = 0;
      }
    }
  }
  return res;
}

int canItGoRight() {
  int res = 1;
  GameInfo_t *game_state = getGameInfo_t();
  T_obj *temp = game_state->current;
  for (int j = 0; j < BLOCKS; j++) {
    int x = temp[j].location.column;
    int y = temp[j].location.row;

    if (temp[j].location.column == WIDTH - 1) {
      res = 0;
    } else if (game_state->field[y][x + 1] == 1) {
      int temp_res = 0;
      for (int i = 0; i < BLOCKS; i++) {
        if (game_state->field[y][x + 1] == 1 &&
            temp[i].location.column == x + 1) {
          temp_res++;
        }
      }
      if (!temp_res) {
        res = 0;
      }
    }
  }
  return res;
}

void rotateFigure() {
  GameInfo_t *game_state = getGameInfo_t();
  if (canItRotate()) {
    removeTetramino();
    T_location old_cell = getTetramino(game_state->current[0].type,
                                       game_state->current[0].rotation, 0);
    int diff_y = game_state->current[0].location.row - old_cell.row;
    int diff_x = game_state->current[0].location.column - old_cell.column;
    int rotation = (game_state->current[0].rotation + 1) % 4;
    for (int i = 0; i < BLOCKS; i++) {
      T_location new_cell =
          getTetramino(game_state->current[i].type, rotation, i);

      game_state->current[i].rotation = rotation;
      game_state->current[i].location.row = new_cell.row + diff_y;
      game_state->current[i].location.column = new_cell.column + diff_x;
      game_state->field[new_cell.row + diff_y][new_cell.column + diff_x] = 1;
    }
  }
}

void moveFigure(UserAction_t direction) {
  GameInfo_t *game_state = getGameInfo_t();
  if (game_state->pause == 0 && game_state->started) {
    if (direction == Down) {
      while (canItGoDown()) {
        moveDown();
      }
    } else if (direction == Left && canItGoLeft()) {
      moveLeft();
    } else if (direction == Right && canItGoRight()) {
      moveRight();
    }
  }
}

void moveLeft() {
  GameInfo_t *game_state = getGameInfo_t();
  int x, y;
  removeTetramino();
  for (int i = 0; i < BLOCKS; i++) {
    x = game_state->current[i].location.column;
    y = game_state->current[i].location.row;
    game_state->current[i].location.column = x - 1;
    game_state->field[y][x - 1] = 1;
  }
}

void moveRight() {
  GameInfo_t *stats = getGameInfo_t();
  int x, y;

  removeTetramino();
  for (int i = 0; i < BLOCKS; i++) {
    x = stats->current[i].location.column;
    y = stats->current[i].location.row;
    stats->current[i].location.column = x + 1;
    stats->field[y][x + 1] = 1;
  }
}

int fastPow(int degree) {
  int res = 1;
  for (int i = 0; i < degree; i++) {
    res *= 2;
  }
  return res;
}

void deleteLine() {
  GameInfo_t *gane_state = getGameInfo_t();
  int deleted_lines = 0;
  for (int i = 1; i < HEIGH; i++) {
    if (isFullLine(i)) {
      for (int j = i; j > 0; j--) {
        memset(gane_state->field[j], 0, 40);
        memcpy(gane_state->field[j], gane_state->field[j - 1], 40);
      }
      deleted_lines++;
    }
  }
  if (deleted_lines) {
    gane_state->score += 100 * fastPow(deleted_lines - 1);
    gane_state->level = gane_state->score / 600 + 1;
  }
}

int isFullLine(int line) {
  int result = 1;
  GameInfo_t *game_state = getGameInfo_t();
  for (int i = 0; i < WIDTH; i++) {
    if (game_state->field[line][i] != 1) {
      result = 0;
    }
  }
  if (result) {
  }
  return result;
}

T_location getTetramino(int tetramino, int rotation, int blocks) {
  static const T_location info[TETRAMINOS][ROTATIONS][BLOCKS] = {

      {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
       {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
       {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
       {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},

      {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
       {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
       {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
       {{0, 1}, {1, 1}, {2, 0}, {2, 1}}},

      {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
       {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
       {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
       {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},

      {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
       {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
       {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
       {{0, 1}, {0, 2}, {1, 1}, {1, 2}}},

      {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
       {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
       {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
       {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},

      {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
       {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
       {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
       {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},

      {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
       {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
       {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
       {{0, 1}, {1, 0}, {1, 1}, {2, 0}}},
  };

  return info[tetramino][rotation][blocks];
}