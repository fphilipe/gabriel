#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum direction { left, up, right, down };
enum cell_type { wall, path, player, target };
enum keys {
  keyLeft  = 'a',
  keyUp    = 'w',
  keyRight = 'd',
  keyDown  = 's',
  keyQuit  = 'q'
};

/*-----------------FUNCTIONS-------------------*/
int prompt_size(char *label);

void print_map(int **map, int width, int height);

int **generate_map(int width, int height);

int **dig(int **map, int width, int height, int x, int y);

int can_dig(int dir, int **map, int width, int height, int x, int y);

int can_move(int dir, int **map, int width, int height, int x, int y);

void explore(int **map, int width, int height);

int directionForKey(char key);

void move(int dir, int **map, int *x, int *y);
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
        case wall:   printf("#"); break;
        case path:   printf(" "); break;
        case player: printf("K"); break;
        case target: printf("O"); break;
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

  map[1][1] = player;

  return dig(map, width, height, 1, 1);
}

int **dig(int **map, int width, int height, int x, int y) {
  int r, w;
  int n_to_dig = 0;

  int where_to_dig[4];

  for (int dir = 0; dir < 4; dir++) {
    if (can_dig(dir, map, width, height, x, y)) {
      where_to_dig[n_to_dig] = dir;
      n_to_dig += 1;
    }
  }
  if (n_to_dig == 0) { //no more possible adjacent possible holes to dig
    map[y][x] = target;
    return map;
  } else { //random choice among possible ways of digging
    r = rand() % n_to_dig;
    w = where_to_dig[r];
    //digging
    switch (w) {
      case left:  x -= 2; map[y][x+1] = path; break;
      case up:    y -= 2; map[y+1][x] = path; break;
      case right: x += 2; map[y][x-1] = path; break;
      case down:  y += 2; map[y-1][x] = path; break;
    }
    map[y][x] = path;
    return dig(map, width, height, x, y);
  }
}

int can_dig(int dir, Map map, int width, int height, int x, int y) {
  switch (dir) {
    case left:  return (x-2 > 0)      && map[y][x-2] == wall;
    case up:    return (y-2 > 0)      && map[y-2][x] == wall;
    case right: return (x+2 < width)  && map[y][x+2] == wall;
    case down:  return (y+2 < height) && map[y+2][x] == wall;
    default: abort();
  }
}

int can_move(int dir, int **map, int width, int height, int x, int y) {
  switch (dir) {
    case left:  return (x-1 > 0)      && map[y][x-1] == path;
    case up:    return (y-1 > 0)      && map[y-1][x] == path;
    case right: return (x+1 < width)  && map[y][x+1] == path;
    case down:  return (y+1 < height) && map[y+1][x] == path;
    default: abort();
  }
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
    switch (input) {
      case keyLeft:
      case keyUp:
      case keyRight:
      case keyDown:
        {
          int dir = directionForKey(input);
          if (can_move(dir, map, width, height, x, y))
            move(dir, map, &x, &y);
          break;
        }
      case keyQuit: break;
      default: continue;
    }
  }
  return;
}

int directionForKey(char key) {
  switch (key) {
    case keyLeft:  return left;
    case keyUp:    return up;
    case keyRight: return right;
    case keyDown:  return down;
    default: abort();
  }
}

void move(int dir, int **map, int *x, int *y) {
  switch (dir) {
    case left:
      {
        *x -= 1;
        map[*y][*x+1] = path;
        map[*y][*x] = player;
        break;
      }
    case up:
      {
        *y -= 1;
        map[*y+1][*x] = path;
        map[*y][*x] = player;
        break;
      }
    case right:
      {
        *x += 1;
        map[*y][*x-1] = path;
        map[*y][*x] = player;
        break;
      }
    case down:
      {
        *y += 1;
        map[*y-1][*x] = path;
        map[*y][*x] = player;
        break;
      }
  }
}
