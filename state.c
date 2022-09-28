#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_x(unsigned int cur_x, char c);
static unsigned int get_next_y(unsigned int cur_y, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);


/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
  game_state_t *state = (game_state_t *) malloc(sizeof(game_state_t));
    state->num_rows = 18;
    state->num_snakes = 1;
    state->board = (char **) malloc(18 * sizeof(char *));
    for (int i = 0; i < 18; i++) {
        state->board[i] = (char *) malloc(21 * sizeof(char));
    }
    char *normal = "#                  #";
    for (int i = 0; i < 18; i++) {
        if (i != 0 && i != 17 && i != 2) {
            strcpy(state->board[i], normal);
        }
    }
    strcpy(state->board[0], "####################");
    strcpy(state->board[2], "# d>D    *         #");
    strcpy(state->board[17], "####################");
    state->snakes = (snake_t *) malloc(sizeof(snake_t));
    state->snakes->head_x = 4;
    state->snakes->head_y = 2;
    state->snakes->tail_x = 2;
    state->snakes->tail_y = 2;
    state->snakes->live = true;
    return state;
}


/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  for (int i = 0; i < state->num_snakes; i++) {
        free(state->snakes + i);
    }
    for (int i = 0; i < state->num_rows; i++) {
        free(state->board[i]);
    }
    free(state->board);
    free(state);
    return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
    for (int i = 0; i < state->num_rows; i++) {
        fprintf(fp, "%s%c", state->board[i], '\n');
    }
    return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}


/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int x, unsigned int y) {
  return state->board[y][x];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch) {
  state->board[y][x] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  switch (c) {
        case 'w':
            return true;
        case 'a':
            return true;
        case 's':
            return true;
        case 'd':
            return true;
        default:
            return false;
  }  
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  switch (c) {
        case 'W':
            return true;
        case 'A':
            return true;
        case 'S':
            return true;
        case 'D':
            return true;
	case 'x':
            return true;
        default:
            return false;
    }
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<>vWASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  if (is_tail(c) || is_head(c)) {
        return true;
    } else {
        switch (c) {
            case '^':
                return true;
            case '<':
                return true;
            case '>':
                return true;
            case 'v':
                return true;
            default:
                return false;
        }
    }
}

/*
  Converts a character in the snake's body ("^<>v")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  switch (c) {
        case '^':
            return 'w';
        case '<':
            return 'a';
        case 'v':
            return 's';
        case '>':
            return 'd';
        default:
            exit(1);
    }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<>v").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  switch (c) {
        case 'W':
            return '^';
        case 'A':
            return '<';
        case 'S':
            return 'V';
        case 'D':
            return '>';
        default:
            exit(1);
    }
}

/*
  Returns cur_x + 1 if c is '>' or 'd' or 'D'.
  Returns cur_x - 1 if c is '<' or 'a' or 'A'.
  Returns cur_x otherwise.
*/
static unsigned int get_next_x(unsigned int cur_x, char c) {
  // TODO: Implement this function.
  switch (c) {
        case '>':
            return cur_x + 1;
        case 'd':
            return cur_x + 1;
        case 'D':
            return cur_x + 1;
        case '<':
            return cur_x - 1;
        case 'a':
            return cur_x - 1;
        case 'A':
            return cur_x - 1;;
        default:
            return cur_x;
    }
}

/*
  Returns cur_y + 1 if c is '^' or 'w' or 'W'.
  Returns cur_y - 1 if c is 'v' or 's' or 'S'.
  Returns cur_y otherwise.
*/
static unsigned int get_next_y(unsigned int cur_y, char c) {
  // TODO: Implement this function.
   switch (c) {
        case 'v':
            return cur_y + 1;
        case 's':
            return cur_y + 1;
        case 'S':
            return cur_y + 1;
        case '^':
            return cur_y - 1;
        case 'w':
            return cur_y - 1;
        case 'W':
            return cur_y - 1;;
        default:
            return cur_y;
    }
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t *snake = (state->snakes + snum);
    char c = get_board_at(state, snake->head_x, snake->head_y);
    switch (c) {
        case 'A':
            return get_board_at(state, snake->head_x - 1, snake->head_y);
        case 'D':
            return get_board_at(state, snake->head_x + 1, snake->head_y);
        case 'W':
            return get_board_at(state, snake->head_x, snake->head_y - 1);
        case 'S':
            return get_board_at(state, snake->head_x, snake->head_y + 1);
        default:
            exit(1);
    }
}


/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the x and y coordinates of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t *snake = (state->snakes + snum);
    char head = get_board_at(state, snake->head_x, snake->head_y);

    set_board_at(state, snake->head_x, snake->head_y, head_to_body(head));
    snake->head_x = get_next_x(snake->head_x, head);
    snake->head_y = get_next_y(snake->head_y, head);
    set_board_at(state, snake->head_x, snake->head_y, head); 
    return;
}


/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^v<>) into a tail character (wasd)

  ...in the snake struct: update the x and y coordinates of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t *snake = state->snakes + snum;
    char tail = get_board_at(state, snake->tail_x, snake->tail_y);
    set_board_at(state, snake->tail_x, snake->tail_y, ' ');
    snake->tail_x = get_next_x(snake->tail_x, tail);
    snake->tail_y = get_next_y(snake->tail_y, tail);
    char body = get_board_at(state, snake->tail_x, snake->tail_y);
    set_board_at(state, snake->tail_x, snake->tail_y, body_to_tail(body));
    return;
}


/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  unsigned int snum = 0;
    snake_t *snake = state->snakes;
    while (snum < state->num_snakes) {
       if (get_board_at(state, snake->head_x, snake->head_y) != 'x') {
	 char c = next_square(state, snum);
    
        if (c == '*') {
            update_head(state, snum);
            add_food(state);
        } else if (is_snake(c) || c == '#') {
		set_board_at(state, snake->head_x, snake->head_y, 'x');
		snake->live = false;
	} else {
            update_head(state, snum);
            update_tail(state, snum);
        }
       }
        snum++;
        snake = (state->snakes + snum);
    }
    return;
}


/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
/*     int rows = 0;
  char c;   
  FILE *fp = NULL;
  fp = fopen(filename, "r");
  while (1) {
	c = fgetc(fp);
      if( feof(fp) ) {
         break;
      }
      if (c != '\n') {
	
    } else {
	    rows++;
    }
  }

 

  fclose(fp);

    game_state_t *gsp = (game_state_t *) malloc(sizeof(game_state_t));
   gsp->num_rows = rows;
    gsp->board = (char **) malloc(gsp->num_rows * sizeof(char *));
   
  int i = 0;
  int prev = 0;
  int length = 0;
  fp = fopen(filename, "r");
    while ((c = getc(fp)) != EOF) {
         char buffer[255];
  // get the length of the rectanagle
  fgets(buffer, 255, fp);
  // delete the \n
  length = strlen(buffer);
  gsp->board[i] = malloc(sizeof(char) * (length - prev + 1));
  prev = length;
  strcpy(gsp->board[0], buffer);

  size_t i = 0;
  for (; fgets(buffer, 255, fp); i++) {
    gsp->board = (char **)realloc(gsp->board, sizeof(char *) * (i + 1));
    gsp->board[i] = (char *)malloc(sizeof(char) * (length + 1));
    buffer[strlen(buffer) - 1] = '\0';
    strcpy(gsp->board[i], buffer);
  }
  gsp->y_size = i;

  fclose(fp);
  return gsp;


*/
	return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail coordinates filled in,
  trace through the board to find the head coordinates, and
  fill in the head coordinates in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}


/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  return NULL;
}
