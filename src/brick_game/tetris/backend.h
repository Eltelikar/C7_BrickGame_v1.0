/**
 * @file backend.h
 * @brief Файл содержит объявления структур игры, а также все функции для работы
 * backend-составляющей игры.
 */

#ifndef BACKEND
#define BACKEND

#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>

/**
 * @brief Ширина игрового поля.
 */
#define WIDTH 10

/**
 * @brief Высота игрового поля.
 */
#define HEIGH 20

/**
 * @brief Количество фигур.
 */
#define TETRAMINOS 7

/**
 * @brief Количество поворотов/положений у фигуры.
 */
#define ROTATIONS 4

/**
 * @brief Количество блоков в любой фигуре.
 */
#define BLOCKS 4

/**
 * @brief Структура, содержащая координаты, используется для блоков фигуры.
 */
typedef struct T_location {
  int row;
  int column;
} T_location;

/**
 * @brief Структура, содержащая информацию о блоках фигуры.
 * @param type Тип фигуры
 * @param rotation Поворот фигуры относительно изначального положения
 */
typedef struct T_obj {
  int type;
  int rotation;
  T_location location;
} T_obj;

/**
 * @brief Структура, содержащая возможные действия пользователя.
 */
typedef enum UserAction_t {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/**
 * @brief Структура, содержащия показатели игры.
 * @param field массив игрового поля
 * @param current текущая фигура (падающая)
 * @param next следующая фигура
 * @param score текущее количество очков
 * @param high_score наибольшее количество очков
 * @param level уровень
 * @param speed скорость игры
 * @param pause флаг паузы
 * @param started флаг старта
 * @param stoped флаг остановки
 */
typedef struct GameInfo_t {
  int **field;
  T_obj *current;
  T_obj *next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  int started;
  int stoped;
} GameInfo_t;

/**
 * @brief Функция выделения и/или получения информации о содержании GameInfo_t.
 */
GameInfo_t *getGameInfo_t();

/**
 * @brief Функция, запускающая игру после инициализации. Загружает статистику,
 * генерирует 1ю фигуру.
 */
void startGame();

/**
 * @brief Функция начала игры заново.
 */
void playAgain();

/**
 * @brief Функция очистки памяти игрового поля.
 */
void freeField();

/**
 * @brief Функция, устанавливающая флаг паузы.
 */
void pauseGame();

/**
 * @brief Функция выхода из игры.
 */
void quitGame();

/**
 * @brief Функция закрытия Ncurses.
 */
void quitNcurses();

/**
 * @brief Функция закрытия Ncurses и вывода ошибки при некорректном завершении.
 */
void errorQuit(char *msg);

/**
 * @brief Функция создает случайную фигуру и помещает ее на поле.
 */
void MakeFigures();

/**
 * @brief Функция размещяет текущую (падающую) фигуру на поле.
 */
void putFigOnField();

/**
 * @brief Удаляет текущую фигуру с поля (используется для смещения фигур).
 */
void removeTetramino();

/**
 * @brief Смещение фигуры вниз.
 */
void moveDown();

/**
 * @brief Функция определяет действие в зависимости от ввода пользователя.
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Функция генерирует текущую и следующую фигуры.
 */
void MakeFigures();

/**
 * @brief Обновляет информацию для отрисовки.
 */
GameInfo_t updateCurrentState();

/**
 * @brief Получение нужной скорости для игры.
 */
int getGameSpeed(int index);

/**
 * @brief Функция прекращения игры.
 */
void TerminateGame();

/**
 * @brief Функция определяет действие в зависимости от ввода пользователя.
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Проверки на возможность движения.
 */
int canItGoDown();

/**
 * @brief Проверки на возможность вращения.
 */
int canItRotate();

/**
 * @brief Проверки на возможность движения.
 */
int canItGoLeft();

/**
 * @brief Проверки на возможность движения.
 */
int canItGoRight();

/**
 * @brief Осуществление вращения.
 */
void rotateFigure();

/**
 * @brief Осуществление движения.
 */
void moveFigure(UserAction_t direction);

/**
 * @brief Осуществление движения.
 */
void moveLeft();

/**
 * @brief Осуществление движения.
 */
void moveRight();

/**
 * @brief Удаление заполненных линий.
 */
void deleteLine();

/**
 * @brief Проверка на заполненность линии.
 */
int isFullLine(int line);

/**
 * @brief Хранение всех положения всех фигур.
 */
T_location getTetramino(int tetramino, int rotation, int blocks);

#endif