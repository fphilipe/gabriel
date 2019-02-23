#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*-----------------FUNCTIONS-------------------*/
int prompt_size(char *label);

void print_map(int **map, int width, int height);

int **generate_map(int width, int height);

int **dig(int **map, int width, int height, int x, int y);

int *check_holes(int **map, int width, int height, int x, int y);

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
  int x, y;
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      if (map[y][x] == 3) {
        printf("O");
      } else if (map[y][x] == 2) {
        printf("K");
      } else if (map[y][x] == 1) {
        printf("#");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
}

int **generate_map(int width, int height) {
  int p, q;
  int x = 1;
  int y = 1;
  int **map = calloc(height, sizeof(int *));
  for (p = 0; p < height; p++) {
    map[p] = calloc(width, sizeof(int));
  }
  for (p = 0; p < height; p++) {
    for (q = 0; q < width; q++) {
      map[p][q] = 1;
    }
  }
  map[y][x] = 2;
  return dig(map, width, height, x, y);
}

int **dig(int **map, int width, int height, int x, int y) {
  int u;
  int r, w;
  int n_to_dig = 0;

  /*  creating an array of possible movements for the digger: */
  /*    - 0 sx      - 1 up      - 2 dx       - 3 down         */
  int where_to_dig[4];

  int *free_to_dig = check_holes(map, width, height, x, y);

  for (u = 0; u < 4; u++) {
    if (free_to_dig[u] == 1) {
      where_to_dig[n_to_dig] = u;
      n_to_dig += 1;
    }
  }
  if (n_to_dig == 0) { //no more possible adjacent possible holes to dig
    map[y][x] = 3;
    return map;
  } else { //random choice among possible ways of digging
    r = rand() % n_to_dig;
    w = where_to_dig[r];
    //digging
    switch (w) {
      case 0: x -= 2; map[y][x+1] = 0; break;
      case 1: y -= 2; map[y+1][x] = 0; break;
      case 2: x += 2; map[y][x-1] = 0; break;
      case 3: y += 2; map[y-1][x] = 0; break;
    }
    map[y][x] = 0;
    return dig(map, width, height, x, y);
  }
}

int *check_holes(int **map, int width, int height, int x, int y) {
  int *dig = calloc(4, sizeof(int));
  int u;
  for (u = 0; u < 4; u++) {
    dig[u] = 0;
  }
  //checking if sx if free to go
  if (x-2 > 0) {
    dig[0] = map[y][x-2];
  }
  //checking if up is free to go
  if (y-2 > 0) {
    dig[1] = map[y-2][x];
  }
  //checking if dx is free to go
  if (x+2 < width) {
    dig[2] = map[y][x+2];
  }
  //checking if down is free to go
  if (y+2 < height) {
    dig[3] = map[y+2][x];
  }
  return dig;
}

int *possible_moves(int **map, int width, int height, int x, int y) {
  int *moves = calloc(4, sizeof(int));
  int u;
  for (u = 0; u < 4; u++) {
    moves[u] = 1;
  }
  //checking if sx if free to go
  if (x-1 > 0) {
    moves[0] = map[y][x-1];
  }
  //checking if up is free to go
  if (y-1 > 0) {
    moves[1] = map[y-1][x];
  }
  //checking if dx is free to go
  if (x+1 < width) {
    moves[2] = map[y][x+1];
  }
  //checking if down is free to go
  if (y+1 < height) {
    moves[3] = map[y+1][x];
  }
  return moves;
}

void explore(int **map, int width, int height) {
  char input;
  int x = 1;
  int y = 1;
  int *move = calloc(4, sizeof(int));
  while (input != 'q') {
    print_map(map, width, height);
    printf("\nChoose your next action (wasd - q to exit):\n");
    scanf("\n");
    scanf("%1c", &input);
    move = possible_moves(map, width, height, x, y);
    /* array example:  {1,1,0,1}   0 is open, 1 is wall*/
    /* move[4] = move[sx,up,dx,down] */
    switch (input) {
      case 'w':
        if (move[1] == 0) {
          y -= 1;
          map[y+1][x] = 0;
          map[y][x] = 2;
        } break;
      case 'a':
        if (move[0] == 0) {
          x -= 1;
          map[y][x+1] = 0;
          map[y][x] = 2;
        } break;
      case 's':
        if (move[3] == 0) {
          y += 1;
          map[y-1][x] = 0;
          map[y][x] = 2;
        } break;
      case 'd':
        if (move[2] == 0) {
          x += 1;
          map[y][x-1] = 0;
          map[y][x] = 2;
        } break;
      case 'q': break;
      default: continue;
    }
  }
  return;
}
