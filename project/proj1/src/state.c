#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state() {
  // TODO: Implement this function.
  //创建蛇
  snake_t snake = {2, 2, 2, 4, true};

  //创建返回对象
  game_state_t *game = malloc(sizeof(game_state_t));
  //为蛇分配内存并赋值
  game->snakes = malloc(sizeof(snake_t));
  game->snakes[0] = snake;
  //为游戏界面赋值
  game->board = malloc(18*sizeof(char*));
  for (int i = 0; i < 18; i++) {
    game->board[i] = malloc(20*sizeof(char));
    for (int j = 0; j < 20; j++) {
      game->board[i][j] = ' ';
    }
  }
  //为边界赋值
  for (int i = 0; i < 20; i++) {
    game->board[0][i] = '#';
    game->board[17][i] = '#';
  }
  for (int i = 1; i < 17; i++) {
    game->board[i][0] = '#';
    game->board[i][19] = '#';
  }
  //加入蛇和食物
  game->board[2][9] = '*';
  game->board[2][2] = 'd';
  game->board[2][3] = '>';
  game->board[2][4] = 'D';
  //赋值其他变量
  game->num_rows = 18;
  game->num_snakes = 1;
  
  return game;
}

/* Task 2 */
void free_state(game_state_t *state) {
  // TODO: Implement this function.
  for (int i = 0; i < 18; i++)
    free(state->board[i]);
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  // TODO: Implement this function.
  for (int i = 0; i < state->num_rows; i++) {
    fprintf(fp, "%s\n", state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  if (c != 'w' && c != 'a' && c != 's' && c != 'd') {
    return false;
  }
  return true;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  if (c != 'W' && c != 'A' && c != 'S' && c != 'D' && c != 'x') {
    return false;
  }
  return true;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  if (!is_head(c) && !is_tail(c) && c != '^' && c != '<' && c != 'v' && c != '>') {
    return false;
  }
  return true;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  switch (c) {
    case '^':
      return 'w';
      break;
    case '<':
      return 'a';
      break;
    case 'v':
      return 's';
      break;
    case '>':
      return 'd';
      break;
  }
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  switch (c) {
    case 'W':
      return '^';
      break;
    case 'A':
      return '<';
      break;
    case 'S':
      return 'v';
      break;
    case 'D':
      return '>';
      break;
  }
  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  if (c == 'v' || c == 's' || c == 'S') {
    return cur_row + 1;
  } else if (c == '^' || c == 'w' || c == 'W') {
    return cur_row - 1;
  }
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
  if (c == '>' || c == 'd' || c == 'D') {
    return cur_col + 1;
  } else if (c == '<' || c == 'a' || c == 'A') {
    return cur_col - 1;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return '?';
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  return;
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  // TODO: Implement this function.
  return NULL;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
  // TODO: Implement this function.
  return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  return NULL;
}
