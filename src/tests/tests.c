#include <check.h>
#include <stdbool.h>
#include <stdio.h>

#include "../brick_game/tetris/backend.h"
#include "../gui/cli/frontend.h"

START_TEST(test_1) {
  GameInfo_t *game_state = getGameInfo_t();
  //   init_game();
  startGame();
  moveFigure(Left);
  moveFigure(Right);
  rotateFigure();
  pauseGame();
  pauseGame();
  moveFigure(Down);
  moveFigure(Down);
  moveFigure(Down);
  TerminateGame();
  ck_assert_int_eq(game_state->stoped, 1);
  quitGame();
}
END_TEST

START_TEST(test_2) {
  GameInfo_t *game_state = getGameInfo_t();
  startGame();
  moveFigure(Left);
  moveFigure(Right);
  UserAction_t action;
  int key = 's';
  parseKey(key, &action);
  key = KEY_LEFT;
  parseKey(key, &action);
  key = KEY_RIGHT;
  parseKey(key, &action);
  key = KEY_DOWN;
  parseKey(key, &action);
  key = 'r';
  parseKey(key, &action);
  userInput(Right, 1);
  userInput(Action, 1);
  userInput(Left, 1);
  userInput(Left, 1);
  userInput(Left, 1);
  userInput(Action, 1);
  userInput(Action, 1);
  userInput(Down, 1);
  userInput(Pause, 1);
  key = 's';
  parseKey(key, &action);
  key = 'n';
  parseKey(key, &action);
  key = 'y';
  parseKey(key, &action);
  userInput(Up, 1);
  userInput(Start, 1);
  rotateFigure();
  pauseGame();
  pauseGame();
  moveFigure(Down);

  parseKey(key, &action);
  userInput(Terminate, 1);
  key = 'y';
  parseKey(key, &action);
  playAgain();
  ck_assert_int_eq(game_state->started, 1);
  deleteLine();
  updateCurrentState();
  getGameSpeed(5);
  userInput(Terminate, 1);
  ck_assert_int_eq(game_state->stoped, 1);
  key = 'n';
  parseKey(key, &action);
}

Suite *back() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("back func");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_1);
  tcase_add_test(tc_core, test_2);
  suite_add_tcase(s, tc_core);

  return s;
}

int test_backend() {
  int no_failed = 0;

  Suite *s;
  SRunner *runner;

  s = back();
  runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return no_failed;
}

int main() {
  int no_failed = 0;

  no_failed |= test_backend();

  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}