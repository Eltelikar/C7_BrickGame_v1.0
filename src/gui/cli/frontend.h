/**
 * @file frontend.h
 * @brief Файл содержит функции для отрисовки игры и считывания действий
 * пользователя
 */

#ifndef FRONTEND
#define FRONTEND
#include "../../brick_game/tetris/backend.h"
/**
 * @brief Ширина отрисовки поля
 */
#define UI_WIDTH 12

/**
 * @brief Начало отрисовки границ
 */
#define BOARDS 2

/**
 * @brief Макрос на отрисовку заполненного блока
 */
#define MVADDCH(y, x, c) mvaddch(BOARDS + (y), BOARDS + (x), c)

/**
 * @brief Макрос на отрисовку частей поля
 */
#define MVPRINTW(y, x, ...) mvprintw(BOARDS + (y), BOARDS + (x), __VA_ARGS__)

/**
 * @brief Функция инициализации игры
 */
int InitGame();

/**
 * @brief Парсинг вввода
 */
void parseKey(int key, UserAction_t *action);

/**
 * @brief Функция отрисовки превью фигуры
 */
void updateField();

/**
 * @brief Функция отрисовки превью фигуры
 */
void DrawNext();

/**
 * @brief Функция отрисовки финальных сообщений
 */
void DrawAgain();

#endif
