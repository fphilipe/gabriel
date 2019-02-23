#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*-----------------FUNCTIONS-------------------*/
int prompt_size();

void print_map(int **map, int width, int height);

int **generate_map(int size);

int **gen_labirinth(int **map, int width, int height, int i, int j);

int *check_holes(int **map, int width, int height, int i, int j);

int *possible_moves(int **map, int width, int height, int i, int j);

void explore(int **map, int width, int height);
/* -------------------MAIN---------------------*/

int main() {
  srand(time(NULL));

  int size = prompt_size();
  int **map = generate_map(size);

  explore(map, size, size);
  return 0;
}

/*---------------------------------------------*/
int prompt_size() {
  int size;
  while (1) {
    printf("Size of the map? (odd number)\n");
    scanf("%d%*c", &size);
    if (size % 2 == 1) {
      return size;
    } else {
      printf("Even number entered!\n");
    }
  }
}

void print_map(int **map, int width, int height) {
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      if (map[i][j] == 3) {
        printf("O");
      } else if (map[i][j] == 2) {
        printf("K");
      } else if (map[i][j] == 1) {
        printf("#");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
}

int **generate_map(int size) {
  int p, q;
  int i = 1, j = 1;
  int **map = calloc(size, sizeof(int *));
  for (p = 0; p < size; p++) {
    map[p] = calloc(size, sizeof(int));
  }
  for (p = 0; p < size; p++) {
    for (q = 0; q < size; q++) {
      map[p][q] = 1;
    }
  }
  map[i][j] = 2;
  return gen_labirinth(map, size, size, i, j);
}

int **gen_labirinth(int **map, int width, int height, int i, int j) {
  /*       i and j position of hole digger          */
  int u;
  int r, w;
  int n_to_dig = 0;

  /*  creating an array of possible movements for the digger: */
  /*    - 0 sx      - 1 up      - 2 dx       - 3 down         */
  int where_to_dig[4];

  int *free_to_dig = check_holes(map, width, height, i, j);

  for (u = 0; u < 4; u++) {
    if (free_to_dig[u] == 1) {
      where_to_dig[n_to_dig] = u;
      n_to_dig += 1;
    }
  }
  if (n_to_dig == 0) { //no more possible adjacent possible holes to dig
    map[i][j] = 3;
    return map;
  } else { //random choice among possible ways of digging
    r = rand() % n_to_dig;
    w = where_to_dig[r];
    //digging
    switch (w) {
      case 0: j -= 2; map[i][j+1] = 0; break;
      case 1: i -= 2; map[i+1][j] = 0; break;
      case 2: j += 2; map[i][j-1] = 0; break;
      case 3: i += 2; map[i-1][j] = 0; break;
    }
    map[i][j] = 0;
    return gen_labirinth(map, width, height, i, j);
  }
}

int *check_holes(int **map, int width, int height, int i, int j) {
  int *dig = calloc(4, sizeof(int));
  int u;
  for (u = 0; u < 4; u++) {
    dig[u] = 0;
  }
  //checking if sx if free to go
  if (j-2 > 0) {
    dig[0] = map[i][j-2];
  }
  //checking if up is free to go
  if (i-2 > 0) {
    dig[1] = map[i-2][j];
  }
  //checking if dx is free to go
  if (j+2 < width) {
    dig[2] = map[i][j+2];
  }
  //checking if down is free to go
  if (i+2 < height) {
    dig[3] = map[i+2][j];
  }
  return dig;
}

int *possible_moves(int **map, int width, int height, int i, int j) {
  int *moves = calloc(4, sizeof(int));
  int u;
  for (u = 0; u < 4; u++) {
    moves[u] = 1;
  }
  //checking if sx if free to go
  if (j-1 > 0) {
    moves[0] = map[i][j-1];
  }
  //checking if up is free to go
  if (i-1 > 0) {
    moves[1] = map[i-1][j];
  }
  //checking if dx is free to go
  if (j+1 < width) {
    moves[2] = map[i][j+1];
  }
  //checking if down is free to go
  if (i+1 < height) {
    moves[3] = map[i+1][j];
  }
  return moves;
}

void explore(int **map, int width, int height) {
  char input;
  int i = 1, j = 1;
  int *move = calloc(4, sizeof(int));
  while (input != 'q') {
    print_map(map, width, height);
    printf("\nChoose your next action (wasd - q to exit):\n");
    scanf("\n");
    scanf("%1c", &input);
    move = possible_moves(map, width, height, i, j);
    /* array example:  {1,1,0,1}   0 is open, 1 is wall*/
    /* move[4] = move[sx,up,dx,down] */
    switch (input) {
      case 'w':
        if (move[1] == 0) {
          i -= 1;
          map[i+1][j] = 0;
          map[i][j] = 2;
        } break;
      case 'a':
        if (move[0] == 0) {
          j -= 1;
          map[i][j+1] = 0;
          map[i][j] = 2;
        } break;
      case 's':
        if (move[3] == 0) {
          i += 1;
          map[i-1][j] = 0;
          map[i][j] = 2;
        } break;
      case 'd':
        if (move[2] == 0) {
          j += 1;
          map[i][j-1] = 0;
          map[i][j] = 2;
        } break;
      case 'q': break;
      default: continue;
    }
  }
  return;
}
