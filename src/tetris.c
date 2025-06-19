#define _POSIX_C_SOURCE 200809L

#include "brick_game/tetris/backend.h"
#include "gui/cli/frontend.h"

#ifndef timerclear
#define timerclear(tvp) ((tvp)->tv_sec = (tvp)->tv_usec = 0)
#endif

void handler(int signum);

void gameLoop() {
  GameInfo_t *game_state = getGameInfo_t();
  if (game_state->pause == 0 && game_state->started == 1 &&
      game_state->stoped == 0) {
    deleteLine();
    if (game_state->speed == getGameSpeed(game_state->level)) {
      game_state->speed = 0;
      moveDown();
    }
    game_state->high_score = game_state->score > game_state->high_score
                                 ? game_state->score
                                 : game_state->high_score;
    game_state->speed++;
  }
  updateField();
}

void setTimer(void) {
  struct itimerval it;
  timerclear(&it.it_interval);
  timerclear(&it.it_value);
  it.it_interval.tv_usec = 50000;
  it.it_value.tv_usec = 50000;
  setitimer(ITIMER_REAL, &it, NULL);
}

void setSignals(void) {
  struct sigaction sa;
  sa.sa_handler = handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGALRM, &sa, NULL);
  sa.sa_handler = SIG_IGN;
  sigaction(SIGTSTP, &sa, NULL);
}

void handler(int signum) {
  switch (signum) {
    case SIGALRM:
      gameLoop();
      return;
    case SIGTERM:
    case SIGINT:
      quitNcurses();
      freeField();
      exit(EXIT_SUCCESS);
  }
}

int main() {
  getGameInfo_t();
  setTimer();
  setSignals();
  InitGame();
  return 0;
}