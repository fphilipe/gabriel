#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LEFT  0
#define UP    1
#define RIGHT 2
#define DOWN  3

#define PATH   0
#define WALL   1
#define PLAYER 2
#define TARGET 3

/*-----------------FUNCTIONS-------------------*/
int prompt_size(char *label);

void print_map(int **map, int width, int height);

int **generate_map(int width, int height);

int **dig(int **map, int width, int height, int x, int y);

int *possible_digs(int **map, int width, int height, int x, int y);

int *possible_moves(int **map, int width, int height, int x, int y);

void explore(int **map, int width, int height);
/* -------------------MAIN---------------------*/

int main() {
  srand(time(NULL));

  int width = prompt_size("Width");
  int height = prompt_size("Height");
  int **map = generate_map(width, height);

  explore(map, width, height);
  return 0;
}

/*---------------------------------------------*/
int prompt_size(char *label) {
  int size;
  while (1) {
    printf("%s of the map? (odd number)\n", label);
    scanf("%d%*c", &size);
    if (size % 2 == 1) {
      return size;
    } else {
      printf("Even number entered!\n");
    }
  }
}

void print_map(int **map, int width, int height) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      switch (map[y][x]) {
        case PATH:   printf(" "); break;
        case WALL:   printf("#"); break;
        case PLAYER: printf("K"); break;
        case TARGET: printf("O"); break;
      }
    }
    printf("\n");
  }
}

int **generate_map(int width, int height) {
  int **map = calloc(height, sizeof(int *));
  for (int y = 0; y < height; y++) {
    map[y] = calloc(width, sizeof(int));
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      map[y][x] = WALL;
    }
  }

  map[1][1] = PLAYER;

  return dig(map, width, height, 1, 1);
}

int **dig(int **map, int width, int height, int x, int y) {
  int r, w;
  int n_to_dig = 0;

  int where_to_dig[4];

  int *can_dig = possible_digs(map, width, height, x, y);
  for (int dir = 0; dir < 4; dir++) {
    if (can_dig[dir]) {
      where_to_dig[n_to_dig] = dir;
      n_to_dig += 1;
    }
  }
  if (n_to_dig == 0) { //no more possible adjacent possible holes to dig
    map[y][x] = TARGET;
    return map;
  } else { //random choice among possible ways of digging
    r = rand() % n_to_dig;
    w = where_to_dig[r];
    //digging
    switch (w) {
      case 0: x -= 2; map[y][x+1] = PATH; break;
      case 1: y -= 2; map[y+1][x] = PATH; break;
      case 2: x += 2; map[y][x-1] = PATH; break;
      case 3: y += 2; map[y-1][x] = PATH; break;
    }
    map[y][x] = PATH;
    return dig(map, width, height, x, y);
  }
}

int *possible_digs(int **map, int width, int height, int x, int y) {
  int *dig = calloc(4, sizeof(int));

  if (x-2 > 0)      { dig[LEFT]  = map[y][x-2] == WALL; }
  if (y-2 > 0)      { dig[UP]    = map[y-2][x] == WALL; }
  if (x+2 < width)  { dig[RIGHT] = map[y][x+2] == WALL; }
  if (y+2 < height) { dig[DOWN]  = map[y+2][x] == WALL; }

  return dig;
}

int *possible_moves(int **map, int width, int height, int x, int y) {
  int *moves = calloc(4, sizeof(int));

  if (x-1 > 0)      { moves[LEFT]  = map[y][x-1] == PATH; }
  if (y-1 > 0)      { moves[UP]    = map[y-1][x] == PATH; }
  if (x+1 < width)  { moves[RIGHT] = map[y][x+1] == PATH; }
  if (y+1 < height) { moves[DOWN]  = map[y+1][x] == PATH; }

  return moves;
}

void explore(int **map, int width, int height) {
  char input;
  int x = 1;
  int y = 1;
  while (input != 'q') {
    print_map(map, width, height);
    printf("\nChoose your next action (wasd - q to exit):\n");
    scanf("\n");
    scanf("%1c", &input);
    int *can_move = possible_moves(map, width, height, x, y);
    switch (input) {
      case 'a':
        if (can_move[LEFT]) {
          x -= 1;
          map[y][x+1] = PATH;
          map[y][x] = PLAYER;
        } break;
      case 'w':
        if (can_move[UP]) {
          y -= 1;
          map[y+1][x] = PATH;
          map[y][x] = PLAYER;
        } break;
      case 'd':
        if (can_move[RIGHT]) {
          x += 1;
          map[y][x-1] = PATH;
          map[y][x] = PLAYER;
        } break;
      case 's':
        if (can_move[DOWN]) {
          y += 1;
          map[y-1][x] = PATH;
          map[y][x] = PLAYER;
        } break;
      case 'q': break;
      default: continue;
    }
  }
  return;
}
